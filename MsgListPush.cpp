#include "MsgListPush.h"
#include "ErrorCode.h"
#include "PhmsLogger.h"
#include "Util.h"
#include <string>

using namespace std;

CMsgListPush::CMsgListPush(string stringUuid, string stringSenderId, string stringCaseId, string stringCaseType, string stringMsgType, string stringMsgDirection, string stringVersion, pHttpPushCallback pCallback, pHttpPushFailCallback pFailCallback)
:CPushBase(pCallback, pFailCallback, MSG_LIST_PUSH), m_stringUuid(stringUuid), m_stringSenderId(stringSenderId), m_stringCaseId(stringCaseId), m_stringCaseType(stringCaseType), m_stringMsgType(stringMsgType), m_stringMsgDirection(stringMsgDirection), m_stringVersion(stringVersion)
{
}

CMsgListPush::~CMsgListPush(void)
{
}

void CMsgListPush::ExecutePushSession()
{
	m_pPhmsPushSession->SetPushCallback(m_pHttpPushCallback);
	while(m_bStopRetry==false)
	{
		int nRet = PHMS_SUCCESSFUL_RESULT;
		nRet = m_pPhmsPushSession->PushMsg(m_stringSenderId, m_stringCaseId, m_stringCaseType, m_stringMsgType, m_stringMsgDirection, m_stringVersion);
		if(nRet!=PHMS_SUCCESSFUL_RESULT)
		{
			CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
			if(nRet==COMMON_SESSION_INVALID || nRet==COMMON_LOGIN_ANOTHER_PLACE || nRet==COMMON_SERVER_NO_SUPPORT || nRet==COMMON_INVALID_CHARSET || nRet==COMMON_NET_CONFIG_ERROR || nRet==COMMON_URL_NOT_FOUND || nRet==COMMON_NO_NET_CONFIG_FILE)
			{
				m_pHttpPushFailCallback(nRet/*, (NET_HANDLE)m_stringUuid.c_str()*/);
				break;
			}
			else
			{
				//只回调通知，不退出推送线程
				m_pHttpPushFailCallback(nRet);
				Poco::Thread::sleep(2000);
			}
		}
	}
}
