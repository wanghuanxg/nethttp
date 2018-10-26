#include "HttpPushSession.h"
#include "Poco/Net/MediaType.h"
#include "Poco/Exception.h"
#include "Poco/Net/MultipartWriter.h"
#include "Poco/Net/NameValueCollection.h"
#include "ErrorCode.h"
#include <fstream>
#include "Poco/Buffer.h"
#include "Poco/Timespan.h"
#include "PhmsLogger.h"
#include "Util.h"
#include "GlobalTerminalLogoString.h"
#include <sstream>
#include "PhmsResponse.h"

using Poco::Exception;
using Poco::TimeoutException;
using Poco::Net::MultipartWriter;
using Poco::Net::MediaType;
using Poco::Net::MessageHeader;
using Poco::Buffer;
using Poco::Timespan;
using Poco::Net::NameValueCollection;
using std::ifstream;

CHttpPushSession::CHttpPushSession(void):m_bSsl(false)
{
	m_pSession = new HTTPClientSession();
	m_pContext = NULL;
}

CHttpPushSession::CHttpPushSession(bool bSsl):m_bSsl(bSsl)
{
	if(!m_bSsl)
	{
		m_pSession = new HTTPClientSession();
		m_pContext = NULL;
	}
	else
	{
		//��֪��Ϊʲô����Poco::Net::initializeSSL()�������ã��޲�����HTTPSClientSession���캯�����������ȴ���һ��Context
		//Ȼ����ô���Context::Ptr���͵�HTTPSClientSession���캯�����Ժ���ʱ�����о�
		//���ؿ���ssl��ʼ���������룬Poco::Crypto::OpenSSLInitializer::initialize()�������˻���
		m_pContext = new Context(Context::CLIENT_USE, "", "", "", Context::VERIFY_NONE, 9, false);
		m_pSession = new HTTPSClientSession(m_pContext);
	}
}

CHttpPushSession::CHttpPushSession(CNetConfig netConfig):m_bSsl(netConfig.GetSsl())
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

CHttpPushSession::~CHttpPushSession(void)
{
	delete m_pSession;
	m_pSession = NULL;
}

//Getter
bool CHttpPushSession::GetSsl()
{
	return m_bSsl;
}
HTTPClientSession* CHttpPushSession::GetHttpClientSession()
{
	return m_pSession;
}


int CHttpPushSession::ExecuteHttpPushSession(CNetConfig& netConfig, CPhmsRequest& phmsRequest, pHttpPushCallback pCallback)
{
	//if(m_pSession->connected())
	{
		m_pSession->release();
	}
	int nRet = PHMS_SUCCESSFUL_RESULT;
	nRet = this->SetNetWork(netConfig);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//д��־
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	HTTPRequest httpRequest;
	string stringBoundary = MultipartWriter::createBoundary();
	this->GenerateHttpRequest(netConfig, phmsRequest, stringBoundary, httpRequest);
	try
	{
		m_pSession->setHttpPush();//��������http
		ostream &requestStream = m_pSession->sendRequest(httpRequest);
		nRet = this->SendPhms(requestStream, stringBoundary, phmsRequest);
		if(nRet != PHMS_SUCCESSFUL_RESULT)
		{
			//д��־
			CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
			return nRet;
		}
	}
	catch(TimeoutException &e)
	{
		//д��־
		CPhmsLogger::GetPhmsLogger()->WriteLog(e, __FUNCTION__, __FILE__, __LINE__);
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_SEND_TIMEOUT), __FUNCTION__, __FILE__, __LINE__);
		return COMMON_SEND_TIMEOUT;
	}
	catch(Exception& e)
	{
		//д��־
		CPhmsLogger::GetPhmsLogger()->WriteLog(e, __FUNCTION__, __FILE__, __LINE__);
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_SEND_FAIL), __FUNCTION__, __FILE__, __LINE__);
		return COMMON_SEND_FAIL;
	}
	HTTPResponse httpResponse;
	try
	{
		m_pSession->receiveResponseHead(httpResponse);
		nRet = HandleHttpResponse(httpResponse);
		if(nRet != PHMS_SUCCESSFUL_RESULT)
		{
			//д��־
			CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
			CPhmsLogger::GetPhmsLogger()->WriteLog(httpResponse.getReason(), __FUNCTION__, __FILE__, __LINE__);
			return nRet;
		}
		else
		{
			pCallback(NULL, 0);
		}
		int nChunkedLen = 0;
		do 
		{
			string stringChunkedContent;
			nChunkedLen = m_pSession->receiveResponseBody_PUSH(stringChunkedContent);
			//�ж�Phms��Ӧͷ
			if(nChunkedLen != 0)
			{
				CPhmsResponse phmsResponse;
				istringstream iPhmsResponse(stringChunkedContent);
				nRet = phmsResponse.SetPhmsResponse(&iPhmsResponse, this);
				if(nRet != PHMS_SUCCESSFUL_RESULT)
				{
					CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
					return nRet;
				}
				string stringXmlContent;
				string stringXmlPath;
				nRet = phmsResponse.GetResult(1, stringXmlContent, stringXmlPath);
				if(nRet != PHMS_SUCCESSFUL_RESULT)
				{
					CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
					return nRet;
				}
				if(pCallback != NULL)
				{
					pCallback(stringXmlContent.c_str(), (unsigned long)stringXmlContent.size());
				}
			}
		} while (nChunkedLen != 0);
	}
	//��timeout��ʱ��������recv���޵ȴ�
	catch(Exception& e)
	{
		//д��־
		CPhmsLogger::GetPhmsLogger()->WriteLog(e, __FUNCTION__, __FILE__, __LINE__);
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_RECV_FAIL), __FUNCTION__, __FILE__, __LINE__);
		return COMMON_RECV_FAIL;
	}
	return PHMS_SUCCESSFUL_RESULT;
}


int CHttpPushSession::GetHttpExceptionInfo(int& nCode, string& stringMsgName, string& stringMsgText)
{
	const Exception* p = m_pSession->networkException();
	if(p != NULL)
		nCode = p->code();
	stringMsgName = p->name();
	stringMsgText = p->message();
	return PHMS_SUCCESSFUL_RESULT;
}

int CHttpPushSession::abort()
{
	try
	{
		m_pSession->abort();
	}
	catch(Exception&)
	{
		//�쳣˵��socket�Ѿ�����Ч���ˣ���Ч����ͬ��abort�ɹ��������쳣�������޲���
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CHttpPushSession::SetNetWork(CNetConfig& netConfig)
{
	if(m_bSsl != netConfig.GetSsl())
	{
		//д��־
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_SSL_CONFLICT_ERROR), __FUNCTION__, __FILE__, __LINE__);
		return COMMON_SSL_CONFLICT_ERROR;
	}
	if(netConfig.ValidatePort()==false || netConfig.ValidAddrAndUrl()==false)
	{
		//д��־
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
int CHttpPushSession::GenerateHttpRequest(CNetConfig& netConfig, CPhmsRequest& phmsRequest, string stringBoundary, HTTPRequest& httpRequest)
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
		//�޸����ļ�
		MediaType mediaType("application", "x-www-form-urlencoded");
		mediaType.setParameter("charset", "GB2312");
		httpRequest.setContentType(mediaType);
		string stringParameterName = "msg=";
		httpRequest.setContentLength(phmsRequest.ToString().size()+stringParameterName.size());
	}
	else
	{
		//�и����ļ�
		MediaType mediaType("multipart", "form-data");
		mediaType.setParameter("boundary", stringBoundary);
		httpRequest.setContentType(mediaType);
		//���ڵ�ǰʹ�õ�Nginx�汾����֧�ֿͻ��˵�chunkedģʽ����������ʱ��ʹ��chunkedģʽ����
		//Nginx���������汾1.3.9��֧��chunkedģʽ������
		//�����������֧��chunked�����ģ��2012-12-25
		httpRequest.setChunkedTransferEncoding(true);
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CHttpPushSession::SendPhms(ostream& outputStream, string stringBoundary, CPhmsRequest& phmsRequest)
{
	if(phmsRequest.GetFilePath().size() == 0)
	{
		//�޸����ļ�
		try
		{
			outputStream.exceptions(ios::failbit|ios::badbit);
			outputStream<<"msg="<<phmsRequest.ToString();
		}
		catch(const ios::failure& error)
		{
			//д��־
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
		//�и����ļ�
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
			//д��־
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

int CHttpPushSession::HandleHttpResponse(HTTPResponse& httpResponse)
{
	switch(httpResponse.getStatus())
	{
	case HTTPResponse::HTTP_OK:
		return PHMS_SUCCESSFUL_RESULT;
	case HTTPResponse::HTTP_NOT_FOUND:
	case HTTPResponse::HTTP_FORBIDDEN:
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_URL_NOT_FOUND), __FUNCTION__, __FILE__, __LINE__);
		return COMMON_URL_NOT_FOUND;
	default:
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_RECV_CONTENT_FAIL), __FUNCTION__, __FILE__, __LINE__);
		return COMMON_RECV_CONTENT_FAIL;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

