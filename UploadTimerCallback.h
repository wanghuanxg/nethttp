#pragma once
#include "Poco/Timer.h"
#include "PhmsDataType.h"
#include <string>

using Poco::Timer;
using namespace std;

class CUploadTimerCallback
{
public:
	CUploadTimerCallback(pProgressAndSpeedCallback pCallback, string stringIniFilePath, unsigned long nAlreadyUploadSize, unsigned long nTotalSize, string stringLocalFilePath);
	~CUploadTimerCallback(void);
private:
	pProgressAndSpeedCallback m_pCallback;
	string m_stringIniFilePath;
	unsigned long m_nCurrentUploadFileSize;
	unsigned long m_nPreUploadFileSize;
	unsigned long m_nTotalSize;
	string m_stringLocalFilePath;
	int m_nSeconds;
public:
	void onTimer(Timer& timer); 
};
