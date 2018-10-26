#pragma once
#include "Poco/Timer.h"
#include "PhmsDataType.h"
#include <string>

using Poco::Timer;
using namespace std;

class CDownloadTimerCallback
{
public:
	CDownloadTimerCallback(pProgressAndSpeedCallback pCallback, unsigned long nFileSize, string stringLocalFilePath, 
		string stringTempDirectory, unsigned long nAlreadyDownloadFileSize, int nThreadCount, string stringServerFilePath);
	~CDownloadTimerCallback(void);
private:
	pProgressAndSpeedCallback m_pCallback;
	unsigned long m_nFileSize;
	string m_stringLocalFilePath;
	string m_stringTempDirectory;
	unsigned long m_nCurrentDownloadFileSize;
	unsigned long m_nPreDownloadFileSize;
	int m_nThreadCount;
	string m_stringServerFilePath;
	int m_nSeconds;

public:
	void onTimer(Timer& timer); 
};
