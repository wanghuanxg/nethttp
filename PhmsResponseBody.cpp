#include "PhmsResponseBody.h"
#include "Poco/StreamCopier.h"
#include "Poco/Buffer.h"
#include <fstream>
#include <sstream>
#include "ErrorCode.h"
#include "Poco/Exception.h"
#include "Poco/File.h"
#include "PhmsSession.h"
#include "PhmsLogger.h"
#include "Util.h"

using Poco::StreamCopier;
using Poco::Buffer;
using Poco::Exception;
using Poco::TimeoutException;
using Poco::File;


CPhmsResponseBody::CPhmsResponseBody(void):m_pInputStream(NULL), m_pPhmsSession(NULL)
{
}


CPhmsResponseBody::CPhmsResponseBody(istream* pInputStream, void* pPhmsSession)
:m_pInputStream(pInputStream), m_pPhmsSession(pPhmsSession)
{
}

CPhmsResponseBody::CPhmsResponseBody(CPhmsResponseBody& phmsResponseBody)
{
	this->m_pInputStream = phmsResponseBody.GetInputStreamPointer();
	this->m_pPhmsSession = phmsResponseBody.GetPhmsSessionPointer();
}

CPhmsResponseBody& CPhmsResponseBody::operator=(CPhmsResponseBody& phmsResponseBody)
{
	this->m_pInputStream = phmsResponseBody.GetInputStreamPointer();
	this->m_pPhmsSession = phmsResponseBody.GetPhmsSessionPointer();
	return *this;
}

CPhmsResponseBody::~CPhmsResponseBody(void)
{
}


//Getter
istream* CPhmsResponseBody::GetInputStreamPointer()
{
	return this->m_pInputStream;
}
void* CPhmsResponseBody::GetPhmsSessionPointer()
{
	return this->m_pPhmsSession;
}
//Setter
void CPhmsResponseBody::SetInputStreamPointer(istream* pInputStream)
{
	this->m_pInputStream = pInputStream;
}
void CPhmsResponseBody::SetPhmsSessionPointer(void* pPhmsSession)
{
	this->m_pPhmsSession = pPhmsSession;
}

int CPhmsResponseBody::InputStreamToString(string& stringContent)
{
	//流形式实现,此处用string输出流可能有问题，因为用户可能将二进制数据也保存到buffer，用string流可能会出问题。
	//应该使用Poco::MemoryIOS
	//不过目前系统中暂时没有这么使用的，有这么用的再说
	ostringstream oStringStream;
	oStringStream.exceptions(ios::failbit|ios::badbit);
	m_pInputStream->exceptions(ios::badbit);
	try
	{
		StreamCopier::copyStream(*m_pInputStream, oStringStream);
	}
	catch(const std::ios::failure& e)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(e.what(), __FUNCTION__, __FILE__, __LINE__);
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_INPUT_STREAM_FAIL), __FUNCTION__, __FILE__, __LINE__);
		return COMMON_INPUT_STREAM_FAIL;
	}
	catch(const TimeoutException& e)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(e, __FUNCTION__, __FILE__, __LINE__);
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_RECV_TIMEOUT), __FUNCTION__, __FILE__, __LINE__);
		return COMMON_RECV_TIMEOUT;
	}
	catch(const Exception& e)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(e, __FUNCTION__, __FILE__, __LINE__);
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_RECV_FAIL), __FUNCTION__, __FILE__, __LINE__);
		return COMMON_RECV_FAIL;
	}
	stringContent = oStringStream.str();
	return PHMS_SUCCESSFUL_RESULT;
}
int CPhmsResponseBody::InputStreamToFile(string& stringFilePath, bool bFileClearBeforeWrite)
{
	try
	{
		File fileTarget(stringFilePath, "gbk");
		fileTarget.createFile();
	}
	catch(Exception& e)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(e, __FUNCTION__, __FILE__, __LINE__);
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_FILE_OPEN_FAIL), __FUNCTION__, __FILE__, __LINE__);
		return COMMON_FILE_OPEN_FAIL;
	}
	ofstream ofLocalTargetFile;
	ofLocalTargetFile.exceptions(ios::failbit|ios::badbit);
	m_pInputStream->exceptions(ios::goodbit);
	try
	{
		if(bFileClearBeforeWrite)
		{
			ofLocalTargetFile.open(stringFilePath.c_str(), ios_base::trunc|ios_base::binary);
		}
		else
		{
			//locale localePre = ofLocalTargetFile.imbue(locale(""));//imbue不起作用
			ofLocalTargetFile.open(stringFilePath.c_str(), ios_base::app|ios_base::binary);
			//ofLocalTargetFile.imbue(localePre);
		}
	}
	catch(const ios::failure& error)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(error.what(), __FUNCTION__, __FILE__, __LINE__);
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_FILE_OPEN_FAIL), __FUNCTION__, __FILE__, __LINE__);
		return COMMON_FILE_OPEN_FAIL;
	}
	m_pInputStream->exceptions(ios::badbit);
	ofLocalTargetFile.exceptions(ios::failbit|ios::badbit);
	try
	{
		StreamCopier::copyStream(*m_pInputStream, ofLocalTargetFile);
	}
	catch(const std::ios::failure& e)
	{
		ofLocalTargetFile.close();
		CPhmsLogger::GetPhmsLogger()->WriteLog(e.what(), __FUNCTION__, __FILE__, __LINE__);
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_INPUT_STREAM_FAIL), __FUNCTION__, __FILE__, __LINE__);
		return COMMON_INPUT_STREAM_FAIL;
	}
	catch(const TimeoutException& e)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(e, __FUNCTION__, __FILE__, __LINE__);
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_RECV_TIMEOUT), __FUNCTION__, __FILE__, __LINE__);
		return COMMON_RECV_TIMEOUT;
	}
	catch(const Exception& e)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(e, __FUNCTION__, __FILE__, __LINE__);
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_RECV_FAIL), __FUNCTION__, __FILE__, __LINE__);
		return COMMON_RECV_FAIL;
	}
	try
	{
		ofLocalTargetFile.flush();
	}
	catch(const ios::failure& error)
	{
		//写日志
		ofLocalTargetFile.close();
		CPhmsLogger::GetPhmsLogger()->WriteLog(error.what(), __FUNCTION__, __FILE__, __LINE__);
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_FILE_WRITE_FAIL), __FUNCTION__, __FILE__, __LINE__);
		return COMMON_FILE_WRITE_FAIL;
	}
	ofLocalTargetFile.close();
	return PHMS_SUCCESSFUL_RESULT;
}