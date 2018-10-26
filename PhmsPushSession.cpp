#include "PhmsPushSession.h"
#include "PhmsLogger.h"
#include "PhmsRequestHead.h"
#include "PhmsRequestBody.h"
#include "Util.h"
#include "ErrorCode.h"
#include "Poco/Exception.h"
#include "Poco/AutoPtr.h"
#include "Poco/Util/IniFileConfiguration.h"
#include "Poco/DOM/Document.h"
#include "Poco/DOM/Element.h"
#include "Poco/DOM/Text.h"
#include "Poco/DOM/ProcessingInstruction.h"
#include "Poco/DOM/DOMWriter.h"
#include "Poco/XML/XMLWriter.h"
#include "Poco/TextEncoding.h"
#include "Poco/GBKEncoding.h"
#include "Poco/KOI8REncoding.h"
#include "Poco/Base64Encoder.h"
#include <sstream>
#include <fstream>
#include "AllObjectManagerUtil.h"

using Poco::Exception;
using Poco::FileNotFoundException;
using Poco::AutoPtr;
using Poco::Util::IniFileConfiguration;
using Poco::XML::Document;
using Poco::XML::Element;
using Poco::XML::Text;
using Poco::XML::ProcessingInstruction;
using Poco::XML::DOMWriter;
using Poco::XML::XMLWriter;
using Poco::Base64Encoder;
using Poco::GBKEncoding;
using Poco::KOI8REncoding;
using Poco::TextEncoding;

CPhmsPushSession::CPhmsPushSession(void):m_pHttpPushSession(NULL), m_bSsl(false)
{
	//获得ssl环境
	{
		string stringWorkingDir = CUtil::GetCurrentAppDir();
		CNetConfig netConfig;
		this->GetNetConfig(stringWorkingDir+"PhmsConfig.ini", netConfig);
		m_bSsl = netConfig.GetSsl();
	}
	m_pHttpPushSession = new CHttpPushSession(m_bSsl);
}

CPhmsPushSession::~CPhmsPushSession(void)
{
	delete m_pHttpPushSession;
	m_pHttpPushSession = NULL;
}

void CPhmsPushSession::abort()
{
	Mutex::ScopedLock lock(m_mutexAbortAndSslSwitch);
	m_pHttpPushSession->abort();
}

string CPhmsPushSession::GetSessionId()
{
	return m_stringSessionId;
}

void CPhmsPushSession::SetSessionId(string stringSessionId)
{
	m_stringSessionId = stringSessionId;
}

void CPhmsPushSession::SetPushCallback(pHttpPushCallback pCallback)
{
	m_pCallback = pCallback;
}

int CPhmsPushSession::PushMsg(string stringSenderId, string stringCaseId, string stringCaseType, string stringMsgType, string stringMsgDirection, string stringVersion)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildSenderId = pDoc->createElement("senderid");
	AutoPtr<Element> pChildCaseId = pDoc->createElement("caseid");
	AutoPtr<Element> pChildCaseType = pDoc->createElement("casetype");
	AutoPtr<Element> pChildMsgType = pDoc->createElement("msgtype");
	AutoPtr<Element> pChildMsgDirection = pDoc->createElement("msgdirection");
	AutoPtr<Element> pChildVersion = pDoc->createElement("version");
	AutoPtr<Text> pTextSenderId = pDoc->createTextNode(stringSenderId);
	AutoPtr<Text> pTextCaseId = pDoc->createTextNode(stringCaseId);
	AutoPtr<Text> pTextCaseType = pDoc->createTextNode(stringCaseType);
	AutoPtr<Text> pTextMsgType = pDoc->createTextNode(stringMsgType);
	AutoPtr<Text> pTextMsgDirection = pDoc->createTextNode(stringMsgDirection);
	AutoPtr<Text> pTextVersion = pDoc->createTextNode(stringVersion);
	pChildSenderId->appendChild(pTextSenderId);
	pChildCaseId->appendChild(pTextCaseId);
	pChildCaseType->appendChild(pTextCaseType);
	pChildMsgType->appendChild(pTextMsgType);
	pChildMsgDirection->appendChild(pTextMsgDirection);
	pChildVersion->appendChild(pTextVersion);
	pRoot->appendChild(pChildSenderId);
	pRoot->appendChild(pChildCaseId);
	pRoot->appendChild(pChildCaseType);
	pRoot->appendChild(pChildMsgType);
	pRoot->appendChild(pChildMsgDirection);
	pRoot->appendChild(pChildVersion);
	//pDoc->appendChild(pi);
	pDoc->appendChild(pRoot);

	string stringXmlContent;
	int nRet = PHMS_SUCCESSFUL_RESULT;
	nRet = this->GetRequestXml(pDoc, stringXmlContent);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}

	CPhmsRequestHead phmsRequestHead("10", "1031", m_stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsPushSession(phmsRequest);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsPushSession::ExecutePhmsPushSession(CPhmsRequest& phmsRequest)
{	
	CNetConfig netConfig;
	int nRet = PHMS_SUCCESSFUL_RESULT;
	//设置网络环境
	try
	{
		string stringWorkingDir = CUtil::GetCurrentAppDir();
		nRet = this->GetNetConfig(stringWorkingDir+"PhmsConfig.ini", netConfig);
		if(nRet != PHMS_SUCCESSFUL_RESULT)
		{
			CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
			return nRet;
		}
		Mutex::ScopedLock lock(m_mutexAbortAndSslSwitch);
		if(m_bSsl != netConfig.GetSsl())
		{
			delete m_pHttpPushSession;
			m_pHttpPushSession = new CHttpPushSession(netConfig.GetSsl());
			m_bSsl = netConfig.GetSsl();
		}
	}
	catch(FileNotFoundException& e)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(e, __FUNCTION__, __FILE__, __LINE__);
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_NO_NET_CONFIG_FILE), __FUNCTION__, __FILE__, __LINE__);
		return COMMON_NO_NET_CONFIG_FILE;
	}

	if(!phmsRequest.ValidataPhmsRequest())
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_PHMS_REQUEST_VALIDATE_FAIL), __FUNCTION__, __FILE__, __LINE__);
		return COMMON_PHMS_REQUEST_VALIDATE_FAIL;
	}
	nRet = m_pHttpPushSession->ExecuteHttpPushSession(netConfig, phmsRequest, m_pCallback);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}


int CPhmsPushSession::GetNetConfig(string stringFilePath, CNetConfig& netConfig)
{
	try
	{
		AutoPtr<IniFileConfiguration> iniFile = new IniFileConfiguration(stringFilePath, "gbk");
		string stringContent;
		unsigned short nContent;
		istringstream iStringStream;

		string stringPriorIp = CAllObjectManagerUtil::GetPriorIp();
		if(stringPriorIp.size() != 0)
		{
			stringContent = stringPriorIp;
		}
		else
		{
			if(iniFile->has("NET.IP"))
			{
				stringContent = iniFile->getString("NET.IP", "");
				if(stringContent.size() == 0)
				{
					stringContent = iniFile->getString("NET.SERVER_NAME", "data2.contec365.com");
					if(stringContent.size() == 0)
					{
						stringContent = "data2.contec365.com";
					}
				}
			}
			else
			{
				stringContent = iniFile->getString("NET.SERVER_NAME", "data2.contec365.com");
				if(stringContent.size() == 0)
				{
					stringContent = "data2.contec365.com";
				}
			}
		}
		netConfig.SetAddr(stringContent);

		stringContent = iniFile->getString("NET.SERVER_NAME", "data2.contec365.com");
		if(stringContent.size() == 0)
		{
			stringContent = "data2.contec365.com";
		}
		netConfig.SetHost(stringContent);

		stringContent = iniFile->getString("NET.PORT", "80");
		iStringStream.str(stringContent);
		iStringStream>>nContent;
		netConfig.SetPort(nContent);

		stringContent = iniFile->getString("NET.SSL", "0");
		if(stringContent == "1")
		{
			netConfig.SetSsl(true);
		}
		else
		{
			netConfig.SetSsl(false);
		}
		//推送暂时只有一个地址
		stringContent = iniFile->getString("NET.HTTP_PUSH", "/submsg");
		netConfig.SetUrl(stringContent);

		stringContent = iniFile->getString("GENERAL.NETWORK", "0");
		iStringStream.clear();
		iStringStream.str(stringContent);
		iStringStream>>nContent;
		switch (nContent)
		{
		case 0:
			netConfig.SetProxyType(NO_PROXY);
			break;
		case 1:
			netConfig.SetProxyType(HTTP_PROXY);
			break;
		case 2:
			netConfig.SetProxyType(SOCK_PROXY);
			break;
		default:
			netConfig.SetProxyType(NO_PROXY);
			break;
		}

		stringContent = iniFile->getString("PROXY.IP", "");
		netConfig.SetProxyAddr(stringContent);

		stringContent = iniFile->getString("PROXY.PORT", "");
		iStringStream.clear();
		iStringStream.str(stringContent);
		iStringStream>>nContent;
		netConfig.SetProxyPort(nContent);

		stringContent = iniFile->getString("PROXY.USERNAME", "");
		netConfig.SetProxyUsername(stringContent);

		stringContent = iniFile->getString("PROXY.PASSWORD", "");
		netConfig.SetProxyPassword(stringContent);
	}
	catch(FileNotFoundException& e)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(e, __FUNCTION__, __FILE__, __LINE__);
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_NO_NET_CONFIG_FILE), __FUNCTION__, __FILE__, __LINE__);
		throw;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsPushSession::GetRequestXml(AutoPtr<Document> pDoc, string& stringXmlContent)
{
	string stringWorkingDir = CUtil::GetCurrentAppDir();
	string stringIniFilePath = stringWorkingDir+"PhmsConfig.ini";
	AutoPtr<IniFileConfiguration> iniFile = new IniFileConfiguration(stringIniFilePath, "gbk");
	string stringEncoding = iniFile->getString("OTHER.Encoding", "GBK");

	DOMWriter domWriter;
	//ProcessingInstruction和XMLWriter::WRITE_XML_DECLARATION 只用一个就好，都用会写两个头
	domWriter.setOptions(XMLWriter::WRITE_XML_DECLARATION | XMLWriter::PRETTY_PRINT);
	domWriter.setNewLine(XMLWriter::NEWLINE_CRLF);
	domWriter.setIndent("	");
	TextEncoding* pEnvEncoding = TextEncoding::find(stringEncoding);
	TextEncoding* pUtf8Encoding = TextEncoding::find("UTF-8");
	//domWriter.setEncoding(stringEncoding, *pEnvEncoding);
	domWriter.setEncoding("UTF-8", *pUtf8Encoding);
	domWriter.setInputEncoding(*pEnvEncoding);


	std::ostringstream oStringStream;
	Base64Encoder encoderBase64(oStringStream);
	encoderBase64.rdbuf()->setLineLength(0);
	try
	{
		domWriter.writeNode(encoderBase64,pDoc);
		ostringstream oStringStreamXmlContent;
		domWriter.writeNode(oStringStreamXmlContent, pDoc);
		//domWriter.writeNode(cout, pDoc);
		//cout<<endl<<endl;
	}
	catch(Exception& e)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(e, __FUNCTION__, __FILE__, __LINE__);
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_WRITE_XML_ERROR), __FUNCTION__, __FILE__, __LINE__);
		return COMMON_WRITE_XML_ERROR;
	}
	encoderBase64.close();
	stringXmlContent = oStringStream.str();
	return PHMS_SUCCESSFUL_RESULT;
}
