#include "PhmsLogger.h"
#include "Util.h"
#include "AllObjectManagerUtil.h"
#include "Poco/Logger.h"
#include "Poco/FileChannel.h"
#include "Poco/Message.h"
#include "Poco/AutoPtr.h"
#include "Poco/File.h"
#include "Poco/DateTime.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/Timestamp.h"
#include "Poco/Timezone.h"
#include "Poco/Timespan.h"
#include "Poco/Exception.h"
#include <stdio.h>
#include <sstream>
#include <iostream>

using Poco::Logger;
using Poco::FileChannel;
using Poco::AutoPtr;
using Poco::File;
using Poco::Message;
using Poco::Timestamp;
using Poco::DateTime;
using Poco::DateTimeFormatter;
using Poco::Timestamp;
using Poco::Timezone;
using Poco::Timespan;
using Poco::Exception;
using std::ostringstream;


CPhmsLogger* CPhmsLogger::m_pPhmsLogger = NULL;
Mutex CPhmsLogger::m_mutex;

CPhmsLogger::CPhmsLogger(void)
{
	try
	{
		string stringWorkingDir = CUtil::GetCurrentAppDir();
		File fileDir(stringWorkingDir+"log/NET_LOG/", "gbk");
		fileDir.createDirectories();
		File fileLog(stringWorkingDir+"log/NET_LOG/log_phms_netlib.log", "gbk");
		fileLog.createFile();
		AutoPtr<FileChannel> pFileChannel = new FileChannel(fileLog.path());
#if defined(POCO_OS_FAMILY_UNIX)
		pFileChannel->setProperty(FileChannel::PROP_ROTATION, "1 M");
#else
		pFileChannel->setProperty(FileChannel::PROP_ROTATION, "1 days");
#endif
		pFileChannel->setProperty(FileChannel::PROP_ARCHIVE, "timestamp");
		pFileChannel->setProperty(FileChannel::PROP_COMPRESS, "false");
#if defined(POCO_OS_FAMILY_UNIX)
		pFileChannel->setProperty(FileChannel::PROP_PURGECOUNT, "2");
#else
		pFileChannel->setProperty(FileChannel::PROP_PURGECOUNT, "30");
#endif
		pFileChannel->setProperty(FileChannel::PROP_TIMES, "utc");
		Logger& logPhms = Logger::get("PHMS.NETLIB");
		logPhms.setChannel(pFileChannel);
		logPhms.setLevel(Message::PRIO_ERROR);
	}
	catch(Exception& e)
	{
		std::cout<<"PhmsHttp:"<<e.what()<<"	File:"<<__FILE__<<"	Line:"<<__LINE__<<std::endl;
		std::cout<<"PhmsHttp:"<<CUtil::GetCurrentAppDir()<<"	File:"<<__FILE__<<"	Line:"<<__LINE__<<std::endl;
	}
	catch(...)
	{

	}
}

CPhmsLogger::~CPhmsLogger(void)
{
	Logger::shutdown();
}


CPhmsLogger* CPhmsLogger::GetPhmsLogger()
{
	Mutex::ScopedLock lock(m_mutex);
	if(CPhmsLogger::m_pPhmsLogger == NULL)
	{
		m_pPhmsLogger = new CPhmsLogger;
	}
	return m_pPhmsLogger;
}

void CPhmsLogger::ReleasePhmsLogger()
{
	//因为在CAllObjectManagerUtil::ReleasePhmsLogger中已经有锁控制了，所以此处不再用m_mutex控制
	if(m_pPhmsLogger != NULL)
	{
		delete m_pPhmsLogger;
		m_pPhmsLogger = NULL;
	}
}


void CPhmsLogger::WriteLog(string stringText, const char* stringFunction, const char* stringFile, int nLine)
{
	try
	{
		Logger& logPhms = Logger::get("PHMS.NETLIB");
		DateTime dt;
		dt += Timespan(Timezone::utcOffset(), 0);
		string stringDateTime = DateTimeFormatter::format(dt, "%Y-%m-%d %H:%M:%S");
		string stringSourceFile = "source file: ";
		stringSourceFile += stringFile;
		ostringstream oStringStream;
		oStringStream<<"line: "<<nLine;
		string stringNLine = oStringStream.str();
		Message logMessage("PHMS.NETLIB", stringDateTime+"---"+stringText+"---"+stringFunction+"---"+stringSourceFile+"---"+stringNLine, Message::PRIO_ERROR);
		logPhms.log(logMessage);
	}
	catch(Exception&)
	{

	}
}


void CPhmsLogger::WriteLog(const Exception& e, const char* stringFunction, const char* stringFile, int nLine)
{
	try
	{
		Logger& logPhms = Logger::get("PHMS.NETLIB");
		string stringException;
		stringException = e.displayText();
		DateTime dt;
		dt += Timespan(Timezone::utcOffset(), 0);
		string stringDateTime = DateTimeFormatter::format(dt, "%Y-%m-%d %H:%M:%S");
		string stringSourceFile = "source file: ";
		stringSourceFile += stringFile;
		ostringstream oStringStream;
		oStringStream<<"line: "<<nLine;
		string stringNLine = oStringStream.str();
		Message logMessage("PHMS.NETLIB", stringDateTime+"---"+stringException+"---"+stringFunction+"---"+stringSourceFile+"---"+stringNLine, Message::PRIO_ERROR);
		logPhms.log(logMessage);
	}
	catch(Exception&)
	{

	}
}
