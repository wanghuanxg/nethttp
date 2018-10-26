#include "NetThreadManager.h"
#include "AllObjectManagerUtil.h"
#include "ErrorCode.h"
#include "Poco/Exception.h"
#include "Util.h"
#include "PhmsLogger.h"

using Poco::FileNotFoundException;

map<string, CExternalNetInterface*> CNetThreadManager::m_map;
map<Thread::TID, string> CNetThreadManager::m_mapTidAndHandle;
map<string, Thread::TID> CNetThreadManager::m_mapHandleAndTid;

int CNetThreadManager::InsertNetThread(string stringUuid)
{
	Mutex::ScopedLock lock(CAllObjectManagerUtil::m_mutex);
	Thread::TID nThreadId = Thread::currentTid();
	if(m_mapTidAndHandle.find(nThreadId) != m_mapTidAndHandle.end())
	{
		//说明该线程已经初始化过了
		return PHMS_SUCCESSFUL_RESULT;
	}
	CExternalNetInterface* pNetInterface = NULL;
	try
	{
		pNetInterface = new CExternalNetInterface();
	}
	catch(FileNotFoundException& e)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(e, __FUNCTION__, __FILE__, __LINE__);
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_NO_NET_CONFIG_FILE), __FUNCTION__, __FILE__, __LINE__);
		return COMMON_NO_NET_CONFIG_FILE;
	}
	catch(Exception& e)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(e, __FUNCTION__, __FILE__, __LINE__);
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_NET_CONFIG_READ_ERROR), __FUNCTION__, __FILE__, __LINE__);
		return COMMON_NET_CONFIG_READ_ERROR;
	}
	m_map.insert(map<string, CExternalNetInterface*>::value_type(stringUuid, pNetInterface));
	m_mapTidAndHandle.insert(map<Thread::TID, string>::value_type(nThreadId, stringUuid));
	m_mapHandleAndTid.insert(map<string, Thread::TID>::value_type(stringUuid, nThreadId));
	string stringSessionId;
	if(CNetThreadManager::m_map.size() != 0)
	{
		map<string, CExternalNetInterface*>::const_iterator iter;
		for(iter=CNetThreadManager::m_map.begin(); iter!=CNetThreadManager::m_map.end(); iter++)
		{
			if(iter->second->GetSessionId().size()!=0 && iter->second->GetSessionId()!="00000000000000000000000000000000")
			{
				stringSessionId = iter->second->GetSessionId();
				pNetInterface->SetSessionId(stringSessionId);
				break;
			}
		}
		//设置公卫系统的SessionId
		for(iter=CNetThreadManager::m_map.begin(); iter!=CNetThreadManager::m_map.end(); iter++)
		{
			if(iter->second->GetGWSessionId().size()!=0 && iter->second->GetGWSessionId()!="00000000000000000000000000000000")
			{
				stringSessionId = iter->second->GetGWSessionId();
				pNetInterface->SetGWSessionId(stringSessionId);
				break;
			}
		}
	}
	else
	{
		map<string, CPushBase*>::const_iterator iter;
		for(iter=CPushObjectManager::m_map.begin(); iter!=CPushObjectManager::m_map.end(); iter++)
		{
			if(iter->second->GetSessionId().size()!=0 && iter->second->GetSessionId()!="00000000000000000000000000000000")
			{
				stringSessionId = iter->second->GetSessionId();
				pNetInterface->SetSessionId(stringSessionId);
				break;
			}
		}
	}
	return PHMS_SUCCESSFUL_RESULT;
}


int CNetThreadManager::DeleteNetThread(string stringUuid)
{
	Mutex::ScopedLock lock(CAllObjectManagerUtil::m_mutex);
	CExternalNetInterface* pNetInterface = NULL;
	if(m_map.find(stringUuid) != m_map.end())
	{
		pNetInterface = m_map[stringUuid];
	}
	if(pNetInterface != NULL)
	{
		m_map.erase(stringUuid);
		Thread::TID nThreadId = m_mapHandleAndTid[stringUuid];
		m_mapTidAndHandle.erase(nThreadId);
		m_mapHandleAndTid.erase(stringUuid);
		delete pNetInterface;
		//CommonNetInterface中执行ReleasePhmsLogger
		return PHMS_SUCCESSFUL_RESULT;
	}
	else
	{
		return COMMON_INVALID_HANDLE;
	}
}

string CNetThreadManager::GetHandleByThreadId(Thread::TID nThreadId)
{
	Mutex::ScopedLock lock(CAllObjectManagerUtil::m_mutex);
	string stringUuid;
	if(m_mapTidAndHandle.find(nThreadId) != m_mapTidAndHandle.end())
	{
		stringUuid = m_mapTidAndHandle[nThreadId];
	}
	return stringUuid;
}

CExternalNetInterface* CNetThreadManager::GetNetThread(string stringUuid, bool bCrossingThread)
{
	Mutex::ScopedLock lock(CAllObjectManagerUtil::m_mutex);
	CExternalNetInterface* pNetInterface = NULL;
	if(m_map.find(stringUuid) != m_map.end())
	{
		if(!bCrossingThread)
		{
			Thread::TID nThreadId = m_mapHandleAndTid[stringUuid];
			if(nThreadId != Thread::currentTid())
			{
				CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_HANDLE_THREAD_MISMATCH), __FUNCTION__, __FILE__, __LINE__);
				return NULL;
			}
		}
		pNetInterface = m_map[stringUuid];
	}
	return pNetInterface;
}


int CNetThreadManager::GetSessionId(string& stringSessionId)
{
	Mutex::ScopedLock lock(CAllObjectManagerUtil::m_mutex);
	map<string, CExternalNetInterface*>::const_iterator iter;
	for(iter=m_map.begin(); iter!=m_map.end(); iter++)
	{
		if(iter->second->GetSessionId().size()!=0 && iter->second->GetSessionId()!="00000000000000000000000000000000")
		{
			stringSessionId = iter->second->GetSessionId();
			break;
		}
	}
	if(iter == m_map.end())
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_NO_VALID_SESSIONID), __FUNCTION__, __FILE__, __LINE__);
		return COMMON_NO_VALID_SESSIONID;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CNetThreadManager::SetSessionId(string stringSessionId)
{
	HandleSessionSync(stringSessionId);
	return PHMS_SUCCESSFUL_RESULT;
}


void CNetThreadManager::HandleSessionSync(string stringSessionId)
{
	Mutex::ScopedLock lock(CAllObjectManagerUtil::m_mutex);
	map<string, CExternalNetInterface*>::iterator iter;
	for(iter=m_map.begin(); iter!=m_map.end(); iter++)
	{
		iter->second->SetSessionId(stringSessionId);
	}
	return;
}

int CNetThreadManager::SetGWSessionId(string stringSessionId)
{
	HandleGWSessionSync(stringSessionId);
	return PHMS_SUCCESSFUL_RESULT;
}

int CNetThreadManager::GetGWSessionId(string& stringSessionId)
{
	Mutex::ScopedLock lock(CAllObjectManagerUtil::m_mutex);
	map<string, CExternalNetInterface*>::const_iterator iter;
	for(iter=m_map.begin(); iter!=m_map.end(); iter++)
	{
		if(iter->second->GetGWSessionId().size()!=0 && iter->second->GetGWSessionId()!="00000000000000000000000000000000")
		{
			stringSessionId = iter->second->GetGWSessionId();
			break;
		}
	}
	if(iter == m_map.end())
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_NO_VALID_SESSIONID), __FUNCTION__, __FILE__, __LINE__);
		return COMMON_NO_VALID_SESSIONID;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

void CNetThreadManager::HandleGWSessionSync(string stringSessionId)
{
	Mutex::ScopedLock lock(CAllObjectManagerUtil::m_mutex);
	map<string, CExternalNetInterface*>::iterator iter;
	for(iter=m_map.begin(); iter!=m_map.end(); iter++)
	{
		iter->second->SetGWSessionId(stringSessionId);
	}
	return;
}
