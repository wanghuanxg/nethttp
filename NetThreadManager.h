#pragma once
#include "ExternalNetInterface.h"
#include <string>
#include "Poco/Mutex.h"
#include "Poco/Thread.h"

using namespace::std;
using Poco::FastMutex;
using Poco::Mutex;
using Poco::Thread;


class CNetThreadManager
{
public:
	static int InsertNetThread(string);
	static int DeleteNetThread(string);
	static string GetHandleByThreadId(Thread::TID nThreadId);
	static CExternalNetInterface* GetNetThread(string stringUuid, bool bCrossingThread=false);
	
	//sessionid相关
	static int GetSessionId(string& stringSessionId);
	static int SetSessionId(string stringSessionId);
	static void HandleSessionSync(string stringSessionId);
	//公卫sessionid相关
	static int SetGWSessionId(string stringSessionId);
	static int GetGWSessionId(string& stringSessionId);
	static void HandleGWSessionSync(string stringSessionId);
public:
	static map<string, CExternalNetInterface*> m_map;
	static map<Thread::TID, string> m_mapTidAndHandle;
	static map<string, Thread::TID> m_mapHandleAndTid;
	static map<string, Mutex*> m_mapHandleAndMutex;
};
