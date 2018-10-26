#include "HttpSession.h"
#include "Poco/Net/MediaType.h"
#include "Poco/Exception.h"
#include "Poco/Net/MultipartWriter.h"
#include "Poco/Net/NameValueCollection.h"
#include "Poco/Net/HTTPCookie.h"
#include "ErrorCode.h"
#include <fstream>
#include "Poco/Buffer.h"
#include "Poco/Timespan.h"
#include "Poco/AutoPtr.h"
#include "Poco/Util/IniFileConfiguration.h"
#include "PhmsLogger.h"
#include "Util.h"
#include "GlobalTerminalLogoString.h"
#include <sstream>

using Poco::Exception;
using Poco::TimeoutException;
using Poco::Net::MultipartWriter;
using Poco::Net::MediaType;
using Poco::Net::MessageHeader;
using Poco::Buffer;
using Poco::Timespan;
using Poco::Net::NameValueCollection;
using Poco::Net::HTTPCookie;
using Poco::AutoPtr;
using Poco::Util::IniFileConfiguration;


using std::ifstream;

HTTPCookie g_cookieGuanXinSessionId;

CHttpSession::CHttpSession(void):m_bSsl(false)
{
	m_pSession = new HTTPClientSession();
	m_pContext = NULL;
}

CHttpSession::CHttpSession(bool bSsl):m_bSsl(bSsl)
{
	if(!m_bSsl)
	{
		m_pSession = new HTTPClientSession();
		m_pContext = NULL;
	}
	else
	{
		//不知道为什么调用Poco::Net::initializeSSL()不起作用，无参数的HTTPSClientSession构造函数报错，必须先创建一个Context
		//然后调用传入Context::Ptr类型的HTTPSClientSession构造函数，以后有时间在研究
		//不必考虑ssl初始化函数重入，Poco::Crypto::OpenSSLInitializer::initialize()函数做了互斥
		m_pContext = new Context(Context::CLIENT_USE, "", "", "", Context::VERIFY_NONE, 9, false);
		m_pSession = new HTTPSClientSession(m_pContext);
	}
}

CHttpSession::CHttpSession(CNetConfig netConfig):m_bSsl(netConfig.GetSsl())
{
	if(!m_bSsl)
	{
		m_pSession = new HTTPClientSession();
		m_pContext = NULL;
	}
	else
	{
		m_pContext = new Context(Context::CLIENT_USE, "", "", "", Context::VERIFY_NONE, 9, false);
		m_pSession = new HTTPSClientSession(m_pContext);
	}
}

CHttpSession::~CHttpSession(void)
{
	delete m_pSession;
}


//Getter
bool CHttpSession::GetSsl()
{
	return m_bSsl;
}
HTTPClientSession* CHttpSession::GetHttpClientSession()
{
	return m_pSession;
}


int CHttpSession::ExecuteHttpSession(CNetConfig& netConfig, CPhmsRequest& phmsRequest, istream* & pInputStream)
{
	//if(m_pSession->connected())
	{
		m_pSession->release();
	}
	int nRet = PHMS_SUCCESSFUL_RESULT;
	nRet = this->SetNetWork(netConfig);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	m_pSession->setTimeout(Timespan(1000000*60));//设置接收超时1分钟
	HTTPRequest httpRequest;
	string stringBoundary = MultipartWriter::createBoundary();
	this->GenerateHttpRequest(netConfig, phmsRequest, stringBoundary, httpRequest);
	try
	{
		ostream &requestStream = m_pSession->sendRequest(httpRequest);
		netConfig.SetIp(m_pSession->getIp());
		nRet = this->SendPhms(requestStream, stringBoundary, phmsRequest);
		if(nRet != PHMS_SUCCESSFUL_RESULT)
		{
			//写日志
			CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
			return nRet;
		}
	}
	catch(TimeoutException &e)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(e, __FUNCTION__, __FILE__, __LINE__);
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_SEND_TIMEOUT), __FUNCTION__, __FILE__, __LINE__);
		return COMMON_SEND_TIMEOUT;
	}
	catch(Exception& e)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(e, __FUNCTION__, __FILE__, __LINE__);
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_SEND_FAIL), __FUNCTION__, __FILE__, __LINE__);
		return COMMON_SEND_FAIL;
	}
	HTTPResponse httpResponse;
	try
	{
		istream& responseStream = m_pSession->receiveResponse(httpResponse);
		nRet = HandleHttpResponse(httpResponse);
		if(nRet != PHMS_SUCCESSFUL_RESULT)
		{
			//写日志
			CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
			CPhmsLogger::GetPhmsLogger()->WriteLog(httpResponse.getReason(), __FUNCTION__, __FILE__, __LINE__);
			return nRet;
		}
		pInputStream = &responseStream;
	}
	catch(TimeoutException& e)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(e, __FUNCTION__, __FILE__, __LINE__);
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_RECV_TIMEOUT), __FUNCTION__, __FILE__, __LINE__);
		return COMMON_RECV_TIMEOUT;
	}
	catch(Exception& e)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(e, __FUNCTION__, __FILE__, __LINE__);
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_RECV_FAIL), __FUNCTION__, __FILE__, __LINE__);
		return COMMON_RECV_FAIL;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CHttpSession::ExecuteNoPhmsHttpSession(CNetConfig& netConfig, istream* & pInputStream, int bContinue, string stringLocalFilePath)
{
	//if(m_pSession->connected())
	{
		m_pSession->release();
	}
	int nRet = PHMS_SUCCESSFUL_RESULT;
	nRet = this->SetNetWork(netConfig);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	m_pSession->setTimeout(Timespan(1000000*60));//设置接收超时1分钟
	HTTPRequest httpRequest;
	this->GenerateNoPhmsHttpRequest(netConfig, httpRequest, bContinue, stringLocalFilePath);
	
	try
	{
		ostream &requestStream = m_pSession->sendRequest(httpRequest);
	}
	catch(TimeoutException &e)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(e, __FUNCTION__, __FILE__, __LINE__);
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_SEND_TIMEOUT), __FUNCTION__, __FILE__, __LINE__);
		return COMMON_SEND_TIMEOUT;
	}
	catch(Exception& e)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(e, __FUNCTION__, __FILE__, __LINE__);
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_SEND_FAIL), __FUNCTION__, __FILE__, __LINE__);
		return COMMON_SEND_FAIL;
	}
	HTTPResponse httpResponse;
	try
	{
		istream& responseStream = m_pSession->receiveResponse(httpResponse);
		nRet = HandleHttpResponse(httpResponse);
		if(nRet != PHMS_SUCCESSFUL_RESULT)
		{
			//写日志
			CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
			CPhmsLogger::GetPhmsLogger()->WriteLog(httpResponse.getReason(), __FUNCTION__, __FILE__, __LINE__);
			return nRet;
		}
		pInputStream = &responseStream;
	}
	catch(TimeoutException& e)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(e, __FUNCTION__, __FILE__, __LINE__);
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_RECV_TIMEOUT), __FUNCTION__, __FILE__, __LINE__);
		return COMMON_RECV_TIMEOUT;
	}
	catch(Exception& e)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(e, __FUNCTION__, __FILE__, __LINE__);
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_RECV_FAIL), __FUNCTION__, __FILE__, __LINE__);
		return COMMON_RECV_FAIL;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int	CHttpSession::ExecuteFormHttpSession(CNetConfig& netConfig, map<string, string> mapParam, istream* & pInputStream)
{
	//if(m_pSession->connected())
	{
		m_pSession->release();
	}
	int nRet = PHMS_SUCCESSFUL_RESULT;
	nRet = this->SetNetWork(netConfig);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	m_pSession->setTimeout(Timespan(1000000*60));//设置接收超时1分钟
	HTTPRequest httpRequest;
	string stringParam;
	this->GenerateFormHttpRequest(netConfig, mapParam, stringParam, httpRequest);
	if(netConfig.GetUrl().find("/chss/web/")!=string::npos || netConfig.GetUrl().find("/ytj/execute/")!=string::npos)
	{
		if(g_cookieGuanXinSessionId.getName().size() != 0)
		{
			//存在cookie：JSESSIONID=92ACC4FFBD1F1148C515DC3C2AA20667; Path=/auth-service
			NameValueCollection nameValueCollection;
			nameValueCollection.add(g_cookieGuanXinSessionId.getName(), g_cookieGuanXinSessionId.getValue());
			httpRequest.setCookies(nameValueCollection);
		}
	}
	try
	{
		ostream &requestStream = m_pSession->sendRequest(httpRequest);
		requestStream<<stringParam;
	}
	catch(TimeoutException &e)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(e, __FUNCTION__, __FILE__, __LINE__);
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_SEND_TIMEOUT), __FUNCTION__, __FILE__, __LINE__);
		return COMMON_SEND_TIMEOUT;
	}
	catch(Exception& e)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(e, __FUNCTION__, __FILE__, __LINE__);
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_SEND_FAIL), __FUNCTION__, __FILE__, __LINE__);
		return COMMON_SEND_FAIL;
	}
	HTTPResponse httpResponse;
	try
	{
		istream& responseStream = m_pSession->receiveResponse(httpResponse);
		nRet = HandleHttpResponse(httpResponse);
		if(nRet != PHMS_SUCCESSFUL_RESULT)
		{
			//写日志
			CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
			CPhmsLogger::GetPhmsLogger()->WriteLog(httpResponse.getReason(), __FUNCTION__, __FILE__, __LINE__);
			return nRet;
		}
		if(netConfig.GetUrl().find("/chss/web/ytjlogin") != string::npos)
		{
			vector<HTTPCookie> vCookie;
			httpResponse.getCookies(vCookie);
			if(vCookie.size() != 0)
			{
				g_cookieGuanXinSessionId = vCookie[0];
			}
		}
		pInputStream = &responseStream;
	}
	catch(TimeoutException& e)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(e, __FUNCTION__, __FILE__, __LINE__);
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_RECV_TIMEOUT), __FUNCTION__, __FILE__, __LINE__);
		return COMMON_RECV_TIMEOUT;
	}
	catch(Exception& e)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(e, __FUNCTION__, __FILE__, __LINE__);
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_RECV_FAIL), __FUNCTION__, __FILE__, __LINE__);
		return COMMON_RECV_FAIL;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int	CHttpSession::ExecuteFormHttpSession(CNetConfig& netConfig, map<string, string> mapParam, string stringLocalFilePath, istream* & pInputStream)
{
	//if(m_pSession->connected())
	{
		m_pSession->release();
	}
	int nRet = PHMS_SUCCESSFUL_RESULT;
	nRet = this->SetNetWork(netConfig);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	m_pSession->setTimeout(Timespan(1000000*60));//设置接收超时1分钟
	HTTPRequest httpRequest;
	string stringBoundary = MultipartWriter::createBoundary();
	this->GenerateFormHttpRequest(netConfig, mapParam, stringLocalFilePath, stringBoundary, httpRequest);
	if(netConfig.GetUrl().find("/chss/web/")!=string::npos || netConfig.GetUrl().find("/ytj/execute/")!=string::npos)
	{
		if(g_cookieGuanXinSessionId.getName().size() != 0)
		{
			//存在cookie：JSESSIONID=92ACC4FFBD1F1148C515DC3C2AA20667; Path=/auth-service
			NameValueCollection nameValueCollection;
			nameValueCollection.add(g_cookieGuanXinSessionId.getName(), g_cookieGuanXinSessionId.getValue());
			httpRequest.setCookies(nameValueCollection);
		}
	}
	try
	{
		ostream &requestStream = m_pSession->sendRequest(httpRequest);
		//requestStream<<stringParam;
		nRet = SendPostMapAndFile(requestStream, stringBoundary, mapParam, stringLocalFilePath);
		if(nRet != PHMS_SUCCESSFUL_RESULT)
		{
			CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
			return nRet;
		}
	}
	catch(TimeoutException &e)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(e, __FUNCTION__, __FILE__, __LINE__);
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_SEND_TIMEOUT), __FUNCTION__, __FILE__, __LINE__);
		return COMMON_SEND_TIMEOUT;
	}
	catch(Exception& e)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(e, __FUNCTION__, __FILE__, __LINE__);
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_SEND_FAIL), __FUNCTION__, __FILE__, __LINE__);
		return COMMON_SEND_FAIL;
	}
	HTTPResponse httpResponse;
	try
	{
		istream& responseStream = m_pSession->receiveResponse(httpResponse);
		nRet = HandleHttpResponse(httpResponse);
		if(nRet != PHMS_SUCCESSFUL_RESULT)
		{
			//写日志
			CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
			CPhmsLogger::GetPhmsLogger()->WriteLog(httpResponse.getReason(), __FUNCTION__, __FILE__, __LINE__);
			return nRet;
		}
		if(netConfig.GetUrl().find("/chss/web/ytjlogin") != string::npos)
		{
			vector<HTTPCookie> vCookie;
			httpResponse.getCookies(vCookie);
			if(vCookie.size() != 0)
			{
				g_cookieGuanXinSessionId = vCookie[0];
			}
		}
		pInputStream = &responseStream;
	}
	catch(TimeoutException& e)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(e, __FUNCTION__, __FILE__, __LINE__);
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_RECV_TIMEOUT), __FUNCTION__, __FILE__, __LINE__);
		return COMMON_RECV_TIMEOUT;
	}
	catch(Exception& e)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(e, __FUNCTION__, __FILE__, __LINE__);
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_RECV_FAIL), __FUNCTION__, __FILE__, __LINE__);
		return COMMON_RECV_FAIL;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CHttpSession::ExeceteCommonPostSession(CNetConfig& netConfig, map<string, string> mapParam, string stringContentType, string stringHttpBody, istream* & pInputStream)
{
	//if(m_pSession->connected())
	{
		m_pSession->release();
	}
	int nRet = PHMS_SUCCESSFUL_RESULT;
	nRet = this->SetNetWork(netConfig);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	m_pSession->setTimeout(Timespan(1000000*60));//设置接收超时1分钟
	HTTPRequest httpRequest;
	this->GeneratePostHttpRequest(netConfig, mapParam, stringContentType, stringHttpBody, httpRequest);
	try
	{
		ostream &requestStream = m_pSession->sendRequest(httpRequest);
		requestStream<<stringHttpBody;
	}
	catch(TimeoutException &e)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(e, __FUNCTION__, __FILE__, __LINE__);
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_SEND_TIMEOUT), __FUNCTION__, __FILE__, __LINE__);
		return COMMON_SEND_TIMEOUT;
	}
	catch(Exception& e)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(e, __FUNCTION__, __FILE__, __LINE__);
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_SEND_FAIL), __FUNCTION__, __FILE__, __LINE__);
		return COMMON_SEND_FAIL;
	}
	HTTPResponse httpResponse;
	try
	{
		istream& responseStream = m_pSession->receiveResponse(httpResponse);
		nRet = HandleHttpResponse(httpResponse);
		if(nRet != PHMS_SUCCESSFUL_RESULT)
		{
			//写日志
			CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
			CPhmsLogger::GetPhmsLogger()->WriteLog(httpResponse.getReason(), __FUNCTION__, __FILE__, __LINE__);
			return nRet;
		}
		pInputStream = &responseStream;
	}
	catch(TimeoutException& e)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(e, __FUNCTION__, __FILE__, __LINE__);
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_RECV_TIMEOUT), __FUNCTION__, __FILE__, __LINE__);
		return COMMON_RECV_TIMEOUT;
	}
	catch(Exception& e)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(e, __FUNCTION__, __FILE__, __LINE__);
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_RECV_FAIL), __FUNCTION__, __FILE__, __LINE__);
		return COMMON_RECV_FAIL;
	}
	return PHMS_SUCCESSFUL_RESULT;
}


int CHttpSession::ExecuteXmlHttpSession(CNetConfig& netConfig, string stringXmlContent, istream* & pInputStream)
{
	//if(m_pSession->connected())
	{
		m_pSession->release();
	}
	int nRet = PHMS_SUCCESSFUL_RESULT;
	nRet = this->SetNetWork(netConfig);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	m_pSession->setTimeout(Timespan(1000000*60));//设置接收超时1分钟
	HTTPRequest httpRequest;
	string stringParam;
	this->GenerateXmlHttpRequest(netConfig, stringXmlContent, httpRequest);
	try
	{
		ostream &requestStream = m_pSession->sendRequest(httpRequest);
		requestStream<<stringXmlContent;
	}
	catch(TimeoutException &e)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(e, __FUNCTION__, __FILE__, __LINE__);
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_SEND_TIMEOUT), __FUNCTION__, __FILE__, __LINE__);
		return COMMON_SEND_TIMEOUT;
	}
	catch(Exception& e)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(e, __FUNCTION__, __FILE__, __LINE__);
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_SEND_FAIL), __FUNCTION__, __FILE__, __LINE__);
		return COMMON_SEND_FAIL;
	}
	HTTPResponse httpResponse;
	try
	{
		istream& responseStream = m_pSession->receiveResponse(httpResponse);
		nRet = HandleHttpResponse(httpResponse);
		if(nRet != PHMS_SUCCESSFUL_RESULT)
		{
			//写日志
			CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
			CPhmsLogger::GetPhmsLogger()->WriteLog(httpResponse.getReason(), __FUNCTION__, __FILE__, __LINE__);
			return nRet;
		}
		pInputStream = &responseStream;
	}
	catch(TimeoutException& e)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(e, __FUNCTION__, __FILE__, __LINE__);
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_RECV_TIMEOUT), __FUNCTION__, __FILE__, __LINE__);
		return COMMON_RECV_TIMEOUT;
	}
	catch(Exception& e)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(e, __FUNCTION__, __FILE__, __LINE__);
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_RECV_FAIL), __FUNCTION__, __FILE__, __LINE__);
		return COMMON_RECV_FAIL;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CHttpSession::ExeceteGetHttpSession(CNetConfig& netConfig, map<string, string> mapParam, istream* & pInputStream)
{
	//if(m_pSession->connected())
	{
		m_pSession->release();
	}
	int nRet = PHMS_SUCCESSFUL_RESULT;
	nRet = this->SetNetWork(netConfig);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	m_pSession->setTimeout(Timespan(1000000*60));//设置接收超时1分钟
	HTTPRequest httpRequest;
	string stringParam;
	this->GenerateGetHttpRequest(netConfig, mapParam, httpRequest);
	try
	{
		ostream &requestStream = m_pSession->sendRequest(httpRequest);
		//requestStream<<stringParam;
	}
	catch(TimeoutException &e)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(e, __FUNCTION__, __FILE__, __LINE__);
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_SEND_TIMEOUT), __FUNCTION__, __FILE__, __LINE__);
		return COMMON_SEND_TIMEOUT;
	}
	catch(Exception& e)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(e, __FUNCTION__, __FILE__, __LINE__);
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_SEND_FAIL), __FUNCTION__, __FILE__, __LINE__);
		return COMMON_SEND_FAIL;
	}
	HTTPResponse httpResponse;
	try
	{
		istream& responseStream = m_pSession->receiveResponse(httpResponse);
		nRet = HandleHttpResponse(httpResponse);
		if(nRet != PHMS_SUCCESSFUL_RESULT)
		{
			//写日志
			CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
			CPhmsLogger::GetPhmsLogger()->WriteLog(httpResponse.getReason(), __FUNCTION__, __FILE__, __LINE__);
			return nRet;
		}
		pInputStream = &responseStream;
	}
	catch(TimeoutException& e)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(e, __FUNCTION__, __FILE__, __LINE__);
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_RECV_TIMEOUT), __FUNCTION__, __FILE__, __LINE__);
		return COMMON_RECV_TIMEOUT;
	}
	catch(Exception& e)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(e, __FUNCTION__, __FILE__, __LINE__);
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_RECV_FAIL), __FUNCTION__, __FILE__, __LINE__);
		return COMMON_RECV_FAIL;
	}
	return PHMS_SUCCESSFUL_RESULT;
}



int CHttpSession::GetHttpExceptionInfo(int& nCode, string& stringMsgName, string& stringMsgText)
{
	const Exception* p = m_pSession->networkException();
	if(p != NULL)
	nCode = p->code();
	stringMsgName = p->name();
	stringMsgText = p->message();
	return PHMS_SUCCESSFUL_RESULT;
}

int CHttpSession::AbortSession()
{
	try
	{
		m_pSession->abort();
	}
	catch(Exception&)
	{
		//异常说明socket已经是无效的了，其效果等同于abort成功，所以异常捕获里无操作
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CHttpSession::SetNetWork(CNetConfig& netConfig)
{
	if(m_bSsl != netConfig.GetSsl())
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_SSL_CONFLICT_ERROR), __FUNCTION__, __FILE__, __LINE__);
		return COMMON_SSL_CONFLICT_ERROR;
	}
	if(netConfig.ValidatePort()==false || netConfig.ValidAddrAndUrl()==false)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_NET_CONFIG_ERROR), __FUNCTION__, __FILE__, __LINE__);
		return COMMON_NET_CONFIG_ERROR;
	}
	if(netConfig.GetPrpxyType() != NO_PROXY)
	{
		if(netConfig.ValidateProxyInfo() == false)
		{
			CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_PROXY_INFO_ERROR), __FUNCTION__, __FILE__, __LINE__);
			return COMMON_PROXY_INFO_ERROR;
		}
	}
	m_pSession->setHost(netConfig.GetHost());
	m_pSession->setAddr(netConfig.GetAddr());
	m_pSession->setPort(netConfig.GetPort());
	if(netConfig.GetPrpxyType() == HTTP_PROXY)
	{
		m_pSession->setProxyType(HTTP_PROXY);
		m_pSession->setProxy(netConfig.GetProxyAddr(), netConfig.GetProxyPort());
		m_pSession->setProxyCredentials(netConfig.GetProxyUsername(), netConfig.GetProxyPassword());
	}
	if(netConfig.GetPrpxyType() == SOCK_PROXY)
	{
		m_pSession->setProxyType(SOCK_PROXY);
		m_pSession->setProxy(netConfig.GetProxyAddr(), netConfig.GetProxyPort());
		m_pSession->setProxyCredentials(netConfig.GetProxyUsername(), netConfig.GetProxyPassword());
	}
	return PHMS_SUCCESSFUL_RESULT;
}
int CHttpSession::GenerateHttpRequest(CNetConfig& netConfig, CPhmsRequest& phmsRequest, string stringBoundary, HTTPRequest& httpRequest)
{
	httpRequest.setVersion(HTTPMessage::HTTP_1_1);
	httpRequest.setMethod(HTTPRequest::HTTP_POST);
	httpRequest.setURI(netConfig.GetUrl());
	httpRequest.setKeepAlive(false);
	httpRequest.set("pragma", "no-cache");
	httpRequest.set("User-Agent", g_stringTerminalType);
	if(phmsRequest.GetPhmsRequestHead().GetOperationCode() != "1002")
	{
		NameValueCollection nameValueCollection;
		nameValueCollection.add("PHPSESSID", phmsRequest.GetPhmsRequestHead().GetSessionId());
		httpRequest.setCookies(nameValueCollection);
	}
	if(phmsRequest.GetFilePath().size() == 0)
	{
		//无附加文件
		MediaType mediaType("application", "x-www-form-urlencoded");
		mediaType.setParameter("charset", "GB2312");
		httpRequest.setContentType(mediaType);
		string stringParameterName = "msg=";
		httpRequest.setContentLength(phmsRequest.ToString().size()+stringParameterName.size());
	}
	else
	{
		//有附件文件
		MediaType mediaType("multipart", "form-data");
		mediaType.setParameter("boundary", stringBoundary);
		httpRequest.setContentType(mediaType);
		//由于当前使用的Nginx版本，不支持客户端的chunked模式请求，所以暂时不使用chunked模式请求
		//Nginx服务器到版本1.3.9才支持chunked模式的请求
		//服务器添加了支持chunked请求的模块2012-12-25
		httpRequest.setChunkedTransferEncoding(true);

		//没办法，只有先获得文件大小，计算content-length的值
// 		unsigned long nHttpBodyLen = 0;
// 		unsigned long nFileLen = 0;
// 		nFileLen = CUtil::GetFileSize(phmsRequest.GetFilePath());
// 		MediaType mediaType1("");
// 		MediaType mediaType2("");
// 		MessageHeader messageHeader1;
// 		MessageHeader messageHeader2;
// 		mediaType1.setType("form-data");
// 		mediaType1.setParameter("name", "msg");
// 		mediaType2.setType("form-data");
// 		mediaType2.setParameter("name", "file");
// 		mediaType2.setParameter("filename", phmsRequest.GetFilePath());
// 		messageHeader1.set("Content-Disposition", mediaType1.toString());
// 		messageHeader2.set("Content-Disposition", mediaType2.toString());
// 		ostringstream oStringStream1;
// 		ostringstream oStringStream2;
// 		messageHeader1.write(oStringStream1);
// 		messageHeader2.write(oStringStream2);
// 		string stringMessageHead1 = oStringStream1.str();
// 		string stringMessageHead2 = oStringStream2.str();
// 		nHttpBodyLen = nFileLen/*文件参数长度*/ + (stringBoundary.size()+2/*前缀--*/+2/*\r\n*/)*3/*分段长度*/+2/*最后一个stringBoundary多出来的--*/ + stringMessageHead1.size()+stringMessageHead2.size() + 
// 			phmsRequest.ToString().size()/*phms请求参数长度*/ + 2*2/*MessageHeader与内容之间的\r\n*/ + 2*2/*内容结束后的\r\n*/;
// 		httpRequest.setContentLength(nHttpBodyLen);
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CHttpSession::GenerateNoPhmsHttpRequest(CNetConfig& netConfig, HTTPRequest& httpRequest, int bContinue, string stringLocalFilePath)
{
	httpRequest.setVersion(HTTPMessage::HTTP_1_1);
	httpRequest.setMethod(HTTPRequest::HTTP_GET);
	httpRequest.setURI(netConfig.GetUrl());
	httpRequest.setKeepAlive(false);
	httpRequest.set("pragma", "no-cache");
	httpRequest.set("User-Agent", g_stringTerminalType);
	if(bContinue)
	{
		string stringLocalFileSize = CUtil::ULongToString(CUtil::GetFileSize(stringLocalFilePath));
		string stringRangeParam = "bytes=";
		stringRangeParam += stringLocalFileSize+"-";
		httpRequest.set("Range", stringRangeParam);
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CHttpSession::GenerateFormHttpRequest(CNetConfig& netConfig, map<string, string> mapParam, string& stringParam, HTTPRequest& httpRequest)
{
	httpRequest.setVersion(HTTPMessage::HTTP_1_1);
	httpRequest.setMethod(HTTPRequest::HTTP_POST);
	httpRequest.setURI(netConfig.GetUrl());
	httpRequest.setKeepAlive(false);
	httpRequest.set("pragma", "no-cache");
	httpRequest.set("User-Agent", g_stringTerminalType);
	
	//无附加文件
	MediaType mediaType("application", "x-www-form-urlencoded");
	//mediaType.setParameter("charset", "UTF8");
	httpRequest.setContentType(mediaType);
	map<string, string>::const_iterator iter;
	for(iter=mapParam.begin(); iter!=mapParam.end();)
	{
		stringParam += iter->first;
		stringParam += "=";
		stringParam += iter->second;
		if(++iter != mapParam.end())
		{
			stringParam += "&";
		}
	}
	httpRequest.setContentLength(stringParam.size());
	return PHMS_SUCCESSFUL_RESULT;
}

int CHttpSession::GenerateFormHttpRequest(CNetConfig& netConfig, map<string, string> mapParam, string stringLocalFilePath, string stringBoundary, HTTPRequest& httpRequest)
{
	httpRequest.setVersion(HTTPMessage::HTTP_1_1);
	httpRequest.setMethod(HTTPRequest::HTTP_POST);
	httpRequest.setURI(netConfig.GetUrl());
	httpRequest.setKeepAlive(false);
	httpRequest.set("pragma", "no-cache");
	httpRequest.set("User-Agent", g_stringTerminalType);
	unsigned long nFileLen = 0;
	if(stringLocalFilePath.size() != 0)
	{
		nFileLen = CUtil::GetFileSize(stringLocalFilePath);;
	}
	
	if(stringLocalFilePath.size()==0 || nFileLen==0)
	{
		//无附加文件
		string stringParam;
		MediaType mediaType("application", "x-www-form-urlencoded");
		//mediaType.setParameter("charset", "UTF8");
		httpRequest.setContentType(mediaType);
		map<string, string>::const_iterator iter;
		for(iter=mapParam.begin(); iter!=mapParam.end();)
		{
			stringParam += iter->first;
			stringParam += "=";
			stringParam += iter->second;
			if(++iter != mapParam.end())
			{
				stringParam += "&";
			}
		}
		httpRequest.setContentLength(stringParam.size());
	}
	else
	{
		//有附件文件
		MediaType mediaType("multipart", "form-data");
		mediaType.setParameter("boundary", stringBoundary);
		//mediaType.setParameter("charset", "UTF8");
		httpRequest.setContentType(mediaType);
		//由于不是所有的服务器都支持Chunked模式，所以此处选择Content-Length模式
		//httpRequest.setChunkedTransferEncoding(true);

		//没办法，只有先获得文件大小，计算content-length的值
		unsigned long nHttpBodyLen = 0;
		unsigned long nFileLen = 0;
		if(stringLocalFilePath.size() != 0)
		{
			nFileLen = CUtil::GetFileSize(stringLocalFilePath);
		}
		map<string, string>::iterator iter;
		for(iter=mapParam.begin(); iter!=mapParam.end(); iter++)
		{
			MediaType mediaType("");
			mediaType.setType("form-data");
			mediaType.setParameter("name", iter->first);
			MessageHeader messageHeader;
			messageHeader.set("Content-Disposition", mediaType.toString());
			ostringstream oStringStream;
			messageHeader.write(oStringStream);
			string stringMessageHead;
			stringMessageHead = oStringStream.str();
			nHttpBodyLen += stringBoundary.size()+2/*前缀--*/+2/*\r\n*/+stringMessageHead.size()+iter->second.size()+2/*MessageHeader与内容之间的\r\n*/+2/*内容结束后的\r\n*/;
		}
		MediaType mediaType2("");
		MessageHeader messageHeader2;
		mediaType2.setType("form-data");
		mediaType2.setParameter("name", "file");
		mediaType2.setParameter("filename", CUtil::GetFileNameFromPath(stringLocalFilePath));
		messageHeader2.set("Content-Disposition", mediaType2.toString());

		MediaType mediaType3("");
		mediaType3.setType("application");
		mediaType3.setSubType("zip");
		messageHeader2.set("Content-Type", mediaType3.toString());

		ostringstream oStringStream2;
		messageHeader2.write(oStringStream2);
		string stringMessageHead2 = oStringStream2.str();
		nHttpBodyLen += nFileLen+stringBoundary.size()+2/*前缀--*/+2/*\r\n*/+stringMessageHead2.size()+2/*MessageHeader与内容之间的\r\n*/+2/*内容结束后的\r\n*/;
		
		nHttpBodyLen += stringBoundary.size()+2/*前缀--*/+2/*\r\n*/+2/*最后一个stringBoundary多出来的--*/;
		//nHttpBodyLen = nFileLen/*文件参数长度*/ + (stringBoundary.size()+2/*前缀--*/+2/*\r\n*/)*3/*分段长度*/+2/*最后一个stringBoundary多出来的--*/ + stringMessageHead1.size()+stringMessageHead2.size() + 
		//phmsRequest.ToString().size()/*phms请求参数长度*/ + 2*2/*MessageHeader与内容之间的\r\n*/ + 2*2/*内容结束后的\r\n*/;
		httpRequest.setContentLength(nHttpBodyLen);
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CHttpSession::GeneratePostHttpRequest(CNetConfig& netConfig, map<string, string> mapParam, string stringContentType, string stringHttpBody, HTTPRequest& httpRequest)
{
	httpRequest.setVersion(HTTPMessage::HTTP_1_1);
	httpRequest.setMethod(HTTPRequest::HTTP_POST);
	httpRequest.setURI(netConfig.GetUrl());
	httpRequest.setKeepAlive(false);
	httpRequest.set("pragma", "no-cache");
	httpRequest.set("User-Agent", g_stringTerminalType);
	httpRequest.set("Content-Type", stringContentType);

	//无附加文件
	//MediaType mediaType("application", "x-www-form-urlencoded");
	//mediaType.setParameter("charset", "UTF8");
	//httpRequest.setContentType(mediaType);
	string stringParam;
	map<string, string>::const_iterator iter;
	for(iter=mapParam.begin(); iter!=mapParam.end();)
	{
		stringParam += iter->first;
		stringParam += "=";
		stringParam += iter->second;
		if(++iter != mapParam.end())
		{
			stringParam += "&";
		}
	}
	string stringUrl = netConfig.GetUrl();
	if(stringParam.size() != 0)
	{
		stringUrl += "?";
		stringUrl += stringParam;
	}
	httpRequest.setURI(stringUrl);
	httpRequest.setContentLength(stringHttpBody.size());
	return PHMS_SUCCESSFUL_RESULT;
}

int CHttpSession::GenerateXmlHttpRequest(CNetConfig& netConfig, string stringXmlContent, HTTPRequest& httpRequest)
{
	httpRequest.setVersion(HTTPMessage::HTTP_1_1);
	httpRequest.setMethod(HTTPRequest::HTTP_POST);
	httpRequest.setURI(netConfig.GetUrl());
	httpRequest.setKeepAlive(false);
	httpRequest.set("pragma", "no-cache");
	httpRequest.set("User-Agent", g_stringTerminalType);

	//无附加文件
	MediaType mediaType("text", "xml");
	string stringWorkingDir = CUtil::GetCurrentAppDir();
	string stringIniFilePath = stringWorkingDir+"PhmsConfig.ini";
	AutoPtr<IniFileConfiguration> iniFile = new IniFileConfiguration(stringIniFilePath, "gbk");
	string stringEncoding = iniFile->getString("OTHER.Encoding", "GBK");
	mediaType.setParameter("charset", stringEncoding);
	httpRequest.setContentType(mediaType);
	httpRequest.setContentLength(stringXmlContent.size());
	return PHMS_SUCCESSFUL_RESULT;
}

int CHttpSession::GenerateGetHttpRequest(CNetConfig& netConfig, map<string, string> mapParam, HTTPRequest& httpRequest)
{
	httpRequest.setVersion(HTTPMessage::HTTP_1_1);
	httpRequest.setMethod(HTTPRequest::HTTP_GET);
	
	httpRequest.setKeepAlive(false);
	httpRequest.set("pragma", "no-cache");
	httpRequest.set("User-Agent", g_stringTerminalType);

	string stringParam;
	map<string, string>::const_iterator iter;
	for(iter=mapParam.begin(); iter!=mapParam.end();)
	{
		stringParam += iter->first;
		stringParam += "=";
		stringParam += iter->second;
		if(++iter != mapParam.end())
		{
			stringParam += "&";
		}
	}
	string stringUrl = netConfig.GetUrl();
	if(stringParam.size() != 0)
	{
		stringUrl += "?";
		stringUrl += stringParam;
	}
	httpRequest.setURI(stringUrl);
	return PHMS_SUCCESSFUL_RESULT;
}


int CHttpSession::SendPhms(ostream& outputStream, string stringBoundary, CPhmsRequest& phmsRequest)
{
	if(phmsRequest.GetFilePath().size() == 0)
	{
		//无附加文件
		try
		{
			outputStream.exceptions(ios::failbit|ios::badbit);
			outputStream<<"msg="<<phmsRequest.ToString();
		}
		catch(const ios::failure& error)
		{
			//写日志
			int nCode = 0;
			string stringMsgName;
			string stringMsgText;
			this->GetHttpExceptionInfo(nCode, stringMsgName, stringMsgText);
			CPhmsLogger::GetPhmsLogger()->WriteLog(stringMsgText, __FUNCTION__, __FILE__, __LINE__);
			CPhmsLogger::GetPhmsLogger()->WriteLog(error.what(), __FUNCTION__, __FILE__, __LINE__);
			CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_OUTPUT_STREAM_FAIL), __FUNCTION__, __FILE__, __LINE__);
			return COMMON_OUTPUT_STREAM_FAIL;
		}
		catch(const TimeoutException& e)
		{
			CPhmsLogger::GetPhmsLogger()->WriteLog(e, __FUNCTION__, __FILE__, __LINE__);
			CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_SEND_TIMEOUT), __FUNCTION__, __FILE__, __LINE__);
			return COMMON_SEND_TIMEOUT;
		}
		catch(const Exception& e)
		{
			CPhmsLogger::GetPhmsLogger()->WriteLog(e, __FUNCTION__, __FILE__, __LINE__);
			CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_SEND_FAIL), __FUNCTION__, __FILE__, __LINE__);
			return COMMON_SEND_FAIL;
		}
	}
	else
	{
		//有附加文件
		MediaType mediaType1("");
		MediaType mediaType2("");
		MessageHeader messageHeader1;
		MessageHeader messageHeader2;
		mediaType1.setType("form-data");
		mediaType1.setParameter("name", "msg");
		mediaType2.setType("form-data");
		mediaType2.setParameter("name", "filename");
		mediaType2.setParameter("filename", phmsRequest.GetFilePath());
		messageHeader1.set("Content-Disposition", mediaType1.toString());
		messageHeader2.set("Content-Disposition", mediaType2.toString());
		try
		{
			MultipartWriter multipartWrite(outputStream, stringBoundary);
			multipartWrite.nextPart(messageHeader1);
			outputStream<<phmsRequest.ToString();
			multipartWrite.nextPart(messageHeader2);
			ifstream inputFileStream;
			inputFileStream.exceptions(ios::badbit);
			inputFileStream.open(phmsRequest.GetFilePath().c_str(), ios::in|ios::binary);
			if(inputFileStream.fail())
			{
				CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_FILE_OPEN_FAIL), __FUNCTION__, __FILE__, __LINE__);
				return COMMON_FILE_OPEN_FAIL;
			}
			outputStream.exceptions(ios::failbit|ios::badbit);
			while(!inputFileStream.eof())
			{
				Buffer<char> bufFileContent(1024);
				memset(bufFileContent.begin(), 0, 1024);
				inputFileStream.read(bufFileContent.begin(), 1024);
				outputStream.write(bufFileContent.begin(), inputFileStream.gcount());
			}
			multipartWrite.close();
			inputFileStream.close();
		}
		catch(const ios::failure& error)
		{
			//写日志
			if(outputStream.fail() || outputStream.bad())
			{
				int nCode = 0;
				string stringMsgName;
				string stringMsgText;
				this->GetHttpExceptionInfo(nCode, stringMsgName, stringMsgText);
				CPhmsLogger::GetPhmsLogger()->WriteLog(stringMsgText, __FUNCTION__, __FILE__, __LINE__);
			}
			CPhmsLogger::GetPhmsLogger()->WriteLog(error.what(), __FUNCTION__, __FILE__, __LINE__);
			CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_OUTPUT_STREAM_FAIL), __FUNCTION__, __FILE__, __LINE__);
			return COMMON_OUTPUT_STREAM_FAIL;
		}
		catch(const TimeoutException& e)
		{
			CPhmsLogger::GetPhmsLogger()->WriteLog(e, __FUNCTION__, __FILE__, __LINE__);
			CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_SEND_TIMEOUT), __FUNCTION__, __FILE__, __LINE__);
			return COMMON_SEND_TIMEOUT;
		}
		catch(const Exception& e)
		{
			CPhmsLogger::GetPhmsLogger()->WriteLog(e, __FUNCTION__, __FILE__, __LINE__);
			CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_SEND_FAIL), __FUNCTION__, __FILE__, __LINE__);
			return COMMON_SEND_FAIL;
		}
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CHttpSession::SendPostMapAndFile(ostream& outputStream, string stringBoundary, map<string, string> mapParam, string stringLocalFilePath)
{
	if(stringLocalFilePath.size()==0 || CUtil::GetFileSize(stringLocalFilePath)==0)
	{
		//无附加文件
		try
		{
			outputStream.exceptions(ios::failbit|ios::badbit);
			map<string, string>::iterator iter;
			int i=0;
			for(iter=mapParam.begin(); iter!=mapParam.end(); iter++, i++)
			{
				outputStream<<iter->first<<"="<<iter->second;
				if(i != mapParam.size()-1)
				{
					outputStream<<"&";
				}
			}
		}
		catch(const ios::failure& error)
		{
			//写日志
			int nCode = 0;
			string stringMsgName;
			string stringMsgText;
			this->GetHttpExceptionInfo(nCode, stringMsgName, stringMsgText);
			CPhmsLogger::GetPhmsLogger()->WriteLog(stringMsgText, __FUNCTION__, __FILE__, __LINE__);
			CPhmsLogger::GetPhmsLogger()->WriteLog(error.what(), __FUNCTION__, __FILE__, __LINE__);
			CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_OUTPUT_STREAM_FAIL), __FUNCTION__, __FILE__, __LINE__);
			return COMMON_OUTPUT_STREAM_FAIL;
		}
		catch(const TimeoutException& e)
		{
			CPhmsLogger::GetPhmsLogger()->WriteLog(e, __FUNCTION__, __FILE__, __LINE__);
			CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_SEND_TIMEOUT), __FUNCTION__, __FILE__, __LINE__);
			return COMMON_SEND_TIMEOUT;
		}
		catch(const Exception& e)
		{
			CPhmsLogger::GetPhmsLogger()->WriteLog(e, __FUNCTION__, __FILE__, __LINE__);
			CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_SEND_FAIL), __FUNCTION__, __FILE__, __LINE__);
			return COMMON_SEND_FAIL;
		}
	}
	else
	{
		//有附加文件
		MultipartWriter multipartWrite(outputStream, stringBoundary);
		int i=0;
		map<string, string>::iterator iter;
		for(iter=mapParam.begin(); iter!=mapParam.end(); iter++, i++)
		{
			MediaType mediaType1("");
			MessageHeader messageHeader1;
			mediaType1.setType("form-data");
			mediaType1.setParameter("name", iter->first);
			messageHeader1.set("Content-Disposition", mediaType1.toString());
			try
			{
				multipartWrite.nextPart(messageHeader1);
				outputStream<<iter->second;
			}
			catch(const TimeoutException& e)
			{
				CPhmsLogger::GetPhmsLogger()->WriteLog(e, __FUNCTION__, __FILE__, __LINE__);
				CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_SEND_TIMEOUT), __FUNCTION__, __FILE__, __LINE__);
				return COMMON_SEND_TIMEOUT;
			}
			catch(const Exception& e)
			{
				CPhmsLogger::GetPhmsLogger()->WriteLog(e, __FUNCTION__, __FILE__, __LINE__);
				CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_SEND_FAIL), __FUNCTION__, __FILE__, __LINE__);
				return COMMON_SEND_FAIL;
			}
		}

		MediaType mediaType2("");
		MediaType mediaType3("");
		MessageHeader messageHeader2;
		mediaType2.setType("form-data");
		mediaType2.setParameter("name", "file");
		mediaType2.setParameter("filename", CUtil::GetFileNameFromPath(stringLocalFilePath));
		mediaType3.setType("application");
		mediaType3.setSubType("zip");
		messageHeader2.set("Content-Disposition", mediaType2.toString());
		messageHeader2.set("Content-Type", mediaType3.toString());
		try
		{
			multipartWrite.nextPart(messageHeader2);
			ifstream inputFileStream;
			inputFileStream.exceptions(ios::badbit);
			inputFileStream.open(stringLocalFilePath.c_str(), ios::in|ios::binary);
			if(inputFileStream.fail())
			{
				CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_FILE_OPEN_FAIL), __FUNCTION__, __FILE__, __LINE__);
				return COMMON_FILE_OPEN_FAIL;
			}
			outputStream.exceptions(ios::failbit|ios::badbit);
			while(!inputFileStream.eof())
			{
				Buffer<char> bufFileContent(1024);
				memset(bufFileContent.begin(), 0, 1024);
				inputFileStream.read(bufFileContent.begin(), 1024);
				outputStream.write(bufFileContent.begin(), inputFileStream.gcount());
			}
			inputFileStream.close();
		}
		catch(const ios::failure& error)
		{
			//写日志
			if(outputStream.fail() || outputStream.bad())
			{
				int nCode = 0;
				string stringMsgName;
				string stringMsgText;
				this->GetHttpExceptionInfo(nCode, stringMsgName, stringMsgText);
				CPhmsLogger::GetPhmsLogger()->WriteLog(stringMsgText, __FUNCTION__, __FILE__, __LINE__);
			}
			CPhmsLogger::GetPhmsLogger()->WriteLog(error.what(), __FUNCTION__, __FILE__, __LINE__);
			CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_OUTPUT_STREAM_FAIL), __FUNCTION__, __FILE__, __LINE__);
			return COMMON_OUTPUT_STREAM_FAIL;
		}
		catch(const TimeoutException& e)
		{
			CPhmsLogger::GetPhmsLogger()->WriteLog(e, __FUNCTION__, __FILE__, __LINE__);
			CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_SEND_TIMEOUT), __FUNCTION__, __FILE__, __LINE__);
			return COMMON_SEND_TIMEOUT;
		}
		catch(const Exception& e)
		{
			CPhmsLogger::GetPhmsLogger()->WriteLog(e, __FUNCTION__, __FILE__, __LINE__);
			CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_SEND_FAIL), __FUNCTION__, __FILE__, __LINE__);
			return COMMON_SEND_FAIL;
		}

		multipartWrite.close();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CHttpSession::HandleHttpResponse(HTTPResponse& httpResponse)
{
	switch(httpResponse.getStatus())
	{
	case HTTPResponse::HTTP_OK:
		return PHMS_SUCCESSFUL_RESULT;
	case HTTPResponse::HTTP_NOT_FOUND:
	case HTTPResponse::HTTP_FORBIDDEN:
	case HTTPResponse::HTTP_UNAUTHORIZED:
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_URL_NOT_FOUND), __FUNCTION__, __FILE__, __LINE__);
		return COMMON_URL_NOT_FOUND;
	default:
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_RECV_CONTENT_FAIL), __FUNCTION__, __FILE__, __LINE__);
		return COMMON_RECV_CONTENT_FAIL;
	}
	return PHMS_SUCCESSFUL_RESULT;
}
