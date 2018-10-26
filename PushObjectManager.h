#pragma once
#include <map>
#include "PushBase.h"

class CPushObjectManager
{
public:
	static int InsertPushMsgListObject(string stringUuid, string stringSenderId, string stringCaseId, string stringCaseType, string stringMsgType, string stringMsgDirection, string stringVersion, 
		pHttpPushCallback pCallback, pHttpPushFailCallback pFailCallback);
	static int DeletePushObject(string);
	static void HandleSessionSync(string stringSessionId);
	static bool SomeTypePushIsRun(int nPushType);
	static int GetSessionId(string&);
	static int SetSessionId(string);
public:
	static map<string, CPushBase*> m_map;
};
