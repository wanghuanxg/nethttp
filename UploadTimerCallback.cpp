#include "UploadTimerCallback.h"
#include "Util.h"
#include "Poco/Util/IniFileConfiguration.h"
#include "Poco/AutoPtr.h"

using Poco::Util::IniFileConfiguration;
using Poco::AutoPtr;


CUploadTimerCallback::CUploadTimerCallback(pProgressAndSpeedCallback pCallback, string stringIniFilePath, unsigned long nAlreadyUploadSize, unsigned long nTotalSize, string stringLocalFilePath)
:m_pCallback(pCallback), m_stringIniFilePath(stringIniFilePath), m_nCurrentUploadFileSize(nAlreadyUploadSize), 
 m_nPreUploadFileSize(nAlreadyUploadSize), m_nTotalSize(nTotalSize), m_stringLocalFilePath(stringLocalFilePath), m_nSeconds(0)
{
}

CUploadTimerCallback::~CUploadTimerCallback(void)
{
}


void CUploadTimerCallback::onTimer(Timer& timer)
{
	m_nSeconds++;
	AutoPtr<IniFileConfiguration> iniFile = new IniFileConfiguration(m_stringIniFilePath, "gbk");
	m_nCurrentUploadFileSize = CUtil::StringToULong(iniFile->getString("ALREADY_UPLOAD_SIZE", "0"));
	//计算进度
	double dProgress = 0.0;
	if(m_nCurrentUploadFileSize <= m_nTotalSize)
	{
		dProgress = ((double)(m_nCurrentUploadFileSize))/m_nTotalSize*100;
	}
	else
	{
		dProgress = 100.0;
	}
	//计算速度
	static double dSpeed = 0.0;
	long nInterval = 1000;
// 	if(timer.getPeriodicInterval() == 0)
// 	{
// 		nInterval = 1000;
// 	}
// 	else
// 	{
// 		nInterval = timer.getPeriodicInterval();
// 	}
	unsigned long nTimeCount = m_nSeconds*nInterval/1000;
	if(m_nCurrentUploadFileSize>m_nPreUploadFileSize && nTimeCount!=0)
	{
		dSpeed = (m_nCurrentUploadFileSize-m_nPreUploadFileSize)/nTimeCount;
		m_nPreUploadFileSize = m_nCurrentUploadFileSize;
		m_nSeconds = 0;
	}
	if(m_pCallback != NULL)
	{
		m_pCallback(dProgress, dSpeed, "", m_stringLocalFilePath.c_str());
	}
	return;
}
