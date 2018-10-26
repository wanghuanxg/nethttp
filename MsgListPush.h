#pragma once
#include "PushBase.h"
#include "PhmsDataType.h"

class CMsgListPush :
	public CPushBase
{
public:
	CMsgListPush(string stringUuid, string stringSenderId, string stringCaseId, string stringCaseType, string stringMsgType, string stringMsgDirection, string stringVersion, 
		pHttpPushCallback pCallback, pHttpPushFailCallback pFailCallback);
	~CMsgListPush(void);

	void ExecutePushSession();
private:
	string m_stringUuid;
	string m_stringSenderId;
	string m_stringCaseId;
	string m_stringCaseType;
	string m_stringMsgType;
	string m_stringMsgDirection;
	string m_stringVersion;
};
