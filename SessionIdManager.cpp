#include "SessionIdManager.h"
#include "ExternalNetInterface.h"
#include "ErrorCode.h"
#include "NetThreadManager.h"
#include "PushObjectManager.h"



int CSessionIdManager::SetSessionId(string stringSessionId)
{
	CNetThreadManager::SetSessionId(stringSessionId);
	CPushObjectManager::SetSessionId(stringSessionId);
	return PHMS_SUCCESSFUL_RESULT;
}



int CSessionIdManager::GetSessionId(string& stringSessionId)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	nRet = CNetThreadManager::GetSessionId(stringSessionId);
	if(nRet == COMMON_NO_VALID_SESSIONID)
	{
		return nRet=CPushObjectManager::GetSessionId(stringSessionId);
	}
	return nRet;
}


void CSessionIdManager::HandleSessionSync(string stringSessionId)
{
	CNetThreadManager::HandleSessionSync(stringSessionId);
	CPushObjectManager::HandleSessionSync(stringSessionId);
}


int CSessionIdManager::SetGWSessionId(string stringSessionId)
{
	CNetThreadManager::SetGWSessionId(stringSessionId);
	return PHMS_SUCCESSFUL_RESULT;
}

int CSessionIdManager::GetGWSessionId(string& stringSessionId)
{
	return CNetThreadManager::GetGWSessionId(stringSessionId);
}

void CSessionIdManager::HandleGWSessionSync(string stringSessionId)
{
	CNetThreadManager::HandleGWSessionSync(stringSessionId);
}
