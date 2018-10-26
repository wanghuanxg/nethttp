#pragma once
#include <string>
#include "Poco/Mutex.h"
#include "NetThreadManager.h"
#include "PushObjectManager.h"

using namespace std;
using Poco::Mutex;

class CAllObjectManagerUtil
{
public:
	static void InitializeSSL();
	static void UnInitializeSSL();
	static void InitializeLocale();
	static void UnInitializeLocale();
	static void ReleasePhmsLogger();
	static void SetPriorIp(string stringIp);
	static string GetPriorIp();
public:
	static Mutex m_mutex;
	static string m_stringPriorIp;
};
