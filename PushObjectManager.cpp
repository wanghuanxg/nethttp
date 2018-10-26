#include "PushObjectManager.h"
#include "AllObjectManagerUtil.h"
#include "ErrorCode.h"
#include "Poco/Exception.h"
#include "Util.h"
#include "PhmsLogger.h"
#include "MsgListPush.h"

using Poco::FileNotFoundException;

map<string, CPushBase*> CPushObjectManager::m_map;

int CPushObjectManager::InsertPushMsgListObject(string stringUuid, string stringSenderId, string stringCaseId, string stringCaseType, string stringMsgType, string stringMsgDirection, string stringVersion, 
												pHttpPushCallback pCallback, pHttpPushFailCallback pFailCallback)
{
	Mutex::ScopedLock lock(CAllObjectManagerUtil::m_mutex);
	CPushBase* pNetInterface = NULL;
	try
	{
		pNetInterface = new CMsgListPush(stringUuid, stringSenderId, stringCaseId, stringCaseType, stringMsgType, stringMsgDirection, stringVersion, pCallback, pFailCallback);
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
	m_map.insert(map<string, CPushBase*>::value_type(stringUuid, pNetInterface));
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
	if(stringSessionId.size() == 0)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_NO_VALID_SESSIONID), __FUNCTION__, __FILE__, __LINE__);
		return COMMON_NO_VALID_SESSIONID;
	}
	return PHMS_SUCCESSFUL_RESULT;
}


int CPushObjectManager::DeletePushObject(string stringUuid)
{
	Mutex::ScopedLock lock(CAllObjectManagerUtil::m_mutex);
	CPushBase* pNetInterface = NULL;
	if(m_map.find(stringUuid) != m_map.end())
	{
		pNetInterface = m_map[stringUuid];
	}
	if(pNetInterface != NULL)
	{
		pNetInterface->EndPush();
		m_map.erase(stringUuid);
		delete pNetInterface;
		return PHMS_SUCCESSFUL_RESULT;
	}
	else
	{
		return COMMON_INVALID_HANDLE;
	}

}


int CPushObjectManager::GetSessionId(string& stringSessionId)
{
	Mutex::ScopedLock lock(CAllObjectManagerUtil::m_mutex);
	map<string, CPushBase*>::const_iterator iter;
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

int CPushObjectManager::SetSessionId(string stringSessionId)
{
	HandleSessionSync(stringSessionId);
	return PHMS_SUCCESSFUL_RESULT;
}


void CPushObjectManager::HandleSessionSync(string stringSessionId)
{
	Mutex::ScopedLock lock(CAllObjectManagerUtil::m_mutex);
	map<string, CPushBase*>::iterator iter;
	for(iter=m_map.begin(); iter!=m_map.end(); iter++)
	{
		iter->second->SetSessionId(stringSessionId);
	}
	return;
}

bool CPushObjectManager::SomeTypePushIsRun(int nPushType)
{
	Mutex::ScopedLock lock(CAllObjectManagerUtil::m_mutex);
	map<string, CPushBase*>::iterator iter;
	for(iter=m_map.begin(); iter!=m_map.end(); iter++)
	{
		if(iter->second->GetPushType() == nPushType)
		{
			return true;
		}
	}
	return false;
}
