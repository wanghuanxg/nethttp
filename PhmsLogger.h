#pragma once
#include "Poco/Exception.h"
#include "Poco/Mutex.h"
#include "string"

using Poco::Exception;
using Poco::Mutex;
using namespace std;

class CPhmsLogger
{
private:
	CPhmsLogger(void);
	~CPhmsLogger(void);
private:
	static CPhmsLogger* m_pPhmsLogger;
	static Mutex m_mutex;
public:
	static CPhmsLogger* GetPhmsLogger();
	static void ReleasePhmsLogger();
	void WriteLog(string stringText, const char* stringFunction, const char* stringFile, int nLine);
	void WriteLog(const Exception& e, const char* stringFunction, const char* stringFile, int nLine);
};
