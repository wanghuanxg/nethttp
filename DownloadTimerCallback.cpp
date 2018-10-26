#include "DownloadTimerCallback.h"
#include "Poco/File.h"
#include "Poco/Exception.h"
#include <vector>
#include "Util.h"
#include "PhmsLogger.h"
#include "ErrorCode.h"

using namespace std;
using Poco::File;
using Poco::Exception;

CDownloadTimerCallback::CDownloadTimerCallback(pProgressAndSpeedCallback pCallback, unsigned long nFileSize, string stringLocalFilePath, 
					   string stringTempDirectory, unsigned long nAlreadyDownloadFileSize, int nThreadCount, string stringServerFilePath)
					   :m_pCallback(pCallback), m_nFileSize(nFileSize), m_stringLocalFilePath(stringLocalFilePath), 
						m_stringTempDirectory(stringTempDirectory), m_nCurrentDownloadFileSize(nAlreadyDownloadFileSize), 
						m_nPreDownloadFileSize(nAlreadyDownloadFileSize),m_nThreadCount(nThreadCount), m_stringServerFilePath(stringServerFilePath), m_nSeconds(0)
{
}

CDownloadTimerCallback::~CDownloadTimerCallback(void)
{
}
void CDownloadTimerCallback::onTimer(Timer& timer)
{
	m_nSeconds++;
	File fileLocalFile(m_stringLocalFilePath, "gbk");
	try
	{
		fileLocalFile.createFile();
		m_nCurrentDownloadFileSize = (unsigned long)fileLocalFile.getSize();
	}
	catch(const Exception& e)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(e, __FUNCTION__, __FILE__, __LINE__);
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_FILE_OPEN_FAIL), __FUNCTION__, __FILE__, __LINE__);
		m_nCurrentDownloadFileSize = 0;
	}
	
	File fileTempDirectory(m_stringTempDirectory, "gbk");
	vector<File> vTempFile;
	vector<File>::const_iterator iter;
	try
	{
		fileTempDirectory.list(vTempFile);
	}
	catch(const Exception&)
	{
		vTempFile.clear();
	}
	for(int nThreadNum=0; nThreadNum<m_nThreadCount; nThreadNum++)
	{
		for(iter=vTempFile.begin(); iter!=vTempFile.end(); iter++)
		{
			if(iter->path().find("PHMS_TEMP_"+CUtil::UIntToString(nThreadNum)) != string::npos)
			{
				try
				{
					m_nCurrentDownloadFileSize += (unsigned long)iter->getSize();
				}
				catch(const Exception&)
				{
					m_nCurrentDownloadFileSize += 0;
				}
				break;
			}
		}
	}
	//计算进度
	double dProgress = 0.0;
	if(m_nCurrentDownloadFileSize <= m_nFileSize)
	{
		dProgress = ((double)(m_nCurrentDownloadFileSize))/m_nFileSize*100;
	}
	else
	{
		dProgress = 100.0;
	}
	//计算速度
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
	double dSpeed = 0.0;
	if(nTimeCount != 0)
	{
		if(m_nCurrentDownloadFileSize<m_nPreDownloadFileSize)
		{
			dSpeed = 0.0;
		}
		else
		{
			dSpeed = (m_nCurrentDownloadFileSize-m_nPreDownloadFileSize)/nTimeCount;
		}
		m_nPreDownloadFileSize = m_nCurrentDownloadFileSize;
	}
	if(m_pCallback != NULL)
	{
		(*m_pCallback)(dProgress, dSpeed, m_stringServerFilePath.c_str(), m_stringLocalFilePath.c_str());
	}
	m_nSeconds = 0;
	return;
}
