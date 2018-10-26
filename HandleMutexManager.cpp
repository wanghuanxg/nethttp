#include "HandleMutexManager.h"
#include "ErrorCode.h"

Mutex CHandleMutexManager::m_mutexHandleMutex;
map<string, Mutex*> CHandleMutexManager::m_mapHandleAndMutex;

int CHandleMutexManager::InsertHandleMutex(string stringUuid)
{
	Mutex::ScopedLock lock(m_mutexHandleMutex);
	if(m_mapHandleAndMutex.find(stringUuid) != m_mapHandleAndMutex.end())
	{
		//说明该handle的mutex已存在
		return PHMS_SUCCESSFUL_RESULT;
	}
	m_mapHandleAndMutex.insert(map<string, Mutex*>::value_type(stringUuid, new Mutex));
	return PHMS_SUCCESSFUL_RESULT;
}
int CHandleMutexManager::DeleteHandleMutex(string stringUuid)
{
	Mutex::ScopedLock lock(m_mutexHandleMutex);
	Mutex* pMutex = NULL;
	if(m_mapHandleAndMutex.find(stringUuid) != m_mapHandleAndMutex.end())
	{
		pMutex = m_mapHandleAndMutex[stringUuid];
	}
	if(pMutex != NULL)
	{
		m_mapHandleAndMutex.erase(stringUuid);
		delete pMutex;
	}
	else
	{
		return COMMON_INVALID_HANDLE;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

Mutex* CHandleMutexManager::GetHandleMutex(string stringUuid)
{
	Mutex::ScopedLock lock(m_mutexHandleMutex);
	Mutex* pMutex = NULL;
	if(m_mapHandleAndMutex.find(stringUuid) != m_mapHandleAndMutex.end())
	{
		pMutex = m_mapHandleAndMutex[stringUuid];
	}
	return pMutex;
}