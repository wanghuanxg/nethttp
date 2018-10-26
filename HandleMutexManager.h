#pragma once
#include "string"
#include <map>
#include "Poco/Mutex.h"
#include "Poco/Thread.h"

using namespace::std;
using Poco::FastMutex;
using Poco::Mutex;
using Poco::Thread;

class CHandleMutexManager
{
public:
	static int InsertHandleMutex(string stringUuid);
	static int DeleteHandleMutex(string stringUuid);
	static Mutex* GetHandleMutex(string stringUuid);
private:
	static Mutex m_mutexHandleMutex;
	static map<string, Mutex*> m_mapHandleAndMutex;
};
