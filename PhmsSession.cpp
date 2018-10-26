#include "PhmsSession.h"
#include "Poco/DOM/Document.h"
#include "Poco/DOM/Text.h"
#include "Poco/DOM/Element.h"
#include "Poco/DOM/ProcessingInstruction.h"
#include "Poco/DOM/DOMWriter.h"
#include "Poco/ASCIIEncoding.h"
#include "Poco/DOM/DOMParser.h"
#include "Poco/DOM/NodeIterator.h"
#include "Poco/SAX/InputSource.h"
#include "Poco/DOM/Node.h"
#include "Poco/DOM/NodeFilter.h"
#include "ErrorCode.h"
#include "Poco/Exception.h"
#include "Poco/XML/XMLWriter.h"
#include "Poco/Util/IniFileConfiguration.h"
#include "Poco/AutoPtr.h"
#include "Poco/Base64Encoder.h"
#include "Poco/ASCIIEncoding.h"
#include "Poco/UTF8Encoding.h"
#include "Poco/TextConverter.h"
#include "Poco/TextEncoding.h"
#include "Poco/GBKEncoding.h"
#include "Poco/KOI8REncoding.h"
#include "Poco/StreamCopier.h"
#include "Poco/File.h"
#include "Poco/Crypto/Cipher.h"
#include "Poco/Crypto/CipherFactory.h"
#include "Poco/Crypto/CipherKey.h"
#include "Poco/DateTime.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/Timestamp.h"
#include "Poco/Timezone.h"
#include "Poco/Timespan.h"
#include "Poco/URI.h"
#include <sstream>
#include <fstream>
#include "Util.h"
#include "PhmsLogger.h"
#include "SessionIdManager.h"
#include "AllObjectManagerUtil.h"
#include "json/json.h"
#include "json/value.h"
#include "json/writer.h"
#include "GlobalTerminalLogoString.h"



using Poco::XML::Document;
using Poco::XML::Text;
using Poco::XML::Element;
using Poco::XML::ProcessingInstruction;
using Poco::XML::DOMWriter;
using Poco::ASCIIEncoding;
using Poco::XML::DOMParser;
using Poco::XML::NodeIterator;
using Poco::XML::InputSource;
using Poco::XML::Node;
using Poco::XML::NodeFilter;
using Poco::Exception;
using Poco::TimeoutException;
using Poco::FileNotFoundException;
using Poco::XML::XMLWriter;
using Poco::Util::IniFileConfiguration;
using Poco::AutoPtr;
using Poco::Base64Encoder;
using Poco::TextConverter;
using Poco::ASCIIEncoding;
using Poco::UTF8Encoding;
using Poco::StreamCopier;
using Poco::TextEncoding;
using Poco::GBKEncoding;
using Poco::KOI8REncoding;
using Poco::File;
using Poco::Crypto::Cipher;
using Poco::Crypto::CipherFactory;
using Poco::Crypto::CipherKey;
using Poco::DateTime;
using Poco::DateTimeFormatter;
using Poco::Timestamp;
using Poco::Timezone;
using Poco::Timespan;
using Poco::URI;

string CPhmsSession::m_stringKey = "5b1b25bf9447a5002ca129a7f9523459";

string CPhmsSession::m_stringToken_XiKang = "";

string CPhmsSession::m_stringClientId_XiKang = "";

string CPhmsSession::m_stringClientSecret_XiKang = "";

string CPhmsSession::m_stringGWSecretKey = "";

CPhmsSession::CPhmsSession(void):m_nSaveType(0), m_enumPhmsCommandType(MAIN_TYPE), m_bUseIp(false), m_enumNetConfigType(ORIGINAL_PHMS)
{
	//获得ssl环境
	{
		string stringWorkingDir = CUtil::GetCurrentAppDir();
		CNetConfig netConfig;
		this->GetNetConfig(stringWorkingDir+"PhmsConfig.ini", netConfig, false);
		m_bSsl = netConfig.GetSsl();
	}
	m_pHttpSession = new CHttpSession(m_bSsl);
}


CPhmsSession::~CPhmsSession(void)
{
	delete m_pHttpSession;
}

//Setter
void CPhmsSession::SetSaveType(int nSaveType)
{
	m_nSaveType = nSaveType;
}
void CPhmsSession::SetFilePath(string stringFilePath)
{
	m_stringFilePath = stringFilePath;
}
void CPhmsSession::SetSessionId(string stringSessionId)
{
	m_stringSessionId = stringSessionId;
}
void CPhmsSession::SetGWSessionId(string stringSessionId)
{
	m_stringGWSessionId = stringSessionId;
}
void CPhmsSession::SetCommandType(EnumPhmsCommandType enumCommandType)
{
	m_enumPhmsCommandType = enumCommandType;
}
void CPhmsSession::SetXmlCommandType(EnumXmlCommandType enumXmlCommandType)
{
	m_enumXmlCommandType = enumXmlCommandType;
}
void CPhmsSession::SetXmlCommandType_Telecom(EnumXmlCommandType_Telecom enumXmlCommandType_Telecom)
{
	m_enumXmlCommandType_Telecom = enumXmlCommandType_Telecom;
}
void CPhmsSession::SetXinJieCommandType(EnumXinJieCommandType enumXinJieCommandType)
{
	m_enumXinJieCommandType = enumXinJieCommandType;
}
void CPhmsSession::SetGuanXinCommandType(EnumGuanXinCommandType enumGuanXinCommandType)
{
	m_enumGuanXinCommandType = enumGuanXinCommandType;
}
void CPhmsSession::SetXiKangCommandType(EnumXiKangCommandType enumXiKangCommandType)
{
	m_enumXiKangCommandType = enumXiKangCommandType;
}
void CPhmsSession::SetNetConfigType(EnumNetConfigType enumNetConfigType)
{
	m_enumNetConfigType = enumNetConfigType;
}
void CPhmsSession::SetUseIp(bool bUseIp)
{
	m_bUseIp = bUseIp;
}
void CPhmsSession::SetGWSecretKey(string stringGWSecretKey)
{
	CPhmsSession::m_stringGWSecretKey = stringGWSecretKey;
}
//Getter
int CPhmsSession::GetSaveType()
{
	return m_nSaveType;
}
string CPhmsSession::GetContent()
{
	return m_stringContent;
}
string CPhmsSession::GetSessionId()
{
	return m_stringSessionId;
}
string CPhmsSession::GetGWSessionId()
{
	return m_stringGWSessionId;
}
EnumPhmsCommandType CPhmsSession::GetCommandType()
{
	return m_enumPhmsCommandType;
}
EnumXmlCommandType CPhmsSession::GetXmlCommandType()
{
	return m_enumXmlCommandType;
}
EnumXmlCommandType_Telecom CPhmsSession::GetXmlCommandType_Telecom()
{
	return m_enumXmlCommandType_Telecom;
}
EnumXinJieCommandType CPhmsSession::GetXinJieCommandType()
{
	return m_enumXinJieCommandType;
}
EnumGuanXinCommandType CPhmsSession::GetGuanXinCommandType()
{
	return m_enumGuanXinCommandType;
}
EnumNetConfigType CPhmsSession::GetNetConfigType()
{
	return m_enumNetConfigType;
}
string CPhmsSession::GetIp()
{
	return m_stringIp;
}
string CPhmsSession::GetGWSecretKey()
{
	return CPhmsSession::m_stringGWSecretKey;
}

//private member function
int CPhmsSession::ExecutePhmsSession(CPhmsRequest& phmsRequest, CPhmsResponse& phmsResponse, bool bMustSsl)
{
	CNetConfig netConfig;
	int nRet = PHMS_SUCCESSFUL_RESULT;
	//设置网络环境
	try
	{
		string stringWorkingDir = CUtil::GetCurrentAppDir();
		EnumNetConfigType enumNetConfigType = this->GetNetConfigType();
		nRet = this->GetNetConfig(stringWorkingDir+"PhmsConfig.ini", netConfig, bMustSsl, enumNetConfigType);
		//nRet = this->GetNetConfig(stringWorkingDir+"PhmsConfig.ini", netConfig, bMustSsl);
		if(nRet != PHMS_SUCCESSFUL_RESULT)
		{
			CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
			return nRet;
		}
		Mutex::ScopedLock lock(m_mutexAbortAndSslSwitch);
		if(bMustSsl)
		{
			if(!m_bSsl)
			{
				delete m_pHttpSession;
				m_pHttpSession = new CHttpSession(bMustSsl);
				m_bSsl = bMustSsl;
			}
		}
		else
		{
			if(m_bSsl != netConfig.GetSsl())
			{
				delete m_pHttpSession;
				m_pHttpSession = new CHttpSession(netConfig.GetSsl());
				m_bSsl = netConfig.GetSsl();
			}
		}
	}
	catch(FileNotFoundException& e)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(e, __FUNCTION__, __FILE__, __LINE__);
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_NO_NET_CONFIG_FILE), __FUNCTION__, __FILE__, __LINE__);
		return COMMON_NO_NET_CONFIG_FILE;
	}
	
	istream* pInputStream = NULL;
	if(!phmsRequest.ValidataPhmsRequest())
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_PHMS_REQUEST_VALIDATE_FAIL), __FUNCTION__, __FILE__, __LINE__);
		return COMMON_PHMS_REQUEST_VALIDATE_FAIL;
	}
	nRet = m_pHttpSession->ExecuteHttpSession(netConfig, phmsRequest, pInputStream);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	m_stringIp = netConfig.GetIp();
	nRet = phmsResponse.SetPhmsResponse(pInputStream, this);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::ExecuteXmlSession(string stringXmlContent, bool bMustSsl)
{
	CNetConfig netConfig;
	int nRet = PHMS_SUCCESSFUL_RESULT;
	//设置网络环境
	try
	{
		string stringWorkingDir = CUtil::GetCurrentAppDir();
		EnumNetConfigType enumNetConfigType = this->GetNetConfigType();
		nRet = this->GetNetConfig(stringWorkingDir+"PhmsConfig.ini", netConfig, bMustSsl, enumNetConfigType);
		//nRet = this->GetXmlNetConfig(stringWorkingDir+"PhmsConfig.ini", netConfig, bMustSsl);
		if(nRet != PHMS_SUCCESSFUL_RESULT)
		{
			CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
			return nRet;
		}
		Mutex::ScopedLock lock(m_mutexAbortAndSslSwitch);
		if(bMustSsl)
		{
			if(!m_bSsl)
			{
				delete m_pHttpSession;
				m_pHttpSession = new CHttpSession(bMustSsl);
				m_bSsl = bMustSsl;
			}
		}
		else
		{
			if(m_bSsl != netConfig.GetSsl())
			{
				delete m_pHttpSession;
				m_pHttpSession = new CHttpSession(netConfig.GetSsl());
				m_bSsl = netConfig.GetSsl();
			}
		}
	}
	catch(FileNotFoundException& e)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(e, __FUNCTION__, __FILE__, __LINE__);
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_NO_NET_CONFIG_FILE), __FUNCTION__, __FILE__, __LINE__);
		return COMMON_NO_NET_CONFIG_FILE;
	}

	istream* pInputStream = NULL;
	nRet = m_pHttpSession->ExecuteXmlHttpSession(netConfig, stringXmlContent, pInputStream);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = NoPhmsGetResult(pInputStream);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::ExecuteXmlSession_Telecom(string stringXmlContent, bool bMustSsl)
{
	CNetConfig netConfig;
	int nRet = PHMS_SUCCESSFUL_RESULT;
	//设置网络环境
	try
	{
		string stringWorkingDir = CUtil::GetCurrentAppDir();
		EnumNetConfigType enumNetConfigType = this->GetNetConfigType();
		nRet = this->GetNetConfig(stringWorkingDir+"PhmsConfig.ini", netConfig, bMustSsl, enumNetConfigType);
		//nRet = this->GetXmlNetConfig_Telecom(stringWorkingDir+"PhmsConfig.ini", netConfig, bMustSsl);
		if(nRet != PHMS_SUCCESSFUL_RESULT)
		{
			CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
			return nRet;
		}
		Mutex::ScopedLock lock(m_mutexAbortAndSslSwitch);
		if(bMustSsl)
		{
			if(!m_bSsl)
			{
				delete m_pHttpSession;
				m_pHttpSession = new CHttpSession(bMustSsl);
				m_bSsl = bMustSsl;
			}
		}
		else
		{
			if(m_bSsl != netConfig.GetSsl())
			{
				delete m_pHttpSession;
				m_pHttpSession = new CHttpSession(netConfig.GetSsl());
				m_bSsl = netConfig.GetSsl();
			}
		}
	}
	catch(FileNotFoundException& e)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(e, __FUNCTION__, __FILE__, __LINE__);
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_NO_NET_CONFIG_FILE), __FUNCTION__, __FILE__, __LINE__);
		return COMMON_NO_NET_CONFIG_FILE;
	}

	istream* pInputStream = NULL;
	//nRet = m_pHttpSession->ExecuteXmlHttpSession(netConfig, stringXmlContent, pInputStream);
	map<string, string> mapParam;
	mapParam["requestXML"] = stringXmlContent;
	nRet = m_pHttpSession->ExecuteFormHttpSession(netConfig, mapParam, pInputStream);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = NoPhmsGetResult(pInputStream);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::ExecuteGuanXinSession(map<string, string> mapParam, string stringDataPath, bool bMustSsl)
{
	CNetConfig netConfig;
	int nRet = PHMS_SUCCESSFUL_RESULT;
	//设置网络环境
	try
	{
		string stringWorkingDir = CUtil::GetCurrentAppDir();
		EnumNetConfigType enumNetConfigType = this->GetNetConfigType();
		nRet = this->GetNetConfig(stringWorkingDir+"PhmsConfig.ini", netConfig, bMustSsl, enumNetConfigType);
		//nRet = this->GetGuanXinNetConfig(stringWorkingDir+"PhmsConfig.ini", netConfig, bMustSsl);
		if(nRet != PHMS_SUCCESSFUL_RESULT)
		{
			CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
			return nRet;
		}
		Mutex::ScopedLock lock(m_mutexAbortAndSslSwitch);
		if(bMustSsl)
		{
			if(!m_bSsl)
			{
				delete m_pHttpSession;
				m_pHttpSession = new CHttpSession(bMustSsl);
				m_bSsl = bMustSsl;
			}
		}
		else
		{
			if(m_bSsl != netConfig.GetSsl())
			{
				delete m_pHttpSession;
				m_pHttpSession = new CHttpSession(netConfig.GetSsl());
				m_bSsl = netConfig.GetSsl();
			}
		}
	}
	catch(FileNotFoundException& e)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(e, __FUNCTION__, __FILE__, __LINE__);
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_NO_NET_CONFIG_FILE), __FUNCTION__, __FILE__, __LINE__);
		return COMMON_NO_NET_CONFIG_FILE;
	}

	istream* pInputStream = NULL;
// 	if(m_enumGuanXinCommandType == ACCESSTOKEN)
// 	{
// 		string stringUrl = netConfig.GetUrl();
// 		//有时间改成map遍历，赶时间，先凑合着
// 		stringUrl += "?grant_type=password&username=";
// 		stringUrl += mapParam["username"];
// 		stringUrl += "&password=";
// 		stringUrl += mapParam["password"];
// 		netConfig.SetUrl(stringUrl);
// 		nRet = m_pHttpSession->ExecuteNoPhmsHttpSession(netConfig, pInputStream, false, m_stringFilePath);
// 	}
//	else
//	{
//		nRet = m_pHttpSession->ExecuteFormHttpSession(netConfig, mapParam, pInputStream);
//	}

	nRet = m_pHttpSession->ExecuteFormHttpSession(netConfig, mapParam, stringDataPath, pInputStream);
	
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = NoPhmsGetResult(pInputStream);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::ExecuteXiKangSession(map<string, string> mapParam, string stringDataPath, string stringUrlOpenId, bool bMustSsl)
{
	CNetConfig netConfig;
	int nRet = PHMS_SUCCESSFUL_RESULT;
	//设置网络环境
	try
	{
		string stringWorkingDir = CUtil::GetCurrentAppDir();
		EnumNetConfigType enumNetConfigType = this->GetNetConfigType();
		nRet = this->GetNetConfig(stringWorkingDir+"PhmsConfig.ini", netConfig, bMustSsl, enumNetConfigType);
		//nRet = this->GetXiKangNetConfig(stringWorkingDir+"PhmsConfig.ini", netConfig, bMustSsl);
		if(nRet != PHMS_SUCCESSFUL_RESULT)
		{
			CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
			return nRet;
		}
		if(netConfig.GetUrl().find("{openID}") != string::npos)
		{
			string stringUrl = netConfig.GetUrl();
			if(stringUrlOpenId.size() != 0)
			{
				CUtil::ReplaceString(stringUrl, "{openID}", stringUrlOpenId);
				netConfig.SetUrl(stringUrl);
			}
			else
			{
				CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_XIKANG_URL_OPENID_NULL), __FUNCTION__, __FILE__, __LINE__);
				return COMMON_XIKANG_URL_OPENID_NULL;
			}
		}
		if(netConfig.GetUrl().find("{ecgID}") != string::npos)
		{
			string stringUrl = netConfig.GetUrl();
			if(stringUrlOpenId.size() != 0)
			{
				CUtil::ReplaceString(stringUrl, "{ecgID}", stringUrlOpenId);
				netConfig.SetUrl(stringUrl);
			}
			else
			{
				CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_XIKANG_URL_OPENID_NULL), __FUNCTION__, __FILE__, __LINE__);
				return COMMON_XIKANG_URL_OPENID_NULL;
			}
		}
		Mutex::ScopedLock lock(m_mutexAbortAndSslSwitch);
		if(bMustSsl)
		{
			if(!m_bSsl)
			{
				delete m_pHttpSession;
				m_pHttpSession = new CHttpSession(bMustSsl);
				m_bSsl = bMustSsl;
			}
		}
		else
		{
			if(m_bSsl != netConfig.GetSsl())
			{
				delete m_pHttpSession;
				m_pHttpSession = new CHttpSession(netConfig.GetSsl());
				m_bSsl = netConfig.GetSsl();
			}
		}
	}
	catch(FileNotFoundException& e)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(e, __FUNCTION__, __FILE__, __LINE__);
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_NO_NET_CONFIG_FILE), __FUNCTION__, __FILE__, __LINE__);
		return COMMON_NO_NET_CONFIG_FILE;
	}

	istream* pInputStream = NULL;
	string stringHttpBody;
	
	//所有向熙康提交的数据全部转为utf-8编码
	map<string, string>::iterator iterA = mapParam.begin();
	for(iterA; iterA!=mapParam.end(); iterA++)
	{
		string stringUtf8Value;
		CUtil::GBKToUTF8(iterA->second, stringUtf8Value);
		if(m_enumXiKangCommandType == SEARCH_PATIENT_ID_XIKANG)
		{
			string stringEncodeUtf8Value;
			URI::encode(stringUtf8Value, "", stringEncodeUtf8Value);
			stringUtf8Value = stringEncodeUtf8Value;
		}
		mapParam[iterA->first] = stringUtf8Value;
	}
	//使用token，处理参数
	if(m_enumXiKangCommandType != GET_TOKEN_XIKANG)
	{
		if(m_enumXiKangCommandType!=LOGIN_XIKANG && m_enumXiKangCommandType!=GET_PATIENT_ID_XIKANG && 
			m_enumXiKangCommandType!=GET_PATIENT_INFO_XIKANG && m_enumXiKangCommandType!=SEARCH_PATIENT_ID_XIKANG &&
			/*m_enumXiKangCommandType!=GET_CASE_LIST_XIKANG && m_enumXiKangCommandType!=SUBMIT_CASE_XIKANG && */
			m_enumXiKangCommandType!=UPLOAD_FILE_XIKANG && m_enumXiKangCommandType!=GET_REPORT_URL_XIKANG)
		{
			map<string, string>::iterator iter = mapParam.begin();
			stringHttpBody = iter->second;
			mapParam.clear();
		}
		DateTime dt;
		//dt += Timespan(Timezone::utcOffset(), 0);
		mapParam["time"] = CUtil::Int64ToString(dt.timestamp().epochMicroseconds());
		mapParam["access_token"] = m_stringToken_XiKang;
		string stringParamString;
		string stringMd5;
		map<string, string>::iterator iter = mapParam.begin();
		for(iter; iter!=mapParam.end(); iter++)
		{
			stringParamString += iter->first;
			stringParamString += "=";
			if(m_enumXiKangCommandType == SEARCH_PATIENT_ID_XIKANG)
			{
				string stringDecodeUtf8Value;
				URI::decode(iter->second, stringDecodeUtf8Value);
				stringParamString += stringDecodeUtf8Value;
			}
			else
			{
				stringParamString += iter->second;
			}
		}
		stringParamString += CPhmsSession::m_stringClientSecret_XiKang;
		stringMd5 = CUtil::GetMd5(stringParamString.c_str(), stringParamString.size());
		std::transform(stringMd5.begin(), stringMd5.end(), stringMd5.begin(),::tolower);
		mapParam["sign"] = stringMd5;
	}
	//不同的m_enumXiKangCommandType，区分get和post
	if(m_enumXiKangCommandType==LOGIN_XIKANG || m_enumXiKangCommandType==GET_PATIENT_ID_XIKANG || 
		m_enumXiKangCommandType==GET_PATIENT_INFO_XIKANG || m_enumXiKangCommandType==SEARCH_PATIENT_ID_XIKANG || 
		m_enumXiKangCommandType==GET_REPORT_URL_XIKANG)
	{
		nRet = m_pHttpSession->ExeceteGetHttpSession(netConfig, mapParam, pInputStream);
	}
	else if(m_enumXiKangCommandType==GET_TOKEN_XIKANG || /*m_enumXiKangCommandType==GET_CASE_LIST_XIKANG || */
		/*m_enumXiKangCommandType==SUBMIT_CASE_XIKANG || */m_enumXiKangCommandType==UPLOAD_FILE_XIKANG)
	{
		nRet = m_pHttpSession->ExecuteFormHttpSession(netConfig, mapParam, stringDataPath, pInputStream);
	}
	else
	{
		nRet = m_pHttpSession->ExeceteCommonPostSession(netConfig, mapParam, "application/json", stringHttpBody, pInputStream);
	}


	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = NoPhmsGetResult(pInputStream);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::ExecuteXinJieSession(string stringBase64Content, bool bMustSsl)
{
	CNetConfig netConfig;
	int nRet = PHMS_SUCCESSFUL_RESULT;
	//设置网络环境
	try
	{
		string stringWorkingDir = CUtil::GetCurrentAppDir();
		EnumNetConfigType enumNetConfigType = this->GetNetConfigType();
		nRet = this->GetNetConfig(stringWorkingDir+"PhmsConfig.ini", netConfig, bMustSsl, enumNetConfigType);
		//nRet = this->GetXinJieNetConfig(stringWorkingDir+"PhmsConfig.ini", netConfig, bMustSsl);
		if(nRet != PHMS_SUCCESSFUL_RESULT)
		{
			CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
			return nRet;
		}
		Mutex::ScopedLock lock(m_mutexAbortAndSslSwitch);
		if(bMustSsl)
		{
			if(!m_bSsl)
			{
				delete m_pHttpSession;
				m_pHttpSession = new CHttpSession(bMustSsl);
				m_bSsl = bMustSsl;
			}
		}
		else
		{
			if(m_bSsl != netConfig.GetSsl())
			{
				delete m_pHttpSession;
				m_pHttpSession = new CHttpSession(netConfig.GetSsl());
				m_bSsl = netConfig.GetSsl();
			}
		}
	}
	catch(FileNotFoundException& e)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(e, __FUNCTION__, __FILE__, __LINE__);
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_NO_NET_CONFIG_FILE), __FUNCTION__, __FILE__, __LINE__);
		return COMMON_NO_NET_CONFIG_FILE;
	}

	istream* pInputStream = NULL;
	nRet = m_pHttpSession->ExecuteXmlHttpSession(netConfig, stringBase64Content, pInputStream);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = NoPhmsGetResult(pInputStream);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

//非PHMS协议接口
int CPhmsSession::ExecuteNoPhmsSession(string stringUrl, int bContinue, bool bMustSsl)
{
	CNetConfig netConfig;
	int nRet = PHMS_SUCCESSFUL_RESULT;
	//设置网络环境
	try
	{
		string stringWorkingDir = CUtil::GetCurrentAppDir();
		nRet = this->GetNoPhmsNetConfig(stringWorkingDir+"PhmsConfig.ini", netConfig, stringUrl, bMustSsl);
		if(nRet != PHMS_SUCCESSFUL_RESULT)
		{
			CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
			return nRet;
		}
		Mutex::ScopedLock lock(m_mutexAbortAndSslSwitch);
		if(bMustSsl)
		{
			if(!m_bSsl)
			{
				delete m_pHttpSession;
				m_pHttpSession = new CHttpSession(bMustSsl);
				m_bSsl = bMustSsl;
			}
		}
		else
		{
			if(m_bSsl != netConfig.GetSsl())
			{
				delete m_pHttpSession;
				m_pHttpSession = new CHttpSession(netConfig.GetSsl());
				m_bSsl = netConfig.GetSsl();
			}
		}
	}
	catch(FileNotFoundException& e)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(e, __FUNCTION__, __FILE__, __LINE__);
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_NO_NET_CONFIG_FILE), __FUNCTION__, __FILE__, __LINE__);
		return COMMON_NO_NET_CONFIG_FILE;
	}

	istream* pInputStream = NULL;
	nRet = m_pHttpSession->ExecuteNoPhmsHttpSession(netConfig, pInputStream, bContinue, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = NoPhmsGetResult(pInputStream);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::ExecuteFormHttpSession(string stringHost, string stringUrl, map<string, string> mapParam, bool bMustSsl)
{
	CNetConfig netConfig;
	int nRet = PHMS_SUCCESSFUL_RESULT;
	//设置网络环境
	try
	{
		string stringWorkingDir = CUtil::GetCurrentAppDir();
		nRet = this->GetFormNetConfig(stringWorkingDir+"PhmsConfig.ini", netConfig, stringHost, stringUrl, bMustSsl);
		if(nRet != PHMS_SUCCESSFUL_RESULT)
		{
			CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
			return nRet;
		}
		Mutex::ScopedLock lock(m_mutexAbortAndSslSwitch);
		if(bMustSsl)
		{
			if(!m_bSsl)
			{
				delete m_pHttpSession;
				m_pHttpSession = new CHttpSession(bMustSsl);
				m_bSsl = bMustSsl;
			}
		}
		else
		{
			if(m_bSsl != netConfig.GetSsl())
			{
				delete m_pHttpSession;
				m_pHttpSession = new CHttpSession(netConfig.GetSsl());
				m_bSsl = netConfig.GetSsl();
			}
		}
	}
	catch(FileNotFoundException& e)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(e, __FUNCTION__, __FILE__, __LINE__);
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_NO_NET_CONFIG_FILE), __FUNCTION__, __FILE__, __LINE__);
		return COMMON_NO_NET_CONFIG_FILE;
	}

	istream* pInputStream = NULL;
	nRet = m_pHttpSession->ExecuteFormHttpSession(netConfig, mapParam, pInputStream);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = NoPhmsGetResult(pInputStream);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::GetSessionId(int nSaveType, string stringContent, string stringFilePath)
{
	int nRet = 0;
	m_stringSessionId = "";
	nRet = CUtil::GetXmlFieldVaule(nSaveType, stringContent, stringFilePath, "sid", m_stringSessionId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_stringSessionId.size() == 0)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_NO_SID), __FUNCTION__, __FILE__, __LINE__);
		return COMMON_NO_SID;
	}
	return nRet;
}

string CPhmsSession::GetXiKangToken(int nSaveType, string stringContent, string stringFilePath)
{
	map<string, string> mapResult;
	CUtil::ParseJsonObjectWithoutArray(nSaveType, stringContent, stringFilePath, mapResult);
	return mapResult["access_token"];
}

int CPhmsSession::GetRequestXml(AutoPtr<Document> pDoc, string& stringXmlContent)
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
// 	 	ostringstream oStringStreamXmlContent;
//  	domWriter.writeNode(oStringStreamXmlContent, pDoc);
//  	CPhmsLogger::GetPhmsLogger()->WriteLog(oStringStreamXmlContent.str(), __FUNCTION__, __FILE__, __LINE__);
// 		domWriter.writeNode(cout, pDoc);
// 		cout<<endl<<endl;
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

int CPhmsSession::GetRequestXml(string stringFilePath, string& stringXmlContent)
{
	
	std::ostringstream oStringStream;
	Base64Encoder encoderBase64(oStringStream);
	encoderBase64.rdbuf()->setLineLength(0);
	try
	{
		std::ifstream ifStream;
		ifStream.exceptions(ios::badbit);
		ifStream.open(stringFilePath.c_str());
		StreamCopier::copyStream(ifStream, encoderBase64);
	}
	catch(Exception& e)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(e, __FUNCTION__, __FILE__, __LINE__);
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_WRITE_XML_ERROR), __FUNCTION__, __FILE__, __LINE__);
		return COMMON_WRITE_XML_ERROR;
	}
	catch (ios::failure& e)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(e.what(), __FUNCTION__, __FILE__, __LINE__);
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_FILE_READ_FAIL), __FUNCTION__, __FILE__, __LINE__);
		return COMMON_FILE_READ_FAIL;
	}
	encoderBase64.close();
	stringXmlContent = oStringStream.str();

	//体检机组的xml趋势文件转码并修改xml编码声明
	string stringWorkingDir = CUtil::GetCurrentAppDir();
	string stringIniFilePath = stringWorkingDir+"PhmsConfig.ini";
	AutoPtr<IniFileConfiguration> iniFile = new IniFileConfiguration(stringIniFilePath, "gbk");
	string stringEncoding = iniFile->getString("OTHER.Encoding", "GBK");
	TextEncoding* pEnvEncoding = TextEncoding::find(stringEncoding);
	TextEncoding* pUtf8Encoding = TextEncoding::find("UTF-8");
	TextConverter converter(*pEnvEncoding, *pUtf8Encoding);
	string stringTempXmlContent;
	converter.convert(stringXmlContent, stringTempXmlContent);
	string stringSrc = "encoding=\"";
	stringSrc += stringEncoding;
	stringSrc += "\"?>";
	string stringDst = "encoding=\"UTF-8\"?>";
	CUtil::ReplaceString(stringTempXmlContent, stringSrc, stringDst);
	stringXmlContent = stringTempXmlContent;
	return PHMS_SUCCESSFUL_RESULT;
}



int CPhmsSession::GetRequestXmlNoBase64(Poco::AutoPtr<Poco::XML::Document> pDoc, string& stringXmlContent, bool bUtf8)
{
	string stringWorkingDir = CUtil::GetCurrentAppDir();
	string stringIniFilePath = stringWorkingDir+"PhmsConfig.ini";
	AutoPtr<IniFileConfiguration> iniFile = new IniFileConfiguration(stringIniFilePath, "gbk");
	string stringEncoding = iniFile->getString("OTHER.Encoding", "GBK");
	DOMWriter domWriter;
	//ProcessingInstruction和XMLWriter::WRITE_XML_DECLARATION 只用一个就好，都用会写两个头
	domWriter.setOptions(XMLWriter::WRITE_XML_DECLARATION/*XMLWriter::CANONICAL_XML*/  | XMLWriter::PRETTY_PRINT);
	domWriter.setNewLine(XMLWriter::NEWLINE_CRLF);
	domWriter.setIndent("	");
	TextEncoding* pEnvEncoding = TextEncoding::find(stringEncoding);
	
	UTF8Encoding utf8Encoding;
	if(bUtf8)
	{
		domWriter.setEncoding("UTF-8", utf8Encoding);
		domWriter.setInputEncoding(*pEnvEncoding);
	}
	else
	{
		domWriter.setEncoding(stringEncoding, *pEnvEncoding);
		domWriter.setInputEncoding(*pEnvEncoding);
	}
	
	std::ostringstream oStringStream;
	try
	{
		domWriter.writeNode(oStringStream,pDoc);
		ofstream of;
		of.open("./Request.xml", ios_base::binary|ios_base::trunc);
		domWriter.writeNode(of, pDoc);
		of.close();
//		domWriter.writeNode(cout, pDoc);
//		cout<<endl<<endl;
	}
	catch(Exception& e)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(e, __FUNCTION__, __FILE__, __LINE__);
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_WRITE_XML_ERROR), __FUNCTION__, __FILE__, __LINE__);
		return COMMON_WRITE_XML_ERROR;
	}
	stringXmlContent = oStringStream.str();
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::GetRequestXmlForAESAndBase64(Poco::AutoPtr<Poco::XML::Document> pDoc, string& stringXmlContent)
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
	domWriter.setEncoding(stringEncoding, *pEnvEncoding);
	domWriter.setInputEncoding(*pEnvEncoding);

	std::ostringstream oStringStream;
	try
	{
		domWriter.writeNode(oStringStream,pDoc);
	}
	catch(Exception& e)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(e, __FUNCTION__, __FILE__, __LINE__);
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_WRITE_XML_ERROR), __FUNCTION__, __FILE__, __LINE__);
		return COMMON_WRITE_XML_ERROR;
	}
	string stringIn = oStringStream.str();
	//使用0102030405060708作为密钥进行AES加密,并将加密的结果进行base64编码
	CipherKey::ByteVec secret;
	CipherKey::ByteVec vi;
	string::iterator iter;
	string stringSecret = "0102030405060708";
	for(iter=stringSecret.begin(); iter!=stringSecret.end(); iter++)
	{
		secret.push_back(*iter);
		vi.push_back(*iter);
	}
	Cipher::Ptr pCipher = CipherFactory::defaultFactory().createCipher(CipherKey("aes128", secret, vi));
	std::string stringOut = pCipher->encryptString(stringIn, Cipher::ENC_BASE64);
	
	for(iter=stringOut.begin(); iter!=stringOut.end(); iter++)
	{
		if(*iter == '*')
		{
			*iter = '+';
		}
	}
	stringXmlContent = stringOut;
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::GetNetConfig(string stringFilePath, CNetConfig& netConfig, bool bMustSsl, EnumNetConfigType enumNetConfigType)
{
	switch(enumNetConfigType)
	{
	case GW_FORM:
		return GetGWNetConfig(stringFilePath, netConfig, bMustSsl);
	case DIRECTLY_TO_HIS:
		return GetDirectlyToHisConfig(stringFilePath, netConfig, bMustSsl);
	case XINJIE_XML:
		return GetXinJieNetConfig(stringFilePath, netConfig, bMustSsl);
	case XIKANG_FORM:
		return GetXiKangNetConfig(stringFilePath, netConfig, bMustSsl);
	case GUANXIN_FORM:
		return GetGuanXinNetConfig(stringFilePath, netConfig, bMustSsl);
	case TELECOM_XML:
		return GetXmlNetConfig_Telecom(stringFilePath, netConfig, bMustSsl);
	case DAOYI_XML:
		return GetXmlNetConfig(stringFilePath, netConfig, bMustSsl);
	case ORIGINAL_PHMS:
	default:
		return GetNetConfig(stringFilePath, netConfig, bMustSsl);
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::GetNetConfig(string stringFilePath, CNetConfig& netConfig, bool bMustSsl)
{
	try
	{
		AutoPtr<IniFileConfiguration> iniFile = new IniFileConfiguration(stringFilePath, "gbk");
		string stringContent;
		unsigned short nContent;
		istringstream iStringStream;
		if(g_stringContecServerAddr.size() == 0)
		{
			if(!m_bUseIp && m_enumPhmsCommandType==LOGIN_TYPE)
			{
				stringContent = iniFile->getString("NET.SERVER_NAME", "data2.contec365.com");
				if(stringContent.size() == 0)
				{
					stringContent = "data2.contec365.com";
				}
			}
			else
			{
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
			}
			netConfig.SetAddr(stringContent);

			stringContent = iniFile->getString("NET.SERVER_NAME", "data2.contec365.com");
			if(stringContent.size() == 0)
			{
				stringContent = "data2.contec365.com";
			}
			netConfig.SetHost(stringContent);
		}
		else
		{
			netConfig.SetAddr(g_stringContecServerAddr);
			netConfig.SetHost(g_stringContecServerAddr);
		}

		if(!bMustSsl)
		{
			stringContent = iniFile->getString("NET.SSL", "0");
			if(stringContent == "1")
			{
				netConfig.SetSsl(true);
			}
			else
			{
				netConfig.SetSsl(false);
			}
			if(g_stringContecServerPort.size() == 0)
			{
				if(netConfig.GetSsl())
				{
					stringContent = iniFile->getString("NET.SSL_PORT", "443");
				}
				else
				{
					stringContent = iniFile->getString("NET.PORT", "80");
				}
				
			}
			else
			{
				stringContent = g_stringContecServerPort;
			}
			iStringStream.str(stringContent);
			iStringStream>>nContent;
			netConfig.SetPort(nContent);
		}
		else
		{
			netConfig.SetSsl(true);
			stringContent = iniFile->getString("NET.SSL_PORT", "443");
			iStringStream.str(stringContent);
			iStringStream>>nContent;
			netConfig.SetPort(nContent);
		}
		
		switch(m_enumPhmsCommandType)
		{
		case LOGIN_TYPE:
			stringContent = iniFile->getString("NET.LOGIN_URI", "/login.php");
			break;
		case MAIN_TYPE:
			stringContent = iniFile->getString("NET.MAIN_URI", "/main.php");
			break;
		case CHANGE_INFO_TYPE:
			stringContent = iniFile->getString("NET.LOGIN_URI", "/login.php");
			break;
		case REGISTER_TYPE:
			stringContent = iniFile->getString("NET.REGISTER_URI", "/register.php");
			break;
		default:
			stringContent = iniFile->getString("NET.MAIN_URI", "/main.php");
			break;
		}
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

int CPhmsSession::GetNoPhmsNetConfig(string stringFilePath, CNetConfig& netConfig, string stringUrl, bool bMustSsl)
{
	try
	{
		AutoPtr<IniFileConfiguration> iniFile = new IniFileConfiguration(stringFilePath, "gbk");
		string stringContent;
		unsigned short nContent;
		istringstream iStringStream;
		if(stringUrl[0] =='/' || stringUrl.find("file://")!=string::npos  || stringUrl.find("data2.contec365.com")!=string::npos)
		{
			if(g_stringContecServerAddr.size() == 0)
			{
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
			}
			else
			{
				netConfig.SetAddr(g_stringContecServerAddr);
				netConfig.SetHost(g_stringContecServerAddr);
			}

			if(!bMustSsl)
			{
				if(g_stringContecServerPort.size() == 0)
				{
					stringContent = iniFile->getString("NET.PORT", "80");
				}
				else
				{
					stringContent = g_stringContecServerPort;
				}
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
			}
			else
			{
				netConfig.SetPort(443);
				netConfig.SetSsl(true);
			}
			netConfig.SetUrl(stringUrl);
		}
		else
		{
			string stringHTTPMark = "http://";
			if(stringUrl.find(stringHTTPMark) != string::npos)
			{
				stringUrl = stringUrl.substr(stringHTTPMark.size(), stringUrl.size()-stringHTTPMark.size());
			}
			if(stringUrl.find(":")!=string::npos && stringUrl.find(":")<stringUrl.find("/"))
			{
				stringContent = stringUrl.substr(0, stringUrl.find(":"));
				netConfig.SetAddr(stringContent);
				netConfig.SetHost(stringContent);
				stringContent = stringUrl.substr(stringUrl.find(":")+1, stringUrl.find("/")-stringUrl.find(":")-1);
				netConfig.SetPort(CUtil::StringToULong(stringContent));
			}
			else
			{
				stringContent = stringUrl.substr(0, stringUrl.find("/"));
				netConfig.SetAddr(stringContent);
				netConfig.SetHost(stringContent);
				netConfig.SetPort(80);
			}
			netConfig.SetSsl(false);
			stringContent = stringUrl.substr(stringUrl.find("/"), string::npos);
			netConfig.SetUrl(stringContent);
		}

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

int CPhmsSession::GetXmlNetConfig(string stringFilePath, CNetConfig& netConfig, bool bMustSsl)
{
	try
	{
		AutoPtr<IniFileConfiguration> iniFile = new IniFileConfiguration(stringFilePath, "gbk");
		string stringContent;
		unsigned short nContent;
		istringstream iStringStream;

		stringContent = iniFile->getString("NET.XML_SERVER_NAME", "crm.999120.net");
		netConfig.SetAddr(stringContent);
		netConfig.SetHost(stringContent);

		if(!bMustSsl)
		{
			stringContent = iniFile->getString("NET.XML_PORT", "80");
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
		}
		else
		{
			netConfig.SetPort(443);
			netConfig.SetSsl(true);
		}
		switch(m_enumXmlCommandType)
		{
		case SELECT_DOCTOR_TYPE:
			stringContent = iniFile->getString("NET.XML_SELECT_DOCTOR_URI", "/interface/select_doctor_new.aspx");
			break;
		case PAY_VERIFICATION_TYPE:
			stringContent = iniFile->getString("NET.XML_PAY_VERIFICATION_URI", "/interface/pay_verification_new.aspx");
			break;
		case PAY_VERIFICATION_CASE_TYPE:
			stringContent = iniFile->getString("NET.XML_PAY_VERIFICATION_CASE_URI", "/interface/pay_verification_case_new.aspx");
			break;
		default:
			stringContent = "";
			break;
		}
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

int CPhmsSession::GetXmlNetConfig_Telecom(string stringFilePath, CNetConfig& netConfig, bool bMustSsl)
{
	try
	{
		AutoPtr<IniFileConfiguration> iniFile = new IniFileConfiguration(stringFilePath, "gbk");
		string stringContent;
		unsigned short nContent;
		istringstream iStringStream;

		stringContent = iniFile->getString("NET.XML_TELECOM_SERVER", "61.178.98.104");
		netConfig.SetAddr(stringContent);
		netConfig.SetHost(stringContent);

		if(!bMustSsl)
		{
			stringContent = iniFile->getString("NET.XML_TELECOM_PORT", "8088");
			iStringStream.str(stringContent);
			iStringStream>>nContent;
			netConfig.SetPort(nContent);

			stringContent = iniFile->getString("NET.XML_TELECOM_SSL", "0");
			if(stringContent == "1")
			{
				netConfig.SetSsl(true);
			}
			else
			{
				netConfig.SetSsl(false);
			}
		}
		else
		{
			netConfig.SetPort(443);
			netConfig.SetSsl(true);
		}
		switch(m_enumXmlCommandType_Telecom)
		{
		case USER_AUTH:
			stringContent = iniFile->getString("NET.XML_TELECOM_AUTH", "/lecare-sync/user/userAuthorize");
			break;
		case SYNC_BP:
			stringContent = iniFile->getString("NET.XML_TELECOM_BP", "/lecare-sync/syncBloodPress");
			break;
		case SYNC_BO:
			stringContent = iniFile->getString("NET.XML_TELECOM_BO", "/lecare-sync/syncBloodOxygen");
			break;
		case SYNC_HW:
			stringContent = iniFile->getString("NET.XML_TELECOM_HW", "/lecare-sync/syncHeightWeight");
			break;
		case SYNC_BS:
			stringContent = iniFile->getString("NET.XML_TELECOM_BS", "/lecare-sync/syncBloodGlucose");
			break;
		case SYNC_LUNG:
			stringContent = iniFile->getString("NET.XML_TELECOM_LUNG", "/lecare-sync/syncLungfunction");
			break;
		case SYNC_ECG:
			stringContent = iniFile->getString("NET.XML_TELECOM_ECG", "/lecare-sync/syncEcg");
			break;
		}
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

int CPhmsSession::GetXinJieNetConfig(string stringFilePath, CNetConfig& netConfig, bool bMustSsl)
{
	try
	{
		AutoPtr<IniFileConfiguration> iniFile = new IniFileConfiguration(stringFilePath, "gbk");
		string stringContent;
		unsigned short nContent;
		istringstream iStringStream;

		stringContent = iniFile->getString("NET.XINJIE_SERVER_NAME", "36.33.16.12");
		netConfig.SetAddr(stringContent);
		netConfig.SetHost(stringContent);

		if(!bMustSsl)
		{
			stringContent = iniFile->getString("NET.SSL", "0");
			if(stringContent == "1")
			{
				netConfig.SetSsl(true);
			}
			else
			{
				netConfig.SetSsl(false);
			}
			if(netConfig.GetSsl())
			{
				stringContent = iniFile->getString("NET.XINJIE_SERVER_SSL_PORT", "443");
			}
			else
			{
				stringContent = iniFile->getString("NET.XINJIE_SERVER_PORT", "9508");
			}
			iStringStream.str(stringContent);
			iStringStream>>nContent;
			netConfig.SetPort(nContent);
		}
		else
		{
			stringContent = iniFile->getString("NET.XINJIE_SERVER_SSL_PORT", "443");
			iStringStream.str(stringContent);
			iStringStream>>nContent;
			netConfig.SetPort(nContent);
			netConfig.SetSsl(true);
		}
		switch(m_enumXinJieCommandType)
		{
		case VALIDATE_PAY:
			stringContent = iniFile->getString("NET.XINJIE_PAY_VERIFICATION_URI", "/dataSynchronization/checkRrn.do");
			break;
		case NOTIFY_PAY:
			stringContent = iniFile->getString("NET.XINJIE_PAY_NOTIFY_URI", "/dataSynchronization/uploadSuccessfully.do");
			break;
		}
		
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

int CPhmsSession::GetGuanXinNetConfig(string stringFilePath, CNetConfig& netConfig, bool bMustSsl)
{
	try
	{
		AutoPtr<IniFileConfiguration> iniFile = new IniFileConfiguration(stringFilePath, "gbk");
		string stringContent;
		unsigned short nContent;
		istringstream iStringStream;

		//stringContent = iniFile->getString("NET.GUANXIN_SERVER_NAME", "114.247.234.148");
		switch(m_enumGuanXinCommandType)
		{
			case ACCESSTOKEN: 
			case PERSONINFO:
			case SESSION:
			case UPLOADPERSONINFO:
				stringContent = iniFile->getString("NET.GUANXIN_SERVER_NAME", "114.247.234.153");
				break;
			case UPLOADECG:
			case UPLOADNONECG:
			case UPLOADMONITOR:
				stringContent = iniFile->getString("NET.GUANXIN_SERVER_NAME2", "114.247.234.153");
				break;
		}
		
		netConfig.SetAddr(stringContent);
		netConfig.SetHost(stringContent);

// 		if(m_enumGuanXinCommandType != PERSONINFO)
// 		{
// 			stringContent = iniFile->getString("NET.GUANXIN_PORT_TOKEN", "8044");
// 		}
// 		else
// 		{
// 			stringContent = iniFile->getString("NET.GUANXIN_PORT_INFO", "8049");
// 		}
		switch(m_enumGuanXinCommandType)
		{
		case ACCESSTOKEN: 
		case PERSONINFO:
		case SESSION:
		case UPLOADPERSONINFO:
			stringContent = iniFile->getString("NET.GUANXIN_PORT_TOKEN", "8081");
			break;
		case UPLOADECG:
		case UPLOADNONECG:
		case UPLOADMONITOR:
			stringContent = iniFile->getString("NET.GUANXIN_PORT_TOKEN2", "8081");
			break;
		}
		
		iStringStream.str(stringContent);
		iStringStream>>nContent;
		netConfig.SetPort(nContent);
		if(!bMustSsl)
		{
			netConfig.SetSsl(false);
		}
		else
		{
			netConfig.SetSsl(true);
		}
		switch(m_enumGuanXinCommandType)
		{
		case ACCESSTOKEN:
			//stringContent = iniFile->getString("NET.GUANXIN_ACCESSTOKEN_URI", "/auth-service/oauth/2.0/token");
			stringContent = iniFile->getString("NET.GUANXIN_ACCESSTOKEN_URI", "/chss/web/ytjlogin");
			break;
		case PERSONINFO:
			//stringContent = iniFile->getString("NET.GUANXIN_PERSONINFO_URI", "/chss/web/findMember");
			stringContent = iniFile->getString("NET.GUANXIN_PERSONINFO_URI", "/chss/web/findMember");
			break;
		case SESSION:
			//stringContent = iniFile->getString("NET.GUANXIN_SESSION_URI", "/auth-service/oauth/2.0/session");
			break;
		case UPLOADECG:
			stringContent = iniFile->getString("NET.GUANXIN_UPLOADECG_URI", "/ytj/execute/uploadECGData");
			break;
		case UPLOADNONECG:
			stringContent = iniFile->getString("NET.GUANXIN_UPLOADNONECG_URI", "/ytj/execute/uploadOtherData");
			break;
		case UPLOADMONITOR:
			stringContent = iniFile->getString("NET.GUANXIN_UPLOADMONITOR_URI", "/ytj/execute/uploadCheckData");
			break;
		case UPLOADPERSONINFO:
			stringContent = iniFile->getString("NET.GUANXIN_UPLOADPERSONINFO_URI", "/ytj/execute/uploadPersonInfo");
			break;
		}

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

int CPhmsSession::GetXiKangNetConfig(string stringFilePath, CNetConfig& netConfig, bool bMustSsl)
{
	try
	{
		AutoPtr<IniFileConfiguration> iniFile = new IniFileConfiguration(stringFilePath, "gbk");
		string stringContent;
		unsigned short nContent;
		istringstream iStringStream;

		switch(m_enumXiKangCommandType)
		{
		case GET_TOKEN_XIKANG: 
			stringContent = iniFile->getString("NET.XIKANG_TOKEN_SERVER_NAME", "dlpassport.xikang.com");
			break;
		case LOGIN_XIKANG:
		case REGISTER_PATIENT_XIKANG:
		case REGISTER_PATIENT_INFO_XIKANG:
		case BIND_PATIENT_XIKANG:
		case GET_PATIENT_ID_XIKANG:
		case GET_PATIENT_INFO_XIKANG:
		case SEARCH_PATIENT_ID_XIKANG:
			stringContent = iniFile->getString("NET.XIKANG_OPENAPI_SERVER_NAME", "dlopen.xikang.com");
			break;
		case GET_CASE_LIST_XIKANG:
		case SUBMIT_CASE_XIKANG:
		case GET_REPORT_URL_XIKANG:
			stringContent = iniFile->getString("NET.XIKANG_DHEALTH_SERVER_NAME", "xk.xikang.com");
			break;
		case UPLOAD_FILE_XIKANG:
			stringContent = iniFile->getString("NET.XIKANG_UPLOADFILE_SERVER_NAME", "tycyl.xikang.com");
			break;
		default:
			stringContent = iniFile->getString("NET.XIKANG_API_SERVER_NAME", "xxxxx");
			break;
		}

		netConfig.SetAddr(stringContent);
		netConfig.SetHost(stringContent);

		
		switch(m_enumXiKangCommandType)
		{
		case GET_TOKEN_XIKANG:
			stringContent = iniFile->getString("NET.XIKANG_TOKEN_PORT", "80");
			break;
		default:
			stringContent = iniFile->getString("NET.XIAKNG_API_PORT", "80");
			break;
		}

		iStringStream.str(stringContent);
		iStringStream>>nContent;
		netConfig.SetPort(nContent);
		if(!bMustSsl)
		{
			netConfig.SetSsl(false);
		}
		else
		{
			netConfig.SetSsl(true);
		}
		switch(m_enumXiKangCommandType)
		{
		case GET_TOKEN_XIKANG:
			stringContent = iniFile->getString("NET.XIKANG_TOKEN_URI", "/oauth/token");
			break;
		case LOGIN_XIKANG:
			stringContent = iniFile->getString("NET.XIKANG_LOGIN_URI", "/openapi/person/account");
			break;
		case REGISTER_PATIENT_XIKANG:
			stringContent = iniFile->getString("NET.XIKANG_REGISTER_URI", "/openapi/person/member");
			break;
		case REGISTER_PATIENT_INFO_XIKANG:
			stringContent = iniFile->getString("NET.XIKANG_REGISTER_INFO_URI", "/openapi/person/account/{openID}");
			break;
		case BIND_PATIENT_XIKANG:
			stringContent = iniFile->getString("NET.XIKANG_BIND_PATIENT_URL", "/openapi/person/membercaregiver/addMemberCaregiver");
			break;
		case GET_PATIENT_ID_XIKANG:
			stringContent = iniFile->getString("NET.XIKANG_GET_PATIENT_ID_URI", "/openapi/person/membercaregiver/findMemberCaregiver");
			break;
		case SEARCH_PATIENT_ID_XIKANG:
			stringContent = iniFile->getString("NET.XIKANG_SEARCH_PATIENT_ID_URI", "/openapi/person/member/getOpenID");
			break;
		case GET_PATIENT_INFO_XIKANG:
			stringContent = iniFile->getString("NET.XIKANG_GET_PATIENT_INFO_URL", "/openapi/person/member/{openID}");
			break;
		//以下为dHealth接口，暂时未加uploadfile接口
		case GET_CASE_LIST_XIKANG:
			stringContent = iniFile->getString("NET.XIKANG_GET_CASE_LIST_URI", "/hcservice/rest/ecg/ecgCnst");
			break;
		case SUBMIT_CASE_XIKANG:
			stringContent = iniFile->getString("NET.XIKANG_SUBMIT_CASE_URI", "/hcservice/rest/ecg/addEcg");
			break;
		case GET_REPORT_URL_XIKANG:
			stringContent = iniFile->getString("NET.XIKANG_GET_REPORT_URL_URI", "/hcservice/rest/ecg/report/{ecgID}");
			break;
		case UPLOAD_FILE_XIKANG:
			stringContent = iniFile->getString("NET.XIKANG_UPLOAD_FILE_URI", "/kt-tmfs/tmfs/ufile");
			break;
		}

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



int CPhmsSession::GetFormNetConfig(string stringFilePath, CNetConfig& netConfig, string stringHost, string stringUrl, bool bMustSsl)
{
	try
	{
		AutoPtr<IniFileConfiguration> iniFile = new IniFileConfiguration(stringFilePath, "gbk");
		string stringContent;
		unsigned short nContent;
		istringstream iStringStream;

		stringContent = stringHost;
		netConfig.SetAddr(stringContent);
		netConfig.SetHost(stringContent);

		if(!bMustSsl)
		{
			stringContent = iniFile->getString("NET.FORM_PORT", "80");
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
		}
		else
		{
			netConfig.SetPort(443);
			netConfig.SetSsl(true);
		}

		stringContent = stringUrl;
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


int CPhmsSession::GetGWNetConfig(string stringFilePath, CNetConfig& netConfig, bool bMustSsl)
{
	try
	{
		AutoPtr<IniFileConfiguration> iniFile = new IniFileConfiguration(stringFilePath, "gbk");
		string stringContent;
		unsigned short nContent;
		istringstream iStringStream;

		stringContent = iniFile->getString("NET.GW_SERVER_NAME", "127.0.0.1");
		netConfig.SetAddr(stringContent);
		netConfig.SetHost(stringContent);

		if(!bMustSsl)
		{
			stringContent = iniFile->getString("NET.GW_PORT", "80");
			iStringStream.str(stringContent);
			iStringStream>>nContent;
			netConfig.SetPort(nContent);
			netConfig.SetSsl(false);
		}
		else
		{
			stringContent = iniFile->getString("NET.GW_SSL_PORT", "443");
			iStringStream.str(stringContent);
			iStringStream>>nContent;
			netConfig.SetPort(nContent);
			netConfig.SetSsl(true);
		}
		stringContent = iniFile->getString("NET.GW_COMMON_PATH", "/xxxx/");
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

int CPhmsSession::GetDirectlyToHisConfig(string stringFilePath, CNetConfig& netConfig, bool bMustSsl)
{
	try
	{
		AutoPtr<IniFileConfiguration> iniFile = new IniFileConfiguration(stringFilePath, "gbk");
		string stringContent;
		unsigned short nContent;
		istringstream iStringStream;

		stringContent = iniFile->getString("NET.TOHIS_SERVER_NAME", "127.0.0.1");
		netConfig.SetAddr(stringContent);
		netConfig.SetHost(stringContent);

		if(!bMustSsl)
		{
			stringContent = iniFile->getString("NET.TOHIS_PORT", "80");
			iStringStream.str(stringContent);
			iStringStream>>nContent;
			netConfig.SetPort(nContent);
			netConfig.SetSsl(false);
		}
		else
		{
			stringContent = iniFile->getString("NET.TOHIS_SSL_PORT", "443");
			iStringStream.str(stringContent);
			iStringStream>>nContent;
			netConfig.SetPort(nContent);
			netConfig.SetSsl(true);
		}
		stringContent = iniFile->getString("NET.TOHIS_GET_PATIENT_INFO_FROM_HIS", "/xxxx");
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

int CPhmsSession::NoPhmsGetResult(istream* pInputStream, int bContinue)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	switch(m_nSaveType)
	{
	case 0:
		//残余的响应内容也收回来，防止影响下次接收
		nRet = NoPhmsInputStreamToString(pInputStream, m_stringContent);
		break;
	case 1:
		nRet = NoPhmsInputStreamToString(pInputStream, m_stringContent);
		if(nRet != PHMS_SUCCESSFUL_RESULT)
		{
			CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
			return nRet;
		}
		break;
	case 2:
		if(bContinue)
		{
			nRet = NoPhmsInputStreamToFile(pInputStream, m_stringFilePath, false);
		}
		else
		{
			nRet = NoPhmsInputStreamToFile(pInputStream, m_stringFilePath, true);
		}
		if(nRet != PHMS_SUCCESSFUL_RESULT)
		{
			CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
			return nRet;
		}
		break;
	default:
		break;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::NoPhmsInputStreamToString(istream* pInputStream, string& stringContent)
{
	//流形式实现
	ostringstream oStringStream;
	oStringStream.exceptions(ios::failbit|ios::badbit);
	pInputStream->exceptions(ios::badbit);
	try
	{
		StreamCopier::copyStream(*pInputStream, oStringStream);
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

int CPhmsSession::NoPhmsInputStreamToFile(istream* pInputStream, string& stringFilePath, bool bFileClearBeforeWrite)
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
	pInputStream->exceptions(ios::goodbit);
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
	pInputStream->exceptions(ios::badbit);
	ofLocalTargetFile.exceptions(ios::failbit|ios::badbit);
	try
	{
		StreamCopier::copyStream(*pInputStream, ofLocalTargetFile);
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

int CPhmsSession::GetHttpExceptionInfo(int& nCode, string& stringMsgName, string& stringMsgText)
{
	return m_pHttpSession->GetHttpExceptionInfo(nCode, stringMsgName, stringMsgText);
}

int CPhmsSession::AbortSession()
{
	Mutex::ScopedLock lock(m_mutexAbortAndSslSwitch);
	return m_pHttpSession->AbortSession();
}

//public member function
int CPhmsSession::Login(string stringRole, string stringUsername, string stringPassword, string stringLanguage, string stringToken, bool bMustSsl)
{
	AutoPtr<Document> pDoc = new Document;
	////AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildRole = pDoc->createElement("role");
	AutoPtr<Element> pChildLoginId = pDoc->createElement("loginid");
	AutoPtr<Element> pChildPwd = pDoc->createElement("pwd");
	AutoPtr<Element> pChildLanguage = pDoc->createElement("lang");
	AutoPtr<Element> pChildToken = pDoc->createElement("token");
	AutoPtr<Text> pTextRole = pDoc->createTextNode(stringRole);
	AutoPtr<Text> pTextUsername = pDoc->createTextNode(stringUsername);
	AutoPtr<Text> pTextPassword = pDoc->createTextNode(stringPassword);
	AutoPtr<Text> pTextLanguage = pDoc->createTextNode(stringLanguage);
	AutoPtr<Text> pTextToken = pDoc->createTextNode(stringToken);
	pChildRole->appendChild(pTextRole);
	pChildLoginId->appendChild(pTextUsername);
	pChildPwd->appendChild(pTextPassword);
	pChildLanguage->appendChild(pTextLanguage);
	pChildToken->appendChild(pTextToken);
	pRoot->appendChild(pChildRole);
	pRoot->appendChild(pChildLoginId);
	pRoot->appendChild(pChildPwd);
	pRoot->appendChild(pChildLanguage);
	pRoot->appendChild(pChildToken);
	////pDoc->appendChild(pi);
	pDoc->appendChild(pRoot);

	string stringXmlContent;
	int nRet = PHMS_SUCCESSFUL_RESULT;
	nRet = this->GetRequestXml(pDoc, stringXmlContent);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}

	//构造PhmsRequest
	string stringSessionId(32, '0');
	CPhmsRequestHead phmsRequestHead("10", "1002", stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	//为什么登录的签名是32个0呢？？？？？？？？
	//因为服务器计算签名需要在session中保存由用户名和密码计算出来的md5，调登录时接口未登录，所以签名为32个0
	//phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	m_enumPhmsCommandType = LOGIN_TYPE;//目前只有登录命令是LOGIN_TYPE，改变个人信息命令是CHANGE_INFO_TYPE，用户注册命令是REGISTER_TYPE
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse, bMustSsl);
	m_enumPhmsCommandType = MAIN_TYPE;//其余命令都是MAIN_TYPE
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	//只有登录命令才需要，保存当前在线的用户名和密码，无论是否能正常登录，这样能保证GetResult中验证签名的时候能够正常计算签名
	CPhmsRequest::SetUsername(stringUsername);
	CPhmsRequest::SetPassword(stringPassword);
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	//只有登录(1002)和注册并登录(1086)命令才需要，解析返回的xml，得到sessionid
	nRet = this->GetSessionId(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	CSessionIdManager::HandleSessionSync(m_stringSessionId);
	return PHMS_SUCCESSFUL_RESULT;
}


int CPhmsSession::MechineRegister(string stringCountry, string stringAreaId, string stringArea, 
					string stringDevice, string stringSignature, string stringSerialnum)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildCountry = pDoc->createElement("country");
	AutoPtr<Element> pChildAreaId = pDoc->createElement("areaid");
	AutoPtr<Element> pChildArea = pDoc->createElement("area");
	AutoPtr<Element> pChildDevice = pDoc->createElement("device");
	AutoPtr<Element> pChildSignature = pDoc->createElement("signature");
	AutoPtr<Element> pChildSerialnum = pDoc->createElement("serialnum");
	AutoPtr<Text> pTextCountry = pDoc->createTextNode(stringCountry);
	AutoPtr<Text> pTextAreaId = pDoc->createTextNode(stringAreaId);
	AutoPtr<Text> pTextArea = pDoc->createTextNode(stringArea);
	AutoPtr<Text> pTextDevice = pDoc->createTextNode(stringDevice);
	AutoPtr<Text> pTextSignature = pDoc->createTextNode(stringSignature);
	AutoPtr<Text> pTextSerialnum = pDoc->createTextNode(stringSerialnum);
	pChildCountry->appendChild(pTextCountry);
	pChildAreaId->appendChild(pTextAreaId);
	pChildArea->appendChild(pTextArea);
	pChildDevice->appendChild(pTextDevice);
	pChildSignature->appendChild(pTextSignature);
	pChildSerialnum->appendChild(pTextSerialnum);
	pRoot->appendChild(pChildCountry);
	pRoot->appendChild(pChildAreaId);
	pRoot->appendChild(pChildArea);
	pRoot->appendChild(pChildDevice);
	pRoot->appendChild(pChildSignature);
	pRoot->appendChild(pChildSerialnum);
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
	//pDoc->collectGarbage();

	string stringSessionId(32, '0');
	CPhmsRequestHead phmsRequestHead("10", "1001", stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
//	phmsRequest.GenerateAndSetSign();//签名为32个的原因同登录接口一样
	CPhmsResponse phmsResponse;
	m_enumPhmsCommandType = REGISTER_TYPE;//目前只有登录命令是LOGIN_TYPE，改变个人信息命令是CHANGE_INFO_TYPE，用户注册命令是REGISTER_TYPE
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	m_enumPhmsCommandType = MAIN_TYPE;//其余都是MAIN_TYPE
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::Logout()
{
	CPhmsRequestHead phmsRequestHead("10", "1003", m_stringSessionId, "10");
	CPhmsRequestBody phmsRequestBody("");
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	int nRet = PHMS_SUCCESSFUL_RESULT;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	m_stringSessionId = "";
	CSessionIdManager::HandleSessionSync("");
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::SetDoctorSign(string stringMd5, string stringFilePath)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildMd5 = pDoc->createElement("md5");
	AutoPtr<Text> pTextMd5 = pDoc->createTextNode(stringMd5);
	pChildMd5->appendChild(pTextMd5);
	pRoot->appendChild(pChildMd5);
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

	CPhmsRequestHead phmsRequestHead("10", "1004", m_stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, stringFilePath);
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::SetDoctorPhoto(string stringMd5, string stringFilePath)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildMd5 = pDoc->createElement("md5");
	AutoPtr<Text> pTextMd5 = pDoc->createTextNode(stringMd5);
	pChildMd5->appendChild(pTextMd5);
	pRoot->appendChild(pChildMd5);
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

	CPhmsRequestHead phmsRequestHead("10", "1005", m_stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, stringFilePath);
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::ApplyNewCase(string stringName, string stringThirdId, string stringDataType, string stringDataTypeName, string stringNotes, string stringHospitalId,
				 string stringHospitalName, string stringDoctorId, string stringDoctorName, string stringCheckTime, string stringOtherParams,string stringDeviceId,
				 string stringDeviceName, string stringMd5, string stringCaseOrigin, string stringYzlb, string stringSex, string stringApplicationNo, string stringAutoMatic)
{
// 	CPhmsLogger::GetPhmsLogger()->WriteLog("stringName:"+stringName, __FUNCTION__, __FILE__, __LINE__);
// 	CPhmsLogger::GetPhmsLogger()->WriteLog("stringThirdId:"+stringThirdId, __FUNCTION__, __FILE__, __LINE__);
// 	CPhmsLogger::GetPhmsLogger()->WriteLog("stringDataType:"+stringDataType, __FUNCTION__, __FILE__, __LINE__);
// 	CPhmsLogger::GetPhmsLogger()->WriteLog("stringDataTypeName:"+stringDataTypeName, __FUNCTION__, __FILE__, __LINE__);
// 	CPhmsLogger::GetPhmsLogger()->WriteLog("stringNotes:"+stringNotes, __FUNCTION__, __FILE__, __LINE__);
// 	CPhmsLogger::GetPhmsLogger()->WriteLog("stringHospitalId:"+stringHospitalId, __FUNCTION__, __FILE__, __LINE__);
// 	CPhmsLogger::GetPhmsLogger()->WriteLog("stringHospitalName:"+stringHospitalName, __FUNCTION__, __FILE__, __LINE__);
// 	CPhmsLogger::GetPhmsLogger()->WriteLog("stringDoctorId:"+stringDoctorId, __FUNCTION__, __FILE__, __LINE__);
// 	CPhmsLogger::GetPhmsLogger()->WriteLog("stringDoctorName:"+stringDoctorName, __FUNCTION__, __FILE__, __LINE__);
// 	CPhmsLogger::GetPhmsLogger()->WriteLog("stringCheckTime:"+stringCheckTime, __FUNCTION__, __FILE__, __LINE__);
// 	CPhmsLogger::GetPhmsLogger()->WriteLog("stringOtherParams:"+stringOtherParams, __FUNCTION__, __FILE__, __LINE__);
// 	CPhmsLogger::GetPhmsLogger()->WriteLog("stringDeviceId:"+stringDeviceId, __FUNCTION__, __FILE__, __LINE__);
// 	CPhmsLogger::GetPhmsLogger()->WriteLog("stringDeviceName:"+stringDeviceName, __FUNCTION__, __FILE__, __LINE__);
// 	CPhmsLogger::GetPhmsLogger()->WriteLog("stringMd5:"+stringMd5, __FUNCTION__, __FILE__, __LINE__);
// 	CPhmsLogger::GetPhmsLogger()->WriteLog("stringCaseOrigin:"+stringCaseOrigin, __FUNCTION__, __FILE__, __LINE__);
// 	CPhmsLogger::GetPhmsLogger()->WriteLog("stringYzlb:"+stringYzlb, __FUNCTION__, __FILE__, __LINE__);
// 	CPhmsLogger::GetPhmsLogger()->WriteLog("stringSex:"+stringSex, __FUNCTION__, __FILE__, __LINE__);
// 	CPhmsLogger::GetPhmsLogger()->WriteLog("stringApplicationNo:"+stringApplicationNo, __FUNCTION__, __FILE__, __LINE__);
// 	CPhmsLogger::GetPhmsLogger()->WriteLog("stringAutoMatic:"+stringAutoMatic, __FUNCTION__, __FILE__, __LINE__);
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildName = pDoc->createElement("name");
	AutoPtr<Element> pChildThirdId = pDoc->createElement("thirdid");
	AutoPtr<Element> pChildDataType = pDoc->createElement("datatype");
	AutoPtr<Element> pChildDataTypeName = pDoc->createElement("datatypename");
	AutoPtr<Element> pChildNotes = pDoc->createElement("notes");
	AutoPtr<Element> pChildHospitalId = pDoc->createElement("hospitalid");
	AutoPtr<Element> pChildHospitalName = pDoc->createElement("hname");
	AutoPtr<Element> pChildDoctorId = pDoc->createElement("doctorid");
	AutoPtr<Element> pChildDoctorName = pDoc->createElement("dname");
	AutoPtr<Element> pChildCheckTime = pDoc->createElement("checktime");
	AutoPtr<Element> pChildOtherParams = pDoc->createElement("otherparams");
	AutoPtr<Element> pChildDeviceId = pDoc->createElement("deviceid");
	AutoPtr<Element> pChildDeviceName = pDoc->createElement("devicename");
	AutoPtr<Element> pChildMd5 = pDoc->createElement("md5");
	AutoPtr<Element> pChildCaseOrigin = pDoc->createElement("caseorigin");
	AutoPtr<Element> pChildYzlb = pDoc->createElement("yzlb");
	AutoPtr<Element> pChildSex =  pDoc->createElement("sex");
	AutoPtr<Element> pChildApplicationNo = pDoc->createElement("applicationno");
	AutoPtr<Element> pChildAutoMatic = pDoc->createElement("isautomatic");
	AutoPtr<Text> pTextName = pDoc->createTextNode(stringName);
	AutoPtr<Text> pTextThirdId = pDoc->createTextNode(stringThirdId);
	AutoPtr<Text> pTextDataType = pDoc->createTextNode(stringDataType);
	AutoPtr<Text> pTextDataTypeName = pDoc->createTextNode(stringDataTypeName);
	AutoPtr<Text> pTextNotes = pDoc->createTextNode(stringNotes);
	AutoPtr<Text> pTextHospitalId = pDoc->createTextNode(stringHospitalId);
	AutoPtr<Text> pTextHospitalName = pDoc->createTextNode(stringHospitalName);
	AutoPtr<Text> pTextDoctorId = pDoc->createTextNode(stringDoctorId);
	AutoPtr<Text> pTextDoctorName = pDoc->createTextNode(stringDoctorName);
	AutoPtr<Text> pTextCheckTime = pDoc->createTextNode(stringCheckTime);

// 	//stringOtherParams参数更改xml编码声明，目的是编码统一
// 	string stringWorkingDir = CUtil::GetCurrentAppDir();
// 	string stringIniFilePath = stringWorkingDir+"PhmsConfig.ini";
// 	AutoPtr<IniFileConfiguration> iniFile = new IniFileConfiguration(stringIniFilePath, "gbk");
// 	string stringEncoding = iniFile->getString("OTHER.Encoding", "GBK");
// 	string stringSrc = "encoding=\"";
// 	stringSrc += stringEncoding;
// 	stringSrc += "\"?>";
// 	string stringDst = "encoding=\"UTF-8\"?>";
// 	CUtil::ReplaceString(stringOtherParams, stringSrc, stringDst);

	AutoPtr<Text> pTextOtherParams = pDoc->createTextNode(stringOtherParams);
	AutoPtr<Text> pTextDeviceId = pDoc->createTextNode(stringDeviceId);
	AutoPtr<Text> pTextDeviceName = pDoc->createTextNode(stringDeviceName);
	AutoPtr<Text> pTextMd5 = pDoc->createTextNode(stringMd5);
	AutoPtr<Text> pTextCaseOrigin = pDoc->createTextNode(stringCaseOrigin);
	AutoPtr<Text> pTextYzlb = pDoc->createTextNode(stringYzlb);
	AutoPtr<Text> pTextSex = pDoc->createTextNode(stringSex);
	AutoPtr<Text> pTextApplicationNo = pDoc->createTextNode(stringApplicationNo);
	AutoPtr<Text> pTextAutoMatic = pDoc->createTextNode(stringAutoMatic);
	pChildName->appendChild(pTextName);
	pChildThirdId->appendChild(pTextThirdId);
	pChildDataType->appendChild(pTextDataType);
	pChildDataTypeName->appendChild(pTextDataTypeName);
	pChildNotes->appendChild(pTextNotes);
	pChildHospitalId->appendChild(pTextHospitalId);
	pChildHospitalName->appendChild(pTextHospitalName);
	pChildDoctorId->appendChild(pTextDoctorId);
	pChildDoctorName->appendChild(pTextDoctorName);
	pChildCheckTime->appendChild(pTextCheckTime);
	pChildOtherParams->appendChild(pTextOtherParams);
	pChildDeviceId->appendChild(pTextDeviceId);
	pChildDeviceName->appendChild(pTextDeviceName);
	pChildMd5->appendChild(pTextMd5);
	pChildCaseOrigin->appendChild(pTextCaseOrigin);
	pChildYzlb->appendChild(pTextYzlb);
	pChildSex->appendChild(pTextSex);
	pChildApplicationNo->appendChild(pTextApplicationNo);
	pChildAutoMatic->appendChild(pTextAutoMatic);
	pRoot->appendChild(pChildName);
	pRoot->appendChild(pChildThirdId);
	pRoot->appendChild(pChildDataType);
	pRoot->appendChild(pChildDataTypeName);
	pRoot->appendChild(pChildNotes);
	pRoot->appendChild(pChildHospitalId);
	pRoot->appendChild(pChildHospitalName);
	pRoot->appendChild(pChildDoctorId);
	pRoot->appendChild(pChildDoctorName);
	pRoot->appendChild(pChildCheckTime);
	pRoot->appendChild(pChildOtherParams);
	pRoot->appendChild(pChildDeviceId);
	pRoot->appendChild(pChildDeviceName);
	pRoot->appendChild(pChildMd5);
	pRoot->appendChild(pChildCaseOrigin);
	pRoot->appendChild(pChildYzlb);
	pRoot->appendChild(pChildSex);
	pRoot->appendChild(pChildApplicationNo);
	pRoot->appendChild(pChildAutoMatic);
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

	CPhmsRequestHead phmsRequestHead("10", "1006", m_stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::UploadCaseSegment(string stringCaseId, string stringOrder, string stringTotalSize, string stringServerUri, string stringStart1, 
					  string stringEnd1, string stringMd51, string stringStart2, string stringEnd2, string stringMd52, string stringStart3, 
					  string stringEnd3,string stringMd53, string stringFilePath)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildCaseId = pDoc->createElement("caseid");
	AutoPtr<Element> pChildOrder = pDoc->createElement("order");
	AutoPtr<Element> pChildTotalSize = pDoc->createElement("totalsize");
	AutoPtr<Element> pChildServerUri = pDoc->createElement("serveruri");
	AutoPtr<Element> pChildPart1 = pDoc->createElement("md51");
	AutoPtr<Element> pGrandChildStart1= pDoc->createElement("start");
	AutoPtr<Element> pGrandChildEnd1 = pDoc->createElement("end");
	AutoPtr<Element> pGrandChildMd51 = pDoc->createElement("md5");
	AutoPtr<Element> pChildPart2 = pDoc->createElement("md52");
	AutoPtr<Element> pGrandChildStart2= pDoc->createElement("start");
	AutoPtr<Element> pGrandChildEnd2 = pDoc->createElement("end");
	AutoPtr<Element> pGrandChildMd52 = pDoc->createElement("md5");
	AutoPtr<Element> pChildPart3 = pDoc->createElement("md53");
	AutoPtr<Element> pGrandChildStart3= pDoc->createElement("start");
	AutoPtr<Element> pGrandChildEnd3 = pDoc->createElement("end");
	AutoPtr<Element> pGrandChildMd53 = pDoc->createElement("md5");
	AutoPtr<Text> pTextCaseId = pDoc->createTextNode(stringCaseId);
	AutoPtr<Text> pTextOrder = pDoc->createTextNode(stringOrder);
	AutoPtr<Text> pTextTotalSize = pDoc->createTextNode(stringTotalSize);
	AutoPtr<Text> pTextServerUri = pDoc->createTextNode(stringServerUri);
	AutoPtr<Text> pTextStart1 = pDoc->createTextNode(stringStart1);
	AutoPtr<Text> pTextEnd1 = pDoc->createTextNode(stringEnd1);
	AutoPtr<Text> pTextMd51 = pDoc->createTextNode(stringMd51);
	AutoPtr<Text> pTextStart2 = pDoc->createTextNode(stringStart2);
	AutoPtr<Text> pTextEnd2 = pDoc->createTextNode(stringEnd2);
	AutoPtr<Text> pTextMd52 = pDoc->createTextNode(stringMd52);
	AutoPtr<Text> pTextStart3 = pDoc->createTextNode(stringStart3);
	AutoPtr<Text> pTextEnd3 = pDoc->createTextNode(stringEnd3);
	AutoPtr<Text> pTextMd53 = pDoc->createTextNode(stringMd53);
	pChildCaseId->appendChild(pTextCaseId);
	pChildOrder->appendChild(pTextOrder);
	pChildTotalSize->appendChild(pTextTotalSize);
	pChildServerUri->appendChild(pTextServerUri);
	pGrandChildStart1->appendChild(pTextStart1);
	pGrandChildEnd1->appendChild(pTextEnd1);
	pGrandChildMd51->appendChild(pTextMd51);
	pGrandChildStart2->appendChild(pTextStart2);
	pGrandChildEnd2->appendChild(pTextEnd2);
	pGrandChildMd52->appendChild(pTextMd52);
	pGrandChildStart3->appendChild(pTextStart3);
	pGrandChildEnd3->appendChild(pTextEnd3);
	pGrandChildMd53->appendChild(pTextMd53);
	pChildPart1->appendChild(pGrandChildStart1);
	pChildPart1->appendChild(pGrandChildEnd1);
	pChildPart1->appendChild(pGrandChildMd51);
	pChildPart2->appendChild(pGrandChildStart2);
	pChildPart2->appendChild(pGrandChildEnd2);
	pChildPart2->appendChild(pGrandChildMd52);
	pChildPart3->appendChild(pGrandChildStart3);
	pChildPart3->appendChild(pGrandChildEnd3);
	pChildPart3->appendChild(pGrandChildMd53);
	pRoot->appendChild(pChildCaseId);
	pRoot->appendChild(pChildOrder);
	pRoot->appendChild(pChildTotalSize);
	pRoot->appendChild(pChildServerUri);
	pRoot->appendChild(pChildPart1);
	pRoot->appendChild(pChildPart2);
	pRoot->appendChild(pChildPart3);
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

	CPhmsRequestHead phmsRequestHead("10", "1007", m_stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, stringFilePath);
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::CaseHoldPlace(string stringCaseId, string stringReceiverId)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildCase = pDoc->createElement("case");
	AutoPtr<Element> pGrandChildCaseId = pDoc->createElement("caseid");
	AutoPtr<Element> pGrandChildReceiverId = pDoc->createElement("receiverid");
	AutoPtr<Text> pTextCaseId = pDoc->createTextNode(stringCaseId);
	AutoPtr<Text> pTextReceiverId = pDoc->createTextNode(stringReceiverId);
	pGrandChildCaseId->appendChild(pTextCaseId);
	pGrandChildReceiverId->appendChild(pTextReceiverId);
	pChildCase->appendChild(pGrandChildCaseId);
	pChildCase->appendChild(pGrandChildReceiverId);
	pRoot->appendChild(pChildCase);
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

	CPhmsRequestHead phmsRequestHead("10", "1008", m_stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::GetAutoDownloadCase(string stringCaseType,string stringSenderId,string stringCaseOrigin)
{

	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");

	AutoPtr<Element> pChildcasetype = pDoc->createElement("casetype");
	AutoPtr<Element> pChildsenderid = pDoc->createElement("senderid");
	AutoPtr<Element> pChildcaseorigin = pDoc->createElement("caseorigin");

	AutoPtr<Text> pTextcasetype = pDoc->createTextNode(stringCaseType);
	AutoPtr<Text> pTextsenderid = pDoc->createTextNode(stringSenderId);
	AutoPtr<Text> pTextcaseorigin = pDoc->createTextNode(stringCaseOrigin);

	pChildcasetype->appendChild(pTextcasetype);
	pChildsenderid->appendChild(pTextsenderid);
	pChildcaseorigin->appendChild(pTextcaseorigin);

	pRoot->appendChild(pChildcasetype);
	pRoot->appendChild(pChildsenderid);
	pRoot->appendChild(pChildcaseorigin);

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

	CPhmsRequestHead phmsRequestHead("10", "1009", m_stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::DownloadCaseBegin(string stringCaseId, string stringReceiverId)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildCaseId = pDoc->createElement("caseid");
	AutoPtr<Element> pChildReceiverId = pDoc->createElement("receiverid");
	AutoPtr<Text> pTextCaseId = pDoc->createTextNode(stringCaseId);
	AutoPtr<Text> pTextReceiverId = pDoc->createTextNode(stringReceiverId);
	pChildCaseId->appendChild(pTextCaseId);
	pChildReceiverId->appendChild(pTextReceiverId);
	pRoot->appendChild(pChildCaseId);
	pRoot->appendChild(pChildReceiverId);
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

	CPhmsRequestHead phmsRequestHead("10", "1010", m_stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::DownloadFileSegment(string stringUri, string stringStart, string stringEnd)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildUri = pDoc->createElement("uri");
	AutoPtr<Element> pChildStart = pDoc->createElement("start");
	AutoPtr<Element> pChildEnd = pDoc->createElement("end");
	AutoPtr<Text> pTextUri = pDoc->createTextNode(stringUri);
	AutoPtr<Text> pTextStart = pDoc->createTextNode(stringStart);
	AutoPtr<Text> pTextEnd = pDoc->createTextNode(stringEnd);
	pChildUri->appendChild(pTextUri);
	pChildStart->appendChild(pTextStart);
	pChildEnd->appendChild(pTextEnd);
	pRoot->appendChild(pChildUri);
	pRoot->appendChild(pChildStart);
	pRoot->appendChild(pChildEnd);
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

	CPhmsRequestHead phmsRequestHead("10", "1011", m_stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath, false, false);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::DownloadCaseComplete(string stringCaseId, string stringReceiverId)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildCaseId = pDoc->createElement("caseid");
	AutoPtr<Element> pChildReceiverId = pDoc->createElement("receiverid");
	AutoPtr<Text> pTextCaseId = pDoc->createTextNode(stringCaseId);
	AutoPtr<Text> pTextReceiverId = pDoc->createTextNode(stringReceiverId);
	pChildCaseId->appendChild(pTextCaseId);
	pChildReceiverId->appendChild(pTextReceiverId);
	pRoot->appendChild(pChildCaseId);
	pRoot->appendChild(pChildReceiverId);
	//pDoc->appendChild(pi);
	pDoc->appendChild(pRoot);

	string stringXmlContent;
	int nRet = PHMS_SUCCESSFUL_RESULT;
	nRet = this->GetRequestXml(pDoc, stringXmlContent);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		return nRet;
	}

	CPhmsRequestHead phmsRequestHead("10", "1012", m_stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::InvalidCase(string stringCaseId, string stringReceiverId, string stringReason)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildCaseId = pDoc->createElement("caseid");
	AutoPtr<Element> pChildReceiverId = pDoc->createElement("receiverid");
	AutoPtr<Element> pChildReason = pDoc->createElement("reason");
	AutoPtr<Text> pTextCaseId = pDoc->createTextNode(stringCaseId);
	AutoPtr<Text> pTextReceiverId = pDoc->createTextNode(stringReceiverId);
	AutoPtr<Text> pTextReason = pDoc->createTextNode(stringReason);
	pChildCaseId->appendChild(pTextCaseId);
	pChildReceiverId->appendChild(pTextReceiverId);
	pChildReason->appendChild(pTextReason);
	pRoot->appendChild(pChildCaseId);
	pRoot->appendChild(pChildReceiverId);
	pRoot->appendChild(pChildReason);
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

	CPhmsRequestHead phmsRequestHead("10", "1013", m_stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::TransmitCase(string stringCaseId, string stringReceiverId, string stringForwardToH, string stringForwardToHospitalName, 
							   string stringForwandToD, string stringForwardToDoctorName)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildCaseId = pDoc->createElement("caseid");
	AutoPtr<Element> pChildReceiverId = pDoc->createElement("receiverid");
	AutoPtr<Element> pChildForwardToH = pDoc->createElement("forwardtoh");
	AutoPtr<Element> pChildForwardToHospitalName = pDoc->createElement("forwardtohn");
	AutoPtr<Element> pChildForwardToD = pDoc->createElement("forwardtod");
	AutoPtr<Element> pChildForwardToDoctorName = pDoc->createElement("forwardtodn");
	AutoPtr<Text> pTextCaseId = pDoc->createTextNode(stringCaseId);
	AutoPtr<Text> pTextReceiverId = pDoc->createTextNode(stringReceiverId);
	AutoPtr<Text> pTextForwardToH = pDoc->createTextNode(stringForwardToH);
	AutoPtr<Text> pTextForwardToHospitalName = pDoc->createTextNode(stringForwardToHospitalName);
	AutoPtr<Text> pTextForwardToD = pDoc->createTextNode(stringForwandToD);
	AutoPtr<Text> pTextForwardToDoctorName = pDoc->createTextNode(stringForwardToDoctorName);
	pChildCaseId->appendChild(pTextCaseId);
	pChildReceiverId->appendChild(pTextReceiverId);
	pChildForwardToH->appendChild(pTextForwardToH);
	pChildForwardToHospitalName->appendChild(pTextForwardToHospitalName);
	pChildForwardToD->appendChild(pTextForwardToD);
	pChildForwardToDoctorName->appendChild(pTextForwardToDoctorName);
	pRoot->appendChild(pChildCaseId);
	pRoot->appendChild(pChildReceiverId);
	pRoot->appendChild(pChildForwardToH);
	pRoot->appendChild(pChildForwardToHospitalName);
	pRoot->appendChild(pChildForwardToD);
	pRoot->appendChild(pChildForwardToDoctorName);
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

	CPhmsRequestHead phmsRequestHead("10", "1014", m_stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::Rollback(string stringCaseId, string stringReceiverId, string stringBackTo)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildCaseId = pDoc->createElement("caseid");
	AutoPtr<Element> pChildReceiverId = pDoc->createElement("receiverid");
	AutoPtr<Element> pChildBackTo = pDoc->createElement("backto");
	AutoPtr<Text> pTextCaseId = pDoc->createTextNode(stringCaseId);
	AutoPtr<Text> pTextReceiverId = pDoc->createTextNode(stringReceiverId);
	AutoPtr<Text> pTextBackTo = pDoc->createTextNode(stringBackTo);
	pChildCaseId->appendChild(pTextCaseId);
	pChildReceiverId->appendChild(pTextReceiverId);
	pChildBackTo->appendChild(pTextBackTo);
	pRoot->appendChild(pChildCaseId);
	pRoot->appendChild(pChildReceiverId);
	pRoot->appendChild(pChildBackTo);
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

	CPhmsRequestHead phmsRequestHead("10", "1015", m_stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::GetHospitalListOfGroup(string stringGroupId, string stringType, string stringCaseType, string stringPageTo, string stringPageSize)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildGroupId = pDoc->createElement("hgroupid");
	AutoPtr<Element> pChildType = pDoc->createElement("type");
	AutoPtr<Element> pChildCaseType = pDoc->createElement("casetype");
	AutoPtr<Element> pChildPageTo = pDoc->createElement("pageto");
	AutoPtr<Element> pChilePageSize = pDoc->createElement("pagesize");
	AutoPtr<Text> pTextGroupId = pDoc->createTextNode(stringGroupId);
	AutoPtr<Text> pTextType = pDoc->createTextNode(stringType);
	AutoPtr<Text> pTextCaseType = pDoc->createTextNode(stringCaseType);
	AutoPtr<Text> pTextPageTo = pDoc->createTextNode(stringPageTo);
	AutoPtr<Text> pTextPageSize = pDoc->createTextNode(stringPageSize);
	pChildGroupId->appendChild(pTextGroupId);
	pChildType->appendChild(pTextType);
	pChildCaseType->appendChild(pTextCaseType);
	pChildPageTo->appendChild(pTextPageTo);
	pChilePageSize->appendChild(pTextPageSize);
	pRoot->appendChild(pChildGroupId);
	pRoot->appendChild(pChildType);
	pRoot->appendChild(pChildCaseType);
	pRoot->appendChild(pChildPageTo);
	pRoot->appendChild(pChilePageSize);
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

	CPhmsRequestHead phmsRequestHead("10", "1016", m_stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::GetDoctorListOfHospital(string stringHospitalId, string stringDoctorId, string stringCaseType, string stringDoctorType, string stringPageTo, string stringPageSize)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildHospitalId = pDoc->createElement("hospitalid");
	AutoPtr<Element> pChildDoctorId = pDoc->createElement("doctorid");
	AutoPtr<Element> pChildCaseType = pDoc->createElement("casetype");
	AutoPtr<Element> pChildDoctorType = pDoc->createElement("doctortype");
	AutoPtr<Element> pChildPageTo = pDoc->createElement("pageto");
	AutoPtr<Element> pChilePageSize = pDoc->createElement("pagesize");
	AutoPtr<Text> pTextHospitalId = pDoc->createTextNode(stringHospitalId);
	AutoPtr<Text> pTextDoctorId = pDoc->createTextNode(stringDoctorId);
	AutoPtr<Text> pTextCaseType = pDoc->createTextNode(stringCaseType);
	AutoPtr<Text> pTextDoctorType = pDoc->createTextNode(stringDoctorType);
	AutoPtr<Text> pTextPageTo = pDoc->createTextNode(stringPageTo);
	AutoPtr<Text> pTextPageSize = pDoc->createTextNode(stringPageSize);
	pChildHospitalId->appendChild(pTextHospitalId);
	pChildDoctorId->appendChild(pTextDoctorId);
	pChildCaseType->appendChild(pTextCaseType);
	pChildDoctorType->appendChild(pTextDoctorType);
	pChildPageTo->appendChild(pTextPageTo);
	pChilePageSize->appendChild(pTextPageSize);
	pRoot->appendChild(pChildHospitalId);
	pRoot->appendChild(pChildDoctorId);
	pRoot->appendChild(pChildCaseType);
	pRoot->appendChild(pChildDoctorType);
	pRoot->appendChild(pChildPageTo);
	pRoot->appendChild(pChilePageSize);
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

	CPhmsRequestHead phmsRequestHead("10", "1017", m_stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::AnalyzeBegin(string stringCaseId, string stringReceiverId, string stringCaseType)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildCaseId = pDoc->createElement("caseid");
	AutoPtr<Element> pChildReceiverId = pDoc->createElement("receiverid");
	AutoPtr<Element> pChildCaseType = pDoc->createElement("casetype");
	AutoPtr<Text> pTextCaseId = pDoc->createTextNode(stringCaseId);
	AutoPtr<Text> pTextReceiverId = pDoc->createTextNode(stringReceiverId);
	AutoPtr<Text> pTextCaseType = pDoc->createTextNode(stringCaseType);
	pChildCaseId->appendChild(pTextCaseId);
	pChildReceiverId->appendChild(pTextReceiverId);
	pChildCaseType->appendChild(pTextCaseType);
	pRoot->appendChild(pChildCaseId);
	pRoot->appendChild(pChildReceiverId);
	pRoot->appendChild(pChildCaseType);
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

	CPhmsRequestHead phmsRequestHead("10", "1018", m_stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::AnalyzeComplete(string stringCaseId, string stringReceiverId, string stringCaseType)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildCaseId = pDoc->createElement("caseid");
	AutoPtr<Element> pChildReceiverId = pDoc->createElement("receiverid");
	AutoPtr<Element> pChildCaseType = pDoc->createElement("casetype");
	AutoPtr<Text> pTextCaseId = pDoc->createTextNode(stringCaseId);
	AutoPtr<Text> pTextReceiverId = pDoc->createTextNode(stringReceiverId);
	AutoPtr<Text> pTextCaseType = pDoc->createTextNode(stringCaseType);
	pChildCaseId->appendChild(pTextCaseId);
	pChildReceiverId->appendChild(pTextReceiverId);
	pChildCaseType->appendChild(pTextCaseType);
	pRoot->appendChild(pChildCaseId);
	pRoot->appendChild(pChildReceiverId);
	pRoot->appendChild(pChildCaseType);
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

	CPhmsRequestHead phmsRequestHead("10", "1019", m_stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::ApplyNewReport(string stringCaseId, string stringReceiverId, string stringDiagnosis, string stringMd5, string stringDigitalSign, string stringTimestampSign, string stringCaseType)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildCaseId = pDoc->createElement("caseid");
	AutoPtr<Element> pChildReceiverId = pDoc->createElement("receiverid");
	AutoPtr<Element> pChildDiagnosis = pDoc->createElement("diagnostic");
	AutoPtr<Element> pChildMd5 = pDoc->createElement("md5");
	AutoPtr<Element> pChildDigitalSign = pDoc->createElement("digitalsign");
	AutoPtr<Element> pChildTimestampSign = pDoc->createElement("timestampsign");
	AutoPtr<Element> pChildCaseType = pDoc->createElement("casetype");
	AutoPtr<Text> pTextCaseId = pDoc->createTextNode(stringCaseId);
	AutoPtr<Text> pTextReceiverId = pDoc->createTextNode(stringReceiverId);

// 	//stringDiagnosis参数更改xml编码声明，目的是编码统一
// 	string stringWorkingDir = CUtil::GetCurrentAppDir();
// 	string stringIniFilePath = stringWorkingDir+"PhmsConfig.ini";
// 	AutoPtr<IniFileConfiguration> iniFile = new IniFileConfiguration(stringIniFilePath, "gbk");
// 	string stringEncoding = iniFile->getString("OTHER.Encoding", "GBK");
// 	string stringSrc = "encoding=\"";
// 	stringSrc += stringEncoding;
// 	stringSrc += "\"?>";
// 	string stringDst = "encoding=\"UTF-8\"?>";
// 	CUtil::ReplaceString(stringDiagnosis, stringSrc, stringDst);

	AutoPtr<Text> pTextDiagnosis = pDoc->createTextNode(stringDiagnosis);
	AutoPtr<Text> pTextMd5 = pDoc->createTextNode(stringMd5);
	AutoPtr<Text> pTextDigitalSign = pDoc->createTextNode(stringDigitalSign);
	AutoPtr<Text> pTextTimestampSign = pDoc->createTextNode(stringTimestampSign);
	AutoPtr<Text> pTextCaseType = pDoc->createTextNode(stringCaseType);
	pChildCaseId->appendChild(pTextCaseId);
	pChildReceiverId->appendChild(pTextReceiverId);
	pChildDiagnosis->appendChild(pTextDiagnosis);
	pChildMd5->appendChild(pTextMd5);
	pChildDigitalSign->appendChild(pTextDigitalSign);
	pChildTimestampSign->appendChild(pTextTimestampSign);
	pChildCaseType->appendChild(pTextCaseType);
	pRoot->appendChild(pChildCaseId);
	pRoot->appendChild(pChildReceiverId);
	pRoot->appendChild(pChildDiagnosis);
	pRoot->appendChild(pChildMd5);
	pRoot->appendChild(pChildDigitalSign);
	pRoot->appendChild(pChildTimestampSign);
	pRoot->appendChild(pChildCaseType);
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

	CPhmsRequestHead phmsRequestHead("10", "1020", m_stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::UploadReportSegment(string stringCaseId, string stringReceiverId, string stringCaseType, string stringReportId, string stringOrder, string stringTotalSize, 
						string stringServerUri, string stringStart1, string stringEnd1, string stringMd51, string stringStart2, string stringEnd2, 
						string stringMd52, string stringStart3, string stringEnd3, string stringMd53, string stringFilePath, string stringFileType)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildCaseId = pDoc->createElement("caseid");
	AutoPtr<Element> pChildReceiverId = pDoc->createElement("receiverid");
	AutoPtr<Element> pChildCaseType = pDoc->createElement("casetype");
	AutoPtr<Element> pChildReportId = pDoc->createElement("reportid");
	AutoPtr<Element> pChildOrder = pDoc->createElement("order");
	AutoPtr<Element> pChildTotalSize = pDoc->createElement("totalsize");
	AutoPtr<Element> pChildServerUri = pDoc->createElement("serveruri");
	AutoPtr<Element> pChildPart1 = pDoc->createElement("md51");
	AutoPtr<Element> pGrandChildStart1= pDoc->createElement("start");
	AutoPtr<Element> pGrandChildEnd1 = pDoc->createElement("end");
	AutoPtr<Element> pGrandChildMd51 = pDoc->createElement("md5");
	AutoPtr<Element> pChildPart2 = pDoc->createElement("md52");
	AutoPtr<Element> pGrandChildStart2= pDoc->createElement("start");
	AutoPtr<Element> pGrandChildEnd2 = pDoc->createElement("end");
	AutoPtr<Element> pGrandChildMd52 = pDoc->createElement("md5");
	AutoPtr<Element> pChildPart3 = pDoc->createElement("md53");
	AutoPtr<Element> pGrandChildStart3= pDoc->createElement("start");
	AutoPtr<Element> pGrandChildEnd3 = pDoc->createElement("end");
	AutoPtr<Element> pGrandChildMd53 = pDoc->createElement("md5");
	AutoPtr<Element> pChildFileType = pDoc->createElement("filetype");
	AutoPtr<Text> pTextCaseId = pDoc->createTextNode(stringCaseId);
	AutoPtr<Text> pTextReceiverId = pDoc->createTextNode(stringReceiverId);
	AutoPtr<Text> pTextCaseType = pDoc->createTextNode(stringCaseType);
	AutoPtr<Text> pTextReportId = pDoc->createTextNode(stringReportId);
	AutoPtr<Text> pTextOrder = pDoc->createTextNode(stringOrder);
	AutoPtr<Text> pTextTotalSize = pDoc->createTextNode(stringTotalSize);
	AutoPtr<Text> pTextServerUri = pDoc->createTextNode(stringServerUri);
	AutoPtr<Text> pTextStart1 = pDoc->createTextNode(stringStart1);
	AutoPtr<Text> pTextEnd1 = pDoc->createTextNode(stringEnd1);
	AutoPtr<Text> pTextMd51 = pDoc->createTextNode(stringMd51);
	AutoPtr<Text> pTextStart2 = pDoc->createTextNode(stringStart2);
	AutoPtr<Text> pTextEnd2 = pDoc->createTextNode(stringEnd2);
	AutoPtr<Text> pTextMd52 = pDoc->createTextNode(stringMd52);
	AutoPtr<Text> pTextStart3 = pDoc->createTextNode(stringStart3);
	AutoPtr<Text> pTextEnd3 = pDoc->createTextNode(stringEnd3);
	AutoPtr<Text> pTextMd53 = pDoc->createTextNode(stringMd53);
	AutoPtr<Text> pTextFileType =  pDoc->createTextNode(stringFileType);
	pChildCaseId->appendChild(pTextCaseId);
	pChildReceiverId->appendChild(pTextReceiverId);
	pChildCaseType->appendChild(pTextCaseType);
	pChildReportId->appendChild(pTextReportId);
	pChildOrder->appendChild(pTextOrder);
	pChildTotalSize->appendChild(pTextTotalSize);
	pChildServerUri->appendChild(pTextServerUri);
	pGrandChildStart1->appendChild(pTextStart1);
	pGrandChildEnd1->appendChild(pTextEnd1);
	pGrandChildMd51->appendChild(pTextMd51);
	pGrandChildStart2->appendChild(pTextStart2);
	pGrandChildEnd2->appendChild(pTextEnd2);
	pGrandChildMd52->appendChild(pTextMd52);
	pGrandChildStart3->appendChild(pTextStart3);
	pGrandChildEnd3->appendChild(pTextEnd3);
	pGrandChildMd53->appendChild(pTextMd53);
	pChildPart1->appendChild(pGrandChildStart1);
	pChildPart1->appendChild(pGrandChildEnd1);
	pChildPart1->appendChild(pGrandChildMd51);
	pChildPart2->appendChild(pGrandChildStart2);
	pChildPart2->appendChild(pGrandChildEnd2);
	pChildPart2->appendChild(pGrandChildMd52);
	pChildPart3->appendChild(pGrandChildStart3);
	pChildPart3->appendChild(pGrandChildEnd3);
	pChildPart3->appendChild(pGrandChildMd53);
	pChildFileType->appendChild(pTextFileType);
	pRoot->appendChild(pChildCaseId);
	pRoot->appendChild(pChildReceiverId);
	pRoot->appendChild(pChildCaseType);
	pRoot->appendChild(pChildReportId);
	pRoot->appendChild(pChildOrder);
	pRoot->appendChild(pChildTotalSize);
	pRoot->appendChild(pChildServerUri);
	pRoot->appendChild(pChildPart1);
	pRoot->appendChild(pChildPart2);
	pRoot->appendChild(pChildPart3);
	pRoot->appendChild(pChildFileType);
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

	CPhmsRequestHead phmsRequestHead("10", "1021", m_stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, stringFilePath);
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::DownloadReportBegin(string stringCaseId, string stringReportId)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildCaseId = pDoc->createElement("caseid");
	AutoPtr<Element> pChildReportId = pDoc->createElement("reportid");
	AutoPtr<Text> pTextCaseId = pDoc->createTextNode(stringCaseId);
	AutoPtr<Text> pTextReportId = pDoc->createTextNode(stringReportId);
	pChildCaseId->appendChild(pTextCaseId);
	pChildReportId->appendChild(pTextReportId);
	pRoot->appendChild(pChildCaseId);
	pRoot->appendChild(pChildReportId);
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

	CPhmsRequestHead phmsRequestHead("10", "1022", m_stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::DownloadReportComplete(string stringCaseId, string stringReportId)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildCaseId = pDoc->createElement("caseid");
	AutoPtr<Element> pChildReportId = pDoc->createElement("reportid");
	AutoPtr<Text> pTextCaseId = pDoc->createTextNode(stringCaseId);
	AutoPtr<Text> pTextReportId = pDoc->createTextNode(stringReportId);
	pChildCaseId->appendChild(pTextCaseId);
	pChildReportId->appendChild(pTextReportId);
	pRoot->appendChild(pChildCaseId);
	pRoot->appendChild(pChildReportId);
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

	CPhmsRequestHead phmsRequestHead("10", "1023", m_stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::GetAlreadyUploadFileSize(string stringServerUri)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildServerUri = pDoc->createElement("serveruri");
	AutoPtr<Text> pTextServerUri = pDoc->createTextNode(stringServerUri);
	pChildServerUri->appendChild(pTextServerUri);
	pRoot->appendChild(pChildServerUri);
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

	CPhmsRequestHead phmsRequestHead("10", "1024", m_stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::GetReviewReportList(string stringReportStartDate, string stringReportEndDate, string stringReviewStartDate, string stringReviewEndDate, 
						string stringReviewState, string stringCaseType, string stringPtName, string stringSenderId, string stringYzlb, string stringAId, string stringReviewerId, 
						string stringReceiverId, string stringCaseId, string stringPageTo, string stringPageSize)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildReportStartDate = pDoc->createElement("reportstartdate");
	AutoPtr<Element> pChildReportEndDate = pDoc->createElement("reportenddate");
	AutoPtr<Element> pChildReviewStartDate = pDoc->createElement("reviewstartdate");
	AutoPtr<Element> pChildReviewEndDate = pDoc->createElement("reviewenddate");
	AutoPtr<Element> pChildReviewState = pDoc->createElement("reviewstate");
	AutoPtr<Element> pChildCaseType = pDoc->createElement("casetype");
	AutoPtr<Element> pChildPtName = pDoc->createElement("ptname");
	AutoPtr<Element> pChildSenderId = pDoc->createElement("senderid");
	AutoPtr<Element> pChildYzlb = pDoc->createElement("yzlb");
	AutoPtr<Element> pChildAId = pDoc->createElement("applicationno");
	AutoPtr<Element> pChildReviewerId = pDoc->createElement("reviewerid");
	AutoPtr<Element> pChildReceiverId = pDoc->createElement("receiverid");
	AutoPtr<Element> pChildCaseId = pDoc->createElement("caseid");
	AutoPtr<Element> pChildPageTo = pDoc->createElement("pageto");
	AutoPtr<Element> pChildPageSize = pDoc->createElement("pagesize");
	AutoPtr<Text> pTextReportStartDate = pDoc->createTextNode(stringReportStartDate);
	AutoPtr<Text> pTextReportEndDate = pDoc->createTextNode(stringReportEndDate);
	AutoPtr<Text> pTextReviewStartDate = pDoc->createTextNode(stringReviewStartDate);
	AutoPtr<Text> pTextReviewEndDate = pDoc->createTextNode(stringReviewEndDate);
	AutoPtr<Text> pTextReviewState = pDoc->createTextNode(stringReviewState);
	AutoPtr<Text> pTextCaseType = pDoc->createTextNode(stringCaseType);
	AutoPtr<Text> pTextPtName = pDoc->createTextNode(stringPtName);
	AutoPtr<Text> pTextSenderId = pDoc->createTextNode(stringSenderId);
	AutoPtr<Text> pTextYzlb = pDoc->createTextNode(stringYzlb);
	AutoPtr<Text> pTextAId = pDoc->createTextNode(stringAId);
	AutoPtr<Text> pTextReviewerId = pDoc->createTextNode(stringReviewerId);
	AutoPtr<Text> pTextReceiverId = pDoc->createTextNode(stringReceiverId);
	AutoPtr<Text> pTextCaseId = pDoc->createTextNode(stringCaseId);
	AutoPtr<Text> pTextPageTo = pDoc->createTextNode(stringPageTo);
	AutoPtr<Text> pTextPageSize = pDoc->createTextNode(stringPageSize);
	pChildReportStartDate->appendChild(pTextReportStartDate);
	pChildReportEndDate->appendChild(pTextReportEndDate);
	pChildReviewStartDate->appendChild(pTextReviewStartDate);
	pChildReviewEndDate->appendChild(pTextReviewEndDate);
	pChildReviewState->appendChild(pTextReviewState);
	pChildCaseType->appendChild(pTextCaseType);
	pChildPtName->appendChild(pTextPtName);
	pChildSenderId->appendChild(pTextSenderId);
	pChildYzlb->appendChild(pTextYzlb);
	pChildAId->appendChild(pTextAId);
	pChildReviewerId->appendChild(pTextReviewerId);
	pChildReceiverId->appendChild(pTextReceiverId);
	pChildCaseId->appendChild(pTextCaseId);
	pChildPageTo->appendChild(pTextPageTo);
	pChildPageSize->appendChild(pTextPageSize);
	pRoot->appendChild(pChildReportStartDate);
	pRoot->appendChild(pChildReportEndDate);
	pRoot->appendChild(pChildReviewStartDate);
	pRoot->appendChild(pChildReviewEndDate);
	pRoot->appendChild(pChildReviewState);
	pRoot->appendChild(pChildCaseType);
	pRoot->appendChild(pChildPtName);
	pRoot->appendChild(pChildSenderId);
	pRoot->appendChild(pChildYzlb);
	pRoot->appendChild(pChildAId);
	pRoot->appendChild(pChildReviewerId);
	pRoot->appendChild(pChildReceiverId);
	pRoot->appendChild(pChildCaseId);
	pRoot->appendChild(pChildPageTo);
	pRoot->appendChild(pChildPageSize);
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

	CPhmsRequestHead phmsRequestHead("10", "1025", m_stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::ReviewReport(string stringReportId, string stringReviewState, string stringPath, string stringDiagnostic, 
							   string stringDigitalSign, string stringTimeStampSign)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildReportId = pDoc->createElement("reportid");
	AutoPtr<Element> pChildReviewState = pDoc->createElement("reviewstate");
	AutoPtr<Element> pChildPath = pDoc->createElement("path");
	AutoPtr<Element> pChildDiagnostic = pDoc->createElement("diagnostic");
	AutoPtr<Element> pChildDigitalSign = pDoc->createElement("digitalsign");
	AutoPtr<Element> pChildTimeStampSign = pDoc->createElement("timestampsign");
	AutoPtr<Text> pTextReportId = pDoc->createTextNode(stringReportId);
	AutoPtr<Text> pTextReviewState = pDoc->createTextNode(stringReviewState);
	AutoPtr<Text> pTextPath = pDoc->createTextNode(stringPath);
	AutoPtr<Text> pTextDiagnostic = pDoc->createTextNode(stringDiagnostic);
	AutoPtr<Text> pTextDigitalSign = pDoc->createTextNode(stringDigitalSign);
	AutoPtr<Text> pTextTimeStampSign = pDoc->createTextNode(stringTimeStampSign);
	pChildReportId->appendChild(pTextReportId);
	pChildReviewState->appendChild(pTextReviewState);
	pChildPath->appendChild(pTextPath);
	pChildDiagnostic->appendChild(pTextDiagnostic);
	pChildDigitalSign->appendChild(pTextDigitalSign);
	pChildTimeStampSign->appendChild(pTextTimeStampSign);
	pRoot->appendChild(pChildReportId);
	pRoot->appendChild(pChildReviewState);
	pRoot->appendChild(pChildPath);
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

	CPhmsRequestHead phmsRequestHead("10", "1026", m_stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::GetReportStateOfCase(string stringCaseId)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildCaseId = pDoc->createElement("caseid");
	AutoPtr<Text> pTextCaseId = pDoc->createTextNode(stringCaseId);
	pChildCaseId->appendChild(pTextCaseId);
	pRoot->appendChild(pChildCaseId);
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

	CPhmsRequestHead phmsRequestHead("10", "1027", m_stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::GetCaseListOfDoctor(string stringCaseId, string stringCaseType, string stringCaseSource, string stringCaseState, string stringPatientName, 
				string stringSenderId, string stringReceiverId, string stringStartDate, string stringEndDate, string stringCheckStartDate, string stringCheckEndDate, 
				string stringSenderName, string stringDiagnostic, string stringCaseOrigin, string stringYzlb, string stringAId, string stringSex, string stringOrderBy, string stringThirdId, 
				string stringLang, string stringIsFullName, string stringPageTo, string stringPageSize)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildCaseId = pDoc->createElement("caseid");
	AutoPtr<Element> pChildCaseType = pDoc->createElement("casetype");
	AutoPtr<Element> pChildCaseSource = pDoc->createElement("casesource");
	AutoPtr<Element> pChildCaseState = pDoc->createElement("casestate");
	AutoPtr<Element> pChildPatientName = pDoc->createElement("ptname");
	AutoPtr<Element> pChildSenderId = pDoc->createElement("senderid");
	AutoPtr<Element> pChildReceiverId = pDoc->createElement("receiverid");
	AutoPtr<Element> pChildStartDate = pDoc->createElement("startdate");
	AutoPtr<Element> pChildEndDate = pDoc->createElement("enddate");
	AutoPtr<Element> pChildCheckStartDate = pDoc->createElement("checkstartdate");
	AutoPtr<Element> pChildCheckEndDate = pDoc->createElement("checkenddate");
	AutoPtr<Element> pChildSenderName = pDoc->createElement("sendername");
	AutoPtr<Element> pChildDiagnostic = pDoc->createElement("diagnostic");
	AutoPtr<Element> pChildCaseOrigin = pDoc->createElement("caseorigin");
	AutoPtr<Element> pChildYzlb = pDoc->createElement("yzlb");
	AutoPtr<Element> pChildAId = pDoc->createElement("applicationno");
	AutoPtr<Element> pChildSex = pDoc->createElement("sex");
	AutoPtr<Element> pChildOrderBy = pDoc->createElement("orderby");
	AutoPtr<Element> pChildThirdId = pDoc->createElement("thirdid");
	AutoPtr<Element> pChildLang = pDoc->createElement("lang");
	AutoPtr<Element> PChildIsFullName = pDoc->createElement("isfullname");
	AutoPtr<Element> pChildPageTo = pDoc->createElement("pageto");
	AutoPtr<Element> pChildPageSize = pDoc->createElement("pagesize");
	AutoPtr<Text> pTextCaseId = pDoc->createTextNode(stringCaseId);
	AutoPtr<Text> pTextCaseType = pDoc->createTextNode(stringCaseType);
	AutoPtr<Text> pTextCaseSource = pDoc->createTextNode(stringCaseSource);
	AutoPtr<Text> pTextCaseState = pDoc->createTextNode(stringCaseState);
	AutoPtr<Text> pTextPatientName = pDoc->createTextNode(stringPatientName);
	AutoPtr<Text> pTextSenderId = pDoc->createTextNode(stringSenderId);
	AutoPtr<Text> pTextReceiverId = pDoc->createTextNode(stringReceiverId);
	AutoPtr<Text> pTextStartDate = pDoc->createTextNode(stringStartDate);
	AutoPtr<Text> pTextEndDate = pDoc->createTextNode(stringEndDate);
	AutoPtr<Text> pTextCheckStartDate = pDoc->createTextNode(stringCheckStartDate);
	AutoPtr<Text> pTextCheckEndDate = pDoc->createTextNode(stringCheckEndDate);
	AutoPtr<Text> pTextSenderName = pDoc->createTextNode(stringSenderName);
	AutoPtr<Text> pTextDiagnostic = pDoc->createTextNode(stringDiagnostic);
	AutoPtr<Text> pTextCaseOrigin = pDoc->createTextNode(stringCaseOrigin);
	AutoPtr<Text> pTextYzlb = pDoc->createTextNode(stringYzlb);
	AutoPtr<Text> pTextAId = pDoc->createTextNode(stringAId);
	AutoPtr<Text> pTextSex = pDoc->createTextNode(stringSex);
	AutoPtr<Text> pTextOrderBy = pDoc->createTextNode(stringOrderBy);
	AutoPtr<Text> pTextThirdId = pDoc->createTextNode(stringThirdId);
	AutoPtr<Text> pTextLang = pDoc->createTextNode(stringLang);
	AutoPtr<Text> PTextIsFullName = pDoc->createTextNode(stringIsFullName);
	AutoPtr<Text> pTextPageTo = pDoc->createTextNode(stringPageTo);
	AutoPtr<Text> pTextPageSize = pDoc->createTextNode(stringPageSize);
	pChildCaseId->appendChild(pTextCaseId);
	pChildCaseType->appendChild(pTextCaseType);
	pChildCaseSource->appendChild(pTextCaseSource);
	pChildCaseState->appendChild(pTextCaseState);
	pChildPatientName->appendChild(pTextPatientName);
	pChildSenderId->appendChild(pTextSenderId);
	pChildReceiverId->appendChild(pTextReceiverId);
	pChildStartDate->appendChild(pTextStartDate);
	pChildEndDate->appendChild(pTextEndDate);
	pChildCheckStartDate->appendChild(pTextCheckStartDate);
	pChildCheckEndDate->appendChild(pTextCheckEndDate);
	pChildSenderName->appendChild(pTextSenderName);
	pChildDiagnostic->appendChild(pTextDiagnostic);
	pChildCaseOrigin->appendChild(pTextCaseOrigin);
	pChildYzlb->appendChild(pTextYzlb);
	pChildAId->appendChild(pTextAId);
	pChildSex->appendChild(pTextSex);
	pChildOrderBy->appendChild(pTextOrderBy);
	pChildThirdId->appendChild(pTextThirdId);
	pChildLang->appendChild(pTextLang);
	PChildIsFullName->appendChild(PTextIsFullName);
	pChildPageTo->appendChild(pTextPageTo);
	pChildPageSize->appendChild(pTextPageSize);
	pRoot->appendChild(pChildCaseId);
	pRoot->appendChild(pChildCaseType);
	pRoot->appendChild(pChildCaseSource);
	pRoot->appendChild(pChildCaseState);
	pRoot->appendChild(pChildPatientName);
	pRoot->appendChild(pChildSenderId);
	pRoot->appendChild(pChildReceiverId);
	pRoot->appendChild(pChildStartDate);
	pRoot->appendChild(pChildEndDate);
	pRoot->appendChild(pChildCheckStartDate);
	pRoot->appendChild(pChildCheckEndDate);
	pRoot->appendChild(pChildSenderName);
	pRoot->appendChild(pChildDiagnostic);
	pRoot->appendChild(pChildCaseOrigin);
	pRoot->appendChild(pChildYzlb);
	pRoot->appendChild(pChildAId);
	pRoot->appendChild(pChildSex);
	pRoot->appendChild(pChildOrderBy);
	pRoot->appendChild(pChildThirdId);
	pRoot->appendChild(pChildLang);
	pRoot->appendChild(PChildIsFullName);
	pRoot->appendChild(pChildPageTo);
	pRoot->appendChild(pChildPageSize);
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

	CPhmsRequestHead phmsRequestHead("10", "1028", m_stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::GetHospitalInfo(string stringHospitalId)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildHospitalId = pDoc->createElement("hospitalid");
	AutoPtr<Text> pTextHospitalId = pDoc->createTextNode(stringHospitalId);
	pChildHospitalId->appendChild(pTextHospitalId);
	pRoot->appendChild(pChildHospitalId);
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

	CPhmsRequestHead phmsRequestHead("10", "1029", m_stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::UploadIm(string stringSenderId, string stringSenderName, string stringReceiverId, string stringMsgDirection, string stringMsgType, 
			 string stringMsgContent, string stringCaseId, string stringCaseType)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildSenderId = pDoc->createElement("senderid");
	AutoPtr<Element> pChildSenderName = pDoc->createElement("sendername");
	AutoPtr<Element> pChildReceiverId = pDoc->createElement("receiverid");
	AutoPtr<Element> pChildMsgDirection = pDoc->createElement("msgdirection");
	AutoPtr<Element> pChildMsgType = pDoc->createElement("msgtype");
	AutoPtr<Element> pChildMstContent = pDoc->createElement("msgcontent");
	AutoPtr<Element> pChildCaseId = pDoc->createElement("caseid");
	AutoPtr<Element> pChildCaseType = pDoc->createElement("casetype");
	AutoPtr<Text> pTextSenderId = pDoc->createTextNode(stringSenderId);
	AutoPtr<Text> pTextSenderName = pDoc->createTextNode(stringSenderName);
	AutoPtr<Text> pTextReceiverId = pDoc->createTextNode(stringReceiverId);
	AutoPtr<Text> pTextMsgDirection = pDoc->createTextNode(stringMsgDirection);
	AutoPtr<Text> pTextMsgType = pDoc->createTextNode(stringMsgType);
	AutoPtr<Text> pTextMsgContent = pDoc->createTextNode(stringMsgContent);
	AutoPtr<Text> pTextCaseId = pDoc->createTextNode(stringCaseId);
	AutoPtr<Text> pTextCaseType = pDoc->createTextNode(stringCaseType);
	pChildSenderId->appendChild(pTextSenderId);
	pChildSenderName->appendChild(pTextSenderName);
	pChildReceiverId->appendChild(pTextReceiverId);
	pChildMsgDirection->appendChild(pTextMsgDirection);
	pChildMsgType->appendChild(pTextMsgType);
	pChildMstContent->appendChild(pTextMsgContent);
	pChildCaseId->appendChild(pTextCaseId);
	pChildCaseType->appendChild(pTextCaseType);
	pRoot->appendChild(pChildSenderId);
	pRoot->appendChild(pChildSenderName);
	pRoot->appendChild(pChildReceiverId);
	pRoot->appendChild(pChildMsgDirection);
	pRoot->appendChild(pChildMsgType);
	pRoot->appendChild(pChildMstContent);
	pRoot->appendChild(pChildCaseId);
	pRoot->appendChild(pChildCaseType);
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

	CPhmsRequestHead phmsRequestHead("10", "1030", m_stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::GetImList(string stringSenderId, string stringCaseId, string stringCaseType, string stringMsgDirection, string stringMsgType)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildSenderId = pDoc->createElement("senderid");
	AutoPtr<Element> pChildCaseId = pDoc->createElement("caseid");
	AutoPtr<Element> pChildCaseType = pDoc->createElement("casetype");
	AutoPtr<Element> pChildMsgType = pDoc->createElement("msgtype");
	AutoPtr<Element> pChildMsgDirection = pDoc->createElement("msgdirection");
	AutoPtr<Text> pTextSenderId = pDoc->createTextNode(stringSenderId);
	AutoPtr<Text> pTextCaseId = pDoc->createTextNode(stringCaseId);
	AutoPtr<Text> pTextCaseType = pDoc->createTextNode(stringCaseType);
	AutoPtr<Text> pTextMsgType = pDoc->createTextNode(stringMsgType);
	AutoPtr<Text> pTextMsgDirection = pDoc->createTextNode(stringMsgDirection);
	pChildSenderId->appendChild(pTextSenderId);
	pChildCaseId->appendChild(pTextCaseId);
	pChildCaseType->appendChild(pTextCaseType);
	pChildMsgType->appendChild(pTextMsgType);
	pChildMsgDirection->appendChild(pTextMsgDirection);
	pRoot->appendChild(pChildSenderId);
	pRoot->appendChild(pChildCaseId);
	pRoot->appendChild(pChildCaseType);
	pRoot->appendChild(pChildMsgType);
	pRoot->appendChild(pChildMsgDirection);
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
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::GetImList2(string stringNextId)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildNextId = pDoc->createElement("nextid");
	AutoPtr<Element> pChildVersion = pDoc->createElement("version");
	AutoPtr<Text> pTextNextId = pDoc->createTextNode(stringNextId);
	AutoPtr<Text> pTextVersion = pDoc->createTextNode("2");
	pChildNextId->appendChild(pTextNextId);
	pChildVersion->appendChild(pTextVersion);
	pRoot->appendChild(pChildNextId);
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
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::ImDownloadComplete(string stringId)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildId = pDoc->createElement("id");
	AutoPtr<Text> pTextId = pDoc->createTextNode(stringId);
	pChildId->appendChild(pTextId);
	pRoot->appendChild(pChildId);
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

	CPhmsRequestHead phmsRequestHead("10", "1032", m_stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::SendPhoneMsg(string stringCaseId, string stringPhone, string stringMsg)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildCaseId = pDoc->createElement("caseid");
	AutoPtr<Element> pChildPhone = pDoc->createElement("phone");
	AutoPtr<Element> pChildMsg = pDoc->createElement("msg");
	AutoPtr<Text> pTextCaseId = pDoc->createTextNode(stringCaseId);
	AutoPtr<Text> pTextPhone = pDoc->createTextNode(stringPhone);
	AutoPtr<Text> pTextMsg = pDoc->createTextNode(stringMsg);
	pChildCaseId->appendChild(pTextCaseId);
	pChildPhone->appendChild(pTextPhone);
	pChildMsg->appendChild(pTextMsg);
	pRoot->appendChild(pChildCaseId);
	pRoot->appendChild(pChildPhone);
	pRoot->appendChild(pChildMsg);
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

	CPhmsRequestHead phmsRequestHead("10", "1033", m_stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::UploadTrendData(string stringCardId, string stringDeviceId, string stringDeviceName, string stringCaseId, vector<RECORD_DATA> vRecordData)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildCardId = pDoc->createElement("cardid");
	AutoPtr<Element> pChildDeviceId = pDoc->createElement("deviceid");
	AutoPtr<Element> pChildDeviceName = pDoc->createElement("devicename");
	AutoPtr<Element> pChildCaseId = pDoc->createElement("caseid");
	AutoPtr<Text> pTextCardId = pDoc->createTextNode(stringCardId);
	AutoPtr<Text> pTextDeviceId = pDoc->createTextNode(stringDeviceId);
	AutoPtr<Text> pTextDeviceName = pDoc->createTextNode(stringDeviceName);
	AutoPtr<Text> pTextCaseId = pDoc->createTextNode(stringCaseId);
	pChildCardId->appendChild(pTextCardId);
	pChildDeviceId->appendChild(pTextDeviceId);
	pChildDeviceName->appendChild(pTextDeviceName);
	pChildCaseId->appendChild(pTextCaseId);
	pRoot->appendChild(pChildCardId);
	pRoot->appendChild(pChildDeviceId);
	pRoot->appendChild(pChildDeviceName);
	pRoot->appendChild(pChildCaseId);
	//构造pChildRecord
	vector<RECORD_DATA>::const_iterator iter;
	for(iter=vRecordData.begin(); iter!=vRecordData.end(); iter++)
	{
		AutoPtr<Element> pChildRecord = pDoc->createElement("record");

		AutoPtr<Element> pGrandHr = pDoc->createElement("hr");
		AutoPtr<Element> pGrandWeight = pDoc->createElement("weight");
		AutoPtr<Element> pGrandHeight = pDoc->createElement("height");
		AutoPtr<Element> pGrandTemp = pDoc->createElement("temp");
		AutoPtr<Element> pGrandResp = pDoc->createElement("resp");
		AutoPtr<Element> pGrandChol = pDoc->createElement("chol");
		AutoPtr<Element> pGrandUa = pDoc->createElement("ua");

		AutoPtr<Element> pGrandBloodSugar = pDoc->createElement("bloodsugar");
		AutoPtr<Element> pGrand2BloodSugar = pDoc->createElement("bloodsugar");
		AutoPtr<Element> pGrand2Flag = pDoc->createElement("flag");

		AutoPtr<Element> pGrandSpo2 = pDoc->createElement("spo2");
		AutoPtr<Element> pGrand2Spo2 = pDoc->createElement("spo2");
		AutoPtr<Element> pGrand2Pr = pDoc->createElement("pr");

		AutoPtr<Element> pGrandBp = pDoc->createElement("bp");
		AutoPtr<Element> pGrand2Sys = pDoc->createElement("sys");
		AutoPtr<Element> pGrand2Mean = pDoc->createElement("mean");
		AutoPtr<Element> pGrand2Dia = pDoc->createElement("dia");

		AutoPtr<Element> pGrandFetal = pDoc->createElement("fetal");
		AutoPtr<Element> pGrand2FetalHeartRate = pDoc->createElement("fetalheartrate");
		AutoPtr<Element> pGrand2PalacePressure = pDoc->createElement("palacepressure");
		AutoPtr<Element> pGrand2Quickening = pDoc->createElement("quickening");

		AutoPtr<Element> pGrandFvc = pDoc->createElement("fvc");
		AutoPtr<Element> pGrand2Fvc = pDoc->createElement("fvc");
		AutoPtr<Element> pGrand2Fev1 = pDoc->createElement("fev1");
		AutoPtr<Element> pGrand2Pef = pDoc->createElement("pef");
		AutoPtr<Element> pGrand2Fev1Rate = pDoc->createElement("fev1rate");
		AutoPtr<Element> pGrand2Fef25 = pDoc->createElement("fef25");
		AutoPtr<Element> pGrand2Fef2575 = pDoc->createElement("fef2575");
		AutoPtr<Element> pGrand2Fef75 = pDoc->createElement("fef75");

		AutoPtr<Element> pGrandPedometer = pDoc->createElement("pedometer");
		AutoPtr<Element> pGrand2Steps = pDoc->createElement("steps");
		AutoPtr<Element> pGrand2Distance = pDoc->createElement("distance");
		AutoPtr<Element> pGrand2Calories = pDoc->createElement("calories");

		AutoPtr<Element> pGrandUrine = pDoc->createElement("urine");
		AutoPtr<Element> pGrand2Value = pDoc->createElement("value");
		AutoPtr<Element> pGrand2ValueAdd = pDoc->createElement("valueadd");
		AutoPtr<Element> pGrand2ValueNew = pDoc->createElement("valuenew");

		AutoPtr<Element> pGrandHrConclusion = pDoc->createElement("hrconclusion");
		AutoPtr<Element> pGrand2HrValue = pDoc->createElement("value");
		AutoPtr<Element> pGrand2Conclusion = pDoc->createElement("conclusion");

		AutoPtr<Element> pGrandBloodTest = pDoc->createElement("blood_test");
		AutoPtr<Element> pGrand2CHOL = pDoc->createElement("CHOL");
		AutoPtr<Element> pGrand2HDL = pDoc->createElement("HDL");
		AutoPtr<Element> pGrand2TG = pDoc->createElement("TG");
		AutoPtr<Element> pGrand2LDL = pDoc->createElement("LDL");
		AutoPtr<Element> pGrand2HB = pDoc->createElement("HB");
		AutoPtr<Element> pGrand2HCT = pDoc->createElement("HCT");

		AutoPtr<Element> pGrandChekcTime = pDoc->createElement("checktime");

		AutoPtr<Text> pTextHr = pDoc->createTextNode(CUtil::DoubleToString(iter->hr));
		AutoPtr<Text> pTextWeight = pDoc->createTextNode(CUtil::DoubleToString(iter->weight));
		AutoPtr<Text> pTextHeight = pDoc->createTextNode(CUtil::DoubleToString(iter->height));
		AutoPtr<Text> pTextTemp = pDoc->createTextNode(CUtil::DoubleToString(iter->temp));
		AutoPtr<Text> pTextResp = pDoc->createTextNode(CUtil::DoubleToString(iter->resp));
		AutoPtr<Text> pTextChol = pDoc->createTextNode(CUtil::DoubleToString(iter->chol));
		AutoPtr<Text> pTextUa = pDoc->createTextNode(CUtil::DoubleToString(iter->ua));
		AutoPtr<Text> pTextBloodSugar = pDoc->createTextNode(CUtil::DoubleToString(iter->bloodsugerData.bloodsuger));
		AutoPtr<Text> pTextFlag = pDoc->createTextNode(iter->bloodsugerData.flag);
		AutoPtr<Text> pTextSpo2 = pDoc->createTextNode(CUtil::DoubleToString(iter->spo2Data.spo2));
		AutoPtr<Text> pTextPr = pDoc->createTextNode(CUtil::DoubleToString(iter->spo2Data.pr));
		AutoPtr<Text> pTextSys = pDoc->createTextNode(CUtil::DoubleToString(iter->bpData.sys));
		AutoPtr<Text> pTextMean = pDoc->createTextNode(CUtil::DoubleToString(iter->bpData.mean));
		AutoPtr<Text> pTextDia = pDoc->createTextNode(CUtil::DoubleToString(iter->bpData.dia));
		AutoPtr<Text> pTextFetalHeartRate = pDoc->createTextNode(CUtil::DoubleToString(iter->fetalData.fetalHeartRate));
		AutoPtr<Text> pTextPalacePressure = pDoc->createTextNode(CUtil::DoubleToString(iter->fetalData.palacePressure));
		AutoPtr<Text> pTextQuickening = pDoc->createTextNode(CUtil::DoubleToString(iter->fetalData.quickening));
		AutoPtr<Text> pTextFvc = pDoc->createTextNode(CUtil::DoubleToString(iter->fvcData.fvc));
		AutoPtr<Text> pTextFev1 = pDoc->createTextNode(CUtil::DoubleToString(iter->fvcData.fev1));
		AutoPtr<Text> pTextPef = pDoc->createTextNode(CUtil::DoubleToString(iter->fvcData.pef));
		AutoPtr<Text> pTextFev1Rate = pDoc->createTextNode(CUtil::DoubleToString(iter->fvcData.fev1Rate));
		AutoPtr<Text> pTextFef25 = pDoc->createTextNode(CUtil::DoubleToString(iter->fvcData.fef25));
		AutoPtr<Text> pTextFef2575 = pDoc->createTextNode(CUtil::DoubleToString(iter->fvcData.fef2575));
		AutoPtr<Text> pTextFef75 = pDoc->createTextNode(CUtil::DoubleToString(iter->fvcData.fef75));
		AutoPtr<Text> pTextSteps = pDoc->createTextNode(CUtil::DoubleToString(iter->pedometerData.setps));
		AutoPtr<Text> pTextDistance = pDoc->createTextNode(CUtil::DoubleToString(iter->pedometerData.distance));
		AutoPtr<Text> pTextCalories = pDoc->createTextNode(CUtil::DoubleToString(iter->pedometerData.calories));
		AutoPtr<Text> pTextValue = pDoc->createTextNode(iter->urineData.urineValue);
		AutoPtr<Text> pTextValueAdd = pDoc->createTextNode(iter->urineData.urineValueAdd);
		AutoPtr<Text> pTextValueNew = pDoc->createTextNode(iter->urineData.urineValueNew);
		AutoPtr<Text> pTextHrValue = pDoc->createTextNode(CUtil::DoubleToString(iter->hrConclusion.vaule));
		AutoPtr<Text> pTextConclusion = pDoc->createTextNode(iter->hrConclusion.conclusion);
		AutoPtr<Text> pTextCHOL = pDoc->createTextNode(CUtil::DoubleToString(iter->bloodTest.CHOL));
		AutoPtr<Text> pTextHDL = pDoc->createTextNode(CUtil::DoubleToString(iter->bloodTest.HDL));
		AutoPtr<Text> pTextTG = pDoc->createTextNode(CUtil::DoubleToString(iter->bloodTest.TG));
		AutoPtr<Text> pTextLDL = pDoc->createTextNode(CUtil::DoubleToString(iter->bloodTest.LDL));
		AutoPtr<Text> pTextHB = pDoc->createTextNode(CUtil::DoubleToString(iter->bloodTest.HB));
		AutoPtr<Text> pTextHCT = pDoc->createTextNode(CUtil::DoubleToString(iter->bloodTest.HCT));
		AutoPtr<Text> pTextCheckTime = pDoc->createTextNode(iter->checkTime);

		pGrandHr->appendChild(pTextHr);
		pGrandWeight->appendChild(pTextWeight);
		pGrandHeight->appendChild(pTextHeight);
		pGrandTemp->appendChild(pTextTemp);
		pGrandResp->appendChild(pTextResp);
		pGrandChol->appendChild(pTextChol);
		pGrandUa->appendChild(pTextUa);

		pGrand2BloodSugar->appendChild(pTextBloodSugar);
		pGrand2Flag->appendChild(pTextFlag);
		pGrandBloodSugar->appendChild(pGrand2BloodSugar);
		pGrandBloodSugar->appendChild(pGrand2Flag);

		pGrand2Spo2->appendChild(pTextSpo2);
		pGrand2Pr->appendChild(pTextPr);
		pGrandSpo2->appendChild(pGrand2Spo2);
		pGrandSpo2->appendChild(pGrand2Pr);

		pGrand2Sys->appendChild(pTextSys);
		pGrand2Mean->appendChild(pTextMean);
		pGrand2Dia->appendChild(pTextDia);
		pGrandBp->appendChild(pGrand2Sys);
		pGrandBp->appendChild(pGrand2Mean);
		pGrandBp->appendChild(pGrand2Dia);

		pGrand2FetalHeartRate->appendChild(pTextFetalHeartRate);
		pGrand2PalacePressure->appendChild(pTextPalacePressure);
		pGrand2Quickening->appendChild(pTextQuickening);
		pGrandFetal->appendChild(pGrand2FetalHeartRate);
		pGrandFetal->appendChild(pGrand2PalacePressure);
		pGrandFetal->appendChild(pGrand2Quickening);

		pGrand2Fvc->appendChild(pTextFvc);
		pGrand2Fev1->appendChild(pTextFev1);
		pGrand2Pef->appendChild(pTextPef);
		pGrand2Fev1Rate->appendChild(pTextFev1Rate);
		pGrand2Fef25->appendChild(pTextFef25);
		pGrand2Fef2575->appendChild(pTextFef2575);
		pGrand2Fef75->appendChild(pTextFef75);
		pGrandFvc->appendChild(pGrand2Fvc);
		pGrandFvc->appendChild(pGrand2Fev1);
		pGrandFvc->appendChild(pGrand2Pef);
		pGrandFvc->appendChild(pGrand2Fev1Rate);
		pGrandFvc->appendChild(pGrand2Fef25);
		pGrandFvc->appendChild(pGrand2Fef2575);
		pGrandFvc->appendChild(pGrand2Fef75);

		pGrand2Steps->appendChild(pTextSteps);
		pGrand2Distance->appendChild(pTextDistance);
		pGrand2Calories->appendChild(pTextCalories);
		pGrandPedometer->appendChild(pGrand2Steps);
		pGrandPedometer->appendChild(pGrand2Distance);
		pGrandPedometer->appendChild(pGrand2Calories);

		pGrand2Value->appendChild(pTextValue);
		pGrand2ValueAdd->appendChild(pTextValueAdd);
		pGrand2ValueNew->appendChild(pTextValueNew);
		pGrandUrine->appendChild(pGrand2Value);
		pGrandUrine->appendChild(pGrand2ValueAdd);
		pGrandUrine->appendChild(pGrand2ValueNew);

		pGrand2HrValue->appendChild(pTextHrValue);
		pGrand2Conclusion->appendChild(pTextConclusion);
		pGrandHrConclusion->appendChild(pGrand2HrValue);
		pGrandHrConclusion->appendChild(pGrand2Conclusion);

		pGrand2CHOL->appendChild(pTextCHOL);
		pGrand2HDL->appendChild(pTextHDL);
		pGrand2TG->appendChild(pTextTG);
		pGrand2LDL->appendChild(pTextLDL);
		pGrand2HB->appendChild(pTextHB);
		pGrand2HCT->appendChild(pTextHCT);
		pGrandBloodTest->appendChild(pGrand2CHOL);
		pGrandBloodTest->appendChild(pGrand2HDL);
		pGrandBloodTest->appendChild(pGrand2TG);
		pGrandBloodTest->appendChild(pGrand2LDL);
		pGrandBloodTest->appendChild(pGrand2HB);
		pGrandBloodTest->appendChild(pGrand2HCT);

		pGrandChekcTime->appendChild(pTextCheckTime);

		pChildRecord->appendChild(pGrandHr);
		pChildRecord->appendChild(pGrandWeight);
		pChildRecord->appendChild(pGrandHeight);
		pChildRecord->appendChild(pGrandTemp);
		pChildRecord->appendChild(pGrandResp);
		pChildRecord->appendChild(pGrandChol);
		pChildRecord->appendChild(pGrandUa);
		pChildRecord->appendChild(pGrandBloodSugar);
		pChildRecord->appendChild(pGrandSpo2);
		pChildRecord->appendChild(pGrandBp);
		pChildRecord->appendChild(pGrandFetal);
		pChildRecord->appendChild(pGrandFvc);
		pChildRecord->appendChild(pGrandPedometer);
		pChildRecord->appendChild(pGrandUrine);
		pChildRecord->appendChild(pGrandHrConclusion);
		pChildRecord->appendChild(pGrandBloodTest);
		pChildRecord->appendChild(pGrandChekcTime);
		pRoot->appendChild(pChildRecord);
	}
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

	CPhmsRequestHead phmsRequestHead("10", "1034", m_stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::UploadTrendDataString(string stringCardId, string stringDeviceId, string stringDeviceName, string stringCaseId, vector<RECORD_DATA_STRING> vRecordData)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildCardId = pDoc->createElement("cardid");
	AutoPtr<Element> pChildDeviceId = pDoc->createElement("deviceid");
	AutoPtr<Element> pChildDeviceName = pDoc->createElement("devicename");
	AutoPtr<Element> pChildCaseId = pDoc->createElement("caseid");
	AutoPtr<Text> pTextCardId = pDoc->createTextNode(stringCardId);
	AutoPtr<Text> pTextDeviceId = pDoc->createTextNode(stringDeviceId);
	AutoPtr<Text> pTextDeviceName = pDoc->createTextNode(stringDeviceName);
	AutoPtr<Text> pTextCaseId = pDoc->createTextNode(stringCaseId);
	pChildCardId->appendChild(pTextCardId);
	pChildDeviceId->appendChild(pTextDeviceId);
	pChildDeviceName->appendChild(pTextDeviceName);
	pChildCaseId->appendChild(pTextCaseId);
	pRoot->appendChild(pChildCardId);
	pRoot->appendChild(pChildDeviceId);
	pRoot->appendChild(pChildDeviceName);
	pRoot->appendChild(pChildCaseId);
	//构造pChildRecord
	vector<RECORD_DATA_STRING>::const_iterator iter;
	for(iter=vRecordData.begin(); iter!=vRecordData.end(); iter++)
	{
		AutoPtr<Element> pChildRecord = pDoc->createElement("record");

		AutoPtr<Element> pGrandHr = pDoc->createElement("hr");
		AutoPtr<Element> pGrandWeight = pDoc->createElement("weight");
		AutoPtr<Element> pGrandHeight = pDoc->createElement("height");
		AutoPtr<Element> pGrandTemp = pDoc->createElement("temp");
		AutoPtr<Element> pGrandResp = pDoc->createElement("resp");
		AutoPtr<Element> pGrandChol = pDoc->createElement("chol");
		AutoPtr<Element> pGrandUa = pDoc->createElement("ua");
		AutoPtr<Element> pGrandAg = pDoc->createElement("ag");

		AutoPtr<Element> pGrandBloodSugar = pDoc->createElement("bloodsugar");
		AutoPtr<Element> pGrand2BloodSugar = pDoc->createElement("bloodsugar");
		AutoPtr<Element> pGrand2Flag = pDoc->createElement("flag");

		AutoPtr<Element> pGrandSpo2 = pDoc->createElement("spo2");
		AutoPtr<Element> pGrand2Spo2 = pDoc->createElement("spo2");
		AutoPtr<Element> pGrand2Pr = pDoc->createElement("pr");

		AutoPtr<Element> pGrandBp = pDoc->createElement("bp");
		AutoPtr<Element> pGrand2Sys = pDoc->createElement("sys");
		AutoPtr<Element> pGrand2Mean = pDoc->createElement("mean");
		AutoPtr<Element> pGrand2Dia = pDoc->createElement("dia");

		AutoPtr<Element> pGrandFetal = pDoc->createElement("fetal");
		AutoPtr<Element> pGrand2FetalHeartRate = pDoc->createElement("fetalheartrate");
		AutoPtr<Element> pGrand2PalacePressure = pDoc->createElement("palacepressure");
		AutoPtr<Element> pGrand2Quickening = pDoc->createElement("quickening");

		AutoPtr<Element> pGrandFvc = pDoc->createElement("fvc");
		AutoPtr<Element> pGrand2Fvc = pDoc->createElement("fvc");
		AutoPtr<Element> pGrand2Fev1 = pDoc->createElement("fev1");
		AutoPtr<Element> pGrand2Pef = pDoc->createElement("pef");
		AutoPtr<Element> pGrand2Fev1Rate = pDoc->createElement("fev1rate");
		AutoPtr<Element> pGrand2Fef25 = pDoc->createElement("fef25");
		AutoPtr<Element> pGrand2Fef2575 = pDoc->createElement("fef2575");
		AutoPtr<Element> pGrand2Fef75 = pDoc->createElement("fef75");

		AutoPtr<Element> pGrandPedometer = pDoc->createElement("pedometer");
		AutoPtr<Element> pGrand2Steps = pDoc->createElement("steps");
		AutoPtr<Element> pGrand2Distance = pDoc->createElement("distance");
		AutoPtr<Element> pGrand2Calories = pDoc->createElement("calories");

		AutoPtr<Element> pGrandUrine = pDoc->createElement("urine");
		AutoPtr<Element> pGrand2Value = pDoc->createElement("value");
		AutoPtr<Element> pGrand2ValueAdd = pDoc->createElement("valueadd");
		AutoPtr<Element> pGrand2ValueNew = pDoc->createElement("valuenew");

		AutoPtr<Element> pGrandHrConclusion = pDoc->createElement("hrconclusion");
		AutoPtr<Element> pGrand2HrValue = pDoc->createElement("value");
		AutoPtr<Element> pGrand2Conclusion = pDoc->createElement("conclusion");

		AutoPtr<Element> pGrandBloodTest = pDoc->createElement("blood_test");
		AutoPtr<Element> pGrand2CHOL = pDoc->createElement("CHOL");
		AutoPtr<Element> pGrand2HDL = pDoc->createElement("HDL");
		AutoPtr<Element> pGrand2TG = pDoc->createElement("TG");
		AutoPtr<Element> pGrand2LDL = pDoc->createElement("LDL");
		AutoPtr<Element> pGrand2HB = pDoc->createElement("HB");
		AutoPtr<Element> pGrand2HCT = pDoc->createElement("HCT");

		AutoPtr<Element> pGrandChekcTime = pDoc->createElement("checktime");

		AutoPtr<Text> pTextHr = pDoc->createTextNode(iter->hr);
		AutoPtr<Text> pTextWeight = pDoc->createTextNode(iter->weight);
		AutoPtr<Text> pTextHeight = pDoc->createTextNode(iter->height);
		AutoPtr<Text> pTextTemp = pDoc->createTextNode(iter->temp);
		AutoPtr<Text> pTextResp = pDoc->createTextNode(iter->resp);
		AutoPtr<Text> pTextChol = pDoc->createTextNode(iter->chol);
		AutoPtr<Text> pTextUa = pDoc->createTextNode(iter->ua);
		AutoPtr<Text> pTextAg = pDoc->createTextNode(iter->ag);
		AutoPtr<Text> pTextBloodSugar = pDoc->createTextNode(iter->bloodsugerData.bloodsuger);
		AutoPtr<Text> pTextFlag = pDoc->createTextNode(iter->bloodsugerData.flag);
		AutoPtr<Text> pTextSpo2 = pDoc->createTextNode(iter->spo2Data.spo2);
		AutoPtr<Text> pTextPr = pDoc->createTextNode(iter->spo2Data.pr);
		AutoPtr<Text> pTextSys = pDoc->createTextNode(iter->bpData.sys);
		AutoPtr<Text> pTextMean = pDoc->createTextNode(iter->bpData.mean);
		AutoPtr<Text> pTextDia = pDoc->createTextNode(iter->bpData.dia);
		AutoPtr<Text> pTextFetalHeartRate = pDoc->createTextNode(iter->fetalData.fetalHeartRate);
		AutoPtr<Text> pTextPalacePressure = pDoc->createTextNode(iter->fetalData.palacePressure);
		AutoPtr<Text> pTextQuickening = pDoc->createTextNode(iter->fetalData.quickening);
		AutoPtr<Text> pTextFvc = pDoc->createTextNode(iter->fvcData.fvc);
		AutoPtr<Text> pTextFev1 = pDoc->createTextNode(iter->fvcData.fev1);
		AutoPtr<Text> pTextPef = pDoc->createTextNode(iter->fvcData.pef);
		AutoPtr<Text> pTextFev1Rate = pDoc->createTextNode(iter->fvcData.fev1Rate);
		AutoPtr<Text> pTextFef25 = pDoc->createTextNode(iter->fvcData.fef25);
		AutoPtr<Text> pTextFef2575 = pDoc->createTextNode(iter->fvcData.fef2575);
		AutoPtr<Text> pTextFef75 = pDoc->createTextNode(iter->fvcData.fef75);
		AutoPtr<Text> pTextSteps = pDoc->createTextNode(iter->pedometerData.setps);
		AutoPtr<Text> pTextDistance = pDoc->createTextNode(iter->pedometerData.distance);
		AutoPtr<Text> pTextCalories = pDoc->createTextNode(iter->pedometerData.calories);
		AutoPtr<Text> pTextValue = pDoc->createTextNode(iter->urineData.urineValue);
		AutoPtr<Text> pTextValueAdd = pDoc->createTextNode(iter->urineData.urineValueAdd);
		AutoPtr<Text> pTextValueNew = pDoc->createTextNode(iter->urineData.urineValueNew);
		AutoPtr<Text> pTextHrValue = pDoc->createTextNode(iter->hrConclusion.vaule);
		AutoPtr<Text> pTextConclusion = pDoc->createTextNode(iter->hrConclusion.conclusion);
		AutoPtr<Text> pTextCHOL = pDoc->createTextNode(iter->bloodTest.CHOL);
		AutoPtr<Text> pTextHDL = pDoc->createTextNode(iter->bloodTest.HDL);
		AutoPtr<Text> pTextTG = pDoc->createTextNode(iter->bloodTest.TG);
		AutoPtr<Text> pTextLDL = pDoc->createTextNode(iter->bloodTest.LDL);
		AutoPtr<Text> pTextHB = pDoc->createTextNode(iter->bloodTest.HB);
		AutoPtr<Text> pTextHCT = pDoc->createTextNode(iter->bloodTest.HCT);
		AutoPtr<Text> pTextCheckTime = pDoc->createTextNode(iter->checkTime);

		pGrandHr->appendChild(pTextHr);
		pGrandWeight->appendChild(pTextWeight);
		pGrandHeight->appendChild(pTextHeight);
		pGrandTemp->appendChild(pTextTemp);
		pGrandResp->appendChild(pTextResp);
		pGrandChol->appendChild(pTextChol);
		pGrandUa->appendChild(pTextUa);
		pGrandAg->appendChild(pTextAg);

		pGrand2BloodSugar->appendChild(pTextBloodSugar);
		pGrand2Flag->appendChild(pTextFlag);
		pGrandBloodSugar->appendChild(pGrand2BloodSugar);
		pGrandBloodSugar->appendChild(pGrand2Flag);

		pGrand2Spo2->appendChild(pTextSpo2);
		pGrand2Pr->appendChild(pTextPr);
		pGrandSpo2->appendChild(pGrand2Spo2);
		pGrandSpo2->appendChild(pGrand2Pr);

		pGrand2Sys->appendChild(pTextSys);
		pGrand2Mean->appendChild(pTextMean);
		pGrand2Dia->appendChild(pTextDia);
		pGrandBp->appendChild(pGrand2Sys);
		pGrandBp->appendChild(pGrand2Mean);
		pGrandBp->appendChild(pGrand2Dia);

		pGrand2FetalHeartRate->appendChild(pTextFetalHeartRate);
		pGrand2PalacePressure->appendChild(pTextPalacePressure);
		pGrand2Quickening->appendChild(pTextQuickening);
		pGrandFetal->appendChild(pGrand2FetalHeartRate);
		pGrandFetal->appendChild(pGrand2PalacePressure);
		pGrandFetal->appendChild(pGrand2Quickening);

		pGrand2Fvc->appendChild(pTextFvc);
		pGrand2Fev1->appendChild(pTextFev1);
		pGrand2Pef->appendChild(pTextPef);
		pGrand2Fev1Rate->appendChild(pTextFev1Rate);
		pGrand2Fef25->appendChild(pTextFef25);
		pGrand2Fef2575->appendChild(pTextFef2575);
		pGrand2Fef75->appendChild(pTextFef75);
		pGrandFvc->appendChild(pGrand2Fvc);
		pGrandFvc->appendChild(pGrand2Fev1);
		pGrandFvc->appendChild(pGrand2Pef);
		pGrandFvc->appendChild(pGrand2Fev1Rate);
		pGrandFvc->appendChild(pGrand2Fef25);
		pGrandFvc->appendChild(pGrand2Fef2575);
		pGrandFvc->appendChild(pGrand2Fef75);

		pGrand2Steps->appendChild(pTextSteps);
		pGrand2Distance->appendChild(pTextDistance);
		pGrand2Calories->appendChild(pTextCalories);
		pGrandPedometer->appendChild(pGrand2Steps);
		pGrandPedometer->appendChild(pGrand2Distance);
		pGrandPedometer->appendChild(pGrand2Calories);

		pGrand2Value->appendChild(pTextValue);
		pGrand2ValueAdd->appendChild(pTextValueAdd);
		pGrand2ValueNew->appendChild(pTextValueNew);
		pGrandUrine->appendChild(pGrand2Value);
		pGrandUrine->appendChild(pGrand2ValueAdd);
		pGrandUrine->appendChild(pGrand2ValueNew);

		pGrand2HrValue->appendChild(pTextHrValue);
		pGrand2Conclusion->appendChild(pTextConclusion);
		pGrandHrConclusion->appendChild(pGrand2HrValue);
		pGrandHrConclusion->appendChild(pGrand2Conclusion);

		pGrand2CHOL->appendChild(pTextCHOL);
		pGrand2HDL->appendChild(pTextHDL);
		pGrand2TG->appendChild(pTextTG);
		pGrand2LDL->appendChild(pTextLDL);
		pGrand2HB->appendChild(pTextHB);
		pGrand2HCT->appendChild(pTextHCT);
		pGrandBloodTest->appendChild(pGrand2CHOL);
		pGrandBloodTest->appendChild(pGrand2HDL);
		pGrandBloodTest->appendChild(pGrand2TG);
		pGrandBloodTest->appendChild(pGrand2LDL);
		pGrandBloodTest->appendChild(pGrand2HB);
		pGrandBloodTest->appendChild(pGrand2HCT);

		pGrandChekcTime->appendChild(pTextCheckTime);

		pChildRecord->appendChild(pGrandHr);
		pChildRecord->appendChild(pGrandWeight);
		pChildRecord->appendChild(pGrandHeight);
		pChildRecord->appendChild(pGrandTemp);
		pChildRecord->appendChild(pGrandResp);
		pChildRecord->appendChild(pGrandChol);
		pChildRecord->appendChild(pGrandUa);
		pChildRecord->appendChild(pGrandAg);
		pChildRecord->appendChild(pGrandBloodSugar);
		pChildRecord->appendChild(pGrandSpo2);
		pChildRecord->appendChild(pGrandBp);
		pChildRecord->appendChild(pGrandFetal);
		pChildRecord->appendChild(pGrandFvc);
		pChildRecord->appendChild(pGrandPedometer);
		pChildRecord->appendChild(pGrandUrine);
		pChildRecord->appendChild(pGrandHrConclusion);
		pChildRecord->appendChild(pGrandBloodTest);
		pChildRecord->appendChild(pGrandChekcTime);
		pRoot->appendChild(pChildRecord);
	}
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

	CPhmsRequestHead phmsRequestHead("10", "1034", m_stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}


int CPhmsSession::UploadTrendDataFile(string stringFilePath)
{
	string stringXmlContent;
	int nRet = PHMS_SUCCESSFUL_RESULT;
	nRet = this->GetRequestXml(stringFilePath, stringXmlContent);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}

	CPhmsRequestHead phmsRequestHead("10", "1034", m_stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::UploadAudioSegment(string stringMsgId, string stringOrder, string stringTotalSize, string stringServerUri, string stringStart1, 
					   string stringEnd1, string stringMd51, string stringStart2, string stringEnd2, string stringMd52, string stringStart3, 
					   string stringEnd3, string stringMd53, string stringFilePath)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildMsgId = pDoc->createElement("msgid");
	AutoPtr<Element> pChildOrder = pDoc->createElement("order");
	AutoPtr<Element> pChildTotalSize = pDoc->createElement("totalsize");
	AutoPtr<Element> pChildServerUri = pDoc->createElement("serveruri");
	AutoPtr<Element> pChildPart1 = pDoc->createElement("md51");
	AutoPtr<Element> pGrandChildStart1= pDoc->createElement("start");
	AutoPtr<Element> pGrandChildEnd1 = pDoc->createElement("end");
	AutoPtr<Element> pGrandChildMd51 = pDoc->createElement("md5");
	AutoPtr<Element> pChildPart2 = pDoc->createElement("md52");
	AutoPtr<Element> pGrandChildStart2= pDoc->createElement("start");
	AutoPtr<Element> pGrandChildEnd2 = pDoc->createElement("end");
	AutoPtr<Element> pGrandChildMd52 = pDoc->createElement("md5");
	AutoPtr<Element> pChildPart3 = pDoc->createElement("md53");
	AutoPtr<Element> pGrandChildStart3= pDoc->createElement("start");
	AutoPtr<Element> pGrandChildEnd3 = pDoc->createElement("end");
	AutoPtr<Element> pGrandChildMd53 = pDoc->createElement("md5");
	AutoPtr<Text> pTextMsgId = pDoc->createTextNode(stringMsgId);
	AutoPtr<Text> pTextOrder = pDoc->createTextNode(stringOrder);
	AutoPtr<Text> pTextTotalSize = pDoc->createTextNode(stringTotalSize);
	AutoPtr<Text> pTextServerUri = pDoc->createTextNode(stringServerUri);
	AutoPtr<Text> pTextStart1 = pDoc->createTextNode(stringStart1);
	AutoPtr<Text> pTextEnd1 = pDoc->createTextNode(stringEnd1);
	AutoPtr<Text> pTextMd51 = pDoc->createTextNode(stringMd51);
	AutoPtr<Text> pTextStart2 = pDoc->createTextNode(stringStart2);
	AutoPtr<Text> pTextEnd2 = pDoc->createTextNode(stringEnd2);
	AutoPtr<Text> pTextMd52 = pDoc->createTextNode(stringMd52);
	AutoPtr<Text> pTextStart3 = pDoc->createTextNode(stringStart3);
	AutoPtr<Text> pTextEnd3 = pDoc->createTextNode(stringEnd3);
	AutoPtr<Text> pTextMd53 = pDoc->createTextNode(stringMd53);
	pChildMsgId->appendChild(pTextMsgId);
	pChildOrder->appendChild(pTextOrder);
	pChildTotalSize->appendChild(pTextTotalSize);
	pChildServerUri->appendChild(pTextServerUri);
	pGrandChildStart1->appendChild(pTextStart1);
	pGrandChildEnd1->appendChild(pTextEnd1);
	pGrandChildMd51->appendChild(pTextMd51);
	pGrandChildStart2->appendChild(pTextStart2);
	pGrandChildEnd2->appendChild(pTextEnd2);
	pGrandChildMd52->appendChild(pTextMd52);
	pGrandChildStart3->appendChild(pTextStart3);
	pGrandChildEnd3->appendChild(pTextEnd3);
	pGrandChildMd53->appendChild(pTextMd53);
	pChildPart1->appendChild(pGrandChildStart1);
	pChildPart1->appendChild(pGrandChildEnd1);
	pChildPart1->appendChild(pGrandChildMd51);
	pChildPart2->appendChild(pGrandChildStart2);
	pChildPart2->appendChild(pGrandChildEnd2);
	pChildPart2->appendChild(pGrandChildMd52);
	pChildPart3->appendChild(pGrandChildStart3);
	pChildPart3->appendChild(pGrandChildEnd3);
	pChildPart3->appendChild(pGrandChildMd53);
	pRoot->appendChild(pChildMsgId);
	pRoot->appendChild(pChildOrder);
	pRoot->appendChild(pChildTotalSize);
	pRoot->appendChild(pChildServerUri);
	pRoot->appendChild(pChildPart1);
	pRoot->appendChild(pChildPart2);
	pRoot->appendChild(pChildPart3);
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

	CPhmsRequestHead phmsRequestHead("10", "1035", m_stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, stringFilePath);
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::UploadAdvice(string stringUserId, string stringUsername, string stringMsg, string stringType, string stringIsSendMsg)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildUserId = pDoc->createElement("userid");
	AutoPtr<Element> pChildUsername = pDoc->createElement("username");
	AutoPtr<Element> pChildMsg = pDoc->createElement("msg");
	AutoPtr<Element> pChildType = pDoc->createElement("type");
	AutoPtr<Element> pChildIsSendMsg = pDoc->createElement("issendmsg");
	AutoPtr<Text> pTextUserId = pDoc->createTextNode(stringUserId);
	AutoPtr<Text> pTextUsername = pDoc->createTextNode(stringUsername);
	AutoPtr<Text> pTextMsg = pDoc->createTextNode(stringMsg);
	AutoPtr<Text> pTextType = pDoc->createTextNode(stringType);
	AutoPtr<Text> pTextIsSendMsg = pDoc->createTextNode(stringIsSendMsg);
	pChildUserId->appendChild(pTextUserId);
	pChildUsername->appendChild(pTextUsername);
	pChildMsg->appendChild(pTextMsg);
	pChildType->appendChild(pTextType);
	pChildIsSendMsg->appendChild(pTextIsSendMsg);
	pRoot->appendChild(pChildUserId);
	pRoot->appendChild(pChildUsername);
	pRoot->appendChild(pChildMsg);
	pRoot->appendChild(pChildType);
	pRoot->appendChild(pChildIsSendMsg);
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

	CPhmsRequestHead phmsRequestHead("10", "1036", m_stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::GetAdvice(string stringSgId, string stringUserId, string stringDoctorId, string stringDownloadState, string stringType, string stringStartDate, 
			  string stringEndDate, string stringPageTo, string stringPageSize)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildSgId = pDoc->createElement("sgid");
	AutoPtr<Element> pChildUserId = pDoc->createElement("userid");
	AutoPtr<Element> pChildDoctorId = pDoc->createElement("doctorid");
	AutoPtr<Element> pChildDownloadState = pDoc->createElement("downstate");
	AutoPtr<Element> pChildType = pDoc->createElement("type");
	AutoPtr<Element> pChildStartDate = pDoc->createElement("startdate");
	AutoPtr<Element> pChildEndDate = pDoc->createElement("enddate");
	AutoPtr<Element> pChildPageTo = pDoc->createElement("pageto");
	AutoPtr<Element> pChildPageSize = pDoc->createElement("pagesize");
	AutoPtr<Text> pTextSgId = pDoc->createTextNode(stringSgId);
	AutoPtr<Text> pTextUserId = pDoc->createTextNode(stringUserId);
	AutoPtr<Text> pTextDoctorId = pDoc->createTextNode(stringDoctorId);
	AutoPtr<Text> pTextDownloadState = pDoc->createTextNode(stringDownloadState);
	AutoPtr<Text> pTextType = pDoc->createTextNode(stringType);
	AutoPtr<Text> pTextStartDate = pDoc->createTextNode(stringStartDate);
	AutoPtr<Text> pTextEndDate = pDoc->createTextNode(stringEndDate);
	AutoPtr<Text> pTextPageTo = pDoc->createTextNode(stringPageTo);
	AutoPtr<Text> pTextPageSize = pDoc->createTextNode(stringPageSize);
	pChildSgId->appendChild(pTextSgId);
	pChildUserId->appendChild(pTextUserId);
	pChildDoctorId->appendChild(pTextDoctorId);
	pChildDownloadState->appendChild(pTextDownloadState);
	pChildType->appendChild(pTextType);
	pChildStartDate->appendChild(pTextStartDate);
	pChildEndDate->appendChild(pTextEndDate);
	pChildPageTo->appendChild(pTextPageTo);
	pChildPageSize->appendChild(pTextPageSize);
	pRoot->appendChild(pChildSgId);
	pRoot->appendChild(pChildUserId);
	pRoot->appendChild(pChildDoctorId);
	pRoot->appendChild(pChildDownloadState);
	pRoot->appendChild(pChildType);
	pRoot->appendChild(pChildStartDate);
	pRoot->appendChild(pChildEndDate);
	pRoot->appendChild(pChildPageTo);
	pRoot->appendChild(pChildPageSize);
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

	CPhmsRequestHead phmsRequestHead("10", "1037", m_stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::DownloadAdviceComplete(string stringAdviceId)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildAdviceId = pDoc->createElement("sgid");
	AutoPtr<Text> pTextAdviceId = pDoc->createTextNode(stringAdviceId);
	pChildAdviceId->appendChild(pTextAdviceId);
	pRoot->appendChild(pChildAdviceId);
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

	CPhmsRequestHead phmsRequestHead("10", "1038", m_stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::GetReportListOfCase(string stringCaseId)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildCaseId = pDoc->createElement("caseid");
	AutoPtr<Text> pTextCaseId = pDoc->createTextNode(stringCaseId);
	pChildCaseId->appendChild(pTextCaseId);
	pRoot->appendChild(pChildCaseId);
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

	CPhmsRequestHead phmsRequestHead("10", "1039", m_stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

//接口有变动，加了一个参数senderid，表示要查找信息的人，如为空表示自己
int CPhmsSession::GetSelfInfo(string stringSenderId)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildSenderId = pDoc->createElement("senderid");
	AutoPtr<Text> pTextSenderId = pDoc->createTextNode(stringSenderId);
	pChildSenderId->appendChild(pTextSenderId);
	pRoot->appendChild(pChildSenderId);
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

	CPhmsRequestHead phmsRequestHead("10", "1040", m_stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}


int CPhmsSession::ChangePwd(string stringOldPassword, string stringNewPassword)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildOldPassword = pDoc->createElement("oldpwd");
	AutoPtr<Element> pChildNewPassword = pDoc->createElement("newpwd");
	AutoPtr<Text> pTextOldPassword = pDoc->createTextNode(stringOldPassword);
	AutoPtr<Text> pTextNewPassword = pDoc->createTextNode(stringNewPassword);
	pChildOldPassword->appendChild(pTextOldPassword);
	pChildNewPassword->appendChild(pTextNewPassword);
	pRoot->appendChild(pChildOldPassword);
	pRoot->appendChild(pChildNewPassword);
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

	CPhmsRequestHead phmsRequestHead("10", "1041", m_stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	m_enumPhmsCommandType = CHANGE_INFO_TYPE;//目前只有登录命令是LOGIN_TYPE，改变个人信息命令是CHANGE_INFO_TYPE，用户注册命令是REGISTER_TYPE
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	m_enumPhmsCommandType = MAIN_TYPE;//其余都是MAIN_TYPE
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::DoctorChangeSelfInfo(string stringName, string stringSex, string stringTel, string stringBirthYear, string stringPosition, 
						 string stringNotes, string stringAgentId)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildName = pDoc->createElement("name");
	AutoPtr<Element> pChildSex = pDoc->createElement("sex");
	AutoPtr<Element> pChildTel = pDoc->createElement("tel");
	AutoPtr<Element> pChildBirthYear = pDoc->createElement("birthyear");
	AutoPtr<Element> pChildPosition = pDoc->createElement("position");
	AutoPtr<Element> pChildNotes= pDoc->createElement("notes");
	AutoPtr<Element> pChildAgentId = pDoc->createElement("agentid");
	AutoPtr<Text> pTextName = pDoc->createTextNode(stringName);
	AutoPtr<Text> pTextSex = pDoc->createTextNode(stringSex);
	AutoPtr<Text> pTextTel = pDoc->createTextNode(stringTel);
	AutoPtr<Text> pTextBirthYead = pDoc->createTextNode(stringBirthYear);
	AutoPtr<Text> pTextPosition = pDoc->createTextNode(stringPosition);
	AutoPtr<Text> pTextNotes = pDoc->createTextNode(stringNotes);
	AutoPtr<Text> pTextAgentId = pDoc->createTextNode(stringAgentId);
	pChildName->appendChild(pTextName);
	pChildSex->appendChild(pTextSex);
	pChildTel->appendChild(pTextTel);
	pChildBirthYear->appendChild(pTextBirthYead);
	pChildPosition->appendChild(pTextPosition);
	pChildNotes->appendChild(pTextNotes);
	pChildAgentId->appendChild(pTextAgentId);
	pRoot->appendChild(pChildName);
	pRoot->appendChild(pChildSex);
	pRoot->appendChild(pChildTel);
	pRoot->appendChild(pChildBirthYear);
	pRoot->appendChild(pChildPosition);
	pRoot->appendChild(pChildNotes);
	pRoot->appendChild(pChildAgentId);
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

	CPhmsRequestHead phmsRequestHead("10", "1042", m_stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::CliniqueChangeSelfInfo(string stringPersonId, string stringName, string stringTel, string stringNotes)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildPersonId = pDoc->createElement("pid");
	AutoPtr<Element> pChildName = pDoc->createElement("name");
	AutoPtr<Element> pChildTel = pDoc->createElement("tel");
	AutoPtr<Element> pChildNotes= pDoc->createElement("notes");
	AutoPtr<Text> pTextPersonId = pDoc->createTextNode(stringPersonId);
	AutoPtr<Text> pTextName = pDoc->createTextNode(stringName);
	AutoPtr<Text> pTextTel = pDoc->createTextNode(stringTel);
	AutoPtr<Text> pTextNotes = pDoc->createTextNode(stringNotes);
	pChildPersonId->appendChild(pTextPersonId);
	pChildName->appendChild(pTextName);
	pChildTel->appendChild(pTextTel);
	pChildNotes->appendChild(pTextNotes);
	pRoot->appendChild(pChildPersonId);
	pRoot->appendChild(pChildName);
	pRoot->appendChild(pChildTel);
	pRoot->appendChild(pChildNotes);
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

	CPhmsRequestHead phmsRequestHead("10", "1043", m_stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::CardUserChangeSelfInfo(string stringPersonId, string stringName, string stringSex, string stringTel, string stringBirthday, 
						   string stringAddress, string stringLmp, string stringToken, string stringHeight, string stringWeight, string stringPhoto, 
						   string stringVCodeBeFollowed, string stringHospitalId, string stringOtherInfo)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildPersonId = pDoc->createElement("pid");
	AutoPtr<Element> pChildName = pDoc->createElement("name");
	AutoPtr<Element> pChildSex = pDoc->createElement("sex");
	AutoPtr<Element> pChildTel = pDoc->createElement("tel");
	AutoPtr<Element> pChildBirthday = pDoc->createElement("birthday");
	AutoPtr<Element> pChildAddress = pDoc->createElement("address");
	AutoPtr<Element> pChildLmp = pDoc->createElement("lmp");
	AutoPtr<Element> pChildToken = pDoc->createElement("token");
	AutoPtr<Element> pChildHeight = pDoc->createElement("height");
	AutoPtr<Element> pChildWeight = pDoc->createElement("weight");
	AutoPtr<Element> pChildPhoto = pDoc->createElement("photo");
	AutoPtr<Element> pChildVCodeBeFollowed = pDoc->createElement("vcode_be_followed");
	AutoPtr<Element> pChildHospitalId = pDoc->createElement("hospitalid");
	AutoPtr<Element> pChildOtherInfo = pDoc->createElement("other_info");
	AutoPtr<Text> pTextPersonId = pDoc->createTextNode(stringPersonId);
	AutoPtr<Text> pTextName = pDoc->createTextNode(stringName);
	AutoPtr<Text> pTextSex = pDoc->createTextNode(stringSex);
	AutoPtr<Text> pTextTel = pDoc->createTextNode(stringTel);
	AutoPtr<Text> pTextBirthday = pDoc->createTextNode(stringBirthday);
	AutoPtr<Text> pTextAddress= pDoc->createTextNode(stringAddress);
	AutoPtr<Text> pTextLmp = pDoc->createTextNode(stringLmp);
	AutoPtr<Text> pTextToken = pDoc->createTextNode(stringToken);
	AutoPtr<Text> pTextHeight = pDoc->createTextNode(stringHeight);
	AutoPtr<Text> pTextWeight = pDoc->createTextNode(stringWeight);
	AutoPtr<Text> pTextPhoto = pDoc->createTextNode(stringPhoto);
	AutoPtr<Text> pTextVCodeBeFollowed = pDoc->createTextNode(stringVCodeBeFollowed);
	AutoPtr<Text> pTextHospitalId = pDoc->createTextNode(stringHospitalId);
	AutoPtr<Text> pTextOtherInfo = pDoc->createTextNode(stringOtherInfo);
	pChildPersonId->appendChild(pTextPersonId);
	pChildName->appendChild(pTextName);
	pChildSex->appendChild(pTextSex);
	pChildTel->appendChild(pTextTel);
	pChildBirthday->appendChild(pTextBirthday);
	pChildAddress->appendChild(pTextAddress);
	pChildLmp->appendChild(pTextLmp);
	pChildToken->appendChild(pTextToken);
	pChildHeight->appendChild(pTextHeight);
	pChildWeight->appendChild(pTextWeight);
	pChildPhoto->appendChild(pTextPhoto);
	pChildVCodeBeFollowed->appendChild(pTextVCodeBeFollowed);
	pChildHospitalId->appendChild(pTextHospitalId);
	pChildOtherInfo->appendChild(pTextOtherInfo);
	pRoot->appendChild(pChildPersonId);
	pRoot->appendChild(pChildName);
	pRoot->appendChild(pChildSex);
	pRoot->appendChild(pChildTel);
	pRoot->appendChild(pChildBirthday);
	pRoot->appendChild(pChildAddress);
	pRoot->appendChild(pChildLmp);
	pRoot->appendChild(pChildToken);
	pRoot->appendChild(pChildHeight);
	pRoot->appendChild(pChildWeight);
	pRoot->appendChild(pChildPhoto);
	pRoot->appendChild(pChildVCodeBeFollowed);
	pRoot->appendChild(pChildHospitalId);
	pRoot->appendChild(pChildOtherInfo);
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

	CPhmsRequestHead phmsRequestHead("10", "1044", m_stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::GetCaseListOfUser(string stringCaseId, string stringCaseType, string stringCaseState, string stringThirdId, string stringName, 
					  string stringStartDate, string stringEndDate, string stringCheckStartDate, string stringCheckEndDate, string stringCaseOrigin, 
					  string stringYzlb, string stringSex, string stringIsRead, string stringOrderBy, string stringPageTo, string stringPageSize)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildCaseId = pDoc->createElement("caseid");
	AutoPtr<Element> pChildCaseType = pDoc->createElement("casetype");
	AutoPtr<Element> pChildCaseState = pDoc->createElement("casestate");
	AutoPtr<Element> pChildThirdId = pDoc->createElement("thirdid");
	AutoPtr<Element> pChildName = pDoc->createElement("name");
	AutoPtr<Element> pChildStartDate = pDoc->createElement("startdate");
	AutoPtr<Element> pChildEndDate = pDoc->createElement("enddate");
	AutoPtr<Element> pChildCheckStartDate = pDoc->createElement("checkstartdate");
	AutoPtr<Element> pChildCheckEndDate = pDoc->createElement("checkenddate");
	AutoPtr<Element> pChildCaseOrigin = pDoc->createElement("caseorigin");
	AutoPtr<Element> pChildYzlb = pDoc->createElement("yzlb");
	AutoPtr<Element> pChildSex = pDoc->createElement("sex");
	AutoPtr<Element> pChildIsRead = pDoc->createElement("isread");
	AutoPtr<Element> pChildOrderBy = pDoc->createElement("orderby");
	AutoPtr<Element> pChildPageTo = pDoc->createElement("pageto");
	AutoPtr<Element> pChildPageSize = pDoc->createElement("pagesize");
	AutoPtr<Text> pTextCaseId = pDoc->createTextNode(stringCaseId);
	AutoPtr<Text> pTextCaseType = pDoc->createTextNode(stringCaseType);
	AutoPtr<Text> pTextCaseState = pDoc->createTextNode(stringCaseState);
	AutoPtr<Text> pTextThirdId = pDoc->createTextNode(stringThirdId);
	AutoPtr<Text> pTextName = pDoc->createTextNode(stringName);
	AutoPtr<Text> pTextStartDate = pDoc->createTextNode(stringStartDate);
	AutoPtr<Text> pTextEndDate = pDoc->createTextNode(stringEndDate);
	AutoPtr<Text> pTextCheckStartDate = pDoc->createTextNode(stringCheckStartDate);
	AutoPtr<Text> pTextCheckEndDate = pDoc->createTextNode(stringCheckEndDate);
	AutoPtr<Text> pTextCaseOrigin = pDoc->createTextNode(stringCaseOrigin);
	AutoPtr<Text> pTextYzlb = pDoc->createTextNode(stringYzlb);
	AutoPtr<Text> pTextSex = pDoc->createTextNode(stringSex);
	AutoPtr<Text> pTextIsRead = pDoc->createTextNode(stringIsRead);
	AutoPtr<Text> pTextOrderBy = pDoc->createTextNode(stringOrderBy);
	AutoPtr<Text> pTextPageTo = pDoc->createTextNode(stringPageTo);
	AutoPtr<Text> pTextPageSize = pDoc->createTextNode(stringPageSize);
	pChildCaseId->appendChild(pTextCaseId);
	pChildCaseType->appendChild(pTextCaseType);
	pChildCaseState->appendChild(pTextCaseState);
	pChildThirdId->appendChild(pTextThirdId);
	pChildName->appendChild(pTextName);
	pChildStartDate->appendChild(pTextStartDate);
	pChildEndDate->appendChild(pTextEndDate);
	pChildCheckStartDate->appendChild(pTextCheckStartDate);
	pChildCheckEndDate->appendChild(pTextCheckEndDate);
	pChildCaseOrigin->appendChild(pTextCaseOrigin);
	pChildYzlb->appendChild(pTextYzlb);
	pChildSex->appendChild(pTextSex);
	pChildIsRead->appendChild(pTextIsRead);
	pChildOrderBy->appendChild(pTextOrderBy);
	pChildPageTo->appendChild(pTextPageTo);
	pChildPageSize->appendChild(pTextPageSize);
	pRoot->appendChild(pChildCaseId);
	pRoot->appendChild(pChildCaseType);
	pRoot->appendChild(pChildCaseState);
	pRoot->appendChild(pChildThirdId);
	pRoot->appendChild(pChildName);
	pRoot->appendChild(pChildStartDate);
	pRoot->appendChild(pChildEndDate);
	pRoot->appendChild(pChildCheckStartDate);
	pRoot->appendChild(pChildCheckEndDate);
	pRoot->appendChild(pChildCaseOrigin);
	pRoot->appendChild(pChildYzlb);
	pRoot->appendChild(pChildSex);
	pRoot->appendChild(pChildIsRead);
	pRoot->appendChild(pChildOrderBy);
	pRoot->appendChild(pChildPageTo);
	pRoot->appendChild(pChildPageSize);
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

	CPhmsRequestHead phmsRequestHead("10", "1045", m_stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::GetReportListOfUser(string stringReportId, string stringReportStartDate, string stringReportEndDate, string stringCaseType, 
						string stringState, string stringOrderBy, string stringPageTo, string stringPageSize)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildReportId = pDoc->createElement("reportid");
	AutoPtr<Element> pChildReportStartDate = pDoc->createElement("reportstartdate");
	AutoPtr<Element> pChildReportEndDate = pDoc->createElement("reportenddate");
	AutoPtr<Element> pChildCaseType = pDoc->createElement("casetype");
	AutoPtr<Element> pChildState = pDoc->createElement("state");
	AutoPtr<Element> pChildOrderBy = pDoc->createElement("orderby");
	AutoPtr<Element> pChildPageTo = pDoc->createElement("pageto");
	AutoPtr<Element> pChildPageSize = pDoc->createElement("pagesize");
	AutoPtr<Text> pTextReportId = pDoc->createTextNode(stringReportId);
	AutoPtr<Text> pTextReportStartDate = pDoc->createTextNode(stringReportStartDate);
	AutoPtr<Text> pTextReportEndDate = pDoc->createTextNode(stringReportEndDate);
	AutoPtr<Text> pTextCaseType = pDoc->createTextNode(stringCaseType);
	AutoPtr<Text> pTextState = pDoc->createTextNode(stringState);
	AutoPtr<Text> pTextOrderBy = pDoc->createTextNode(stringOrderBy);
	AutoPtr<Text> pTextPageTo = pDoc->createTextNode(stringPageTo);
	AutoPtr<Text> pTextPageSize = pDoc->createTextNode(stringPageSize);
	pChildReportId->appendChild(pTextReportId);
	pChildReportStartDate->appendChild(pTextReportStartDate);
	pChildReportEndDate->appendChild(pTextReportEndDate);
	pChildCaseType->appendChild(pTextCaseType);
	pChildState->appendChild(pTextState);
	pChildOrderBy->appendChild(pTextOrderBy);
	pChildPageTo->appendChild(pTextPageTo);
	pChildPageSize->appendChild(pTextPageSize);
	pRoot->appendChild(pChildReportId);
	pRoot->appendChild(pChildReportStartDate);
	pRoot->appendChild(pChildReportEndDate);
	pRoot->appendChild(pChildCaseType);
	pRoot->appendChild(pChildState);
	pRoot->appendChild(pChildOrderBy);
	pRoot->appendChild(pChildPageTo);
	pRoot->appendChild(pChildPageSize);
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

	CPhmsRequestHead phmsRequestHead("10", "1046", m_stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::GetCardUserList(string stringCardId, string stringSenderId, string stringPersonId, string stringTel, string stringName, string stringType,
					string stringPageTo, string stringPageSize)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildCardId = pDoc->createElement("cardid");
	AutoPtr<Element> pChildSenderId = pDoc->createElement("senderid");
	AutoPtr<Element> pChildPersonId = pDoc->createElement("pid");
	AutoPtr<Element> pChildTel = pDoc->createElement("tel");
	AutoPtr<Element> pChildName = pDoc->createElement("name");
	AutoPtr<Element> pChildType = pDoc->createElement("type");
	AutoPtr<Element> pChildPageTo = pDoc->createElement("pageto");
	AutoPtr<Element> pChildPageSize = pDoc->createElement("pagesize");
	AutoPtr<Text> pTextCardId = pDoc->createTextNode(stringCardId);
	AutoPtr<Text> pTextSenderId = pDoc->createTextNode(stringSenderId);
	AutoPtr<Text> pTextPersonId = pDoc->createTextNode(stringPersonId);
	AutoPtr<Text> pTextTel = pDoc->createTextNode(stringTel);
	AutoPtr<Text> pTextName = pDoc->createTextNode(stringName);
	AutoPtr<Text> pTextType = pDoc->createTextNode(stringType);
	AutoPtr<Text> pTextPageTo = pDoc->createTextNode(stringPageTo);
	AutoPtr<Text> pTextPageSize = pDoc->createTextNode(stringPageSize);
	pChildCardId->appendChild(pTextCardId);
	pChildSenderId->appendChild(pTextSenderId);
	pChildPersonId->appendChild(pTextPersonId);
	pChildTel->appendChild(pTextTel);
	pChildName->appendChild(pTextName);
	pChildType->appendChild(pTextType);
	pChildPageTo->appendChild(pTextPageTo);
	pChildPageSize->appendChild(pTextPageSize);
	pRoot->appendChild(pChildCardId);
	pRoot->appendChild(pChildSenderId);
	pRoot->appendChild(pChildPersonId);
	pRoot->appendChild(pChildTel);
	pRoot->appendChild(pChildName);
	pRoot->appendChild(pChildType);
	pRoot->appendChild(pChildPageTo);
	pRoot->appendChild(pChildPageSize);
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

	CPhmsRequestHead phmsRequestHead("10", "1047", m_stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::KeepSession()
{
	CPhmsRequestHead phmsRequestHead("10", "1048", m_stringSessionId, "10");
	CPhmsRequestBody phmsRequestBody("");
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	int nRet = PHMS_SUCCESSFUL_RESULT;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::MarkCaseException(string stringCaseId, string stringReceiverId, int nResultFlag)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildCaseId = pDoc->createElement("caseid");
	AutoPtr<Element> pChildReceiverId = pDoc->createElement("receiverid");
	AutoPtr<Element> pChildSymptoms = pDoc->createElement("symptoms");
	AutoPtr<Text> pTextCaseId = pDoc->createTextNode(stringCaseId);
	AutoPtr<Text> pTextReceiverId = pDoc->createTextNode(stringReceiverId);
	string stringResultFlag = CUtil::UIntToString(nResultFlag);
	AutoPtr<Text> pTextSymptoms = pDoc->createTextNode(stringResultFlag);
	pChildCaseId->appendChild(pTextCaseId);
	pChildReceiverId->appendChild(pTextReceiverId);
	pChildSymptoms->appendChild(pTextSymptoms);
	pRoot->appendChild(pChildCaseId);
	pRoot->appendChild(pChildReceiverId);
	pRoot->appendChild(pChildSymptoms);
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

	CPhmsRequestHead phmsRequestHead("10", "1049", m_stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::MarkReportRead(string stringReportId)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildReportId = pDoc->createElement("reportid");
	AutoPtr<Text> pTextReportId = pDoc->createTextNode(stringReportId);
	pChildReportId->appendChild(pTextReportId);
	pRoot->appendChild(pChildReportId);
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

	CPhmsRequestHead phmsRequestHead("10", "1050", m_stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::Feedback(string stringTitle, string stringNotes, string stringPhone, string stringFilePath)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildTitle = pDoc->createElement("title");
	AutoPtr<Element> pChildNotes = pDoc->createElement("notes");
	AutoPtr<Element> pChildPhone = pDoc->createElement("phone");
	AutoPtr<Text> pTextTitle = pDoc->createTextNode(stringTitle);
	AutoPtr<Text> pTextNotes = pDoc->createTextNode(stringNotes);
	AutoPtr<Text> pTextPhone = pDoc->createTextNode(stringPhone);
	pChildTitle->appendChild(pTextTitle);
	pChildNotes->appendChild(pTextNotes);
	pChildPhone->appendChild(pTextPhone);
	pRoot->appendChild(pChildTitle);
	pRoot->appendChild(pChildNotes);
	pRoot->appendChild(pChildPhone);
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

	CPhmsRequestHead phmsRequestHead("10", "1051", m_stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, stringFilePath);
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::UploadTrendData2(string stringTargetPath)
{
	ifstream ifTarget;
	ostringstream ostringTarget;
	Base64Encoder encoderBase64(ostringTarget);
	encoderBase64.rdbuf()->setLineLength(0);
	try
	{
		ifTarget.exceptions(ios::badbit);
		ifTarget.open(stringTargetPath.c_str(), ios_base::in|ios_base::binary);
		StreamCopier::copyStream(ifTarget, encoderBase64);
		ifTarget.close();
	}
	catch (ios_base::failure& e)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(e.what(), __FUNCTION__, __FILE__, __LINE__);
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_FILE_READ_FAIL), __FUNCTION__, __FILE__, __LINE__);
		return COMMON_FILE_READ_FAIL;

	}
	encoderBase64.close();
	string stringXmlContent = ostringTarget.str();
	int nRet = PHMS_SUCCESSFUL_RESULT;
	CPhmsRequestHead phmsRequestHead("10", "1052", m_stringSessionId, "12");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::GetAllTeamList()
{
	CPhmsRequestHead phmsRequestHead("10", "1053", m_stringSessionId, "10");
	CPhmsRequestBody phmsRequestBody("");
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	int nRet = PHMS_SUCCESSFUL_RESULT;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::GetDoctorListOfTeam(string stringTeamId)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildTeamId = pDoc->createElement("teamid");
	AutoPtr<Text> pTextTeamId = pDoc->createTextNode(stringTeamId);
	pChildTeamId->appendChild(pTextTeamId);
	pRoot->appendChild(pChildTeamId);
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

	CPhmsRequestHead phmsRequestHead("10", "1054", m_stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}
int CPhmsSession::GetEventStream(string stringCaseId, string stringOriginalHospital)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildCaseId = pDoc->createElement("caseid");
	AutoPtr<Element> pChildSourceHospital = pDoc->createElement("horgorder");
	AutoPtr<Text> pTextCaseId = pDoc->createTextNode(stringCaseId);
	AutoPtr<Text> pTextOriginalHospital = pDoc->createTextNode(stringOriginalHospital);
	pChildCaseId->appendChild(pTextCaseId);
	pChildSourceHospital->appendChild(pTextOriginalHospital);
	pRoot->appendChild(pChildCaseId);
	pRoot->appendChild(pChildSourceHospital);
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

	CPhmsRequestHead phmsRequestHead("10", "1055", m_stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}
int CPhmsSession::GetCaseTypeList(string stringLanguage)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildLanguage = pDoc->createElement("lang");
	AutoPtr<Text> pTextLanguage = pDoc->createTextNode(stringLanguage);
	pChildLanguage->appendChild(pTextLanguage);
	pRoot->appendChild(pChildLanguage);
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

	CPhmsRequestHead phmsRequestHead("10", "1056", m_stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::DoctorHandledCaseStatistics(string stringCaseType, string stringStartTime, string stringEndTime)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildCaseType = pDoc->createElement("casetype");
	AutoPtr<Element> pChildStartTime = pDoc->createElement("startdate");
	AutoPtr<Element> pChildEndTime = pDoc->createElement("enddate");
	AutoPtr<Text> pTextCaseType = pDoc->createTextNode(stringCaseType);
	AutoPtr<Text> pTextStartTime = pDoc->createTextNode(stringStartTime);
	AutoPtr<Text> pTextEndTime = pDoc->createTextNode(stringEndTime);
	pChildCaseType->appendChild(pTextCaseType);
	pChildStartTime->appendChild(pTextStartTime);
	pChildEndTime->appendChild(pTextEndTime);
	pRoot->appendChild(pChildCaseType);
	pRoot->appendChild(pChildStartTime);
	pRoot->appendChild(pChildEndTime);
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

	CPhmsRequestHead phmsRequestHead("10", "1057", m_stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::UserRegister(string stringPhone, string stringName, string stringHeigth)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildPhone = pDoc->createElement("phone");
	AutoPtr<Element> pChildName = pDoc->createElement("name");
	AutoPtr<Element> pChildHeigth = pDoc->createElement("heigth");
	AutoPtr<Text> pTextPhone = pDoc->createTextNode(stringPhone);
	AutoPtr<Text> pTextName = pDoc->createTextNode(stringName);
	AutoPtr<Text> pTextHeigth = pDoc->createTextNode(stringHeigth);
	pChildPhone->appendChild(pTextPhone);
	pChildName->appendChild(pTextName);
	pChildHeigth->appendChild(pTextHeigth);
	pRoot->appendChild(pChildPhone);
	pRoot->appendChild(pChildName);
	pRoot->appendChild(pChildHeigth);
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

	string stringSessionId(32, '0');
	CPhmsRequestHead phmsRequestHead("10", "1058", stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	//phmsRequest.GenerateAndSetSign();签名为32个0的原因与登录一致
	CPhmsResponse phmsResponse;
	m_enumPhmsCommandType = REGISTER_TYPE;//目前只有登录命令是LOGIN_TYPE，改变个人信息命令是CHANGE_INFO_TYPE，用户注册命令是REGISTER_TYPE
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	m_enumPhmsCommandType = MAIN_TYPE;//其余都是MAIN_TYPE
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}
int CPhmsSession::GetTrendData(string stringUId, string stringInterUId, string stringTrendType, string stringStartDate, string stringEndDate, string stringNextId)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildUId = pDoc->createElement("uid");
	AutoPtr<Element> pChildInterUId = pDoc->createElement("interuid");
	AutoPtr<Element> pChildTrendType = pDoc->createElement("trendtype");
	AutoPtr<Element> pChildStartDate = pDoc->createElement("startdate");
	AutoPtr<Element> pChildEndDate = pDoc->createElement("enddate");
	AutoPtr<Element> pChildNextId = pDoc->createElement("nextid");
	AutoPtr<Text> pTextUId = pDoc->createTextNode(stringUId);
	AutoPtr<Text> pTextInterUId = pDoc->createTextNode(stringInterUId);
	AutoPtr<Text> pTextTrendType = pDoc->createTextNode(stringTrendType);
	AutoPtr<Text> pTextStartDate = pDoc->createTextNode(stringStartDate);
	AutoPtr<Text> pTextEndDate = pDoc->createTextNode(stringEndDate);
	AutoPtr<Text> pTextNextId = pDoc->createTextNode(stringNextId);
	pChildUId->appendChild(pTextUId);
	pChildInterUId->appendChild(pTextInterUId);
	pChildTrendType->appendChild(pTextTrendType);
	pChildStartDate->appendChild(pTextStartDate);
	pChildEndDate->appendChild(pTextEndDate);
	pChildNextId->appendChild(pTextNextId);
	pRoot->appendChild(pChildUId);
	pRoot->appendChild(pChildInterUId);
	pRoot->appendChild(pChildTrendType);
	pRoot->appendChild(pChildStartDate);
	pRoot->appendChild(pChildEndDate);
	pRoot->appendChild(pChildNextId);
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

	CPhmsRequestHead phmsRequestHead("10", "1059", m_stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::GetLastPhysiologicalData()
{
	CPhmsRequestHead phmsRequestHead("10", "1060", m_stringSessionId, "10");
	CPhmsRequestBody phmsRequestBody("");
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	int nRet = PHMS_SUCCESSFUL_RESULT;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}
int CPhmsSession::GetHealthDegree(string stringUId, string stringInterUId, string stringDate)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildUId = pDoc->createElement("uid");
	AutoPtr<Element> pChildInterUId = pDoc->createElement("interuid");
	AutoPtr<Element> pChildDate = pDoc->createElement("date");
	AutoPtr<Text> pTextUId = pDoc->createTextNode(stringUId);
	AutoPtr<Text> pTextInterUId = pDoc->createTextNode(stringInterUId);
	AutoPtr<Text> pTextDate = pDoc->createTextNode(stringDate);
	pChildUId->appendChild(pTextUId);
	pChildInterUId->appendChild(pTextInterUId);
	pChildDate->appendChild(pTextDate);
	pRoot->appendChild(pChildUId);
	pRoot->appendChild(pChildInterUId);
	pRoot->appendChild(pChildDate);
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

	CPhmsRequestHead phmsRequestHead("10", "1061", m_stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}
int CPhmsSession::GetUserOfNoAdviceAndMostData(string stringDays)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildDays = pDoc->createElement("days");
	AutoPtr<Text> pTextDays = pDoc->createTextNode(stringDays);
	pChildDays->appendChild(pTextDays);
	pRoot->appendChild(pChildDays);
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

	CPhmsRequestHead phmsRequestHead("10", "1062", m_stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}
int CPhmsSession::AdviceDelay(string stringUId, string stringMinutes)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildUId = pDoc->createElement("uid");
	AutoPtr<Element> pChildMinutes = pDoc->createElement("minutes");
	AutoPtr<Text> pTextUId = pDoc->createTextNode(stringUId);
	AutoPtr<Text> pTextMinutes = pDoc->createTextNode(stringMinutes);
	pChildUId->appendChild(pTextUId);
	pChildMinutes->appendChild(pTextMinutes);
	pRoot->appendChild(pChildUId);
	pRoot->appendChild(pChildMinutes);
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

	CPhmsRequestHead phmsRequestHead("10", "1063", m_stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}
int CPhmsSession::AdviceComplete(string stringUId, string stringOptionType, string stringAdvice)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildUId = pDoc->createElement("uid");
	AutoPtr<Element> pChildOptionType = pDoc->createElement("optype");
	AutoPtr<Element> pChildAdvice = pDoc->createElement("advice");
	AutoPtr<Text> pTextUId = pDoc->createTextNode(stringUId);
	AutoPtr<Text> pTextOptionType = pDoc->createTextNode(stringOptionType);
	AutoPtr<Text> pTextAdvice = pDoc->createTextNode(stringAdvice);
	pChildUId->appendChild(pTextUId);
	pChildOptionType->appendChild(pTextOptionType);
	pChildAdvice->appendChild(pTextAdvice);
	pRoot->appendChild(pChildUId);
	pRoot->appendChild(pChildOptionType);
	pRoot->appendChild(pChildAdvice);
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

	CPhmsRequestHead phmsRequestHead("10", "1064", m_stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}
int CPhmsSession::GetUserListOfNoAdvice(string stringDays, string stringPageTo, string stringPageSize)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildDays = pDoc->createElement("days");
	AutoPtr<Element> pChildPageTo = pDoc->createElement("pageto");
	AutoPtr<Element> pChildPageSize = pDoc->createElement("pagesize");
	AutoPtr<Text> pTextDays = pDoc->createTextNode(stringDays);
	AutoPtr<Text> pTextPageTo = pDoc->createTextNode(stringPageTo);
	AutoPtr<Text> pTextPageSize = pDoc->createTextNode(stringPageSize);
	pChildDays->appendChild(pTextDays);
	pChildPageTo->appendChild(pTextPageTo);
	pChildPageSize->appendChild(pTextPageSize);
	pRoot->appendChild(pChildDays);
	pRoot->appendChild(pChildPageTo);
	pRoot->appendChild(pChildPageSize);
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

	CPhmsRequestHead phmsRequestHead("10", "1065", m_stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}
int CPhmsSession::AdviceBegin(string stringUId)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildUId = pDoc->createElement("uid");
	AutoPtr<Text> pTextUId = pDoc->createTextNode(stringUId);
	pChildUId->appendChild(pTextUId);
	pRoot->appendChild(pChildUId);
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

	CPhmsRequestHead phmsRequestHead("10", "1066", m_stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}
int CPhmsSession::GetUserListOfAdvicing(string stringPageTo, string stringPageSize)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildPageTo = pDoc->createElement("pageto");
	AutoPtr<Element> pChildPageSize = pDoc->createElement("pagesize");
	AutoPtr<Text> pTextPageTo = pDoc->createTextNode(stringPageTo);
	AutoPtr<Text> pTextPageSize = pDoc->createTextNode(stringPageSize);
	pChildPageTo->appendChild(pTextPageTo);
	pChildPageSize->appendChild(pTextPageSize);
	pRoot->appendChild(pChildPageTo);
	pRoot->appendChild(pChildPageSize);
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

	CPhmsRequestHead phmsRequestHead("10", "1067", m_stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}
int CPhmsSession::VerifyThirdId(string stringUId, string stringType)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildUId = pDoc->createElement("uid");
	AutoPtr<Element> pChildType = pDoc->createElement("type");
	AutoPtr<Text> pTextUId = pDoc->createTextNode(stringUId);
	AutoPtr<Text> pTextType = pDoc->createTextNode(stringType);
	pChildUId->appendChild(pTextUId);
	pChildType->appendChild(pTextType);
	pRoot->appendChild(pChildUId);
	pRoot->appendChild(pChildType);
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

	string stringSessionId(32, '0');
	CPhmsRequestHead phmsRequestHead("10", "1068", stringSessionId/*m_stringSessionId*/, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	//phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::PersonIdRegister(string stringType, string stringRegFrom, string stringPersonId, string stringName, string stringSex, string stringBirthday, string stringAddress, 
					 string stringPhone, string stringHeigth, string stringUploadUnitId, string stringHospitalId, string stringHGroupId,string stringScode, string stringWxUId, string stringLmp, string stringWeight, string stringPassword, 
					 string stringThirdCode, string stringIsRegister)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildType = pDoc->createElement("type");
	AutoPtr<Element> pChildRegFrom = pDoc->createElement("regfrom");
	AutoPtr<Element> pChildPersonId = pDoc->createElement("personid");
	AutoPtr<Element> pChildName = pDoc->createElement("name");
	AutoPtr<Element> pChildSex = pDoc->createElement("sex");
	AutoPtr<Element> pChildBirthday = pDoc->createElement("birthday");
	AutoPtr<Element> pChildAddress = pDoc->createElement("address");
	AutoPtr<Element> pChildPhone = pDoc->createElement("phone");
	AutoPtr<Element> pChildHeigth = pDoc->createElement("height");
	AutoPtr<Element> pChildUploadUnitId = pDoc->createElement("upload_unitid");
	AutoPtr<Element> pChildHospitalId = pDoc->createElement("hospitalid");
	AutoPtr<Element> pChildHGroupId = pDoc->createElement("hgroupid");
	AutoPtr<Element> pChildScode = pDoc->createElement("scode");
	AutoPtr<Element> pChildWxUId = pDoc->createElement("wxuid");
	AutoPtr<Element> pChildLmp = pDoc->createElement("lmp");
	AutoPtr<Element> pChildWeight = pDoc->createElement("weight");
	AutoPtr<Element> pChildPassword = pDoc->createElement("password");
	AutoPtr<Element> pChildThirdCode = pDoc->createElement("thirdcode");
	AutoPtr<Element> pChildIsRegister = pDoc->createElement("isregister");
	AutoPtr<Text> pTextType = pDoc->createTextNode(stringType);
	AutoPtr<Text> pTextRegFrom = pDoc->createTextNode(stringRegFrom);
	AutoPtr<Text> pTextPersonId = pDoc->createTextNode(stringPersonId);
	AutoPtr<Text> pTextName = pDoc->createTextNode(stringName);
	AutoPtr<Text> pTextSex = pDoc->createTextNode(stringSex);
	AutoPtr<Text> pTextBirthday = pDoc->createTextNode(stringBirthday);
	AutoPtr<Text> pTextAddress = pDoc->createTextNode(stringAddress);
	AutoPtr<Text> pTextPhone = pDoc->createTextNode(stringPhone);
	AutoPtr<Text> pTextHeigth = pDoc->createTextNode(stringHeigth);
	AutoPtr<Text> pTextUploadUnitId = pDoc->createTextNode(stringUploadUnitId);
	AutoPtr<Text> pTextHospitalId = pDoc->createTextNode(stringHospitalId);
	AutoPtr<Text> pTextHGroupId = pDoc->createTextNode(stringHGroupId);
	AutoPtr<Text> pTextScode = pDoc->createTextNode(stringScode);
	AutoPtr<Text> pTextWxUId = pDoc->createTextNode(stringWxUId);
	AutoPtr<Text> pTextLmp = pDoc->createTextNode(stringLmp);
	AutoPtr<Text> pTextWeight = pDoc->createTextNode(stringWeight);
	AutoPtr<Text> pTextPassword = pDoc->createTextNode(stringPassword);
	AutoPtr<Text> pTextThirdCode = pDoc->createTextNode(stringThirdCode);
	AutoPtr<Text> pTextIsRegister = pDoc->createTextNode(stringIsRegister);
	pChildType->appendChild(pTextType);
	pChildRegFrom->appendChild(pTextRegFrom);
	pChildPersonId->appendChild(pTextPersonId);
	pChildName->appendChild(pTextName);
	pChildSex->appendChild(pTextSex);
	pChildBirthday->appendChild(pTextBirthday);
	pChildAddress->appendChild(pTextAddress);
	pChildPhone->appendChild(pTextPhone);
	pChildHeigth->appendChild(pTextHeigth);
	pChildUploadUnitId->appendChild(pTextUploadUnitId);
	pChildHospitalId->appendChild(pTextHospitalId);
	pChildHGroupId->appendChild(pTextHGroupId);
	pChildScode->appendChild(pTextScode);
	pChildWxUId->appendChild(pTextWxUId);
	pChildLmp->appendChild(pTextLmp);
	pChildWeight->appendChild(pTextWeight);
	pChildPassword->appendChild(pTextPassword);
	pChildThirdCode->appendChild(pTextThirdCode);
	pChildIsRegister->appendChild(pTextIsRegister);
	pRoot->appendChild(pChildType);
	pRoot->appendChild(pChildRegFrom);
	pRoot->appendChild(pChildPersonId);
	pRoot->appendChild(pChildName);
	pRoot->appendChild(pChildSex);
	pRoot->appendChild(pChildBirthday);
	pRoot->appendChild(pChildAddress);
	pRoot->appendChild(pChildPhone);
	pRoot->appendChild(pChildHeigth);
	pRoot->appendChild(pChildUploadUnitId);
	pRoot->appendChild(pChildHospitalId);
	pRoot->appendChild(pChildHGroupId);
	pRoot->appendChild(pChildScode);
	pRoot->appendChild(pChildWxUId);
	pRoot->appendChild(pChildLmp);
	pRoot->appendChild(pChildWeight);
	pRoot->appendChild(pChildPassword);
	pRoot->appendChild(pChildThirdCode);
	pRoot->appendChild(pChildIsRegister);
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
	string stringSessionId(32, '0');
	CPhmsRequestHead phmsRequestHead("10", "1069", stringSessionId, "11");
	if(stringType=="1")
	{
		phmsRequestHead.SetSessionId(m_stringSessionId);
	}
	if(stringType=="4" || stringType=="5")
	{
		if(m_stringSessionId.size() != 0)
		{
			phmsRequestHead.SetSessionId(m_stringSessionId);
		}
	}
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	if(stringType=="1" || ((stringType=="4" || stringType=="5") && m_stringSessionId.size() != 0))
	{
		phmsRequest.GenerateAndSetSign();
	}
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::GetVerificationCode(string stringType, string stringPhone)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildType = pDoc->createElement("type");
	AutoPtr<Element> pChildPhone = pDoc->createElement("phone");
	AutoPtr<Text> pTextType = pDoc->createTextNode(stringType);
	AutoPtr<Text> pTextPhone = pDoc->createTextNode(stringPhone);
	pChildType->appendChild(pTextType);
	pChildPhone->appendChild(pTextPhone);
	pRoot->appendChild(pChildType);
	pRoot->appendChild(pChildPhone);
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

	string stringSessionId(32, '0');
	CPhmsRequestHead phmsRequestHead("10", "1070", stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	//phmsRequest.GenerateAndSetSign();签名为32个0的原因与登录一样
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::GetAskList(string stringAskType, string stringAskEnd, string stringDoctorId, string stringIsAsk, string stringUserId, string stringStartTime, string stringEndTime, string stringPageTo, string stringPageSize)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildAskType = pDoc->createElement("asktype");
	AutoPtr<Element> pChildAskEnd = pDoc->createElement("askend");
	AutoPtr<Element> pChildDoctorId = pDoc->createElement("doctorid");
	AutoPtr<Element> pChildIsAsk = pDoc->createElement("isask");
	AutoPtr<Element> pChildUserId = pDoc->createElement("userid");
	AutoPtr<Element> pChildStartTime = pDoc->createElement("starttime");
	AutoPtr<Element> pChildEndTime = pDoc->createElement("endtime");
	AutoPtr<Element> pChildPageTo = pDoc->createElement("pageto");
	AutoPtr<Element> pChildPageSize = pDoc->createElement("pagesize");
	AutoPtr<Text> pTextAskType = pDoc->createTextNode(stringAskType);
	AutoPtr<Text> pTextAskEnd = pDoc->createTextNode(stringAskEnd);
	AutoPtr<Text> pTextDoctorId = pDoc->createTextNode(stringDoctorId);
	AutoPtr<Text> pTextIsAsk = pDoc->createTextNode(stringIsAsk);
	AutoPtr<Text> pTextUserId = pDoc->createTextNode(stringUserId);
	AutoPtr<Text> pTextStartTime = pDoc->createTextNode(stringStartTime);
	AutoPtr<Text> pTextEndTime = pDoc->createTextNode(stringEndTime);
	AutoPtr<Text> pTextPageTo = pDoc->createTextNode(stringPageTo);
	AutoPtr<Text> pTextPageSize = pDoc->createTextNode(stringPageSize);
	pChildAskType->appendChild(pTextAskType);
	pChildAskEnd->appendChild(pTextAskEnd);
	pChildDoctorId->appendChild(pTextDoctorId);
	pChildIsAsk->appendChild(pTextIsAsk);
	pChildUserId->appendChild(pTextUserId);
	pChildStartTime->appendChild(pTextStartTime);
	pChildEndTime->appendChild(pTextEndTime);
	pChildPageTo->appendChild(pTextPageTo);
	pChildPageSize->appendChild(pTextPageSize);
	pRoot->appendChild(pChildAskType);
	pRoot->appendChild(pChildAskEnd);
	pRoot->appendChild(pChildDoctorId);
	pRoot->appendChild(pChildIsAsk);
	pRoot->appendChild(pChildUserId);
	pRoot->appendChild(pChildStartTime);
	pRoot->appendChild(pChildEndTime);
	pRoot->appendChild(pChildPageTo);
	pRoot->appendChild(pChildPageSize);
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

	CPhmsRequestHead phmsRequestHead("10", "1071", m_stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::GetAskDetail(string stringAskId)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildAskId = pDoc->createElement("askid");
	AutoPtr<Element> pChildIsAsked = pDoc->createElement("isasked");
	AutoPtr<Element> pChildDoctorId = pDoc->createElement("doctorid");
	AutoPtr<Text> pTextAskId = pDoc->createTextNode(stringAskId);
	AutoPtr<Text> pTextIsAsked = pDoc->createTextNode("");
	AutoPtr<Text> pTextDoctorId = pDoc->createTextNode("");
	pChildAskId->appendChild(pTextAskId);
	pChildIsAsked->appendChild(pTextIsAsked);
	pChildDoctorId->appendChild(pTextDoctorId);
	pRoot->appendChild(pChildAskId);
	pRoot->appendChild(pChildIsAsked);
	pRoot->appendChild(pChildDoctorId);
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

	CPhmsRequestHead phmsRequestHead("10", "1072", m_stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::GetQuestionList(string stringDoctorId, string stringReportType, string stringAskId)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildDoctorId = pDoc->createElement("doctorid");
	AutoPtr<Element> pChildReportType = pDoc->createElement("reporttype");
	AutoPtr<Element> pChildAskId = pDoc->createElement("askid");
	AutoPtr<Text> pTextDoctorId = pDoc->createTextNode(stringDoctorId);
	AutoPtr<Text> pTextReportType = pDoc->createTextNode(stringReportType);
	AutoPtr<Text> pTextAskId = pDoc->createTextNode(stringAskId);
	pChildDoctorId->appendChild(pTextDoctorId);
	pChildReportType->appendChild(pTextReportType);
	pChildAskId->appendChild(pTextAskId);
	pRoot->appendChild(pChildDoctorId);
	pRoot->appendChild(pChildReportType);
	pRoot->appendChild(pChildAskId);
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

	CPhmsRequestHead phmsRequestHead("10", "1073", m_stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::SubmitQuestion(string stringUserId, string stringAdvise, string stringReportType, string stringAskId, string stringAskedId, string stringDoctorPhotoPath)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildUserId = pDoc->createElement("userid");
	AutoPtr<Element> pChildAdvise = pDoc->createElement("advise");
	AutoPtr<Element> pChildReportType = pDoc->createElement("reporttype");
	AutoPtr<Element> pChildAskId = pDoc->createElement("askid");
	AutoPtr<Element> pChildAskedId = pDoc->createElement("askedid");
	AutoPtr<Element> pChildDoctorPhoto = pDoc->createElement("doctorphoto");;
	AutoPtr<Text> pTextUserId = pDoc->createTextNode(stringUserId);
	AutoPtr<Text> pTextAdvise = pDoc->createTextNode(stringAdvise);
	AutoPtr<Text> pTextReportType = pDoc->createTextNode(stringReportType);
	AutoPtr<Text> pTextAskId = pDoc->createTextNode(stringAskId);
	AutoPtr<Text> pTextAskedId = pDoc->createTextNode(stringAskedId);
	AutoPtr<Text> pTextDoctorPhoto = pDoc->createTextNode(stringDoctorPhotoPath);
	pChildUserId->appendChild(pTextUserId);
	pChildAdvise->appendChild(pTextAdvise);
	pChildReportType->appendChild(pTextReportType);
	pChildAskId->appendChild(pTextAskId);
	pChildAskedId->appendChild(pTextAskedId);
	pChildDoctorPhoto->appendChild(pTextDoctorPhoto);
	pRoot->appendChild(pChildUserId);
	pRoot->appendChild(pChildAdvise);
	pRoot->appendChild(pChildReportType);
	pRoot->appendChild(pChildAskId);
	pRoot->appendChild(pChildAskedId);
	pRoot->appendChild(pChildDoctorPhoto);
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

	CPhmsRequestHead phmsRequestHead("10", "1074", m_stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::GetDataDictionary()
{
	CPhmsRequestHead phmsRequestHead("10", "1075", m_stringSessionId, "10");
	CPhmsRequestBody phmsRequestBody("");
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	int nRet = PHMS_SUCCESSFUL_RESULT;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::LoggingHealthBaseInfo(string stringUserId, string stringWeblock, string stringNation, string stringProfression, string stringCrowd, string stringBloodType, 
						  string stringJwsJiBing, string stringJwsShouShu, string stringJwsWaiShang, string stringJwsShuXue, string stringJwsBaoLuShi, string stringJwsBaoLuShiBz, 
						  string stringJwsYiChuanJiBing, string stringGmsGuoMinShi, string stringGmsGuoMinShiBz, string stringJzsFuQin, string stringJzsFuQinBz, string stringJzsMuQin, 
						  string stringJzsMuQinBz, string stringJzsXdjm, string stringJzsXdjmBz, string stringJzsZiNv, string stringJzsZiNvBz, string stringJzsQiTa)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildUserId = pDoc->createElement("userid");
	AutoPtr<Element> pChildWeblock = pDoc->createElement("wedlock");
	AutoPtr<Element> pChildNation = pDoc->createElement("nation");
	AutoPtr<Element> pChildProfression = pDoc->createElement("profession");
	AutoPtr<Element> pChildCrowd = pDoc->createElement("crowd");
	AutoPtr<Element> pChildBloodType = pDoc->createElement("bloodtype");
	AutoPtr<Element> pChildJwsJiBing = pDoc->createElement("jws_jibing");
	AutoPtr<Element> pChildJwsShouShu = pDoc->createElement("jws_shoushu");
	AutoPtr<Element> pChildJwsWaiShang = pDoc->createElement("jws_waishang");
	AutoPtr<Element> pChildJwsShuXue = pDoc->createElement("jws_shuxue");
	AutoPtr<Element> pChildJwsBaoLuShi = pDoc->createElement("jws_baolushi");
	AutoPtr<Element> pChildJwsBaoLuShiBz = pDoc->createElement("jws_baolushi_bz");
	AutoPtr<Element> pChildJwsYiChuanJiBing = pDoc->createElement("jws_yichuanjibing");
	AutoPtr<Element> pChildGmsGuoMinShi = pDoc->createElement("gms_guominshi");
	AutoPtr<Element> pChildGmsGuoMinShiBz = pDoc->createElement("gms_guominshi_bz");
	AutoPtr<Element> pChildJzsFuQin = pDoc->createElement("jzs_fuqin");
	AutoPtr<Element> pChildJzsFuQinBz = pDoc->createElement("jzs_fuqin_bz");
	AutoPtr<Element> pChildJzsMuQin = pDoc->createElement("jzs_muqin");
	AutoPtr<Element> pChildJzsMuQinBz = pDoc->createElement("jzs_muqin_bz");
	AutoPtr<Element> pChildJzsXdjm = pDoc->createElement("jzs_xdjm");
	AutoPtr<Element> pChildJzsXdjmBz = pDoc->createElement("jzs_xdjm_bz");
	AutoPtr<Element> pChildJzsZiNv = pDoc->createElement("jzs_zinv");
	AutoPtr<Element> pChildJzsZiNvBz = pDoc->createElement("jzs_zinv_bz");
	AutoPtr<Element> pChildJzsQiTa = pDoc->createElement("jzs_qita");
	AutoPtr<Text> pTextUserId = pDoc->createTextNode(stringUserId);
	AutoPtr<Text> pTextWeblock = pDoc->createTextNode(stringWeblock);
	AutoPtr<Text> pTextNation = pDoc->createTextNode(stringNation);
	AutoPtr<Text> pTextProfression = pDoc->createTextNode(stringProfression);
	AutoPtr<Text> pTextCrowd = pDoc->createTextNode(stringCrowd);
	AutoPtr<Text> pTextBloodType = pDoc->createTextNode(stringBloodType);
	AutoPtr<Text> pTextJwsJiBing = pDoc->createTextNode(stringJwsJiBing);
	AutoPtr<Text> pTextJwsShouShu = pDoc->createTextNode(stringJwsShouShu);
	AutoPtr<Text> pTextJwsWaiShang = pDoc->createTextNode(stringJwsWaiShang);
	AutoPtr<Text> pTextJwsShuXue = pDoc->createTextNode(stringJwsShuXue);
	AutoPtr<Text> pTextJwsBaoLuShi = pDoc->createTextNode(stringJwsBaoLuShi);
	AutoPtr<Text> pTextJwsBaoLuShiBz = pDoc->createTextNode(stringJwsBaoLuShiBz);
	AutoPtr<Text> pTextJwsYiChuanJiBing = pDoc->createTextNode(stringJwsYiChuanJiBing);
	AutoPtr<Text> pTextGmsGuoMinShi = pDoc->createTextNode(stringGmsGuoMinShi);
	AutoPtr<Text> pTextGmsGuoMinShiBz = pDoc->createTextNode(stringGmsGuoMinShiBz);
	AutoPtr<Text> pTextJzsFuQin = pDoc->createTextNode(stringJzsFuQin);
	AutoPtr<Text> pTextJzsFuQinBz = pDoc->createTextNode(stringJzsFuQinBz);
	AutoPtr<Text> pTextJzsMuQin = pDoc->createTextNode(stringJzsMuQin);
	AutoPtr<Text> pTextJzsMuQinBz = pDoc->createTextNode(stringJzsMuQinBz);
	AutoPtr<Text> pTextJzsXdjm = pDoc->createTextNode(stringJzsXdjm);
	AutoPtr<Text> pTextJzsXdjmBz = pDoc->createTextNode(stringJzsXdjmBz);
	AutoPtr<Text> pTextJzsZiNv = pDoc->createTextNode(stringJzsZiNv);
	AutoPtr<Text> pTextJzsZiNvBz = pDoc->createTextNode(stringJzsZiNvBz);
	AutoPtr<Text> pTextJzsQiTa = pDoc->createTextNode(stringJzsQiTa);
	pChildUserId->appendChild(pTextUserId);
	pChildWeblock->appendChild(pTextWeblock);
	pChildNation->appendChild(pTextNation);
	pChildProfression->appendChild(pTextProfression);
	pChildCrowd->appendChild(pTextCrowd);
	pChildBloodType->appendChild(pTextBloodType);
	pChildJwsJiBing->appendChild(pTextJwsJiBing);
	pChildJwsShouShu->appendChild(pTextJwsShouShu);
	pChildJwsWaiShang->appendChild(pTextJwsWaiShang);
	pChildJwsShuXue->appendChild(pTextJwsShuXue);
	pChildJwsBaoLuShi->appendChild(pTextJwsBaoLuShi);
	pChildJwsBaoLuShiBz->appendChild(pTextJwsBaoLuShiBz);
	pChildJwsYiChuanJiBing->appendChild(pTextJwsYiChuanJiBing);
	pChildGmsGuoMinShi->appendChild(pTextGmsGuoMinShi);
	pChildGmsGuoMinShiBz->appendChild(pTextGmsGuoMinShiBz);
	pChildJzsFuQin->appendChild(pTextJzsFuQin);
	pChildJzsFuQinBz->appendChild(pTextJzsFuQinBz);
	pChildJzsMuQin->appendChild(pTextJzsMuQin);
	pChildJzsMuQinBz->appendChild(pTextJzsMuQinBz);
	pChildJzsXdjm->appendChild(pTextJzsXdjm);
	pChildJzsXdjmBz->appendChild(pTextJzsXdjmBz);
	pChildJzsZiNv->appendChild(pTextJzsZiNv);
	pChildJzsZiNvBz->appendChild(pTextJzsZiNvBz);
	pChildJzsQiTa->appendChild(pTextJzsQiTa);
	pRoot->appendChild(pChildUserId);
	pRoot->appendChild(pChildWeblock);
	pRoot->appendChild(pChildNation);
	pRoot->appendChild(pChildProfression);
	pRoot->appendChild(pChildCrowd);
	pRoot->appendChild(pChildBloodType);
	pRoot->appendChild(pChildJwsJiBing);
	pRoot->appendChild(pChildJwsShouShu);
	pRoot->appendChild(pChildJwsWaiShang);
	pRoot->appendChild(pChildJwsShuXue);
	pRoot->appendChild(pChildJwsBaoLuShi);
	pRoot->appendChild(pChildJwsBaoLuShiBz);
	pRoot->appendChild(pChildJwsYiChuanJiBing);
	pRoot->appendChild(pChildGmsGuoMinShi);
	pRoot->appendChild(pChildGmsGuoMinShiBz);
	pRoot->appendChild(pChildJzsFuQin);
	pRoot->appendChild(pChildJzsFuQinBz);
	pRoot->appendChild(pChildJzsMuQin);
	pRoot->appendChild(pChildJzsMuQinBz);
	pRoot->appendChild(pChildJzsXdjm);
	pRoot->appendChild(pChildJzsXdjmBz);
	pRoot->appendChild(pChildJzsZiNv);
	pRoot->appendChild(pChildJzsZiNvBz);
	pRoot->appendChild(pChildJzsQiTa);
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

	CPhmsRequestHead phmsRequestHead("10", "1076", m_stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::LoggingHealthCurrentInfo(string stringUserId, string stringShfsYinShi, string stringShfsXlzk, string stringShfsXlzkBz, string stringShfsXltz, string stringShfsXiYan, 
							 string stringShfsYinJiu, string stringShfsSyqk, string stringShfsYunDong, string stringShfsYunDongShiJian, string stringSysjcDanGuChun, string stringSysjcXueChangGui, 
							 string stringSysjcXueChangGuiBz, string stringSysjcNiaoChangGui, string stringSysjcNiaoChangGuiBz, string stringSysjcQiTa, string stringYongYao, string stringZhuYuan, 
							 string stringJkwtNaoXueGuan, string stringJkwtNaoXueGuanBz, string stringJkwtShenZang, string stringJkwtShenZangBz, string stringJkwtXinZang, string stringJkwtXinZangBz, 
							 string stringJkwtXueGuan, string stringJkwtXueGuanBz, string stringJkwtYanBu, string stringJkwtYanBuBz, string stringJkwtShenJing, string stringJkwtQiTa, string stringDate)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildUserId = pDoc->createElement("userid");
	AutoPtr<Element> pChildShfsYinShi = pDoc->createElement("shfs_yinshi");
	AutoPtr<Element> pChildShfsXlzk = pDoc->createElement("shfs_xinlizhuangkuang");
	AutoPtr<Element> pChildShfsXlzkBz = pDoc->createElement("shfs_xlzk_bz");
	AutoPtr<Element> pChildShfsXltz = pDoc->createElement("shfs_xltz");
	AutoPtr<Element> pChildShfsXiYan = pDoc->createElement("shfs_xiyan");
	AutoPtr<Element> pChildShfsYinJiu = pDoc->createElement("shfs_yinjiu");
	AutoPtr<Element> pChildShfsSyqk = pDoc->createElement("shfs_syqk");
	AutoPtr<Element> pChildShfsYunDong = pDoc->createElement("shfs_yundong");
	AutoPtr<Element> pChildShfsYunDongShiJian = pDoc->createElement("shfs_yundongshijian");
	AutoPtr<Element> pChildSysjcDanGuChun = pDoc->createElement("sysjc_danguchun");
	AutoPtr<Element> pChildSysjcXueChangGui = pDoc->createElement("sysjc_xuechanggui");
	AutoPtr<Element> pChildSysjcXueChangGuiBz = pDoc->createElement("sysjc_xuechanggui_bz");
	AutoPtr<Element> pChildSysjcNiaoChangGui = pDoc->createElement("sysjc_niaochanggui");
	AutoPtr<Element> pChildSysjcNiaoChangGuiBz = pDoc->createElement("sysjc_niaochanggui_bz");
	AutoPtr<Element> pChildSysjcQiTa = pDoc->createElement("sysjc_qita");
	AutoPtr<Element> pChildYongYao = pDoc->createElement("yongyao");
	AutoPtr<Element> pChildZhuYuan = pDoc->createElement("zhuyuan");
	AutoPtr<Element> pChildJkwtNaoXueGuan = pDoc->createElement("jkwt_naoxueguan");
	AutoPtr<Element> pChildJkwtNaoXueGuanBz = pDoc->createElement("jkwt_naoxueguan_bz");
	AutoPtr<Element> pChildJkwtShenZang = pDoc->createElement("jkwt_shenzang");
	AutoPtr<Element> pChildJkwtShenZangBz = pDoc->createElement("jkwt_shenzang_bz");
	AutoPtr<Element> pChildJkwtXinZang = pDoc->createElement("jkwt_xinzang");
	AutoPtr<Element> pChildJkwtXinZangBz = pDoc->createElement("jkwt_xinzang_bz");
	AutoPtr<Element> pChildJkwtXueGuan = pDoc->createElement("jkwt_xueguan");
	AutoPtr<Element> pChildJkwtXueGuanBz = pDoc->createElement("jkwt_xueguan_bz");
	AutoPtr<Element> pChildJkwtYanBu = pDoc->createElement("jkwt_yanbu");
	AutoPtr<Element> pChildJkwtYanBuBz = pDoc->createElement("jkwt_yanbu_bz");
	AutoPtr<Element> pChildJkwtShenJing = pDoc->createElement("jkwt_shenjing");
	AutoPtr<Element> pChildJkwtQiTa = pDoc->createElement("jkwt_qita");
	AutoPtr<Element> pChildDate = pDoc->createElement("date");
	AutoPtr<Text> pTextUserId = pDoc->createTextNode(stringUserId);
	AutoPtr<Text> pTextShfsYinShi = pDoc->createTextNode(stringShfsYinShi);
	AutoPtr<Text> pTextShfsXlzk = pDoc->createTextNode(stringShfsXlzk);
	AutoPtr<Text> pTextShfsXlzkBz = pDoc->createTextNode(stringShfsXlzkBz);
	AutoPtr<Text> pTextShfsXltz = pDoc->createTextNode(stringShfsXltz);
	AutoPtr<Text> pTextShfsXiYan = pDoc->createTextNode(stringShfsXiYan);
	AutoPtr<Text> pTextShfsYinJiu = pDoc->createTextNode(stringShfsYinJiu);
	AutoPtr<Text> pTextShfsSyqk = pDoc->createTextNode(stringShfsSyqk);
	AutoPtr<Text> pTextShfsYunDong = pDoc->createTextNode(stringShfsYunDong);
	AutoPtr<Text> pTextShfsYunDongShiJian = pDoc->createTextNode(stringShfsYunDongShiJian);
	AutoPtr<Text> pTextSysjcDanGuChun = pDoc->createTextNode(stringSysjcDanGuChun);
	AutoPtr<Text> pTextSysjcXueChangGui = pDoc->createTextNode(stringSysjcXueChangGui);
	AutoPtr<Text> pTextSysjcXueChangGuiBz = pDoc->createTextNode(stringSysjcXueChangGuiBz);
	AutoPtr<Text> pTextSysjcNiaoChangGui = pDoc->createTextNode(stringSysjcNiaoChangGui);
	AutoPtr<Text> pTextSysjcNiaoChangGuiBz = pDoc->createTextNode(stringSysjcNiaoChangGuiBz);
	AutoPtr<Text> pTextSysjcQiTa = pDoc->createTextNode(stringSysjcQiTa);
	AutoPtr<Text> pTextYongYao = pDoc->createTextNode(stringYongYao);
	AutoPtr<Text> pTextZhuYuan = pDoc->createTextNode(stringZhuYuan);
	AutoPtr<Text> pTextJkwtNaoXueGuan = pDoc->createTextNode(stringJkwtNaoXueGuan);
	AutoPtr<Text> pTextJkwtNaoXueGuanBz = pDoc->createTextNode(stringJkwtNaoXueGuanBz);
	AutoPtr<Text> pTextJkwtShenZang = pDoc->createTextNode(stringJkwtShenZang);
	AutoPtr<Text> pTextJkwtShenZangBz = pDoc->createTextNode(stringJkwtShenZangBz);
	AutoPtr<Text> pTextJkwtXinZang = pDoc->createTextNode(stringJkwtXinZang);
	AutoPtr<Text> pTextJkwtXinZangBz = pDoc->createTextNode(stringJkwtXinZangBz);
	AutoPtr<Text> pTextJkwtXueGuan = pDoc->createTextNode(stringJkwtXueGuan);
	AutoPtr<Text> pTextJkwtXueGuanBz = pDoc->createTextNode(stringJkwtXueGuanBz);
	AutoPtr<Text> pTextJkwtYanBu = pDoc->createTextNode(stringJkwtYanBu);
	AutoPtr<Text> pTextJkwtYanBuBz = pDoc->createTextNode(stringJkwtYanBuBz);
	AutoPtr<Text> pTextJkwtShenJing = pDoc->createTextNode(stringJkwtShenJing);
	AutoPtr<Text> pTextJkwtQiTa = pDoc->createTextNode(stringJkwtQiTa);
	AutoPtr<Text> pTextDate = pDoc->createTextNode(stringDate);
	pChildUserId->appendChild(pTextUserId);
	pChildShfsYinShi->appendChild(pTextShfsYinShi);
	pChildShfsXlzk->appendChild(pTextShfsXlzk);
	pChildShfsXlzkBz->appendChild(pTextShfsXlzkBz);
	pChildShfsXltz->appendChild(pTextShfsXltz);
	pChildShfsXiYan->appendChild(pTextShfsXiYan);
	pChildShfsYinJiu->appendChild(pTextShfsYinJiu);
	pChildShfsSyqk->appendChild(pTextShfsSyqk);
	pChildShfsYunDong->appendChild(pTextShfsYunDong);
	pChildShfsYunDongShiJian->appendChild(pTextShfsYunDongShiJian);
	pChildSysjcDanGuChun->appendChild(pTextSysjcDanGuChun);
	pChildSysjcXueChangGui->appendChild(pTextSysjcXueChangGui);
	pChildSysjcXueChangGuiBz->appendChild(pTextSysjcXueChangGuiBz);
	pChildSysjcNiaoChangGui->appendChild(pTextSysjcNiaoChangGui);
	pChildSysjcNiaoChangGuiBz->appendChild(pTextSysjcNiaoChangGuiBz);
	pChildSysjcQiTa->appendChild(pTextSysjcQiTa);
	pChildYongYao->appendChild(pTextYongYao);
	pChildZhuYuan->appendChild(pTextZhuYuan);
	pChildJkwtNaoXueGuan->appendChild(pTextJkwtNaoXueGuan);
	pChildJkwtNaoXueGuanBz->appendChild(pTextJkwtNaoXueGuanBz);
	pChildJkwtShenZang->appendChild(pTextJkwtShenZang);
	pChildJkwtShenZangBz->appendChild(pTextJkwtShenZangBz);
	pChildJkwtXinZang->appendChild(pTextJkwtXinZang);
	pChildJkwtXinZangBz->appendChild(pTextJkwtXinZangBz);
	pChildJkwtXueGuan->appendChild(pTextJkwtXueGuan);
	pChildJkwtXueGuanBz->appendChild(pTextJkwtXueGuanBz);
	pChildJkwtYanBu->appendChild(pTextJkwtYanBu);
	pChildJkwtYanBuBz->appendChild(pTextJkwtYanBuBz);
	pChildJkwtShenJing->appendChild(pTextJkwtShenJing);
	pChildJkwtQiTa->appendChild(pTextJkwtQiTa);
	pChildDate->appendChild(pTextDate);
	pRoot->appendChild(pChildUserId);
	pRoot->appendChild(pChildShfsYinShi);
	pRoot->appendChild(pChildShfsXlzk);
	pRoot->appendChild(pChildShfsXlzkBz);
	pRoot->appendChild(pChildShfsXltz);
	pRoot->appendChild(pChildShfsXiYan);
	pRoot->appendChild(pChildShfsYinJiu);
	pRoot->appendChild(pChildShfsSyqk);
	pRoot->appendChild(pChildShfsYunDong);
	pRoot->appendChild(pChildShfsYunDongShiJian);
	pRoot->appendChild(pChildSysjcDanGuChun);
	pRoot->appendChild(pChildSysjcXueChangGui);
	pRoot->appendChild(pChildSysjcXueChangGuiBz);
	pRoot->appendChild(pChildSysjcNiaoChangGui);
	pRoot->appendChild(pChildSysjcNiaoChangGuiBz);
	pRoot->appendChild(pChildSysjcQiTa);
	pRoot->appendChild(pChildYongYao);
	pRoot->appendChild(pChildZhuYuan);
	pRoot->appendChild(pChildJkwtNaoXueGuan);
	pRoot->appendChild(pChildJkwtNaoXueGuanBz);
	pRoot->appendChild(pChildJkwtShenZang);
	pRoot->appendChild(pChildJkwtShenZangBz);
	pRoot->appendChild(pChildJkwtXinZang);
	pRoot->appendChild(pChildJkwtXinZangBz);
	pRoot->appendChild(pChildJkwtXueGuan);
	pRoot->appendChild(pChildJkwtXueGuanBz);
	pRoot->appendChild(pChildJkwtYanBu);
	pRoot->appendChild(pChildJkwtYanBuBz);
	pRoot->appendChild(pChildJkwtShenJing);
	pRoot->appendChild(pChildJkwtQiTa);
	pRoot->appendChild(pChildDate);
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

	CPhmsRequestHead phmsRequestHead("10", "1077", m_stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::GetCliniqueListOfHospital(string stringHospitalId)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildHospitalId = pDoc->createElement("hospitalid");
	AutoPtr<Text> pTextHospitalId = pDoc->createTextNode(stringHospitalId);
	pChildHospitalId->appendChild(pTextHospitalId);
	pRoot->appendChild(pChildHospitalId);
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

	CPhmsRequestHead phmsRequestHead("10", "1078", m_stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::GetStatisticsInfo(string stringTypeId, string stringMonth, string stringMonthDay, string stringPageTo, string stringPageSize)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildTypeId = pDoc->createElement("typeid");
	AutoPtr<Element> pChildMonth = pDoc->createElement("statisticsdate");
	AutoPtr<Element> pChildMonthDay = pDoc->createElement("statisticsstartdate");
	AutoPtr<Element> pChildPageTo = pDoc->createElement("pageto");
	AutoPtr<Element> pChildPageSize = pDoc->createElement("pagesize");
	AutoPtr<Text> pTextTypeId = pDoc->createTextNode(stringTypeId);
	AutoPtr<Text> pTextMonth = pDoc->createTextNode(stringMonth);
	AutoPtr<Text> pTextMonthDay = pDoc->createTextNode(stringMonthDay);
	AutoPtr<Text> pTextPageTo = pDoc->createTextNode(stringPageTo);
	AutoPtr<Text> pTextPageSize = pDoc->createTextNode(stringPageSize);
	pChildTypeId->appendChild(pTextTypeId);
	pChildMonth->appendChild(pTextMonth);
	pChildMonthDay->appendChild(pTextMonthDay);
	pChildPageTo->appendChild(pTextPageTo);
	pChildPageSize->appendChild(pTextPageSize);
	pRoot->appendChild(pChildTypeId);
	pRoot->appendChild(pChildMonth);
	pRoot->appendChild(pChildMonthDay);
	pRoot->appendChild(pChildPageTo);
	pRoot->appendChild(pChildPageSize);
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

	CPhmsRequestHead phmsRequestHead("10", "1079", m_stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::UpdateCaseInfo(string stringHospitalId, string stringCaseId, string stringOtherParam, string stringFlag, string stringNotes, string stringReceiverId)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildHospitalId = pDoc->createElement("hospitalid");
	AutoPtr<Element> pChildCaseId = pDoc->createElement("caseid");
	AutoPtr<Element> pChildNotes = pDoc->createElement("notes");
	AutoPtr<Element> pChildOtherParam = pDoc->createElement("otherparam");
	AutoPtr<Element> pChildFlag = pDoc->createElement("flag");
	AutoPtr<Element> pChildReceiverId = pDoc->createElement("receiverid");
	AutoPtr<Text> pTextHospitalId = pDoc->createTextNode(stringHospitalId);
	AutoPtr<Text> pTextCaseId = pDoc->createTextNode(stringCaseId);
	AutoPtr<Text> pTextNotes = pDoc->createTextNode(stringNotes);
	AutoPtr<Text> pTextOtherParam = pDoc->createTextNode(stringOtherParam);
	AutoPtr<Text> pTextFlag = pDoc->createTextNode(stringFlag);
	AutoPtr<Text> pTextReceiverId = pDoc->createTextNode(stringReceiverId);
	pChildHospitalId->appendChild(pTextHospitalId);
	pChildCaseId->appendChild(pTextCaseId);
	pChildNotes->appendChild(pTextNotes);
	pChildOtherParam->appendChild(pTextOtherParam);
	pChildFlag->appendChild(pTextFlag);
	pChildReceiverId->appendChild(pTextReceiverId);
	pRoot->appendChild(pChildHospitalId);
	pRoot->appendChild(pChildCaseId);
	pRoot->appendChild(pChildNotes);
	pRoot->appendChild(pChildOtherParam);
	pRoot->appendChild(pChildFlag);
	pRoot->appendChild(pChildReceiverId);
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

	CPhmsRequestHead phmsRequestHead("10", "1080", m_stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::GetUserHealthList(string stringNextId, string stringTypeId, string stringSecondTypeId, string stringName, string stringSex, string strinThirdIdType, 
									string stringThirdId, string stringHospitalId, 
									string stringUnitSenderId, string stringIsData, string stringIntervalTime, string stringPopulations, string stringContractDoctor, string stringIsContract, string stringPageSize)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildNextId = pDoc->createElement("nextid");
	AutoPtr<Element> pChildTypeId = pDoc->createElement("typeid");
	AutoPtr<Element> pChildSecondTypeId = pDoc->createElement("secondtypeid");
	AutoPtr<Element> pChildName = pDoc->createElement("name");
	AutoPtr<Element> pChildSex = pDoc->createElement("sex");
	AutoPtr<Element> pChildThirdIdType = pDoc->createElement("thirdidtype");
	AutoPtr<Element> pChildThirdId = pDoc->createElement("thirdid");
	AutoPtr<Element> pChildHospitalId = pDoc->createElement("hospitalid");
	AutoPtr<Element> pChildUnitSenderId = pDoc->createElement("unitsenderid");
	AutoPtr<Element> pChildIsData = pDoc->createElement("isdata");
	AutoPtr<Element> pChildIntervalTime = pDoc->createElement("intervaltime");
	AutoPtr<Element> pChildPopulations = pDoc->createElement("populations");
	AutoPtr<Element> pChildContractDoctor = pDoc->createElement("contractdoctor");
	AutoPtr<Element> pChildIsContract = pDoc->createElement("iscontract");
	AutoPtr<Element> pChildPageSize = pDoc->createElement("pagesize");
	
	AutoPtr<Text> pTextNextId = pDoc->createTextNode(stringNextId);
	AutoPtr<Text> pTextTypeId = pDoc->createTextNode(stringTypeId);
	AutoPtr<Text> pTextSecondTypeId = pDoc->createTextNode(stringSecondTypeId);
	AutoPtr<Text> pTextName = pDoc->createTextNode(stringName);
	AutoPtr<Text> pTextSex = pDoc->createTextNode(stringSex);
	AutoPtr<Text> pTextThirdIdType = pDoc->createTextNode(strinThirdIdType);
	AutoPtr<Text> pTextThirdId = pDoc->createTextNode(stringThirdId);
	AutoPtr<Text> pTextHospitalId = pDoc->createTextNode(stringHospitalId);
	AutoPtr<Text> pTextUnitSenderId = pDoc->createTextNode(stringUnitSenderId);
	AutoPtr<Text> pTextIsData = pDoc->createTextNode(stringIsData);
	AutoPtr<Text> pTextIntervalTime = pDoc->createTextNode(stringIntervalTime);
	AutoPtr<Text> pTextPopulations = pDoc->createTextNode(stringPopulations);
	AutoPtr<Text> pTextContractDoctor = pDoc->createTextNode(stringContractDoctor);
	AutoPtr<Text> pTextIsContract = pDoc->createTextNode(stringIsContract);
	AutoPtr<Text> pTextPageSize = pDoc->createTextNode(stringPageSize);
	
	pChildNextId->appendChild(pTextNextId);
	pChildTypeId->appendChild(pTextTypeId);
	pChildSecondTypeId->appendChild(pTextSecondTypeId);
	pChildName->appendChild(pTextName);
	pChildSex->appendChild(pTextSex);
	pChildThirdIdType->appendChild(pTextThirdIdType);
	pChildThirdId->appendChild(pTextThirdId);
	pChildHospitalId->appendChild(pTextHospitalId);
	pChildUnitSenderId->appendChild(pTextUnitSenderId);
	pChildIsData->appendChild(pTextIsData);
	pChildIntervalTime->appendChild(pTextIntervalTime);
	pChildPopulations->appendChild(pTextPopulations);
	pChildContractDoctor->appendChild(pTextContractDoctor);
	pChildIsContract->appendChild(pTextIsContract);
	pChildPageSize->appendChild(pTextPageSize);
	
	pRoot->appendChild(pChildNextId);
	pRoot->appendChild(pChildTypeId);
	pRoot->appendChild(pChildSecondTypeId);
	pRoot->appendChild(pChildName);
	pRoot->appendChild(pChildSex);
	pRoot->appendChild(pChildThirdIdType);
	pRoot->appendChild(pChildThirdId);
	pRoot->appendChild(pChildHospitalId);
	pRoot->appendChild(pChildUnitSenderId);
	pRoot->appendChild(pChildIsData);
	pRoot->appendChild(pChildIntervalTime);
	pRoot->appendChild(pChildPopulations);
	pRoot->appendChild(pChildContractDoctor);
	pRoot->appendChild(pChildIsContract);
	pRoot->appendChild(pChildPageSize);
	
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

	CPhmsRequestHead phmsRequestHead("10", "1081", m_stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::GetAdviceImplemention(string stringThirdId, string stringHospitalId)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildThirdId = pDoc->createElement("thirdid");
	AutoPtr<Element> pChildHospitalId = pDoc->createElement("hospitalid");
	AutoPtr<Text> pTextThirdId = pDoc->createTextNode(stringThirdId);
	AutoPtr<Text> pTextHospitalId = pDoc->createTextNode(stringHospitalId);
	pChildThirdId->appendChild(pTextThirdId);
	pChildHospitalId->appendChild(pTextHospitalId);
	pRoot->appendChild(pChildThirdId);
	pRoot->appendChild(pChildHospitalId);
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

	CPhmsRequestHead phmsRequestHead("10", "1082", m_stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::ChangePersonType(string stringThirdId, string stringPeopleType, string stringHospitalId)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildThirdId = pDoc->createElement("thirdid");
	AutoPtr<Element> pChildPeopleType = pDoc->createElement("peopletype");
	AutoPtr<Element> pChildHospitalId = pDoc->createElement("hospitalid");
	AutoPtr<Text> pTextThirdId = pDoc->createTextNode(stringThirdId);
	AutoPtr<Text> pTextPeopleType = pDoc->createTextNode(stringPeopleType);
	AutoPtr<Text> pTextHospitalId = pDoc->createTextNode(stringHospitalId);
	pChildThirdId->appendChild(pTextThirdId);
	pChildPeopleType->appendChild(pTextPeopleType);
	pChildHospitalId->appendChild(pTextHospitalId);
	pRoot->appendChild(pChildThirdId);
	pRoot->appendChild(pChildPeopleType);
	pRoot->appendChild(pChildHospitalId);
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

	CPhmsRequestHead phmsRequestHead("10", "1083", m_stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::SetNextVisitTime(string stringThirdId, string stringInterViewTime, string stringHospitalId)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildThirdId = pDoc->createElement("thirdid");
	AutoPtr<Element> pChildInterViewDate = pDoc->createElement("interviewdate");
	AutoPtr<Element> pChildHospitalId = pDoc->createElement("hospitalid");
	AutoPtr<Text> pTextThirdId = pDoc->createTextNode(stringThirdId);
	AutoPtr<Text> pTextInterViewDate = pDoc->createTextNode(stringInterViewTime);
	AutoPtr<Text> pTextHospitalId = pDoc->createTextNode(stringHospitalId);
	pChildThirdId->appendChild(pTextThirdId);
	pChildInterViewDate->appendChild(pTextInterViewDate);
	pChildHospitalId->appendChild(pTextHospitalId);
	pRoot->appendChild(pChildThirdId);
	pRoot->appendChild(pChildInterViewDate);
	pRoot->appendChild(pChildHospitalId);
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

	CPhmsRequestHead phmsRequestHead("10", "1084", m_stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::GetPersonTypeList(string stringLanguage)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildLang = pDoc->createElement("lang");
	AutoPtr<Text> pTextLang = pDoc->createTextNode(stringLanguage);
	pChildLang->appendChild(pTextLang);
	pRoot->appendChild(pChildLang);
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

	CPhmsRequestHead phmsRequestHead("10", "1085", m_stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::PersonIdRegisterAndLogin(string stringRole, string stringPId, string stringName, string stringSex, string stringBirthday, string stringAddress, string stringDeviceId, string stringMd5, 
							 string stringLang, string stringRegFrom)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildDeviceId = pDoc->createElement("deviceid");
	AutoPtr<Element> pChildRegFrom = pDoc->createElement("regfrom");
	AutoPtr<Element> pChildPId = pDoc->createElement("pid");
	AutoPtr<Element> pChildName = pDoc->createElement("name");
	AutoPtr<Element> pChildSex = pDoc->createElement("sex");
	AutoPtr<Element> pChildBirthday = pDoc->createElement("birthday");
	AutoPtr<Element> pChildAddress = pDoc->createElement("address");
	AutoPtr<Element> pChildLang = pDoc->createElement("lang");
	AutoPtr<Element> pChildRole = pDoc->createElement("role");
	AutoPtr<Element> pChildMd5 = pDoc->createElement("md5");
	AutoPtr<Text> pTextDeviceId = pDoc->createTextNode(stringDeviceId);
	AutoPtr<Text> pTextRegFrom = pDoc->createTextNode(stringRegFrom);
	AutoPtr<Text> pTextPId = pDoc->createTextNode(stringPId);
	AutoPtr<Text> pTextName = pDoc->createTextNode(stringName);
	AutoPtr<Text> pTextSex = pDoc->createTextNode(stringSex);
	AutoPtr<Text> pTextBirthday = pDoc->createTextNode(stringBirthday);
	AutoPtr<Text> pTextAddress = pDoc->createTextNode(stringAddress);
	AutoPtr<Text> pTextLang = pDoc->createTextNode(stringLang);
	AutoPtr<Text> pTextRole = pDoc->createTextNode(stringRole);
	AutoPtr<Text> pTextMd5 = pDoc->createTextNode(stringMd5);
	pChildDeviceId->appendChild(pTextDeviceId);
	pChildRegFrom->appendChild(pTextRegFrom);
	pChildPId->appendChild(pTextPId);
	pChildName->appendChild(pTextName);
	pChildSex->appendChild(pTextSex);
	pChildBirthday->appendChild(pTextBirthday);
	pChildAddress->appendChild(pTextAddress);
	pChildLang->appendChild(pTextLang);
	pChildRole->appendChild(pTextRole);
	pChildMd5->appendChild(pTextMd5);
	pRoot->appendChild(pChildDeviceId);
	pRoot->appendChild(pChildRegFrom);
	pRoot->appendChild(pChildPId);
	pRoot->appendChild(pChildName);
	pRoot->appendChild(pChildSex);
	pRoot->appendChild(pChildBirthday);
	pRoot->appendChild(pChildAddress);
	pRoot->appendChild(pChildLang);
	pRoot->appendChild(pChildRole);
	pRoot->appendChild(pChildMd5);
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
	string stringSessionId(32, '0');
	CPhmsRequestHead phmsRequestHead("10", "1086", stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
//	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;

	m_enumPhmsCommandType = LOGIN_TYPE;//目前只有登录命令是LOGIN_TYPE，改变个人信息命令是CHANGE_INFO_TYPE，用户注册命令是REGISTER_TYPE
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	m_enumPhmsCommandType = MAIN_TYPE;//其余命令都是MAIN_TYPE
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	CPhmsRequest::SetUsername(stringPId);
	CPhmsRequest::SetPassword("");
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	//只有登录(1002)和注册并登录(1086)命令才需要，解析返回的xml，得到sessionid
	nRet = this->GetSessionId(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	CSessionIdManager::HandleSessionSync(m_stringSessionId);
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::CardBindPhone(string stringPhone, string stringScode)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildPhone = pDoc->createElement("phone");
	AutoPtr<Element> pChildScode = pDoc->createElement("scode");
	AutoPtr<Text> pTextPhone = pDoc->createTextNode(stringPhone);
	AutoPtr<Text> pTextSCode = pDoc->createTextNode(stringScode);
	pChildPhone->appendChild(pTextPhone);
	pChildScode->appendChild(pTextSCode);
	pRoot->appendChild(pChildPhone);
	pRoot->appendChild(pChildScode);
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

	CPhmsRequestHead phmsRequestHead("10", "1088", m_stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::ResetPassword(string stringPhone, string stringScode, string stringNewPwd)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildPhone = pDoc->createElement("phone");
	AutoPtr<Element> pChildScode = pDoc->createElement("scode");
	AutoPtr<Element> pChildNewPwd = pDoc->createElement("newpwd");
	AutoPtr<Text> pTextPhone = pDoc->createTextNode(stringPhone);
	AutoPtr<Text> pTextSCode = pDoc->createTextNode(stringScode);
	AutoPtr<Text> pTextNewPwd = pDoc->createTextNode(stringNewPwd);
	pChildPhone->appendChild(pTextPhone);
	pChildScode->appendChild(pTextSCode);
	pChildNewPwd->appendChild(pTextNewPwd);
	pRoot->appendChild(pChildPhone);
	pRoot->appendChild(pChildScode);
	pRoot->appendChild(pChildNewPwd);
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

	string stringSessionId(32, '0');
	CPhmsRequestHead phmsRequestHead("10", "1089", stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	//phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::UploadDaily(string stringType, string stringCreateTime, string stringFiles)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	//首先生成子xml文件,并获得内容
// 	string stringSubXmlContent;
// 	AutoPtr<Document> pSubDoc = new Document;
// 	//AutoPtr<ProcessingInstruction> pSubPi = pSubDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'");
// 	AutoPtr<Element> pSubRoot = pSubDoc->createElement("files");
// 	AutoPtr<Element> pSubChildMood = pSubDoc->createElement("feeling");
// 	AutoPtr<Text> pSubTextMood = pSubDoc->createTextNode(stringMood);
// 	pSubChildMood->appendChild(pSubTextMood);
// 	pSubRoot->appendChild(pSubChildMood);
// 	vector<string>::iterator iter;
// 	for(iter=vFileLocalPath.begin(); iter!=vFileLocalPath.end(); iter++)
// 	{
// 		AutoPtr<Element> pSubChildFile = pSubDoc->createElement("file");
// 		AutoPtr<Element> pSubGrandChildUri = pSubDoc->createElement("uri");
// 		AutoPtr<Text> pSubTextUri = pSubDoc->createTextNode(*iter);
// 		pSubGrandChildUri->appendChild(pSubTextUri);
// 		pSubChildFile->appendChild(pSubGrandChildUri);
// 		pSubRoot->appendChild(pSubChildFile);
// 	}
// 	//pSubDoc->appendChild(pSubPi);
// 	pSubDoc->appendChild(pSubRoot);
// 	nRet = this->GetRequestXmlNoBase64(pSubDoc, stringSubXmlContent, false);
// 	if(nRet != PHMS_SUCCESSFUL_RESULT)
// 	{
// 		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
// 		return nRet;
// 	}

	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildType = pDoc->createElement("type");
	AutoPtr<Element> pChildCreateTime = pDoc->createElement("createtime");
	AutoPtr<Element> pChildFiles = pDoc->createElement("files");
	AutoPtr<Text> pTextType = pDoc->createTextNode(stringType);
	AutoPtr<Text> pTextCreateTime = pDoc->createTextNode(stringCreateTime);
	AutoPtr<Text> pTextFiles = pDoc->createTextNode(stringFiles);
	pChildType->appendChild(pTextType);
	pChildCreateTime->appendChild(pTextCreateTime);
	pChildFiles->appendChild(pTextFiles);
	pRoot->appendChild(pChildType);
	pRoot->appendChild(pChildCreateTime);
	pRoot->appendChild(pChildFiles);
	//pDoc->appendChild(pi);
	pDoc->appendChild(pRoot);

	string stringXmlContent;
	
	nRet = this->GetRequestXml(pDoc, stringXmlContent);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}

	string stringSessionId(32, '0');
	if(m_stringSessionId.size() != 0)
	{
		stringSessionId = m_stringSessionId;
	}
	CPhmsRequestHead phmsRequestHead("10", "1090", stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	if(m_stringSessionId.size() != 0)
	{
		phmsRequest.GenerateAndSetSign();
	}
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::GetDateListOfDaily(string stringYear, string stringMonth)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildYear = pDoc->createElement("year");
	AutoPtr<Element> pChildMonth = pDoc->createElement("month");
	AutoPtr<Text> pTextYear = pDoc->createTextNode(stringYear);
	AutoPtr<Text> pTextMonth = pDoc->createTextNode(stringMonth);
	pChildYear->appendChild(pTextYear);
	pChildMonth->appendChild(pTextMonth);
	pRoot->appendChild(pChildYear);
	pRoot->appendChild(pChildMonth);
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

	string stringSessionId(32, '0');
	if(m_stringSessionId.size() != 0)
	{
		stringSessionId = m_stringSessionId;
	}
	CPhmsRequestHead phmsRequestHead("10", "1091", stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	if(m_stringSessionId.size() != 0)
	{
		phmsRequest.GenerateAndSetSign();
	}
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}
int CPhmsSession::GetDailyOfDate(string stringDate, string stringType)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildDate = pDoc->createElement("date");
	AutoPtr<Element> pChildType = pDoc->createElement("type");
	AutoPtr<Text> pTextDate = pDoc->createTextNode(stringDate);
	AutoPtr<Text> pTextType = pDoc->createTextNode(stringType);
	pChildDate->appendChild(pTextDate);
	pChildType->appendChild(pTextType);
	pRoot->appendChild(pChildDate);
	pRoot->appendChild(pChildType);
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

	string stringSessionId(32, '0');
	if(m_stringSessionId.size() != 0)
	{
		stringSessionId = m_stringSessionId;
	}
	CPhmsRequestHead phmsRequestHead("10", "1092", stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	if(m_stringSessionId.size() != 0)
	{
		phmsRequest.GenerateAndSetSign();
	}
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}
int CPhmsSession::ShareDaily(string stringType, string stringCreateTime)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildType = pDoc->createElement("type");
	AutoPtr<Element> pChildCreateTime = pDoc->createElement("createtime");
	AutoPtr<Text> pTextType = pDoc->createTextNode(stringType);
	AutoPtr<Text> pTextCreateTime = pDoc->createTextNode(stringCreateTime);
	pChildType->appendChild(pTextType);
	pChildCreateTime->appendChild(pTextCreateTime);
	pRoot->appendChild(pChildType);
	pRoot->appendChild(pChildCreateTime);
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

	string stringSessionId(32, '0');
	if(m_stringSessionId.size() != 0)
	{
		stringSessionId = m_stringSessionId;
	}
	CPhmsRequestHead phmsRequestHead("10", "1093", stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	if(m_stringSessionId.size() != 0)
	{
		phmsRequest.GenerateAndSetSign();
	}
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}
int CPhmsSession::DeleteDaily(string stringType, string stringDate)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildType = pDoc->createElement("type");
	AutoPtr<Element> pChildDate = pDoc->createElement("day");
	AutoPtr<Text> pTextType = pDoc->createTextNode(stringType);
	AutoPtr<Text> pTextDate = pDoc->createTextNode(stringDate);
	pChildType->appendChild(pTextType);
	pChildDate->appendChild(pTextDate);
	pRoot->appendChild(pChildType);
	pRoot->appendChild(pChildDate);
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

	string stringSessionId(32, '0');
	if(m_stringSessionId.size() != 0)
	{
		stringSessionId = m_stringSessionId;
	}
	CPhmsRequestHead phmsRequestHead("10", "1094", stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	if(m_stringSessionId.size() != 0)
	{
		phmsRequest.GenerateAndSetSign();
	}
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::PhoneScanQR(string stringQRCode)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildQRCode = pDoc->createElement("qrcode");
	AutoPtr<Text> pTextQRCode = pDoc->createTextNode(stringQRCode);
	pChildQRCode->appendChild(pTextQRCode);
	pRoot->appendChild(pChildQRCode);
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

	string stringSessionId(32, '0');
	if(m_stringSessionId.size() != 0)
	{
		stringSessionId = m_stringSessionId;
	}
	CPhmsRequestHead phmsRequestHead("10", "1095", stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	if(m_stringSessionId.size() != 0)
	{
		phmsRequest.GenerateAndSetSign();
	}
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::PhoneScanQRVerify(string stringQRCode)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildQRCode = pDoc->createElement("qrcode");
	AutoPtr<Text> pTextQRCode = pDoc->createTextNode(stringQRCode);
	pChildQRCode->appendChild(pTextQRCode);
	pRoot->appendChild(pChildQRCode);
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

	string stringSessionId(32, '0');
	if(m_stringSessionId.size() != 0)
	{
		stringSessionId = m_stringSessionId;
	}
	CPhmsRequestHead phmsRequestHead("10", "1096", stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	if(m_stringSessionId.size() != 0)
	{
		phmsRequest.GenerateAndSetSign();
	}
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::ApplyQRCode(string stringType, string stringScene)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildType = pDoc->createElement("type");
	AutoPtr<Element> pChildData = pDoc->createElement("data");
	AutoPtr<Element> pGrandChildScene = pDoc->createElement("scene");
	AutoPtr<Text> pTextType = pDoc->createTextNode(stringType);
	AutoPtr<Text> pTextScene = pDoc->createTextNode(stringScene);
	pChildType->appendChild(pTextType);
	pGrandChildScene->appendChild(pTextScene);
	pChildData->appendChild(pGrandChildScene);
	pRoot->appendChild(pChildType);
	pRoot->appendChild(pChildData);
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

	string stringSessionId(32, '0');
	if(m_stringSessionId.size() != 0)
	{
		stringSessionId = m_stringSessionId;
	}
	CPhmsRequestHead phmsRequestHead("10", "1097", stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	if(m_stringSessionId.size() != 0)
	{
		phmsRequest.GenerateAndSetSign();
	}
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::ValidateUserAccount(string stringLoginId, string stringPassword, string stringRole)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildRole = pDoc->createElement("role");
	AutoPtr<Element> pChildLoginId = pDoc->createElement("loginid");
	AutoPtr<Element> pChildPwd = pDoc->createElement("pwd");
	AutoPtr<Text> pTextRole = pDoc->createTextNode(stringRole);
	AutoPtr<Text> pTextLoginId = pDoc->createTextNode(stringLoginId);
	AutoPtr<Text> pTextPwd = pDoc->createTextNode(stringPassword);
	pChildRole->appendChild(pTextRole);
	pChildLoginId->appendChild(pTextLoginId);
	pChildPwd->appendChild(pTextPwd);
	pRoot->appendChild(pChildRole);
	pRoot->appendChild(pChildLoginId);
	pRoot->appendChild(pChildPwd);
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

	string stringSessionId(32, '0');
	if(m_stringSessionId.size() != 0)
	{
		stringSessionId = m_stringSessionId;
	}
	CPhmsRequestHead phmsRequestHead("10", "1099", stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	if(m_stringSessionId.size() != 0)
	{
		phmsRequest.GenerateAndSetSign();
	}
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse, true);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::RegisterBluetoothDevice(string stringUniqueSN, string stringSerial, string stringBluetoothAddress, string stringBluetoothName, string stringDeviceType)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildUniqueSN = pDoc->createElement("uniquesn");
	AutoPtr<Element> pChildSerial = pDoc->createElement("serial");
	AutoPtr<Element> pChildBluetoothAddress = pDoc->createElement("bluetoothaddress");
	AutoPtr<Element> pChildBluetoothName = pDoc->createElement("bluetoothname");
	AutoPtr<Element> pChildDeviceType = pDoc->createElement("devicetype");
	AutoPtr<Text> pTextUniqueSN = pDoc->createTextNode(stringUniqueSN);
	AutoPtr<Text> pTextSerial = pDoc->createTextNode(stringSerial);
	AutoPtr<Text> pTextBluetoothAddress = pDoc->createTextNode(stringBluetoothAddress);
	AutoPtr<Text> pTextBluetoothName = pDoc->createTextNode(stringBluetoothName);
	AutoPtr<Text> pTextDeviceType = pDoc->createTextNode(stringDeviceType);
	pChildUniqueSN->appendChild(pTextUniqueSN);
	pChildSerial->appendChild(pTextSerial);
	pChildBluetoothAddress->appendChild(pTextBluetoothAddress);
	pChildBluetoothName->appendChild(pTextBluetoothName);
	pChildDeviceType->appendChild(pTextDeviceType);
	pRoot->appendChild(pChildUniqueSN);
	pRoot->appendChild(pChildSerial);
	pRoot->appendChild(pChildBluetoothAddress);
	pRoot->appendChild(pChildBluetoothName);
	pRoot->appendChild(pChildDeviceType);
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

	CPhmsRequestHead phmsRequestHead("10", "1100", m_stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}
int CPhmsSession::GetBluetoothInfoOfDevice(string stringUniqueSN)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildUniqueSN = pDoc->createElement("uniquesn");
	AutoPtr<Text> pTextUniqueSN = pDoc->createTextNode(stringUniqueSN);
	pChildUniqueSN->appendChild(pTextUniqueSN);
	pRoot->appendChild(pChildUniqueSN);
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

	CPhmsRequestHead phmsRequestHead("10", "1101", m_stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}
int CPhmsSession::ChangeDeviceType(string stringUniqueSN, string stringDeviceType)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildUniqueSN = pDoc->createElement("uniquesn");
	AutoPtr<Element> pChildDeviceType = pDoc->createElement("devicetype");
	AutoPtr<Text> pTextUniqueSN = pDoc->createTextNode(stringUniqueSN);
	AutoPtr<Text> pTextDeviceType = pDoc->createTextNode(stringDeviceType);
	pChildUniqueSN->appendChild(pTextUniqueSN);
	pChildDeviceType->appendChild(pTextDeviceType);
	pRoot->appendChild(pChildUniqueSN);
	pRoot->appendChild(pChildDeviceType);
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

	CPhmsRequestHead phmsRequestHead("10", "1102", m_stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}
int CPhmsSession::UnregisterBluetoothDevice(string stringUniqueSN)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildUniqueSN = pDoc->createElement("uniquesn");
	AutoPtr<Text> pTextUniqueSN = pDoc->createTextNode(stringUniqueSN);
	pChildUniqueSN->appendChild(pTextUniqueSN);
	pRoot->appendChild(pChildUniqueSN);
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

	CPhmsRequestHead phmsRequestHead("10", "1103", m_stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::GetHospitalListOfArea(string stringAreaId, string stringRank, string stringBeta)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildAreaId = pDoc->createElement("areaid");
	AutoPtr<Element> pChildRank = pDoc->createElement("rank");
	AutoPtr<Element> pChildBeta = pDoc->createElement("beta");
	AutoPtr<Text> pTextAreaId = pDoc->createTextNode(stringAreaId);
	AutoPtr<Text> pTextRank = pDoc->createTextNode(stringRank);
	AutoPtr<Text> pTextBeta = pDoc->createTextNode(stringBeta);
	pChildAreaId->appendChild(pTextAreaId);
	pChildRank->appendChild(pTextRank);
	pChildBeta->appendChild(pTextBeta);
	pRoot->appendChild(pChildAreaId);
	pRoot->appendChild(pChildRank);
	pRoot->appendChild(pChildBeta);
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
	string stringSessionId(32, '0');
	CPhmsRequestHead phmsRequestHead("10", "1104", stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	//phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::UploadCommonFileSegment(string stringType, string stringExtra, string stringOrder, string stringTotalSize, 
					 string stringServerUri, string stringStart1, string stringEnd1, string stringMd51, string stringStart2, string stringEnd2, string stringMd52, string stringStart3, string stringEnd3, string stringMd53, string stringFilePath, string stringFileType)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildType = pDoc->createElement("type");
	AutoPtr<Element> pChildExtra = pDoc->createElement("extra");
	AutoPtr<Element> pChildOrder = pDoc->createElement("order");
	AutoPtr<Element> pChildTotalSize = pDoc->createElement("totalsize");
	AutoPtr<Element> pChildServerUri = pDoc->createElement("serveruri");
	AutoPtr<Element> pChildPart1 = pDoc->createElement("md51");
	AutoPtr<Element> pGrandChildStart1= pDoc->createElement("start");
	AutoPtr<Element> pGrandChildEnd1 = pDoc->createElement("end");
	AutoPtr<Element> pGrandChildMd51 = pDoc->createElement("md5");
	AutoPtr<Element> pChildPart2 = pDoc->createElement("md52");
	AutoPtr<Element> pGrandChildStart2= pDoc->createElement("start");
	AutoPtr<Element> pGrandChildEnd2 = pDoc->createElement("end");
	AutoPtr<Element> pGrandChildMd52 = pDoc->createElement("md5");
	AutoPtr<Element> pChildPart3 = pDoc->createElement("md53");
	AutoPtr<Element> pGrandChildStart3= pDoc->createElement("start");
	AutoPtr<Element> pGrandChildEnd3 = pDoc->createElement("end");
	AutoPtr<Element> pGrandChildMd53 = pDoc->createElement("md5");
	AutoPtr<Element> pChildFileType = pDoc->createElement("filetype");
	AutoPtr<Text> pTextType = pDoc->createTextNode(stringType);
	AutoPtr<Text> pTextExtra = pDoc->createTextNode(stringExtra);
	AutoPtr<Text> pTextOrder = pDoc->createTextNode(stringOrder);
	AutoPtr<Text> pTextTotalSize = pDoc->createTextNode(stringTotalSize);
	AutoPtr<Text> pTextServerUri = pDoc->createTextNode(stringServerUri);
	AutoPtr<Text> pTextStart1 = pDoc->createTextNode(stringStart1);
	AutoPtr<Text> pTextEnd1 = pDoc->createTextNode(stringEnd1);
	AutoPtr<Text> pTextMd51 = pDoc->createTextNode(stringMd51);
	AutoPtr<Text> pTextStart2 = pDoc->createTextNode(stringStart2);
	AutoPtr<Text> pTextEnd2 = pDoc->createTextNode(stringEnd2);
	AutoPtr<Text> pTextMd52 = pDoc->createTextNode(stringMd52);
	AutoPtr<Text> pTextStart3 = pDoc->createTextNode(stringStart3);
	AutoPtr<Text> pTextEnd3 = pDoc->createTextNode(stringEnd3);
	AutoPtr<Text> pTextMd53 = pDoc->createTextNode(stringMd53);
	AutoPtr<Text> pTextFileType = pDoc->createTextNode(stringFileType);
	pChildType->appendChild(pTextType);
	pChildExtra->appendChild(pTextExtra);
	pChildOrder->appendChild(pTextOrder);
	pChildTotalSize->appendChild(pTextTotalSize);
	pChildServerUri->appendChild(pTextServerUri);
	pGrandChildStart1->appendChild(pTextStart1);
	pGrandChildEnd1->appendChild(pTextEnd1);
	pGrandChildMd51->appendChild(pTextMd51);
	pGrandChildStart2->appendChild(pTextStart2);
	pGrandChildEnd2->appendChild(pTextEnd2);
	pGrandChildMd52->appendChild(pTextMd52);
	pGrandChildStart3->appendChild(pTextStart3);
	pGrandChildEnd3->appendChild(pTextEnd3);
	pGrandChildMd53->appendChild(pTextMd53);
	pChildPart1->appendChild(pGrandChildStart1);
	pChildPart1->appendChild(pGrandChildEnd1);
	pChildPart1->appendChild(pGrandChildMd51);
	pChildPart2->appendChild(pGrandChildStart2);
	pChildPart2->appendChild(pGrandChildEnd2);
	pChildPart2->appendChild(pGrandChildMd52);
	pChildPart3->appendChild(pGrandChildStart3);
	pChildPart3->appendChild(pGrandChildEnd3);
	pChildPart3->appendChild(pGrandChildMd53);
	pChildFileType->appendChild(pTextFileType);
	pRoot->appendChild(pChildType);
	pRoot->appendChild(pChildExtra);
	pRoot->appendChild(pChildOrder);
	pRoot->appendChild(pChildTotalSize);
	pRoot->appendChild(pChildServerUri);
	pRoot->appendChild(pChildPart1);
	pRoot->appendChild(pChildPart2);
	pRoot->appendChild(pChildPart3);
	pRoot->appendChild(pChildFileType);
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

	CPhmsRequestHead phmsRequestHead("10", "1105", m_stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, stringFilePath);
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::SearchSomebody(string stringUserId)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildUId = pDoc->createElement("uid");
	AutoPtr<Text> pTextUId = pDoc->createTextNode(stringUserId);
	pChildUId->appendChild(pTextUId);
	pRoot->appendChild(pChildUId);
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

	CPhmsRequestHead phmsRequestHead("10", "1106", m_stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}
int CPhmsSession::FollowSomebody(string stringUserId, string stringUsername, string stringNickname, string stringVCode)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildUId = pDoc->createElement("uid");
	AutoPtr<Element> pChildUName = pDoc->createElement("uname");
	AutoPtr<Element> pChildNickname = pDoc->createElement("nickname");
	AutoPtr<Element> pChildVCode = pDoc->createElement("vcode");
	AutoPtr<Text> pTextUId = pDoc->createTextNode(stringUserId);
	AutoPtr<Text> pTextUName = pDoc->createTextNode(stringUsername);
	AutoPtr<Text> pTextNickname = pDoc->createTextNode(stringNickname);
	AutoPtr<Text> pTextVCode = pDoc->createTextNode(stringVCode);
	pChildUId->appendChild(pTextUId);
	pChildUName->appendChild(pTextUName);
	pChildNickname->appendChild(pTextNickname);
	pChildVCode->appendChild(pTextVCode);
	pRoot->appendChild(pChildUId);
	pRoot->appendChild(pChildUName);
	pRoot->appendChild(pChildNickname);
	pRoot->appendChild(pChildVCode);
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

	CPhmsRequestHead phmsRequestHead("10", "1107", m_stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}
int CPhmsSession::MyFollowing(string stringUserId, string stringNextId, string stringFType)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildUId = pDoc->createElement("uid");
	AutoPtr<Element> pChildNextId = pDoc->createElement("nextid");
	AutoPtr<Element> pChildFType = pDoc->createElement("ftype");
	AutoPtr<Text> pTextUId = pDoc->createTextNode(stringUserId);
	AutoPtr<Text> pTextNextId = pDoc->createTextNode(stringNextId);
	AutoPtr<Text> pTextFType = pDoc->createTextNode(stringFType);
	pChildUId->appendChild(pTextUId);
	pChildNextId->appendChild(pTextNextId);
	pChildFType->appendChild(pTextFType);
	pRoot->appendChild(pChildUId);
	pRoot->appendChild(pChildNextId);
	pRoot->appendChild(pChildFType);
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

	CPhmsRequestHead phmsRequestHead("10", "1108", m_stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}
int CPhmsSession::MyFans(string stringUserId, string stringNextId)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildUId = pDoc->createElement("uid");
	AutoPtr<Element> pChildNextId = pDoc->createElement("nextid");
	AutoPtr<Text> pTextUId = pDoc->createTextNode(stringUserId);
	AutoPtr<Text> pTextNextId = pDoc->createTextNode(stringNextId);
	pChildUId->appendChild(pTextUId);
	pChildNextId->appendChild(pTextNextId);
	pRoot->appendChild(pChildUId);
	pRoot->appendChild(pChildNextId);
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

	CPhmsRequestHead phmsRequestHead("10", "1109", m_stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}
int CPhmsSession::ChangeNicknameOfFollowed(string stringUserId, string stringNickname)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildUId = pDoc->createElement("uid");
	AutoPtr<Element> pChildNickname = pDoc->createElement("nickname");
	AutoPtr<Text> pTextUId = pDoc->createTextNode(stringUserId);
	AutoPtr<Text> pTextNickname = pDoc->createTextNode(stringNickname);
	pChildUId->appendChild(pTextUId);
	pChildNickname->appendChild(pTextNickname);
	pRoot->appendChild(pChildUId);
	pRoot->appendChild(pChildNickname);
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

	CPhmsRequestHead phmsRequestHead("10", "1110", m_stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}
int CPhmsSession::CancelFollowing(string stringUserId)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildUId = pDoc->createElement("uid");
	AutoPtr<Text> pTextUId = pDoc->createTextNode(stringUserId);
	pChildUId->appendChild(pTextUId);
	pRoot->appendChild(pChildUId);
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

	CPhmsRequestHead phmsRequestHead("10", "1111", m_stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::PublishHealthyKnowledge(vector<HEALTHY_KNOWLEDGE> vHealthKnowledge)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	for(int i=0; i<vHealthKnowledge.size(); i++)
	{
		AutoPtr<Element> pChildMsg = pDoc->createElement("msg");
		AutoPtr<Element> pGrandChildTitle = pDoc->createElement("title");
		AutoPtr<Element> pGrandChildDescription = pDoc->createElement("description");
		AutoPtr<Element> pGrandChildUrl = pDoc->createElement("url");
		AutoPtr<Element> pGrandChildPicUrl = pDoc->createElement("picurl");
		AutoPtr<Text> pTextTitle = pDoc->createTextNode(vHealthKnowledge[i].title);
		AutoPtr<Text> pTextDescription = pDoc->createTextNode(vHealthKnowledge[i].description);
		AutoPtr<Text> pTextUrl = pDoc->createTextNode(vHealthKnowledge[i].url);
		AutoPtr<Text> pTextPicUrl = pDoc->createTextNode(vHealthKnowledge[i].picurl);
		pGrandChildTitle->appendChild(pTextTitle);
		pGrandChildDescription->appendChild(pTextDescription);
		pGrandChildUrl->appendChild(pTextUrl);
		pGrandChildPicUrl->appendChild(pTextPicUrl);
		pChildMsg->appendChild(pGrandChildTitle);
		pChildMsg->appendChild(pGrandChildDescription);
		pChildMsg->appendChild(pGrandChildUrl);
		pChildMsg->appendChild(pGrandChildPicUrl);
		pRoot->appendChild(pChildMsg);
	}
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

	CPhmsRequestHead phmsRequestHead("10", "1112", m_stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::GetListOfHealthyKnowledge(string stringSenderId, string stringNextId, string stringIds, string stringPageSize, string stringOrderBy)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildSenderId = pDoc->createElement("senderid");
	AutoPtr<Element> pChildNextId  = pDoc->createElement("nextid");
	AutoPtr<Element> pChildIds = pDoc->createElement("ids");
	AutoPtr<Element> pChildPageSize = pDoc->createElement("pagesize");
	AutoPtr<Element> pChildOrderBy = pDoc->createElement("orderby");
	AutoPtr<Text> pTextSenderId = pDoc->createTextNode(stringSenderId);
	AutoPtr<Text> pTextNextId = pDoc->createTextNode(stringNextId);
	AutoPtr<Text> pTextIds = pDoc->createTextNode(stringIds);
	AutoPtr<Text> pTextPageSize = pDoc->createTextNode(stringPageSize);
	AutoPtr<Text> pTextOrderBy = pDoc->createTextNode(stringOrderBy);
	pChildSenderId->appendChild(pTextSenderId);
	pChildNextId->appendChild(pTextNextId);
	pChildIds->appendChild(pTextIds);
	pChildPageSize->appendChild(pTextPageSize);
	pChildOrderBy->appendChild(pTextOrderBy);
	pRoot->appendChild(pChildSenderId);
	pRoot->appendChild(pChildNextId);
	pRoot->appendChild(pChildIds);
	pRoot->appendChild(pChildPageSize);
	pRoot->appendChild(pChildOrderBy);
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

	CPhmsRequestHead phmsRequestHead("10", "1113", m_stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::GenerateOrDeletePngZlibReport(string stringServerPath, string stringFlag)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildReportPath = pDoc->createElement("reportpath");
	AutoPtr<Element> pChildType  = pDoc->createElement("type");
	AutoPtr<Text> pTextReportPath = pDoc->createTextNode(stringServerPath);
	AutoPtr<Text> pTextType = pDoc->createTextNode(stringFlag);
	pChildReportPath->appendChild(pTextReportPath);
	pChildType->appendChild(pTextType);
	pRoot->appendChild(pChildReportPath);
	pRoot->appendChild(pChildType);
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

	CPhmsRequestHead phmsRequestHead("10", /*"1114"*/"", m_stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::GetDailyList(string stringType, string stringStartDate, string stringEndDate, string stringOrderBy, string stringNextId, string stringPageSize)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildType = pDoc->createElement("type");
	AutoPtr<Element> pChildStartDate  = pDoc->createElement("startdate");
	AutoPtr<Element> pChildEndDate = pDoc->createElement("enddate");
	AutoPtr<Element> pChildOrderBy = pDoc->createElement("orderby");
	AutoPtr<Element> pChildNextId = pDoc->createElement("nextid");
	AutoPtr<Element> pChildPageSize = pDoc->createElement("pagesize");
	AutoPtr<Text> pTextType = pDoc->createTextNode(stringType);
	AutoPtr<Text> pTextStartDate = pDoc->createTextNode(stringStartDate);
	AutoPtr<Text> pTextEndDate = pDoc->createTextNode(stringEndDate);
	AutoPtr<Text> pTextOrderBy = pDoc->createTextNode(stringOrderBy);
	AutoPtr<Text> pTextNextId = pDoc->createTextNode(stringNextId);
	AutoPtr<Text> pTextPageSize = pDoc->createTextNode(stringPageSize);
	pChildType->appendChild(pTextType);
	pChildStartDate->appendChild(pTextStartDate);
	pChildEndDate->appendChild(pTextEndDate);
	pChildOrderBy->appendChild(pTextOrderBy);
	pChildNextId->appendChild(pTextNextId);
	pChildPageSize->appendChild(pTextPageSize);
	pRoot->appendChild(pChildType);
	pRoot->appendChild(pChildStartDate);
	pRoot->appendChild(pChildEndDate);
	pRoot->appendChild(pChildOrderBy);
	pRoot->appendChild(pChildNextId);
	pRoot->appendChild(pChildPageSize);
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

	CPhmsRequestHead phmsRequestHead("10", "1114", m_stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}


int CPhmsSession::GetCardUserReportList(string stringReportId, string stringReportStartDate, string stringReportEndDate, string stringCaseType, string stringState, string stringOrderBy, 
		string stringThirdIdType, string stringThirdId, string stringNextId, string stringPageSize)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildReportId = pDoc->createElement("reportid");
	AutoPtr<Element> pChildReportStartDate  = pDoc->createElement("reportstartdate");
	AutoPtr<Element> pChildReportEndDate = pDoc->createElement("reportenddate");
	AutoPtr<Element> pChildCaseType = pDoc->createElement("casetype");
	AutoPtr<Element> pChildState = pDoc->createElement("state");
	AutoPtr<Element> pChildOrderBy = pDoc->createElement("orderby");
	AutoPtr<Element> pChildThirdIdType = pDoc->createElement("thirdidtype");
	AutoPtr<Element> pChildThirdId = pDoc->createElement("thirdid");
	AutoPtr<Element> pChildNextId = pDoc->createElement("nextid");
	AutoPtr<Element> pChildPageSize = pDoc->createElement("pagesize");
	AutoPtr<Text> pTextReportId = pDoc->createTextNode(stringReportId);
	AutoPtr<Text> pTextReportStartDate = pDoc->createTextNode(stringReportStartDate);
	AutoPtr<Text> pTextReportEndDate = pDoc->createTextNode(stringReportEndDate);
	AutoPtr<Text> pTextCaseType = pDoc->createTextNode(stringCaseType);
	AutoPtr<Text> pTextState = pDoc->createTextNode(stringState);
	AutoPtr<Text> pTextOrderBy = pDoc->createTextNode(stringOrderBy);
	AutoPtr<Text> pTextThirdIdType = pDoc->createTextNode(stringThirdIdType);
	AutoPtr<Text> pTextThirdId = pDoc->createTextNode(stringThirdId);
	AutoPtr<Text> pTextNextId = pDoc->createTextNode(stringNextId);
	AutoPtr<Text> pTextPageSize = pDoc->createTextNode(stringPageSize);
	pChildReportId->appendChild(pTextReportId);
	pChildReportStartDate->appendChild(pTextReportStartDate);
	pChildReportEndDate->appendChild(pTextReportEndDate);
	pChildCaseType->appendChild(pTextCaseType);
	pChildState->appendChild(pTextState);
	pChildOrderBy->appendChild(pTextOrderBy);
	pChildThirdIdType->appendChild(pTextThirdIdType);
	pChildThirdId->appendChild(pTextThirdId);
	pChildNextId->appendChild(pTextNextId);
	pChildPageSize->appendChild(pTextPageSize);
	pRoot->appendChild(pChildReportId);
	pRoot->appendChild(pChildReportStartDate);
	pRoot->appendChild(pChildReportEndDate);
	pRoot->appendChild(pChildCaseType);
	pRoot->appendChild(pChildState);
	pRoot->appendChild(pChildOrderBy);
	pRoot->appendChild(pChildThirdIdType);
	pRoot->appendChild(pChildThirdId);
	pRoot->appendChild(pChildNextId);
	pRoot->appendChild(pChildPageSize);
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

	CPhmsRequestHead phmsRequestHead("10", "1115", m_stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::ChangeViewLikeCollectCount(string stringHealthTipId, string stringUri, string stringTitle, string stringLike, string stringRead, string stringCollect)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildHealthTipId = pDoc->createElement("healthtip_id");
	AutoPtr<Element> pChildUri  = pDoc->createElement("uri");
	AutoPtr<Element> pChildTitle = pDoc->createElement("title");
	AutoPtr<Element> pChildLike = pDoc->createElement("like");
	AutoPtr<Element> pChildRead = pDoc->createElement("read");
	AutoPtr<Element> pChildCollect = pDoc->createElement("collect");
	AutoPtr<Text> pTextHealthTipId = pDoc->createTextNode(stringHealthTipId);
	AutoPtr<Text> pTextUri = pDoc->createTextNode(stringUri);
	AutoPtr<Text> pTextTitle = pDoc->createTextNode(stringTitle);
	AutoPtr<Text> pTextLike = pDoc->createTextNode(stringLike);
	AutoPtr<Text> pTextRead = pDoc->createTextNode(stringRead);
	AutoPtr<Text> pTextCollect = pDoc->createTextNode(stringCollect);
	pChildHealthTipId->appendChild(pTextHealthTipId);
	pChildUri->appendChild(pTextUri);
	pChildTitle->appendChild(pTextTitle);
	pChildLike->appendChild(pTextLike);
	pChildRead->appendChild(pTextRead);
	pChildCollect->appendChild(pTextCollect);
	pRoot->appendChild(pChildHealthTipId);
	pRoot->appendChild(pChildUri);
	pRoot->appendChild(pChildTitle);
	pRoot->appendChild(pChildLike);
	pRoot->appendChild(pChildRead);
	pRoot->appendChild(pChildCollect);
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

	CPhmsRequestHead phmsRequestHead("10", "1116", m_stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::ChangeAdviceExecutiveState(string stringSgId, string stringState)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildSgId = pDoc->createElement("sgid");
	AutoPtr<Element> pChildState  = pDoc->createElement("state");
	AutoPtr<Text> pTextSgId = pDoc->createTextNode(stringSgId);
	AutoPtr<Text> pTextState = pDoc->createTextNode(stringState);
	pChildSgId->appendChild(pTextSgId);
	pChildState->appendChild(pTextState);
	pRoot->appendChild(pChildSgId);
	pRoot->appendChild(pChildState);
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

	CPhmsRequestHead phmsRequestHead("10", "1117", m_stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::GetTrendRangeValue()
{
	CPhmsRequestHead phmsRequestHead("10", "1118", m_stringSessionId, "10");
	CPhmsRequestBody phmsRequestBody("");
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	int nRet = PHMS_SUCCESSFUL_RESULT;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::GetCliniqueReportList(string stringCliniqueSenderId, string stringStartDate, 
						  string stringEndDate, string stringState, string stringName, string stringPatientSourceCode, string stringYzlb, string stringPIdType, string stringPId, string stringHb, string stringASectionId, string stringNextId, string stringPageSize)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildSenderId = pDoc->createElement("senderid");
	AutoPtr<Element> pChildStartDate = pDoc->createElement("startdate");
	AutoPtr<Element> pChildEndDate = pDoc->createElement("enddate");
	AutoPtr<Element> pChildState  = pDoc->createElement("state");
	AutoPtr<Element> pChildName = pDoc->createElement("cname");
	AutoPtr<Element> pChildPatientSourceCode = pDoc->createElement("patientsourceid");
	AutoPtr<Element> pChildYzlb = pDoc->createElement("yzlb");
	AutoPtr<Element> pChildPIdType = pDoc->createElement("pidtype");
	AutoPtr<Element> pChildPId = pDoc->createElement("pid");
	AutoPtr<Element> pChildHb = pDoc->createElement("hb");
	AutoPtr<Element> pChildASectionId = pDoc->createElement("asectionid");
	AutoPtr<Element> pChildNextId = pDoc->createElement("nextid");
	AutoPtr<Element> pChildPageSize = pDoc->createElement("pagesize");
	AutoPtr<Text> pTextSenderId = pDoc->createTextNode(stringCliniqueSenderId);
	AutoPtr<Text> pTextStartDate = pDoc->createTextNode(stringStartDate);
	AutoPtr<Text> pTextEndDate = pDoc->createTextNode(stringEndDate);
	AutoPtr<Text> pTextState = pDoc->createTextNode(stringState);
	AutoPtr<Text> pTextName = pDoc->createTextNode(stringName);
	AutoPtr<Text> pTextPatientSourceCode = pDoc->createTextNode(stringPatientSourceCode);
	AutoPtr<Text> pTextYzlb = pDoc->createTextNode(stringYzlb);
	AutoPtr<Text> pTextPIdType = pDoc->createTextNode(stringPIdType);
	AutoPtr<Text> pTextPId = pDoc->createTextNode(stringPId);
	AutoPtr<Text> pTextHb = pDoc->createTextNode(stringHb);
	AutoPtr<Text> pTextASectionId = pDoc->createTextNode(stringASectionId);
	AutoPtr<Text> pTextNextId = pDoc->createTextNode(stringNextId);
	AutoPtr<Text> pTextPageSize = pDoc->createTextNode(stringPageSize);
	pChildSenderId->appendChild(pTextSenderId);
	pChildStartDate->appendChild(pTextStartDate);
	pChildEndDate->appendChild(pTextEndDate);
	pChildState->appendChild(pTextState);
	pChildName->appendChild(pTextName);
	pChildPatientSourceCode->appendChild(pTextPatientSourceCode);
	pChildYzlb->appendChild(pTextYzlb);
	pChildPIdType->appendChild(pTextPIdType);
	pChildPId->appendChild(pTextPId);
	pChildHb->appendChild(pTextHb);
	pChildASectionId->appendChild(pTextASectionId);
	pChildNextId->appendChild(pTextNextId);
	pChildPageSize->appendChild(pTextPageSize);
	pRoot->appendChild(pChildSenderId);
	pRoot->appendChild(pChildStartDate);
	pRoot->appendChild(pChildEndDate);
	pRoot->appendChild(pChildState);
	pRoot->appendChild(pChildName);
	pRoot->appendChild(pChildPatientSourceCode);
	pRoot->appendChild(pChildYzlb);
	pRoot->appendChild(pChildPIdType);
	pRoot->appendChild(pChildPId);
	pRoot->appendChild(pChildHb);
	pRoot->appendChild(pChildASectionId);
	pRoot->appendChild(pChildNextId);
	pRoot->appendChild(pChildPageSize);
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

	CPhmsRequestHead phmsRequestHead("10", "1119", m_stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::GetUserListBindingOrNot(string stringNextId, string stringPageSize, string stringIsBind, string stringIsActive)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildIsBind = pDoc->createElement("isbind");
	AutoPtr<Element> pChildIsActive = pDoc->createElement("isactive");
	AutoPtr<Element> pChildNextId = pDoc->createElement("nextid");
	AutoPtr<Element> pChildPageSize = pDoc->createElement("pagesize");
	AutoPtr<Text> pTextIsBind = pDoc->createTextNode(stringIsBind);
	AutoPtr<Text> pTextIsActive = pDoc->createTextNode(stringIsActive);
	AutoPtr<Text> pTextNextId = pDoc->createTextNode(stringNextId);
	AutoPtr<Text> pTextPageSize = pDoc->createTextNode(stringPageSize);
	pChildIsBind->appendChild(pTextIsBind);
	pChildIsActive->appendChild(pTextIsActive);
	pChildNextId->appendChild(pTextNextId);
	pChildPageSize->appendChild(pTextPageSize);
	pRoot->appendChild(pChildNextId);
	pRoot->appendChild(pChildPageSize);
	pRoot->appendChild(pChildIsBind);
	pRoot->appendChild(pChildIsActive);
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

	CPhmsRequestHead phmsRequestHead("10", "1120", m_stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}
int CPhmsSession::BindUser(string stringUId, string stringUName)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildUId = pDoc->createElement("uid");
	AutoPtr<Element> pChildUName = pDoc->createElement("uname");
	AutoPtr<Text> pTextUId = pDoc->createTextNode(stringUId);
	AutoPtr<Text> pTextUName = pDoc->createTextNode(stringUName);
	pChildUId->appendChild(pTextUId);
	pChildUName->appendChild(pTextUName);
	pRoot->appendChild(pChildUId);
	pRoot->appendChild(pChildUName);
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

	CPhmsRequestHead phmsRequestHead("10", "1121", m_stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}
int CPhmsSession::UnbindUser(string stringUId)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildUId = pDoc->createElement("uid");
	AutoPtr<Text> pTextUId = pDoc->createTextNode(stringUId);
	pChildUId->appendChild(pTextUId);
	pRoot->appendChild(pChildUId);
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

	CPhmsRequestHead phmsRequestHead("10", "1122", m_stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::GetCardUserListOfCustomer(string stringThirdId, string stringName, string stringCaseType, string stringStartDate, string stringEndDate, string stringNextId, string stringPageSize)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildThirdId = pDoc->createElement("thirdid");
	AutoPtr<Element> pChildName = pDoc->createElement("name");
	AutoPtr<Element> pChildCaseType = pDoc->createElement("casetype");
	AutoPtr<Element> pChildStartDate = pDoc->createElement("startdate");
	AutoPtr<Element> pChildEndDate = pDoc->createElement("enddate");
	AutoPtr<Element> pChildNextId = pDoc->createElement("nextid");
	AutoPtr<Element> pChildPageSize = pDoc->createElement("pagesize");
	AutoPtr<Text> pTextThirdId = pDoc->createTextNode(stringThirdId);
	AutoPtr<Text> pTextName = pDoc->createTextNode(stringName);
	AutoPtr<Text> pTextCaseType = pDoc->createTextNode(stringCaseType);
	AutoPtr<Text> pTextStartDate = pDoc->createTextNode(stringStartDate);
	AutoPtr<Text> pTextEndDate = pDoc->createTextNode(stringEndDate);
	AutoPtr<Text> pTextNextId = pDoc->createTextNode(stringNextId);
	AutoPtr<Text> pTextPageSize = pDoc->createTextNode(stringPageSize);
	pChildThirdId->appendChild(pTextThirdId);
	pChildName->appendChild(pTextName);
	pChildCaseType->appendChild(pTextCaseType);
	pChildStartDate->appendChild(pTextStartDate);
	pChildEndDate->appendChild(pTextEndDate);
	pChildNextId->appendChild(pTextNextId);
	pChildPageSize->appendChild(pTextPageSize);
	pRoot->appendChild(pChildThirdId);
	pRoot->appendChild(pChildName);
	pRoot->appendChild(pChildCaseType);
	pRoot->appendChild(pChildStartDate);
	pRoot->appendChild(pChildEndDate);
	pRoot->appendChild(pChildNextId);
	pRoot->appendChild(pChildPageSize);
	
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

	CPhmsRequestHead phmsRequestHead("10", "1124", m_stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::SearchCaseAdvanced(string stringHospitalId, string stringDiagnostic, 
					   string stringName, string stringSex, QUERY_RANGE rangeAge, QUERY_RANGE rangeCreateTime, string stringMode, 
					   string stringCaseType, QUERY_ECG ecg, string stringFrom, string stringSize)
{
	string stringXmlContent;
	int nRet = PHMS_SUCCESSFUL_RESULT;
	//构造json
	Json::Value jRoot;
	Json::FastWriter jWriter;
	
	jRoot["hospitalid"] = CUtil::EnvToUtf8(stringHospitalId);
	jRoot["diagnostic"] = CUtil::EnvToUtf8(stringDiagnostic);
	jRoot["name"] = CUtil::EnvToUtf8(stringName);
	jRoot["sex"] = CUtil::EnvToUtf8(stringSex);
	Json::Value jAge;
	jAge["gte"] = CUtil::EnvToUtf8(rangeAge.lowerLimit);
	jAge["lte"] = CUtil::EnvToUtf8(rangeAge.upperLimit);
	jRoot["age"] = jAge;
	Json::Value jCreateTime;
	jCreateTime["gte"] = CUtil::EnvToUtf8(rangeCreateTime.lowerLimit);
	jCreateTime["lte"] = CUtil::EnvToUtf8(rangeCreateTime.upperLimit);
	jRoot["createtime"] = jCreateTime;
	jRoot["mode"] = CUtil::EnvToUtf8(stringMode);
	Json::Value jCaseType;
	jCaseType["id"] = CUtil::EnvToUtf8(stringCaseType);
	Json::Value HR;
	HR["gte"] = CUtil::EnvToUtf8(ecg.hr.lowerLimit);
	HR["lte"] = CUtil::EnvToUtf8(ecg.hr.upperLimit);
	jCaseType["HR"] = HR;
	Json::Value PR_Int;
	PR_Int["gte"] = CUtil::EnvToUtf8(ecg.pr_int.lowerLimit);
	PR_Int["lte"] = CUtil::EnvToUtf8(ecg.pr_int.upperLimit);
	jCaseType["PR_Int"] = PR_Int;
	Json::Value QT_Int;
	QT_Int["gte"] = CUtil::EnvToUtf8(ecg.qt_int.lowerLimit);
	QT_Int["lte"] = CUtil::EnvToUtf8(ecg.qt_int.upperLimit);
	jCaseType["QT_Int"] = QT_Int;
	jCaseType["PR_Int"] = PR_Int;
	Json::Value QTc_Int;
	QTc_Int["gte"] = CUtil::EnvToUtf8(ecg.qtc_int.lowerLimit);
	QTc_Int["lte"] = CUtil::EnvToUtf8(ecg.qtc_int.upperLimit);
	jCaseType["QTc_Int"] = QTc_Int;
	Json::Value P_Dur;
	P_Dur["gte"] = CUtil::EnvToUtf8(ecg.p_dur.lowerLimit);
	P_Dur["lte"] = CUtil::EnvToUtf8(ecg.p_dur.upperLimit);
	jCaseType["P_Dur"] = P_Dur;
	Json::Value QRS_Dur;
	QRS_Dur["gte"] = CUtil::EnvToUtf8(ecg.qrs_dur.lowerLimit);
	QRS_Dur["lte"] = CUtil::EnvToUtf8(ecg.qrs_dur.upperLimit);
	jCaseType["QRS_Dur"] = QRS_Dur;
	Json::Value P_Axis;
	P_Axis["gte"] = CUtil::EnvToUtf8(ecg.p_axis.lowerLimit);
	P_Axis["lte"] = CUtil::EnvToUtf8(ecg.p_axis.upperLimit);
	jCaseType["P_Axis"] = P_Axis;
	Json::Value QRS_Axis;
	QRS_Axis["gte"] = CUtil::EnvToUtf8(ecg.qrs_axis.lowerLimit);
	QRS_Axis["lte"] = CUtil::EnvToUtf8(ecg.qrs_axis.upperLimit);
	jCaseType["QRS_Axis"] = QRS_Axis;
	Json::Value T_Axis;
	T_Axis["gte"] = CUtil::EnvToUtf8(ecg.t_axis.lowerLimit);
	T_Axis["lte"] = CUtil::EnvToUtf8(ecg.t_axis.upperLimit);
	jCaseType["T_Axis"] = T_Axis;
	Json::Value RV5;
	RV5["gte"] = CUtil::EnvToUtf8(ecg.rv5.lowerLimit);
	RV5["lte"] = CUtil::EnvToUtf8(ecg.rv5.upperLimit);
	jCaseType["RV5"] = RV5;
	Json::Value SV1;
	SV1["gte"] = CUtil::EnvToUtf8(ecg.sv1.lowerLimit);
	SV1["lte"] = CUtil::EnvToUtf8(ecg.sv1.upperLimit);
	jCaseType["SV1"] = SV1;
	jRoot["casetype"] = jCaseType;
	jRoot["from"] = CUtil::EnvToUtf8(stringFrom);
	jRoot["size"] = CUtil::EnvToUtf8(stringSize);
	stringXmlContent = jWriter.write(jRoot);
	CPhmsLogger::GetPhmsLogger()->WriteLog(stringXmlContent, __FUNCTION__, __FILE__, __LINE__);

	std::ostringstream oStringStream;
	Base64Encoder encoderBase64(oStringStream);
	encoderBase64.rdbuf()->setLineLength(0);
	istringstream istr(stringXmlContent);
	StreamCopier::copyStream(istr, encoderBase64);
	encoderBase64.close();
	stringXmlContent = oStringStream.str();
	CPhmsLogger::GetPhmsLogger()->WriteLog(stringXmlContent, __FUNCTION__, __FILE__, __LINE__);
	

	CPhmsRequestHead phmsRequestHead("10", "1123", m_stringSessionId, "13");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::SearchCaseAdvancedString(string stringHospitalId, string stringDiagnostic, 
							 string stringName, string stringSex, string stringJsonAge, string stringJsonCreateTime, string stringMode, 
							 string stringCaseType, string stringJsonEcg, string stringFrom, string stringSize)
{
	string stringXmlContent;
	int nRet = PHMS_SUCCESSFUL_RESULT;
	//构造json
	Json::Value jRoot;
	Json::FastWriter jWriter;
	Json::Reader jReader;
	try
	{
		jRoot["hospitalid"] = CUtil::EnvToUtf8(stringHospitalId);
		jRoot["diagnostic"] = CUtil::EnvToUtf8(stringDiagnostic);
		jRoot["name"] = CUtil::EnvToUtf8(stringName);
		jRoot["sex"] = CUtil::EnvToUtf8(stringSex);
 		Json::Value jAge;
// 		jAge["gte"] = CUtil::EnvToUtf8(rangeAge.lowerLimit);
// 		jAge["lte"] = CUtil::EnvToUtf8(rangeAge.upperLimit);
// 		jRoot["age"] = jAge;
		jReader.parse(stringJsonAge, jAge);
		Json::Value::Members arrayMemberAge = jAge.getMemberNames(); 
		for(Json::Value::Members::iterator iter = arrayMemberAge.begin(); iter != arrayMemberAge.end(); ++iter) //遍历json成员
		{
			jRoot[*iter] = jAge[*iter];
		}
	//	jRoot.append(jAge);
 		Json::Value jCreateTime;
// 		jCreateTime["gte"] = CUtil::EnvToUtf8(rangeCreateTime.lowerLimit);
// 		jCreateTime["lte"] = CUtil::EnvToUtf8(rangeCreateTime.upperLimit);
// 		jRoot["createtime"] = jCreateTime;
		jReader.parse(stringJsonCreateTime, jCreateTime);
		Json::Value::Members arrayMemberCreateTime = jCreateTime.getMemberNames(); 
		for(Json::Value::Members::iterator iter = arrayMemberCreateTime.begin(); iter != arrayMemberCreateTime.end(); ++iter) //遍历json成员
		{
			jRoot[*iter] = jCreateTime[*iter];
		}
	//	jRoot.append(jCreateTime);
		jRoot["mode"] = CUtil::EnvToUtf8(stringMode);
		Json::Value jCaseType;
		jCaseType["id"] = CUtil::EnvToUtf8(stringCaseType);
// 		Json::Value HR;
// 		HR["gte"] = CUtil::EnvToUtf8(ecg.hr.lowerLimit);
// 		HR["lte"] = CUtil::EnvToUtf8(ecg.hr.upperLimit);
// 		jCaseType["HR"] = HR;
// 		Json::Value PR_Int;
// 		PR_Int["gte"] = CUtil::EnvToUtf8(ecg.pr_int.lowerLimit);
// 		PR_Int["lte"] = CUtil::EnvToUtf8(ecg.pr_int.upperLimit);
// 		jCaseType["PR_Int"] = PR_Int;
// 		Json::Value QT_Int;
// 		QT_Int["gte"] = CUtil::EnvToUtf8(ecg.qt_int.lowerLimit);
// 		QT_Int["lte"] = CUtil::EnvToUtf8(ecg.qt_int.upperLimit);
// 		jCaseType["QT_Int"] = QT_Int;
// 		jCaseType["PR_Int"] = PR_Int;
// 		Json::Value QTc_Int;
// 		QTc_Int["gte"] = CUtil::EnvToUtf8(ecg.qtc_int.lowerLimit);
// 		QTc_Int["lte"] = CUtil::EnvToUtf8(ecg.qtc_int.upperLimit);
// 		jCaseType["QTc_Int"] = QTc_Int;
// 		Json::Value P_Dur;
// 		P_Dur["gte"] = CUtil::EnvToUtf8(ecg.p_dur.lowerLimit);
// 		P_Dur["lte"] = CUtil::EnvToUtf8(ecg.p_dur.upperLimit);
// 		jCaseType["P_Dur"] = P_Dur;
// 		Json::Value QRS_Dur;
// 		QRS_Dur["gte"] = CUtil::EnvToUtf8(ecg.qrs_dur.lowerLimit);
// 		QRS_Dur["lte"] = CUtil::EnvToUtf8(ecg.qrs_dur.upperLimit);
// 		jCaseType["QRS_Dur"] = QRS_Dur;
// 		Json::Value P_Axis;
// 		P_Axis["gte"] = CUtil::EnvToUtf8(ecg.p_axis.lowerLimit);
// 		P_Axis["lte"] = CUtil::EnvToUtf8(ecg.p_axis.upperLimit);
// 		jCaseType["P_Axis"] = P_Axis;
// 		Json::Value QRS_Axis;
// 		QRS_Axis["gte"] = CUtil::EnvToUtf8(ecg.qrs_axis.lowerLimit);
// 		QRS_Axis["lte"] = CUtil::EnvToUtf8(ecg.qrs_axis.upperLimit);
// 		jCaseType["QRS_Axis"] = QRS_Axis;
// 		Json::Value T_Axis;
// 		T_Axis["gte"] = CUtil::EnvToUtf8(ecg.t_axis.lowerLimit);
// 		T_Axis["lte"] = CUtil::EnvToUtf8(ecg.t_axis.upperLimit);
// 		jCaseType["T_Axis"] = T_Axis;
// 		Json::Value RV5;
// 		RV5["gte"] = CUtil::EnvToUtf8(ecg.rv5.lowerLimit);
// 		RV5["lte"] = CUtil::EnvToUtf8(ecg.rv5.upperLimit);
// 		jCaseType["RV5"] = RV5;
// 		Json::Value SV1;
// 		SV1["gte"] = CUtil::EnvToUtf8(ecg.sv1.lowerLimit);
// 		SV1["lte"] = CUtil::EnvToUtf8(ecg.sv1.upperLimit);
// 		jCaseType["SV1"] = SV1;
		Json::Value jEcg;
		jReader.parse(stringJsonEcg, jEcg);
		Json::Value::Members arrayMemberECG = jEcg.getMemberNames(); 
		for(Json::Value::Members::iterator iter = arrayMemberECG.begin(); iter != arrayMemberECG.end(); ++iter) //遍历json成员
		{
			jCaseType[*iter] = jEcg[*iter];
		}
	//	jCaseType.append(jEcg);
		jRoot["casetype"] = jCaseType;
		jRoot["from"] = CUtil::EnvToUtf8(stringFrom);
		jRoot["size"] = CUtil::EnvToUtf8(stringSize);
		stringXmlContent = jWriter.write(jRoot);
		CPhmsLogger::GetPhmsLogger()->WriteLog(stringXmlContent, __FUNCTION__, __FILE__, __LINE__);
	}
	catch(...)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog("Json Handle Exception", __FUNCTION__, __FILE__, __LINE__);
	}

	std::ostringstream oStringStream;
	Base64Encoder encoderBase64(oStringStream);
	encoderBase64.rdbuf()->setLineLength(0);
	istringstream istr(stringXmlContent);
	StreamCopier::copyStream(istr, encoderBase64);
	encoderBase64.close();
	stringXmlContent = oStringStream.str();
	CPhmsLogger::GetPhmsLogger()->WriteLog(stringXmlContent, __FUNCTION__, __FILE__, __LINE__);

	CPhmsRequestHead phmsRequestHead("10", "1123", m_stringSessionId, "13");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::GetPayCode(string stringType, string stringCaseId)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildType = pDoc->createElement("type");
	AutoPtr<Element> pChildCaseId = pDoc->createElement("caseid");
	AutoPtr<Text> pTextType = pDoc->createTextNode(stringType);
	AutoPtr<Text> pTextCaseId = pDoc->createTextNode(stringCaseId);
	pChildType->appendChild(pTextType);
	pChildCaseId->appendChild(pTextCaseId);
	pRoot->appendChild(pChildType);
	pRoot->appendChild(pChildCaseId);

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

	CPhmsRequestHead phmsRequestHead("10", "1125", m_stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::GetOrderInfo(string stringType, string stringCaseId)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildType = pDoc->createElement("type");
	AutoPtr<Element> pChildCaseId = pDoc->createElement("caseid");
	AutoPtr<Text> pTextType = pDoc->createTextNode(stringType);
	AutoPtr<Text> pTextCaseId = pDoc->createTextNode(stringCaseId);
	pChildType->appendChild(pTextType);
	pChildCaseId->appendChild(pTextCaseId);
	pRoot->appendChild(pChildType);
	pRoot->appendChild(pChildCaseId);

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

	CPhmsRequestHead phmsRequestHead("10", "1126", m_stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::SignDoctor(string stringUId)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildUId = pDoc->createElement("uid");
	AutoPtr<Text> pTextUId = pDoc->createTextNode(stringUId);
	pChildUId->appendChild(pTextUId);
	pRoot->appendChild(pChildUId);

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

	CPhmsRequestHead phmsRequestHead("10", "1127", m_stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::RefundFee(string stringType, string stringCaseId, string stringRefundFee)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildType = pDoc->createElement("type");
	AutoPtr<Element> pChildCaseId = pDoc->createElement("caseid");
	AutoPtr<Element> pChildRefundFee = pDoc->createElement("refund_fee");
	AutoPtr<Text> pTextType = pDoc->createTextNode(stringType);
	AutoPtr<Text> pTextCaseId = pDoc->createTextNode(stringCaseId);
	AutoPtr<Text> pTextRefundFee = pDoc->createTextNode(stringRefundFee);
	pChildType->appendChild(pTextType);
	pChildCaseId->appendChild(pTextCaseId);
	pChildRefundFee->appendChild(pTextRefundFee);
	pRoot->appendChild(pChildType);
	pRoot->appendChild(pChildCaseId);
	pRoot->appendChild(pChildRefundFee);

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

	CPhmsRequestHead phmsRequestHead("10", "1128", m_stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::HoldPlaceReview(string stringReportId)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildReportId = pDoc->createElement("reportid");
	AutoPtr<Text> pTextReportId = pDoc->createTextNode(stringReportId);
	pChildReportId->appendChild(pTextReportId);
	pRoot->appendChild(pChildReportId);

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

	CPhmsRequestHead phmsRequestHead("10", "1129", m_stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::RollBackReview(string stringReportId)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildReportId = pDoc->createElement("reportid");
	AutoPtr<Text> pTextReportId = pDoc->createTextNode(stringReportId);
	pChildReportId->appendChild(pTextReportId);
	pRoot->appendChild(pChildReportId);

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

	CPhmsRequestHead phmsRequestHead("10", "1130", m_stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::ResetReviewHoldPlace(string stringReportId, string stringOccupantId, string stringOccupantName, string stringSuperKey)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildReportId = pDoc->createElement("reportid");
	AutoPtr<Element> pChildOccupantId = pDoc->createElement("occupant_id");
	AutoPtr<Element> pChildOccupantName = pDoc->createElement("occupant_name");
	AutoPtr<Element> pChildSuperKey = pDoc->createElement("super_key");
	AutoPtr<Text> pTextReportId = pDoc->createTextNode(stringReportId);
	AutoPtr<Text> pTextOccupantId = pDoc->createTextNode(stringOccupantId);
	AutoPtr<Text> pTextOccupantName = pDoc->createTextNode(stringOccupantName);
	AutoPtr<Text> pTextSuperKey = pDoc->createTextNode(stringSuperKey);
	pChildReportId->appendChild(pTextReportId);
	pChildOccupantId->appendChild(pTextOccupantId);
	pChildOccupantName->appendChild(pTextOccupantName);
	pChildSuperKey->appendChild(pTextSuperKey);
	pRoot->appendChild(pChildReportId);
	pRoot->appendChild(pChildOccupantId);
	pRoot->appendChild(pChildOccupantName);
	pRoot->appendChild(pChildSuperKey);

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

	CPhmsRequestHead phmsRequestHead("10", "1131", m_stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::ResetCaseHoldPlace(string stringCaseId, string stringReceiverId, string stringOccupantId, string stringOccupantName, string stringSuperKey)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildCaseId = pDoc->createElement("caseid");
	AutoPtr<Element> pChildReceiverId = pDoc->createElement("receiverid");
	AutoPtr<Element> pChildOccupantId = pDoc->createElement("occupant_id");
	AutoPtr<Element> pChildOccupantName = pDoc->createElement("occupant_name");
	AutoPtr<Element> pChildSuperKey = pDoc->createElement("super_key");
	AutoPtr<Text> pTextCaseId = pDoc->createTextNode(stringCaseId);
	AutoPtr<Text> pTextReceiverId = pDoc->createTextNode(stringReceiverId);
	AutoPtr<Text> pTextOccupantId = pDoc->createTextNode(stringOccupantId);
	AutoPtr<Text> pTextOccupantName = pDoc->createTextNode(stringOccupantName);
	AutoPtr<Text> pTextSuperKey = pDoc->createTextNode(stringSuperKey);
	pChildCaseId->appendChild(pTextCaseId);
	pChildReceiverId->appendChild(pTextReceiverId);
	pChildOccupantId->appendChild(pTextOccupantId);
	pChildOccupantName->appendChild(pTextOccupantName);
	pChildSuperKey->appendChild(pTextSuperKey);
	pRoot->appendChild(pChildCaseId);
	pRoot->appendChild(pChildReceiverId);
	pRoot->appendChild(pChildOccupantId);
	pRoot->appendChild(pChildOccupantName);
	pRoot->appendChild(pChildSuperKey);

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

	CPhmsRequestHead phmsRequestHead("10", "1132", m_stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::AddTag(string stringTagName, string stringTagDescription)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildTagName = pDoc->createElement("favoritename");
	AutoPtr<Element> pChildDescription = pDoc->createElement("description");
	AutoPtr<Text> pTextTagName = pDoc->createTextNode(stringTagName);
	AutoPtr<Text> pTextDescription = pDoc->createTextNode(stringTagDescription);
	pChildTagName->appendChild(pTextTagName);
	pChildDescription->appendChild(pTextDescription);
	pRoot->appendChild(pChildTagName);
	pRoot->appendChild(pChildDescription);
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

	CPhmsRequestHead phmsRequestHead("10", "1133", m_stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}
int CPhmsSession::GetTagList(string stringSenderId)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildSenderId = pDoc->createElement("senderid");
	AutoPtr<Text> pTextSenderId = pDoc->createTextNode(stringSenderId);
	pChildSenderId->appendChild(pTextSenderId);
	pRoot->appendChild(pChildSenderId);
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

	CPhmsRequestHead phmsRequestHead("10", "1134", m_stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}
int CPhmsSession::AddCaseToTag(string stringTagId, string stringTagName, string stringCaseId, string stringCaseType, string stringCaseTypeName, string stringPatientName)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildTagId = pDoc->createElement("favoritetid");
	AutoPtr<Element> pChildTagName = pDoc->createElement("favoritetname");
	AutoPtr<Element> pChildCaseId = pDoc->createElement("caseid");
	AutoPtr<Element> pChildCaseType = pDoc->createElement("casetype");
	AutoPtr<Element> pChildCaseTypeName = pDoc->createElement("casetypename");
	AutoPtr<Element> pChildPatientName = pDoc->createElement("patientname");
	AutoPtr<Text> pTextTagId = pDoc->createTextNode(stringTagId);
	AutoPtr<Text> pTextTagName = pDoc->createTextNode(stringTagName);
	AutoPtr<Text> pTextCaseId = pDoc->createTextNode(stringCaseId);
	AutoPtr<Text> pTextCaseType = pDoc->createTextNode(stringCaseType);
	AutoPtr<Text> pTextCaseTypeName = pDoc->createTextNode(stringCaseTypeName);
	AutoPtr<Text> pTextPatientName = pDoc->createTextNode(stringPatientName);
	pChildTagId->appendChild(pTextTagId);
	pChildTagName->appendChild(pTextTagName);
	pChildCaseId->appendChild(pTextCaseId);
	pChildCaseType->appendChild(pTextCaseType);
	pChildCaseTypeName->appendChild(pTextCaseTypeName);
	pChildPatientName->appendChild(pTextPatientName);
	pRoot->appendChild(pChildTagId);
	pRoot->appendChild(pChildTagName);
	pRoot->appendChild(pChildCaseId);
	pRoot->appendChild(pChildCaseType);
	pRoot->appendChild(pChildCaseTypeName);
	pRoot->appendChild(pChildPatientName);
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

	CPhmsRequestHead phmsRequestHead("10", "1135", m_stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}
int CPhmsSession::GetCaseListOfTag(string stringTagId, string stringSenderId, string stringCaseType)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildTagId = pDoc->createElement("tagid");
	AutoPtr<Element> pChildSenderId = pDoc->createElement("senderid");
	AutoPtr<Element> pChildCaseType = pDoc->createElement("casetype");
	AutoPtr<Text> pTextTagId = pDoc->createTextNode(stringTagId);
	AutoPtr<Text> pTextSenderId = pDoc->createTextNode(stringSenderId);
	AutoPtr<Text> pTextCaseType = pDoc->createTextNode(stringCaseType);
	pChildTagId->appendChild(pTextTagId);
	pChildSenderId->appendChild(pTextSenderId);
	pChildCaseType->appendChild(pTextCaseType);
	pRoot->appendChild(pChildTagId);
	pRoot->appendChild(pChildSenderId);
	pRoot->appendChild(pChildCaseType);
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

	CPhmsRequestHead phmsRequestHead("10", "1136", m_stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	phmsRequest.GenerateAndSetSign();
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

// int CPhmsSession::CreateSession_GuanXin(string stringUsername, string stringPassword)
// {
// 	map<string, string> mapParam;
// 	mapParam["username"] = stringUsername;
// 	mapParam["password"] = stringPassword;
// 	int nRet = PHMS_SUCCESSFUL_RESULT;
// 	this->SetGuanXinCommandType(SESSION);
// 	nRet = ExecuteGuanXinSession(mapParam);
// 	if(nRet != PHMS_SUCCESSFUL_RESULT)
// 	{
// 		//写日志
// 		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
// 		return nRet;
// 	}
// 	return PHMS_SUCCESSFUL_RESULT;
// }

int CPhmsSession::GetAccessToken_GuanXin(string stringUsername, string stringPassword)
{
	map<string, string> mapParam;
	//mapParam["grant_type"]="password";
	mapParam["username"] = stringUsername;
	mapParam["password"] = stringPassword;
	int nRet = PHMS_SUCCESSFUL_RESULT;
	this->SetGuanXinCommandType(ACCESSTOKEN);
	this->SetNetConfigType(GUANXIN_FORM);
	nRet = ExecuteGuanXinSession(mapParam, "");
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}
int CPhmsSession::GetPersonInfo_GuanXin(string stringAccessToken, string stringPersonName, string stringPersonGender, string stringPersonIdentity, string stringPersonBirthDate)
{
// 	string stringJsonContent;
// 	Json::Value root;
// 	root["accessToken"] = stringAccessToken;
// 	root["personName"] = stringPersonName;
// 	root["personGender"] = stringPersonGender;
// 	root["personIdentity"] = stringPersonIdentity;
// 	root["personBirthDate"] = stringPersonBirthDate;
// 	stringJsonContent = root.toStyledString();
	map<string, string> mapParam;
//	mapParam["personInfo"]=stringJsonContent;
	mapParam["accessToken"] = stringAccessToken;
	mapParam["personName"] = stringPersonName;
	mapParam["personGender"] = stringPersonGender;
	mapParam["personIdentity"] = stringPersonIdentity;
	mapParam["personBirthDate"] = stringPersonBirthDate;
	int nRet = PHMS_SUCCESSFUL_RESULT;
	this->SetGuanXinCommandType(PERSONINFO);
	this->SetNetConfigType(GUANXIN_FORM);
	nRet = ExecuteGuanXinSession(mapParam, "");
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::UploadECGData_GuanXin(string stringECGXmlPath, string stringECGDataPath)
{
	map<string, string> mapParam;
	int nRet = PHMS_SUCCESSFUL_RESULT;
	if(stringECGDataPath.size()==0 || CUtil::GetFileSize(stringECGDataPath)==0)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_FILE_OPEN_FAIL), __FUNCTION__, __FILE__, __LINE__);
		return COMMON_FILE_OPEN_FAIL;
	}
	ifstream iFStream;
	ostringstream oStringStream;
	iFStream.open(stringECGXmlPath.c_str());
	StreamCopier::copyStream(iFStream, oStringStream);
	iFStream.close();
	string stringXmlContent = oStringStream.str();
	nRet = CUtil::GetMapFromRequestXml(stringXmlContent, "root", mapParam);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	this->SetGuanXinCommandType(UPLOADECG);
	this->SetNetConfigType(GUANXIN_FORM);
	nRet = ExecuteGuanXinSession(mapParam, stringECGDataPath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::UploadOtherData_GuanXin(string stringOtherXmlPath)
{
	map<string, string> mapParam;
	int nRet = PHMS_SUCCESSFUL_RESULT;
	if(stringOtherXmlPath.size()==0 || CUtil::GetFileSize(stringOtherXmlPath)==0)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_FILE_OPEN_FAIL), __FUNCTION__, __FILE__, __LINE__);
		return COMMON_FILE_OPEN_FAIL;
	}
	ifstream iFStream;
	ostringstream oStringStream;
	iFStream.open(stringOtherXmlPath.c_str());
	StreamCopier::copyStream(iFStream, oStringStream);
	iFStream.close();
	string stringXmlContent = oStringStream.str();
	nRet = CUtil::GetMapFromRequestXml(stringXmlContent, "root", mapParam);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	this->SetGuanXinCommandType(UPLOADNONECG);
	this->SetNetConfigType(GUANXIN_FORM);
	nRet = ExecuteGuanXinSession(mapParam, "");
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::UploadCheckData_GuanXin(string stringCheckXmlPath)
{
	map<string, string> mapParam;
	int nRet = PHMS_SUCCESSFUL_RESULT;
	if(stringCheckXmlPath.size()==0 || CUtil::GetFileSize(stringCheckXmlPath)==0)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_FILE_OPEN_FAIL), __FUNCTION__, __FILE__, __LINE__);
		return COMMON_FILE_OPEN_FAIL;
	}
	ifstream iFStream;
	ostringstream oStringStream;
	iFStream.open(stringCheckXmlPath.c_str());
	StreamCopier::copyStream(iFStream, oStringStream);
	iFStream.close();
	string stringXmlContent = oStringStream.str();
	nRet = CUtil::GetMapFromRequestXml(stringXmlContent, "root", mapParam);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	this->SetGuanXinCommandType(UPLOADMONITOR);
	this->SetNetConfigType(GUANXIN_FORM);
	nRet = ExecuteGuanXinSession(mapParam, "");
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::UploadPersonInfo_GuanXin(string stringPersonInfoXmlPath)
{
	map<string, string> mapParam;
	int nRet = PHMS_SUCCESSFUL_RESULT;
	if(stringPersonInfoXmlPath.size()==0 || CUtil::GetFileSize(stringPersonInfoXmlPath)==0)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_FILE_OPEN_FAIL), __FUNCTION__, __FILE__, __LINE__);
		return COMMON_FILE_OPEN_FAIL;
	}
	ifstream iFStream;
	ostringstream oStringStream;
	iFStream.open(stringPersonInfoXmlPath.c_str());
	StreamCopier::copyStream(iFStream, oStringStream);
	iFStream.close();
	string stringXmlContent = oStringStream.str();
	nRet = CUtil::GetMapFromRequestXml(stringXmlContent, "root", mapParam);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	this->SetGuanXinCommandType(UPLOADPERSONINFO);
	this->SetNetConfigType(GUANXIN_FORM);
	nRet = ExecuteGuanXinSession(mapParam, "");
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}



int CPhmsSession::SetCliniqueState(string stringCliniqueId, string stringState)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildCliniqueId = pDoc->createElement("unitid");
	AutoPtr<Element> pChildState = pDoc->createElement("state");
	AutoPtr<Text> pTextCliniqueId = pDoc->createTextNode(stringCliniqueId);
	AutoPtr<Text> pTextState = pDoc->createTextNode(stringState);
	pChildCliniqueId->appendChild(pTextCliniqueId);
	pChildState->appendChild(pTextState);
	pRoot->appendChild(pChildCliniqueId);
	pRoot->appendChild(pChildState);
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

	string stringSessionId(32, '0');
	if(m_stringSessionId.size() != 0)
	{
		stringSessionId = m_stringSessionId;
	}
	CPhmsRequestHead phmsRequestHead("10", "2201", stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	if(m_stringSessionId.size() != 0)
	{
		phmsRequest.GenerateAndSetSign();
	}
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::GetPatientInfoByAppNum(string stringHospitalId, string stringOrderType, string stringOrderId, string stringReqDepart, string stringOrderState, string stringCheckType, string stringStartDate, string stringEndDate, string stringClinicId)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildHospitalId = pDoc->createElement("hospitalid");
	AutoPtr<Element> pChildOrderType = pDoc->createElement("order_type");
	AutoPtr<Element> pChildOrderId = pDoc->createElement("order_id");
	AutoPtr<Element> pChildReqDepart = pDoc->createElement("req_depart");
	AutoPtr<Element> pChildOrderState = pDoc->createElement("order_state");
	AutoPtr<Element> pChildCheckType = pDoc->createElement("check_type");
	AutoPtr<Element> pChildStartDate = pDoc->createElement("startdate");
	AutoPtr<Element> pChildEndDate = pDoc->createElement("enddate");
	AutoPtr<Element> pClinicId = pDoc->createElement("clinic_id");
	AutoPtr<Text> pTextHospitalId = pDoc->createTextNode(stringHospitalId);
	AutoPtr<Text> pTextOrderType = pDoc->createTextNode(stringOrderType);
	AutoPtr<Text> pTextOrderId = pDoc->createTextNode(stringOrderId);
	AutoPtr<Text> pTextReqDepart = pDoc->createTextNode(stringReqDepart);
	AutoPtr<Text> pTextOrderState = pDoc->createTextNode(stringOrderState);
	AutoPtr<Text> pTextCheckType = pDoc->createTextNode(stringCheckType);
	AutoPtr<Text> pTextStartDate = pDoc->createTextNode(stringStartDate);
	AutoPtr<Text> pTextEndDate = pDoc->createTextNode(stringEndDate);
	AutoPtr<Text> pTextClinicId = pDoc->createTextNode(stringClinicId);
	pChildHospitalId->appendChild(pTextHospitalId);
	pChildOrderType->appendChild(pTextOrderType);
	pChildOrderId->appendChild(pTextOrderId);
	pChildReqDepart->appendChild(pTextReqDepart);
	pChildOrderState->appendChild(pTextOrderState);
	pChildCheckType->appendChild(pTextCheckType);
	pChildStartDate->appendChild(pTextStartDate);
	pChildEndDate->appendChild(pTextEndDate);
	pClinicId->appendChild(pTextClinicId);
	pRoot->appendChild(pChildHospitalId);
	pRoot->appendChild(pChildOrderType);
	pRoot->appendChild(pChildOrderId);
	pRoot->appendChild(pChildReqDepart);
	pRoot->appendChild(pChildOrderState);
	pRoot->appendChild(pChildCheckType);
	pRoot->appendChild(pChildStartDate);
	pRoot->appendChild(pChildEndDate);
	pRoot->appendChild(pClinicId);
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

	string stringSessionId(32, '0');
	if(m_stringSessionId.size() != 0)
	{
		stringSessionId = m_stringSessionId;
	}
	CPhmsRequestHead phmsRequestHead("10", "2202", stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	if(m_stringSessionId.size() != 0)
	{
		phmsRequest.GenerateAndSetSign();
	}
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::GetPatientInfoByAppNumToHis(string stringHospitalId, string stringOrderType, string stringOrderId)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	string stringId;
	string stringIdType;
	nRet = CUtil::ParseStandardQueryString(stringOrderId, stringId, stringIdType);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildId = pDoc->createElement("id");
	AutoPtr<Element> pChildIdType = pDoc->createElement("id_type");
	AutoPtr<Element> pChildOrderType = pDoc->createElement("order_type");
	AutoPtr<Text> pTextId = pDoc->createTextNode(stringId);
	AutoPtr<Text> pTextIdType = pDoc->createTextNode(stringIdType);
	AutoPtr<Text> pTextOrderType = pDoc->createTextNode(stringOrderType);
	pChildId->appendChild(pTextId);
	pChildIdType->appendChild(pTextIdType);
	pChildOrderType->appendChild(pTextOrderType);
	pRoot->appendChild(pChildId);
	pRoot->appendChild(pChildIdType);
	pRoot->appendChild(pChildOrderType);

	//pDoc->appendChild(pi);
	pDoc->appendChild(pRoot);

	string stringXmlContent;
	
	nRet = this->GetRequestXmlNoBase64(pDoc, stringXmlContent);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	this->SetNetConfigType(DIRECTLY_TO_HIS);
	//暂时使用GetXmlNetConfig读取网络配置，所以不能与导医服务器连用，其实应该有类似SetXmlCommandType的SetXmlServer/Port
	//而且SSL采用通用配置，不能单独配置
	nRet = ExecuteXmlSession(stringXmlContent);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}
int CPhmsSession::GetPatientInfoByAppNumToPhmsAndHis(string stringHospitalId, string stringOrderType, string stringOrderId)
{
	this->SetNetConfigType(DIRECTLY_TO_HIS);
	return this->GetPatientInfoByAppNum(stringHospitalId, stringOrderType, stringOrderId, "", "", "", "", "", "");
}

int CPhmsSession::ApplyCheck(string stringCliniqueId, string stringOrderId, string stringOrderType)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildCliniqueId = pDoc->createElement("unitid");
	AutoPtr<Element> pChildOrderType = pDoc->createElement("order_type");
	AutoPtr<Element> pChildOrderId = pDoc->createElement("order_id");
	AutoPtr<Text> pTextCliniqueId = pDoc->createTextNode(stringCliniqueId);
	AutoPtr<Text> pTextOrderType = pDoc->createTextNode(stringOrderType);
	AutoPtr<Text> pTextOrderId = pDoc->createTextNode(stringOrderId);
	pChildCliniqueId->appendChild(pTextCliniqueId);
	pChildOrderType->appendChild(pTextOrderType);
	pChildOrderId->appendChild(pTextOrderId);
	pRoot->appendChild(pChildCliniqueId);
	pRoot->appendChild(pChildOrderType);
	pRoot->appendChild(pChildOrderId);
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

	string stringSessionId(32, '0');
	if(m_stringSessionId.size() != 0)
	{
		stringSessionId = m_stringSessionId;
	}
	CPhmsRequestHead phmsRequestHead("10", "2203", stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	if(m_stringSessionId.size() != 0)
	{
		phmsRequest.GenerateAndSetSign();
	}
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		if(nRet != 220303)
		{
			CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		}
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::GetWaitingCheck(string stringCliniqueId)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildCliniqueId = pDoc->createElement("unitid");
	AutoPtr<Text> pTextCliniqueId = pDoc->createTextNode(stringCliniqueId);
	pChildCliniqueId->appendChild(pTextCliniqueId);
	pRoot->appendChild(pChildCliniqueId);
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

	string stringSessionId(32, '0');
	if(m_stringSessionId.size() != 0)
	{
		stringSessionId = m_stringSessionId;
	}
	CPhmsRequestHead phmsRequestHead("10", "2204", stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	if(m_stringSessionId.size() != 0)
	{
		phmsRequest.GenerateAndSetSign();
	}
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		if(nRet!=220403 && nRet!=220405)
		{
			CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		}
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::GetCliniqueState(string stringCliniqueId)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildCliniqueId = pDoc->createElement("unitid");
	AutoPtr<Text> pTextCliniqueId = pDoc->createTextNode(stringCliniqueId);
	pChildCliniqueId->appendChild(pTextCliniqueId);
	pRoot->appendChild(pChildCliniqueId);
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

	string stringSessionId(32, '0');
	if(m_stringSessionId.size() != 0)
	{
		stringSessionId = m_stringSessionId;
	}
	CPhmsRequestHead phmsRequestHead("10", "2205", stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	if(m_stringSessionId.size() != 0)
	{
		phmsRequest.GenerateAndSetSign();
	}
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::NotifyCaseState(string stringHospitalId, string stringOrderType, string stringOrderId)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildHospitalId = pDoc->createElement("hospitalid");
	AutoPtr<Element> pChildOrderType = pDoc->createElement("order_type");
	AutoPtr<Element> pChildOrderId = pDoc->createElement("order_id");
	AutoPtr<Text> pTextHospitalId = pDoc->createTextNode(stringHospitalId);
	AutoPtr<Text> pTextOrderType = pDoc->createTextNode(stringOrderType);
	AutoPtr<Text> pTextOrderId = pDoc->createTextNode(stringOrderId);
	pChildHospitalId->appendChild(pTextHospitalId);
	pChildOrderType->appendChild(pTextOrderType);
	pChildOrderId->appendChild(pTextOrderId);
	pRoot->appendChild(pChildHospitalId);
	pRoot->appendChild(pChildOrderType);
	pRoot->appendChild(pChildOrderId);
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

	string stringSessionId(32, '0');
	if(m_stringSessionId.size() != 0)
	{
		stringSessionId = m_stringSessionId;
	}
	CPhmsRequestHead phmsRequestHead("10", "2206", stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	if(m_stringSessionId.size() != 0)
	{
		phmsRequest.GenerateAndSetSign();
	}
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::NotifyReportState(string stringReportId)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildReportId = pDoc->createElement("reportid");
	AutoPtr<Text> pTextReportId = pDoc->createTextNode(stringReportId);
	pChildReportId->appendChild(pTextReportId);
	pRoot->appendChild(pChildReportId);
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

	string stringSessionId(32, '0');
	if(m_stringSessionId.size() != 0)
	{
		stringSessionId = m_stringSessionId;
	}
	CPhmsRequestHead phmsRequestHead("10", "2207", stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	if(m_stringSessionId.size() != 0)
	{
		phmsRequest.GenerateAndSetSign();
	}
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::NotifyAppState(string stringUnitId, string stringOrderId, string stringOrderType, string stringState)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildUnitId = pDoc->createElement("unitid");
	AutoPtr<Element> pChildOrderId = pDoc->createElement("order_id");
	AutoPtr<Element> pChildOrderType = pDoc->createElement("order_type");
	AutoPtr<Element> pChildState = pDoc->createElement("state");
	AutoPtr<Text> pTextUnitId = pDoc->createTextNode(stringUnitId);
	AutoPtr<Text> pTextOrderType = pDoc->createTextNode(stringOrderType);
	AutoPtr<Text> pTextOrderId = pDoc->createTextNode(stringOrderId);
	AutoPtr<Text> pTextState = pDoc->createTextNode(stringState);
	pChildUnitId->appendChild(pTextUnitId);
	pChildOrderType->appendChild(pTextOrderType);
	pChildOrderId->appendChild(pTextOrderId);
	pChildState->appendChild(pTextState);
	pRoot->appendChild(pChildUnitId);
	pRoot->appendChild(pChildOrderId);
	pRoot->appendChild(pChildOrderType);
	pRoot->appendChild(pChildState);
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

	string stringSessionId(32, '0');
	if(m_stringSessionId.size() != 0)
	{
		stringSessionId = m_stringSessionId;
	}
	CPhmsRequestHead phmsRequestHead("10", "2208", stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	if(m_stringSessionId.size() != 0)
	{
		phmsRequest.GenerateAndSetSign();
	}
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::VerifyAppState(string stringOrderId, string stringOrderType)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildOrderId = pDoc->createElement("order_id");
	AutoPtr<Element> pChildOrderType = pDoc->createElement("order_type");
	AutoPtr<Text> pTextOrderType = pDoc->createTextNode(stringOrderType);
	AutoPtr<Text> pTextOrderId = pDoc->createTextNode(stringOrderId);
	pChildOrderType->appendChild(pTextOrderType);
	pChildOrderId->appendChild(pTextOrderId);
	pRoot->appendChild(pChildOrderId);
	pRoot->appendChild(pChildOrderType);
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

	string stringSessionId(32, '0');
	if(m_stringSessionId.size() != 0)
	{
		stringSessionId = m_stringSessionId;
	}
	CPhmsRequestHead phmsRequestHead("10", "2209", stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	if(m_stringSessionId.size() != 0)
	{
		phmsRequest.GenerateAndSetSign();
	}
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::GetHisReportState(string stringHospitalId, string stringOrderType, string stringOrderId)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildHospitalId = pDoc->createElement("hospitalid");
	AutoPtr<Element> pChildOrderType = pDoc->createElement("order_type");
	AutoPtr<Element> pChildOrderId = pDoc->createElement("order_id");
	AutoPtr<Text> pTextHospitalId = pDoc->createTextNode(stringHospitalId);
	AutoPtr<Text> pTextOrderType = pDoc->createTextNode(stringOrderType);
	AutoPtr<Text> pTextOrderId = pDoc->createTextNode(stringOrderId);
	pChildHospitalId->appendChild(pTextHospitalId);
	pChildOrderType->appendChild(pTextOrderType);
	pChildOrderId->appendChild(pTextOrderId);
	pRoot->appendChild(pChildHospitalId);
	pRoot->appendChild(pChildOrderType);
	pRoot->appendChild(pChildOrderId);
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

	string stringSessionId(32, '0');
	if(m_stringSessionId.size() != 0)
	{
		stringSessionId = m_stringSessionId;
	}
	CPhmsRequestHead phmsRequestHead("10", "2211", stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	if(m_stringSessionId.size() != 0)
	{
		phmsRequest.GenerateAndSetSign();
	}
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::SetHisCriticalValue(string stringHospitalId, string stringCaseId, string stringCriticalValues)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildHospitalId = pDoc->createElement("hospitalid");
	AutoPtr<Element> pChildCaseId = pDoc->createElement("caseid");
	AutoPtr<Element> pChildCriticalValues = pDoc->createElement("critical_values");
	AutoPtr<Text> pTextHospitalId = pDoc->createTextNode(stringHospitalId);
	AutoPtr<Text> pTextCaseId = pDoc->createTextNode(stringCaseId);
	AutoPtr<Text> pTextCriticalValues = pDoc->createTextNode(stringCriticalValues);
	pChildHospitalId->appendChild(pTextHospitalId);
	pChildCaseId->appendChild(pTextCaseId);
	pChildCriticalValues->appendChild(pTextCriticalValues);
	pRoot->appendChild(pChildHospitalId);
	pRoot->appendChild(pChildCaseId);
	pRoot->appendChild(pChildCriticalValues);
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

	string stringSessionId(32, '0');
	if(m_stringSessionId.size() != 0)
	{
		stringSessionId = m_stringSessionId;
	}
	CPhmsRequestHead phmsRequestHead("10", "2212", stringSessionId, "11");
	CPhmsRequestBody phmsRequestBody(stringXmlContent);
	CPhmsRequest phmsRequest(phmsRequestHead, phmsRequestBody, "");
	if(m_stringSessionId.size() != 0)
	{
		phmsRequest.GenerateAndSetSign();
	}
	CPhmsResponse phmsResponse;
	nRet = ExecutePhmsSession(phmsRequest, phmsResponse);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = phmsResponse.GetResult(m_nSaveType, m_stringContent, m_stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::GetDoctorList_999120(string stringKtCliniqueId, string stringCaseTypeId)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildTopic = pDoc->createElement("topic");
	AutoPtr<Element> pChildDetail = pDoc->createElement("detail");
	AutoPtr<Element> pChildSign = pDoc->createElement("sign");
	AutoPtr<Element> pGrandChildVersion = pDoc->createElement("version");
	AutoPtr<Element> pGrandChildCharset = pDoc->createElement("charset");
	AutoPtr<Element> pGrandChildDevice = pDoc->createElement("device");
	AutoPtr<Element> pGrandChildKtCliniqueId = pDoc->createElement("kt_cliniqueid");
	AutoPtr<Element> pGrandChildSignContent = pDoc->createElement("signcontent");
	AutoPtr<Text> pTextVersion = pDoc->createTextNode("1.0");
	AutoPtr<Text> pTextCharset = pDoc->createTextNode("GBK");
	AutoPtr<Text> pTextDevice = pDoc->createTextNode(stringCaseTypeId);
	AutoPtr<Text> pTextKtCliniqueId = pDoc->createTextNode(stringKtCliniqueId);
	string stringSignSrc;
	stringSignSrc = "version=1.0&charset=GBK";
	stringSignSrc += "&kt_cliniqueid=";
	stringSignSrc += stringKtCliniqueId;
	stringSignSrc += "&device=";
	stringSignSrc += stringCaseTypeId;
	stringSignSrc += "&poskey=";
	stringSignSrc += m_stringKey;
//	std::cout<<stringSignSrc<<std::endl;
	AutoPtr<Text> pTextSignContent = pDoc->createTextNode(CUtil::GetMd5(stringSignSrc.c_str(), stringSignSrc.size()));
	pGrandChildVersion->appendChild(pTextVersion);
	pGrandChildCharset->appendChild(pTextCharset);
	pGrandChildDevice->appendChild(pTextDevice);
	pGrandChildKtCliniqueId->appendChild(pTextKtCliniqueId);
	pGrandChildSignContent->appendChild(pTextSignContent);
	pChildTopic->appendChild(pGrandChildVersion);
	pChildTopic->appendChild(pGrandChildCharset);
	pChildTopic->appendChild(pGrandChildDevice);
	pChildDetail->appendChild(pGrandChildKtCliniqueId);
	pChildSign->appendChild(pGrandChildSignContent);
	pRoot->appendChild(pChildTopic);
	pRoot->appendChild(pChildDetail);
	pRoot->appendChild(pChildSign);
	//pDoc->appendChild(pi);
	pDoc->appendChild(pRoot);

	string stringXmlContent;
	int nRet = PHMS_SUCCESSFUL_RESULT;
	nRet = this->GetRequestXmlNoBase64(pDoc, stringXmlContent);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	this->SetXmlCommandType(SELECT_DOCTOR_TYPE);
	this->SetNetConfigType(DAOYI_XML);
	nRet = ExecuteXmlSession(stringXmlContent);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}
int CPhmsSession::ValidatePay_999120(string stringConsumeId, string stringKtCliniqueId, string stringCardNumber, string stringKtDoctorId, string stringConsumeMoney, string stringCaseTypeId)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildTopic = pDoc->createElement("topic");
	AutoPtr<Element> pChildDetail = pDoc->createElement("detail");
	AutoPtr<Element> pChildSign = pDoc->createElement("sign");
	AutoPtr<Element> pGrandChildVersion = pDoc->createElement("version");
	AutoPtr<Element> pGrandChildCharset = pDoc->createElement("charset");
	AutoPtr<Element> pGrandChildDevice = pDoc->createElement("device");
	AutoPtr<Element> pGrandChildConsumeId = pDoc->createElement("consume_id");
	AutoPtr<Element> pGrandChildKtCliniqueId = pDoc->createElement("kt_cliniqueid");
	AutoPtr<Element> pGrandChildCardNumber = pDoc->createElement("cardnumber");
	AutoPtr<Element> pGrandChildKtDoctorId = pDoc->createElement("kt_doctorid");
	AutoPtr<Element> pGrandChildConsumeMoney = pDoc->createElement("consumemoney");
	AutoPtr<Element> pGrandChildSignContent = pDoc->createElement("signcontent");
	AutoPtr<Text> pTextVersion = pDoc->createTextNode("1.0");
	AutoPtr<Text> pTextCharset = pDoc->createTextNode("GBK");
	AutoPtr<Text> pTextDevice = pDoc->createTextNode(stringCaseTypeId);
	AutoPtr<Text> pTextConsumeId = pDoc->createTextNode(stringConsumeId);
	AutoPtr<Text> pTextKtCliniqueId = pDoc->createTextNode(stringKtCliniqueId);
	AutoPtr<Text> pTextCardNumber = pDoc->createTextNode(stringCardNumber);
	AutoPtr<Text> pTextKtDoctorId = pDoc->createTextNode(stringKtDoctorId);
	AutoPtr<Text> pTextConsumeMoney = pDoc->createTextNode(stringConsumeMoney);
	string stringSignSrc;
	stringSignSrc = "version=1.0&charset=GBK";
	stringSignSrc += "&consume_id=";
	stringSignSrc += stringConsumeId;
	stringSignSrc += "&kt_cliniqueid=";
	stringSignSrc += stringKtCliniqueId;
	stringSignSrc += "&cardnumber=";
	stringSignSrc += stringCardNumber;
	stringSignSrc += "&kt_doctorid=";
	stringSignSrc += stringKtDoctorId;
	stringSignSrc += "&consumemoney=";
	stringSignSrc += stringConsumeMoney;
	stringSignSrc += "&device=";
	stringSignSrc += stringCaseTypeId;
	stringSignSrc += "&poskey=";
	stringSignSrc += m_stringKey;
//	std::cout<<stringSignSrc<<std::endl;
	AutoPtr<Text> pTextSignContent = pDoc->createTextNode(CUtil::GetMd5(stringSignSrc.c_str(), stringSignSrc.size()));
	pGrandChildVersion->appendChild(pTextVersion);
	pGrandChildCharset->appendChild(pTextCharset);
	pGrandChildDevice->appendChild(pTextDevice);
	pGrandChildConsumeId->appendChild(pTextConsumeId);
	pGrandChildKtCliniqueId->appendChild(pTextKtCliniqueId);
	pGrandChildCardNumber->appendChild(pTextCardNumber);
	pGrandChildKtDoctorId->appendChild(pTextKtDoctorId);
	pGrandChildConsumeMoney->appendChild(pTextConsumeMoney);
	pGrandChildSignContent->appendChild(pTextSignContent);
	pChildTopic->appendChild(pGrandChildVersion);
	pChildTopic->appendChild(pGrandChildCharset);
	pChildTopic->appendChild(pGrandChildDevice);
	pChildDetail->appendChild(pGrandChildConsumeId);
	pChildDetail->appendChild(pGrandChildKtCliniqueId);
	pChildDetail->appendChild(pGrandChildCardNumber);
	pChildDetail->appendChild(pGrandChildKtDoctorId);
	pChildDetail->appendChild(pGrandChildConsumeMoney);
	pChildSign->appendChild(pGrandChildSignContent);
	pRoot->appendChild(pChildTopic);
	pRoot->appendChild(pChildDetail);
	pRoot->appendChild(pChildSign);
	//pDoc->appendChild(pi);
	pDoc->appendChild(pRoot);

	string stringXmlContent;
	int nRet = PHMS_SUCCESSFUL_RESULT;
	nRet = this->GetRequestXmlNoBase64(pDoc, stringXmlContent);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	this->SetXmlCommandType(PAY_VERIFICATION_TYPE);
	this->SetNetConfigType(DAOYI_XML);
	nRet = ExecuteXmlSession(stringXmlContent);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}
int CPhmsSession::UpdateCaseForConsumerRecord_999120(string stringConsumeId, string stringKtCaseId, string stringCaseTypeId)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("request");
	AutoPtr<Element> pChildTopic = pDoc->createElement("topic");
	AutoPtr<Element> pChildDetail = pDoc->createElement("detail");
	AutoPtr<Element> pChildSign = pDoc->createElement("sign");
	AutoPtr<Element> pGrandChildVersion = pDoc->createElement("version");
	AutoPtr<Element> pGrandChildCharset = pDoc->createElement("charset");
	AutoPtr<Element> pGrandChildDevice = pDoc->createElement("device");
	AutoPtr<Element> pGrandChildConsumeId = pDoc->createElement("consume_id");
	AutoPtr<Element> pGrandChildKtCaseId = pDoc->createElement("kt_case_id");
	AutoPtr<Element> pGrandChildSignContent = pDoc->createElement("signcontent");
	AutoPtr<Text> pTextVersion = pDoc->createTextNode("1.0");
	AutoPtr<Text> pTextCharset = pDoc->createTextNode("GBK");
	AutoPtr<Text> pTextDevice = pDoc->createTextNode(stringCaseTypeId);
	AutoPtr<Text> pTextConsumeId = pDoc->createTextNode(stringConsumeId);
	AutoPtr<Text> pTextKtCaseId = pDoc->createTextNode(stringKtCaseId);
	string stringSignSrc;
	stringSignSrc = "version=1.0&charset=GBK";
	stringSignSrc += "&consume_id=";
	stringSignSrc += stringConsumeId;
	stringSignSrc += "&kt_case_id=";
	stringSignSrc += stringKtCaseId;
	stringSignSrc += "&device=";
	stringSignSrc += stringCaseTypeId;
	stringSignSrc += "&poskey=";
	stringSignSrc += m_stringKey;
//	std::cout<<stringSignSrc<<std::endl;
	AutoPtr<Text> pTextSignContent = pDoc->createTextNode(CUtil::GetMd5(stringSignSrc.c_str(), stringSignSrc.size()));
	pGrandChildVersion->appendChild(pTextVersion);
	pGrandChildCharset->appendChild(pTextCharset);
	pGrandChildDevice->appendChild(pTextDevice);
	pGrandChildConsumeId->appendChild(pTextConsumeId);
	pGrandChildKtCaseId->appendChild(pTextKtCaseId);
	pGrandChildSignContent->appendChild(pTextSignContent);
	pChildTopic->appendChild(pGrandChildVersion);
	pChildTopic->appendChild(pGrandChildCharset);
	pChildTopic->appendChild(pGrandChildDevice);
	pChildDetail->appendChild(pGrandChildConsumeId);
	pChildDetail->appendChild(pGrandChildKtCaseId);
	pChildSign->appendChild(pGrandChildSignContent);
	pRoot->appendChild(pChildTopic);
	pRoot->appendChild(pChildDetail);
	pRoot->appendChild(pChildSign);
	//pDoc->appendChild(pi);
	pDoc->appendChild(pRoot);

	string stringXmlContent;
	int nRet = PHMS_SUCCESSFUL_RESULT;
	nRet = this->GetRequestXmlNoBase64(pDoc, stringXmlContent);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	this->SetXmlCommandType(PAY_VERIFICATION_CASE_TYPE);
	this->SetNetConfigType(DAOYI_XML);
	nRet = ExecuteXmlSession(stringXmlContent);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}


int CPhmsSession::ValidatePay_XinJie(string stringRrn, string stringLocalDate)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("xml");
	AutoPtr<Element> pChildRrn = pDoc->createElement("rrn");
	AutoPtr<Element> pChildLocalDate = pDoc->createElement("localdate");
	AutoPtr<Text> pTextRrn = pDoc->createTextNode(stringRrn);
	AutoPtr<Text> pTextLocalDate = pDoc->createTextNode(stringLocalDate);
	pChildRrn->appendChild(pTextRrn);
	pChildLocalDate->appendChild(pTextLocalDate);
	pRoot->appendChild(pChildRrn);
	pRoot->appendChild(pChildLocalDate);
	//pDoc->appendChild(pi);
	pDoc->appendChild(pRoot);

	string stringXmlContent;
	int nRet = PHMS_SUCCESSFUL_RESULT;
	nRet = this->GetRequestXmlForAESAndBase64(pDoc, stringXmlContent);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	this->SetXinJieCommandType(VALIDATE_PAY);
	this->SetNetConfigType(XINJIE_XML);
	nRet = ExecuteXinJieSession(stringXmlContent);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::NotifyCheck_XinJie(string stringRrn, string stringLocalDate)
{
	AutoPtr<Document> pDoc = new Document;
	//AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='GBK'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("xml");
	AutoPtr<Element> pChildRrn = pDoc->createElement("rrn");
	AutoPtr<Element> pChildLocalDate = pDoc->createElement("localdate");
	AutoPtr<Text> pTextRrn = pDoc->createTextNode(stringRrn);
	AutoPtr<Text> pTextLocalDate = pDoc->createTextNode(stringLocalDate);
	pChildRrn->appendChild(pTextRrn);
	pChildLocalDate->appendChild(pTextLocalDate);
	pRoot->appendChild(pChildRrn);
	pRoot->appendChild(pChildLocalDate);
	//pDoc->appendChild(pi);
	pDoc->appendChild(pRoot);

	string stringXmlContent;
	int nRet = PHMS_SUCCESSFUL_RESULT;
	nRet = this->GetRequestXmlForAESAndBase64(pDoc, stringXmlContent);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	this->SetXinJieCommandType(NOTIFY_PAY);
	this->SetNetConfigType(XINJIE_XML);
	nRet = ExecuteXinJieSession(stringXmlContent);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::UserAuthorize_Telecom(string stringPersonId, string stringName, string stringNation, string stringAddress, string stringRoomId)
{
	bool bNfc = false;
	if(stringName.size()==0 && stringNation.size()==0 && stringAddress.size()==0)
	{
		bNfc = true;
	}
	DateTime dt;
	dt += Timespan(Timezone::utcOffset(), 0);
	string stringDateTime = DateTimeFormatter::format(dt, "%Y-%m-%d %H:%M:%S");
	AutoPtr<Document> pDoc = new Document;
	AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='utf-8'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("root");
	AutoPtr<Element> pChildTimeStamp = pDoc->createElement("TimeStamp");
	AutoPtr<Element> pChildAccessCode = pDoc->createElement("AccessCode");
	AutoPtr<Element> pChildAuthType = pDoc->createElement("AuthType");
	AutoPtr<Element> pChildAuthData = pDoc->createElement("AuthData");
	AutoPtr<Element> pGrandChildName = 0;
	AutoPtr<Element> pGrandChildNation = 0;
	AutoPtr<Element> pGrandChildAddress = 0;
	AutoPtr<Element> pGrandChildProofNum = 0;
	if(!bNfc)
	{
		pGrandChildName = pDoc->createElement("Name");
		pGrandChildNation = pDoc->createElement("Nation");
		pGrandChildAddress = pDoc->createElement("Address");
		pGrandChildProofNum = pDoc->createElement("ProofNum");
	}
	AutoPtr<Element> pChildRoomId = pDoc->createElement("RoomId");
	AutoPtr<Text> pTextTimeStamp = pDoc->createTextNode(stringDateTime);
	AutoPtr<Text> pTextAccessCode = pDoc->createTextNode("CONTEC");
	AutoPtr<Text> pTextAuthType = 0;
	if(!bNfc)
	{
		pTextAuthType = pDoc->createTextNode("08");
	}
	else
	{
		pTextAuthType = pDoc->createTextNode("01");
	}
	AutoPtr<Text> pTextName = 0;
	AutoPtr<Text> pTextNation = 0;
	AutoPtr<Text> pTextAddress = 0;
	AutoPtr<Text> pTextProofNum = 0;
	AutoPtr<Text> pTextNfcId = 0;
	if(!bNfc)
	{
		pTextName = pDoc->createTextNode(stringName);
		pTextNation = pDoc->createTextNode(stringNation);
		pTextAddress = pDoc->createTextNode(stringAddress);
		pTextProofNum = pDoc->createTextNode(stringPersonId);
	}
	else
	{
		pTextNfcId = pDoc->createTextNode(stringPersonId);
	}
	AutoPtr<Text> pTextRoomId = pDoc->createTextNode(stringRoomId);
	pChildTimeStamp->appendChild(pTextTimeStamp);
	pChildAccessCode->appendChild(pTextAccessCode);
	pChildAuthType->appendChild(pTextAuthType);
	if(!bNfc)
	{
		pGrandChildName->appendChild(pTextName);
		pGrandChildNation->appendChild(pTextNation);
		pGrandChildAddress->appendChild(pTextAddress);
		pGrandChildProofNum->appendChild(pTextProofNum);
		pChildAuthData->appendChild(pGrandChildName);
		pChildAuthData->appendChild(pGrandChildNation);
		pChildAuthData->appendChild(pGrandChildAddress);
		pChildAuthData->appendChild(pGrandChildProofNum);
	}
	else
	{
		pChildAuthData->appendChild(pTextNfcId);
	}
	pChildRoomId->appendChild(pTextRoomId);
	pRoot->appendChild(pChildTimeStamp);
	pRoot->appendChild(pChildAccessCode);
	pRoot->appendChild(pChildAuthType);
	pRoot->appendChild(pChildAuthData);
	pRoot->appendChild(pChildRoomId);
	//pDoc->appendChild(pi);
	pDoc->appendChild(pRoot);

	string stringXmlContent;
	int nRet = PHMS_SUCCESSFUL_RESULT;
	nRet = this->GetRequestXmlNoBase64(pDoc, stringXmlContent, true);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	this->SetXmlCommandType_Telecom(USER_AUTH);
	this->SetNetConfigType(TELECOM_XML);
	nRet = ExecuteXmlSession_Telecom(stringXmlContent);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}


int CPhmsSession::BloodPressureUpload_Telecom(string stringPersonType, string stringPersonId, string stringSBP, string stringDBP, string stringAverageValue, string stringPulseRate, string stringDetectionTime, 
								string stringEquCode, string stringRoomId)
{
	DateTime dt;
	dt += Timespan(Timezone::utcOffset(), 0);
	string stringDateTime = DateTimeFormatter::format(dt, "%Y-%m-%d %H:%M:%S");
	AutoPtr<Document> pDoc = new Document;
	AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='utf-8'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("root");
	AutoPtr<Element> pChildTimeStamp = pDoc->createElement("TimeStamp");
	AutoPtr<Element> pChildAccessCode = pDoc->createElement("AccessCode");
	AutoPtr<Element> pChildDiscernType = pDoc->createElement("UserDiscernType");
	AutoPtr<Element> pChildDiscernData = pDoc->createElement("UserDiscernData");
	AutoPtr<Element> pChildSBP = pDoc->createElement("SBP");
	AutoPtr<Element> pChildDBP = pDoc->createElement("DBP");
	AutoPtr<Element> pChildAverageValue = pDoc->createElement("AverageValue");
	AutoPtr<Element> pChildPulseRate = pDoc->createElement("PulseRate");
	AutoPtr<Element> pChildDetectionTime  = pDoc->createElement("DetectionTime");
	AutoPtr<Element> pChildDetectionResult = pDoc->createElement("DetectionResult");
	AutoPtr<Element> pChildEquCode = pDoc->createElement("EquCode");
	AutoPtr<Element> pChildId = pDoc->createElement("Id");
	AutoPtr<Element> pChildRoomId = pDoc->createElement("RoomId");
	AutoPtr<Text> pTextTimeStamp = pDoc->createTextNode(stringDateTime);
	AutoPtr<Text> pTextAccessCode = pDoc->createTextNode("CONTEC");
	AutoPtr<Text> pTextDiscernType = 0;
	pTextDiscernType = pDoc->createTextNode(stringPersonType);
	// 	if(nCardType == 1)
	// 	{
	// 		pTextDiscernType = pDoc->createTextNode("01");
	// 	}
	// 	else
	// 	{
	// 		pTextDiscernType = pDoc->createTextNode("02");
	// 	}
	AutoPtr<Text> pTextDiscernData = pDoc->createTextNode(stringPersonId);
	AutoPtr<Text> pTextSBP = pDoc->createTextNode(stringSBP);
	AutoPtr<Text> pTextDBP = pDoc->createTextNode(stringDBP);
	AutoPtr<Text> pTextAverageValue = pDoc->createTextNode(stringAverageValue);
	AutoPtr<Text> pTextPulseRate = pDoc->createTextNode(stringPulseRate);
	AutoPtr<Text> pTextDetectionTime = pDoc->createTextNode(stringDetectionTime);
	AutoPtr<Text> pTextDetectionResutl = pDoc->createTextNode("");
	AutoPtr<Text> pTextEquCode = pDoc->createTextNode(stringEquCode);
	AutoPtr<Text> pTextId = pDoc->createTextNode("");
	AutoPtr<Text> pTextRoomId = pDoc->createTextNode(stringRoomId);
	pChildTimeStamp->appendChild(pTextTimeStamp);
	pChildAccessCode->appendChild(pTextAccessCode);
	pChildDiscernType->appendChild(pTextDiscernType);
	pChildDiscernData->appendChild(pTextDiscernData);
	pChildSBP->appendChild(pTextSBP);
	pChildDBP->appendChild(pTextDBP);
	pChildAverageValue->appendChild(pTextAverageValue);
	pChildPulseRate->appendChild(pTextPulseRate);
	pChildDetectionTime->appendChild(pTextDetectionTime);
	pChildDetectionResult->appendChild(pTextDetectionResutl);
	pChildEquCode->appendChild(pTextEquCode);
	pChildId->appendChild(pTextId);
	pChildRoomId->appendChild(pTextRoomId);
	pRoot->appendChild(pChildTimeStamp);
	pRoot->appendChild(pChildAccessCode);
	pRoot->appendChild(pChildDiscernType);
	pRoot->appendChild(pChildDiscernData);
	pRoot->appendChild(pChildSBP);
	pRoot->appendChild(pChildDBP);
	pRoot->appendChild(pChildAverageValue);
	pRoot->appendChild(pChildPulseRate);
	pRoot->appendChild(pChildDetectionTime);
	pRoot->appendChild(pChildDetectionResult);
	pRoot->appendChild(pChildEquCode);
	pRoot->appendChild(pChildId);
	pRoot->appendChild(pChildRoomId);
	//pDoc->appendChild(pi);
	pDoc->appendChild(pRoot);

	string stringXmlContent;
	int nRet = PHMS_SUCCESSFUL_RESULT;
	nRet = this->GetRequestXmlNoBase64(pDoc, stringXmlContent);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	this->SetXmlCommandType_Telecom(SYNC_BP);
	this->SetNetConfigType(TELECOM_XML);
	nRet = ExecuteXmlSession_Telecom(stringXmlContent);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::BloodOxygenUpload_Telecom(string stringPersonType, string stringPersonId, string stringBloodOxygen, string stringPulseRate, string stringDetectionTime, 
											  string stringEquCode, string stringRoomId)
{
	DateTime dt;
	dt += Timespan(Timezone::utcOffset(), 0);
	string stringDateTime = DateTimeFormatter::format(dt, "%Y-%m-%d %H:%M:%S");
	AutoPtr<Document> pDoc = new Document;
	AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='utf-8'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("root");
	AutoPtr<Element> pChildTimeStamp = pDoc->createElement("TimeStamp");
	AutoPtr<Element> pChildAccessCode = pDoc->createElement("AccessCode");
	AutoPtr<Element> pChildDiscernType = pDoc->createElement("UserDiscernType");
	AutoPtr<Element> pChildDiscernData = pDoc->createElement("UserDiscernData");
	AutoPtr<Element> pChildBloodOxygen = pDoc->createElement("BloodOxygen");
	AutoPtr<Element> pChildPulseRate = pDoc->createElement("PulseRate");
	AutoPtr<Element> pChildDetectionTime  = pDoc->createElement("DetectionTime");
	AutoPtr<Element> pChildDetectionResult = pDoc->createElement("DetectionResult");
	AutoPtr<Element> pChildEquCode = pDoc->createElement("EquCode");
	AutoPtr<Element> pChildId = pDoc->createElement("Id");
	AutoPtr<Element> pChildRoomId = pDoc->createElement("RoomId");
	AutoPtr<Text> pTextTimeStamp = pDoc->createTextNode(stringDateTime);
	AutoPtr<Text> pTextAccessCode = pDoc->createTextNode("CONTEC");
	AutoPtr<Text> pTextDiscernType = 0;
	pTextDiscernType = pDoc->createTextNode(stringPersonType);
	// 	if(nCardType == 1)
	// 	{
	// 		pTextDiscernType = pDoc->createTextNode("01");
	// 	}
	// 	else
	// 	{
	// 		pTextDiscernType = pDoc->createTextNode("02");
	// 	}
	AutoPtr<Text> pTextDiscernData = pDoc->createTextNode(stringPersonId);
	AutoPtr<Text> pTextBloodOxygen = pDoc->createTextNode(stringBloodOxygen);
	AutoPtr<Text> pTextPulseRate = pDoc->createTextNode(stringPulseRate);
	AutoPtr<Text> pTextDetectionTime = pDoc->createTextNode(stringDetectionTime);
	AutoPtr<Text> pTextDetectionResutl = pDoc->createTextNode("");
	AutoPtr<Text> pTextEquCode = pDoc->createTextNode(stringEquCode);
	AutoPtr<Text> pTextId = pDoc->createTextNode("");
	AutoPtr<Text> pTextRoomId = pDoc->createTextNode(stringRoomId);
	pChildTimeStamp->appendChild(pTextTimeStamp);
	pChildAccessCode->appendChild(pTextAccessCode);
	pChildDiscernType->appendChild(pTextDiscernType);
	pChildDiscernData->appendChild(pTextDiscernData);
	pChildBloodOxygen->appendChild(pTextBloodOxygen);
	pChildPulseRate->appendChild(pTextPulseRate);
	pChildDetectionTime->appendChild(pTextDetectionTime);
	pChildDetectionResult->appendChild(pTextDetectionResutl);
	pChildEquCode->appendChild(pTextEquCode);
	pChildId->appendChild(pTextId);
	pChildRoomId->appendChild(pTextRoomId);
	pRoot->appendChild(pChildTimeStamp);
	pRoot->appendChild(pChildAccessCode);
	pRoot->appendChild(pChildDiscernType);
	pRoot->appendChild(pChildDiscernData);
	pRoot->appendChild(pChildBloodOxygen);
	pRoot->appendChild(pChildPulseRate);
	pRoot->appendChild(pChildDetectionTime);
	pRoot->appendChild(pChildDetectionResult);
	pRoot->appendChild(pChildEquCode);
	pRoot->appendChild(pChildId);
	pRoot->appendChild(pChildRoomId);
	//pDoc->appendChild(pi);
	pDoc->appendChild(pRoot);

	string stringXmlContent;
	int nRet = PHMS_SUCCESSFUL_RESULT;
	nRet = this->GetRequestXmlNoBase64(pDoc, stringXmlContent);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	this->SetXmlCommandType_Telecom(SYNC_BO);
	this->SetNetConfigType(TELECOM_XML);
	nRet = ExecuteXmlSession_Telecom(stringXmlContent);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::BloodSugarUpload_Telecom(string stringPersonType, string stringPersonId, string stringBloodSugar, string stringDetectionTime, 
											string stringEquCode, string stringRoomId)
{
	DateTime dt;
	dt += Timespan(Timezone::utcOffset(), 0);
	string stringDateTime = DateTimeFormatter::format(dt, "%Y-%m-%d %H:%M:%S");
	AutoPtr<Document> pDoc = new Document;
	AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='utf-8'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("root");
	AutoPtr<Element> pChildTimeStamp = pDoc->createElement("TimeStamp");
	AutoPtr<Element> pChildAccessCode = pDoc->createElement("AccessCode");
	AutoPtr<Element> pChildDiscernType = pDoc->createElement("UserDiscernType");
	AutoPtr<Element> pChildDiscernData = pDoc->createElement("UserDiscernData");
	AutoPtr<Element> pChildBloodGlucose = pDoc->createElement("BloodGlucose");
	AutoPtr<Element> pChildTestType = pDoc->createElement("TestType");
	AutoPtr<Element> pChildDetectionTime  = pDoc->createElement("DetectionTime");
	AutoPtr<Element> pChildDetectionResult = pDoc->createElement("DetectionResult");
	AutoPtr<Element> pChildEquCode = pDoc->createElement("EquCode");
	AutoPtr<Element> pChildId = pDoc->createElement("Id");
	AutoPtr<Element> pChildRoomId = pDoc->createElement("RoomId");
	AutoPtr<Text> pTextTimeStamp = pDoc->createTextNode(stringDateTime);
	AutoPtr<Text> pTextAccessCode = pDoc->createTextNode("CONTEC");
	AutoPtr<Text> pTextDiscernType = 0;
	pTextDiscernType = pDoc->createTextNode(stringPersonType);
	// 	if(nCardType == 1)
	// 	{
	// 		pTextDiscernType = pDoc->createTextNode("01");
	// 	}
	// 	else
	// 	{
	// 		pTextDiscernType = pDoc->createTextNode("02");
	// 	}
	AutoPtr<Text> pTextDiscernData = pDoc->createTextNode(stringPersonId);
	AutoPtr<Text> pTextBloodGlucose = pDoc->createTextNode(stringBloodSugar);
	AutoPtr<Text> pTextTestType = pDoc->createTextNode("");
	AutoPtr<Text> pTextDetectionTime = pDoc->createTextNode(stringDetectionTime);
	AutoPtr<Text> pTextDetectionResutl = pDoc->createTextNode("");
	AutoPtr<Text> pTextEquCode = pDoc->createTextNode(stringEquCode);
	AutoPtr<Text> pTextId = pDoc->createTextNode("");
	AutoPtr<Text> pTextRoomId = pDoc->createTextNode(stringRoomId);
	pChildTimeStamp->appendChild(pTextTimeStamp);
	pChildAccessCode->appendChild(pTextAccessCode);
	pChildDiscernType->appendChild(pTextDiscernType);
	pChildDiscernData->appendChild(pTextDiscernData);
	pChildBloodGlucose->appendChild(pTextBloodGlucose);
	pChildTestType->appendChild(pTextTestType);
	pChildDetectionTime->appendChild(pTextDetectionTime);
	pChildDetectionResult->appendChild(pTextDetectionResutl);
	pChildEquCode->appendChild(pTextEquCode);
	pChildId->appendChild(pTextId);
	pChildRoomId->appendChild(pTextRoomId);
	pRoot->appendChild(pChildTimeStamp);
	pRoot->appendChild(pChildAccessCode);
	pRoot->appendChild(pChildDiscernType);
	pRoot->appendChild(pChildDiscernData);
	pRoot->appendChild(pChildBloodGlucose);
	pRoot->appendChild(pChildTestType);
	pRoot->appendChild(pChildDetectionTime);
	pRoot->appendChild(pChildDetectionResult);
	pRoot->appendChild(pChildEquCode);
	pRoot->appendChild(pChildId);
	pRoot->appendChild(pChildRoomId);
	//pDoc->appendChild(pi);
	pDoc->appendChild(pRoot);

	string stringXmlContent;
	int nRet = PHMS_SUCCESSFUL_RESULT;
	nRet = this->GetRequestXmlNoBase64(pDoc, stringXmlContent);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	this->SetXmlCommandType_Telecom(SYNC_BS);
	this->SetNetConfigType(TELECOM_XML);
	nRet = ExecuteXmlSession_Telecom(stringXmlContent);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::HeightWeightUpload_Telecom(string stringPersonType, string stringPersonId, string stringHeight, string stringWeight, string stringBMI, string stringDetectionTime, 
							   string stringEquCode, string stringRoomId)
{
	DateTime dt;
	dt += Timespan(Timezone::utcOffset(), 0);
	string stringDateTime = DateTimeFormatter::format(dt, "%Y-%m-%d %H:%M:%S");
	AutoPtr<Document> pDoc = new Document;
	AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='utf-8'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("root");
	AutoPtr<Element> pChildTimeStamp = pDoc->createElement("TimeStamp");
	AutoPtr<Element> pChildAccessCode = pDoc->createElement("AccessCode");
	AutoPtr<Element> pChildDiscernType = pDoc->createElement("UserDiscernType");
	AutoPtr<Element> pChildDiscernData = pDoc->createElement("UserDiscernData");
	AutoPtr<Element> pChildHeight = pDoc->createElement("Height");
	AutoPtr<Element> pChildWeight = pDoc->createElement("Weight");
	AutoPtr<Element> pChildBMI = pDoc->createElement("BMI");
	AutoPtr<Element> pChildDetectionTime  = pDoc->createElement("DetectionTime");
	AutoPtr<Element> pChildDetectionResult = pDoc->createElement("DetectionResult");
	AutoPtr<Element> pChildEquCode = pDoc->createElement("EquCode");
	AutoPtr<Element> pChildId = pDoc->createElement("Id");
	AutoPtr<Element> pChildRoomId = pDoc->createElement("RoomId");
	AutoPtr<Text> pTextTimeStamp = pDoc->createTextNode(stringDateTime);
	AutoPtr<Text> pTextAccessCode = pDoc->createTextNode("CONTEC");
	AutoPtr<Text> pTextDiscernType = 0;
	pTextDiscernType = pDoc->createTextNode(stringPersonType);
	// 	if(nCardType == 1)
	// 	{
	// 		pTextDiscernType = pDoc->createTextNode("01");
	// 	}
	// 	else
	// 	{
	// 		pTextDiscernType = pDoc->createTextNode("02");
	// 	}
	AutoPtr<Text> pTextDiscernData = pDoc->createTextNode(stringPersonId);
	AutoPtr<Text> pTextHeight = pDoc->createTextNode(stringHeight);
	AutoPtr<Text> pTextWeight = pDoc->createTextNode(stringWeight);
	AutoPtr<Text> pTextBMI = pDoc->createTextNode(stringBMI);
	AutoPtr<Text> pTextDetectionTime = pDoc->createTextNode(stringDetectionTime);
	AutoPtr<Text> pTextDetectionResutl = pDoc->createTextNode("");
	AutoPtr<Text> pTextEquCode = pDoc->createTextNode(stringEquCode);
	AutoPtr<Text> pTextId = pDoc->createTextNode("");
	AutoPtr<Text> pTextRoomId = pDoc->createTextNode(stringRoomId);
	pChildTimeStamp->appendChild(pTextTimeStamp);
	pChildAccessCode->appendChild(pTextAccessCode);
	pChildDiscernType->appendChild(pTextDiscernType);
	pChildDiscernData->appendChild(pTextDiscernData);
	pChildHeight->appendChild(pTextHeight);
	pChildWeight->appendChild(pTextWeight);
	pChildBMI->appendChild(pTextBMI);
	pChildDetectionTime->appendChild(pTextDetectionTime);
	pChildDetectionResult->appendChild(pTextDetectionResutl);
	pChildEquCode->appendChild(pTextEquCode);
	pChildId->appendChild(pTextId);
	pChildRoomId->appendChild(pTextRoomId);
	pRoot->appendChild(pChildTimeStamp);
	pRoot->appendChild(pChildAccessCode);
	pRoot->appendChild(pChildDiscernType);
	pRoot->appendChild(pChildDiscernData);
	pRoot->appendChild(pChildHeight);
	pRoot->appendChild(pChildWeight);
	pRoot->appendChild(pChildBMI);
	pRoot->appendChild(pChildDetectionTime);
	pRoot->appendChild(pChildDetectionResult);
	pRoot->appendChild(pChildEquCode);
	pRoot->appendChild(pChildId);
	pRoot->appendChild(pChildRoomId);
	//pDoc->appendChild(pi);
	pDoc->appendChild(pRoot);

	string stringXmlContent;
	int nRet = PHMS_SUCCESSFUL_RESULT;
	nRet = this->GetRequestXmlNoBase64(pDoc, stringXmlContent);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	this->SetXmlCommandType_Telecom(SYNC_HW);
	this->SetNetConfigType(TELECOM_XML);
	nRet = ExecuteXmlSession_Telecom(stringXmlContent);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::LungUpload_Telecom(string stringPersonType, string stringPersonId, string stringM_B1, string stringBSA, string stringBSA1, string stringMMF, string stringMefSeventyFive, string stringMefFifty, 
					   string stringMefTwentyFive, string stringMefFVST, string stringIC, string stringIRV, string stringERV, string stringFevOne, string stringFevTwo, string stringFevThree, 
					   string stringFevOnePer, string stringFevTwoPer, string stringFevThreePer, string stringRR, string stringFVC, string stringPEF, string stringTV, string stringVC, string stringMVV, 
					   string stringMV, string stringVSTHeight, string stringVSMvvBsa, string stringVSMvvOne, string stringSVC, string stringFevOneZeroFive, string stringFevOneZeroFivePer, string stringPefOne, 
					   string stringPefTwo, string stringPefThree, string stringFive, string stringFiveOne, string stringFiveTwo, string stringFiveThree, string stringPIF, string stringMifFifty, 
					   string stringDetectionTime, string stringEquCode, string stringRoomId)
{
	DateTime dt;
	dt += Timespan(Timezone::utcOffset(), 0);
	string stringDateTime = DateTimeFormatter::format(dt, "%Y-%m-%d %H:%M:%S");
	AutoPtr<Document> pDoc = new Document;
	AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='utf-8'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("root");
	AutoPtr<Element> pChildTimeStamp = pDoc->createElement("TimeStamp");
	AutoPtr<Element> pChildAccessCode = pDoc->createElement("AccessCode");
	AutoPtr<Element> pChildDiscernType = pDoc->createElement("UserDiscernType");
	AutoPtr<Element> pChildDiscernData = pDoc->createElement("UserDiscernData");
	AutoPtr<Element> pChildM_B1 = pDoc->createElement("M_B1");
	AutoPtr<Element> pChildBSA = pDoc->createElement("BSA");
	AutoPtr<Element> pChildBSA1 = pDoc->createElement("BSA1");
	AutoPtr<Element> pChildMMF = pDoc->createElement("MMF");
	AutoPtr<Element> pChildMefSeventyFive = pDoc->createElement("MefSeventyFive");
	AutoPtr<Element> pChildMefFifty = pDoc->createElement("MefFifty");
	AutoPtr<Element> pChildMefTwentyFive = pDoc->createElement("MefTwentyFive");
	AutoPtr<Element> pChildMefFVST = pDoc->createElement("MefFVST");
	AutoPtr<Element> pChildIC = pDoc->createElement("IC");
	AutoPtr<Element> pChildIRV = pDoc->createElement("IRV");
	AutoPtr<Element> pChildERV = pDoc->createElement("ERV");
	AutoPtr<Element> pChildFevOne = pDoc->createElement("FevOne");
	AutoPtr<Element> pChildFevTwo = pDoc->createElement("FevTwo");
	AutoPtr<Element> pChildFevThree = pDoc->createElement("FevThree");
	AutoPtr<Element> pChildFevOnePer = pDoc->createElement("FevOnePer");
	AutoPtr<Element> pChildFevTwoPer = pDoc->createElement("FevTwoPer");
	AutoPtr<Element> pChildFevThreePer = pDoc->createElement("FevThreePer");
	AutoPtr<Element> pChildRR = pDoc->createElement("RR");
	AutoPtr<Element> pChildFVC = pDoc->createElement("FVC");
	AutoPtr<Element> pChildPEF = pDoc->createElement("PEF");
	AutoPtr<Element> pChildTV = pDoc->createElement("TV");
	AutoPtr<Element> pChildVC = pDoc->createElement("VC");
	AutoPtr<Element> pChildMVV = pDoc->createElement("MVV");
	AutoPtr<Element> pChildMV = pDoc->createElement("MV");
	AutoPtr<Element> pChildVSTHeight = pDoc->createElement("VSTHeight");
	AutoPtr<Element> pChildVSMvvBsa = pDoc->createElement("VSMvvBsa");
	AutoPtr<Element> pChildVSMvvOne = pDoc->createElement("VSMvvOne");
	AutoPtr<Element> pChildSVC = pDoc->createElement("SVC");
	AutoPtr<Element> pChildFevOneZeroFive = pDoc->createElement("FevOneZeroFive");
	AutoPtr<Element> pChildFevOneZeroFivePer = pDoc->createElement("FevOneZeroFivePer");
	AutoPtr<Element> pChildPefOne = pDoc->createElement("PefOne");
	AutoPtr<Element> pChildPefTwo = pDoc->createElement("PefTwo");
	AutoPtr<Element> pChildPefThree = pDoc->createElement("PefThree");
	AutoPtr<Element> pChildFive = pDoc->createElement("Five");
	AutoPtr<Element> pChildFiveOne = pDoc->createElement("FiveOne");
	AutoPtr<Element> pChildFiveTwo = pDoc->createElement("FiveTwo");
	AutoPtr<Element> pChildFiveThree = pDoc->createElement("FiveThree");
	AutoPtr<Element> pChildPif = pDoc->createElement("Pif");
	AutoPtr<Element> pChildMifFifty = pDoc->createElement("MifFifty");
	AutoPtr<Element> pChildDetectionTime  = pDoc->createElement("DetectionTime");
	AutoPtr<Element> pChildDetectionResult = pDoc->createElement("DetectionResult");
	AutoPtr<Element> pChildEquCode = pDoc->createElement("EquCode");
	AutoPtr<Element> pChildId = pDoc->createElement("Id");
	AutoPtr<Element> pChildRoomId = pDoc->createElement("RoomId");
	AutoPtr<Text> pTextTimeStamp = pDoc->createTextNode(stringDateTime);
	AutoPtr<Text> pTextAccessCode = pDoc->createTextNode("CONTEC");
	AutoPtr<Text> pTextDiscernType = 0;
	pTextDiscernType = pDoc->createTextNode(stringPersonType);
	// 	if(nCardType == 1)
	// 	{
	// 		pTextDiscernType = pDoc->createTextNode("01");
	// 	}
	// 	else
	// 	{
	// 		pTextDiscernType = pDoc->createTextNode("02");
	// 	}
	AutoPtr<Text> pTextDiscernData = pDoc->createTextNode(stringPersonId);
	AutoPtr<Text> pTextM_B1 = pDoc->createTextNode(stringM_B1);
	AutoPtr<Text> pTextBSA = pDoc->createTextNode(stringBSA);
	AutoPtr<Text> pTextBSA1 = pDoc->createTextNode(stringBSA1);
	AutoPtr<Text> pTextMMF = pDoc->createTextNode(stringMMF);
	AutoPtr<Text> pTextMefSeventyFive = pDoc->createTextNode(stringMefSeventyFive);
	AutoPtr<Text> pTextMefFifty = pDoc->createTextNode(stringMefFifty);
	AutoPtr<Text> pTextMefTwentyFive = pDoc->createTextNode(stringMefTwentyFive);
	AutoPtr<Text> pTextMefFVST = pDoc->createTextNode(stringMefFVST);
	AutoPtr<Text> pTextIC = pDoc->createTextNode(stringIC);
	AutoPtr<Text> pTextIRV = pDoc->createTextNode(stringIRV);
	AutoPtr<Text> pTextERV = pDoc->createTextNode(stringERV);
	AutoPtr<Text> pTextFevOne = pDoc->createTextNode(stringFevOne);
	AutoPtr<Text> pTextFevTwo = pDoc->createTextNode(stringFevTwo);
	AutoPtr<Text> pTextFevThree = pDoc->createTextNode(stringFevThree);
	AutoPtr<Text> pTextFevOnePer = pDoc->createTextNode(stringFevOnePer);
	AutoPtr<Text> pTextFevTwoPer = pDoc->createTextNode(stringFevTwoPer);
	AutoPtr<Text> pTextFevThreePer = pDoc->createTextNode(stringFevThreePer);
	AutoPtr<Text> pTextRR = pDoc->createTextNode(stringRR);
	AutoPtr<Text> pTextFVC = pDoc->createTextNode(stringFVC);
	AutoPtr<Text> pTextPEF = pDoc->createTextNode(stringPEF);
	AutoPtr<Text> pTextTV = pDoc->createTextNode(stringTV);
	AutoPtr<Text> pTextVC = pDoc->createTextNode(stringVC);
	AutoPtr<Text> pTextMVV = pDoc->createTextNode(stringMVV);
	AutoPtr<Text> pTextMV = pDoc->createTextNode(stringMV);
	AutoPtr<Text> pTextVSTHeight = pDoc->createTextNode(stringVSTHeight);
	AutoPtr<Text> pTextVSMvvBsa = pDoc->createTextNode(stringVSMvvBsa);
	AutoPtr<Text> pTextVSMvvOne = pDoc->createTextNode(stringVSMvvOne);
	AutoPtr<Text> pTextSVC = pDoc->createTextNode(stringSVC);
	AutoPtr<Text> pTextFevOneZeroFive = pDoc->createTextNode(stringFevOneZeroFive);
	AutoPtr<Text> pTextFevOneZeroFivePer = pDoc->createTextNode(stringFevOneZeroFivePer);
	AutoPtr<Text> pTextPefOne = pDoc->createTextNode(stringPefOne);
	AutoPtr<Text> pTextPefTwo = pDoc->createTextNode(stringPefTwo);
	AutoPtr<Text> pTextPefThree = pDoc->createTextNode(stringPefThree);
	AutoPtr<Text> pTextFive = pDoc->createTextNode(stringFive);
	AutoPtr<Text> pTextFiveOne = pDoc->createTextNode(stringFiveOne);
	AutoPtr<Text> pTextFiveTwo = pDoc->createTextNode(stringFiveTwo);
	AutoPtr<Text> pTextFiveThree = pDoc->createTextNode(stringFiveThree);
	AutoPtr<Text> pTextPif = pDoc->createTextNode(stringPIF);
	AutoPtr<Text> pTextMifFifty = pDoc->createTextNode(stringMifFifty);
	AutoPtr<Text> pTextDetectionTime = pDoc->createTextNode(stringDetectionTime);
	AutoPtr<Text> pTextDetectionResutl = pDoc->createTextNode("");
	AutoPtr<Text> pTextEquCode = pDoc->createTextNode(stringEquCode);
	AutoPtr<Text> pTextId = pDoc->createTextNode("");
	AutoPtr<Text> pTextRoomId = pDoc->createTextNode(stringRoomId);
	pChildTimeStamp->appendChild(pTextTimeStamp);
	pChildAccessCode->appendChild(pTextAccessCode);
	pChildDiscernType->appendChild(pTextDiscernType);
	pChildDiscernData->appendChild(pTextDiscernData);
	pChildM_B1->appendChild(pTextM_B1);
	pChildBSA->appendChild(pTextBSA);
	pChildBSA1->appendChild(pTextBSA1);
	pChildMMF->appendChild(pTextMMF);
	pChildMefSeventyFive->appendChild(pTextMefSeventyFive);
	pChildMefFifty->appendChild(pTextMefFifty);
	pChildMefTwentyFive->appendChild(pTextMefTwentyFive);
	pChildMefFVST->appendChild(pTextMefFVST);
	pChildIC->appendChild(pTextIC);
	pChildIRV->appendChild(pTextIRV);
	pChildERV->appendChild(pTextERV);
	pChildFevOne->appendChild(pTextFevOne);
	pChildFevTwo->appendChild(pTextFevTwo);
	pChildFevThree->appendChild(pTextFevThree);
	pChildFevOnePer->appendChild(pTextFevOnePer);
	pChildFevTwoPer->appendChild(pTextFevTwoPer);
	pChildFevThreePer->appendChild(pTextFevThreePer);
	pChildRR->appendChild(pTextRR);
	pChildFVC->appendChild(pTextFVC);
	pChildPEF->appendChild(pTextPEF);
	pChildTV->appendChild(pTextTV);
	pChildVC->appendChild(pTextVC);
	pChildMVV->appendChild(pTextMVV);
	pChildMV->appendChild(pTextMV);
	pChildVSTHeight->appendChild(pTextVSTHeight);
	pChildVSMvvBsa->appendChild(pTextVSMvvBsa);
	pChildVSMvvOne->appendChild(pTextVSMvvOne);
	pChildSVC->appendChild(pTextSVC);
	pChildFevOneZeroFive->appendChild(pTextFevOneZeroFive);
	pChildFevOneZeroFivePer->appendChild(pTextFevOneZeroFivePer);
	pChildPefOne->appendChild(pTextPefOne);
	pChildPefTwo->appendChild(pTextPefTwo);
	pChildPefThree->appendChild(pTextPefThree);
	pChildFive->appendChild(pTextFive);
	pChildFiveOne->appendChild(pTextFiveOne);
	pChildFiveTwo->appendChild(pTextFiveTwo);
	pChildFiveThree->appendChild(pTextFiveThree);
	pChildPif->appendChild(pTextPif);
	pChildMifFifty->appendChild(pTextMifFifty);
	pChildDetectionTime->appendChild(pTextDetectionTime);
	pChildDetectionResult->appendChild(pTextDetectionResutl);
	pChildEquCode->appendChild(pTextEquCode);
	pChildId->appendChild(pTextId);
	pChildRoomId->appendChild(pTextRoomId);
	pRoot->appendChild(pChildTimeStamp);
	pRoot->appendChild(pChildAccessCode);
	pRoot->appendChild(pChildDiscernType);
	pRoot->appendChild(pChildDiscernData);
	pRoot->appendChild(pChildM_B1);
	pRoot->appendChild(pChildBSA);
	pRoot->appendChild(pChildBSA1);
	pRoot->appendChild(pChildMMF);
	pRoot->appendChild(pChildMefSeventyFive);
	pRoot->appendChild(pChildMefFifty);
	pRoot->appendChild(pChildMefTwentyFive);
	pRoot->appendChild(pChildMefFVST);
	pRoot->appendChild(pChildIC);
	pRoot->appendChild(pChildIRV);
	pRoot->appendChild(pChildERV);
	pRoot->appendChild(pChildFevOne);
	pRoot->appendChild(pChildFevTwo);
	pRoot->appendChild(pChildFevThree);
	pRoot->appendChild(pChildFevOnePer);
	pRoot->appendChild(pChildFevTwoPer);
	pRoot->appendChild(pChildFevThreePer);
	pRoot->appendChild(pChildRR);
	pRoot->appendChild(pChildFVC);
	pRoot->appendChild(pChildPEF);
	pRoot->appendChild(pChildTV);
	pRoot->appendChild(pChildVC);
	pRoot->appendChild(pChildMVV);
	pRoot->appendChild(pChildMV);
	pRoot->appendChild(pChildVSTHeight);
	pRoot->appendChild(pChildVSMvvBsa);
	pRoot->appendChild(pChildVSMvvOne);
	pRoot->appendChild(pChildSVC);
	pRoot->appendChild(pChildFevOneZeroFive);
	pRoot->appendChild(pChildFevOneZeroFivePer);
	pRoot->appendChild(pChildPefOne);
	pRoot->appendChild(pChildPefTwo);
	pRoot->appendChild(pChildPefThree);
	pRoot->appendChild(pChildFive);
	pRoot->appendChild(pChildFiveOne);
	pRoot->appendChild(pChildFiveTwo);
	pRoot->appendChild(pChildFiveThree);
	pRoot->appendChild(pChildPif);
	pRoot->appendChild(pChildMifFifty);
	pRoot->appendChild(pChildDetectionTime);
	pRoot->appendChild(pChildDetectionResult);
	pRoot->appendChild(pChildEquCode);
	pRoot->appendChild(pChildId);
	pRoot->appendChild(pChildRoomId);
	//pDoc->appendChild(pi);
	pDoc->appendChild(pRoot);

	string stringXmlContent;
	int nRet = PHMS_SUCCESSFUL_RESULT;
	nRet = this->GetRequestXmlNoBase64(pDoc, stringXmlContent);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	this->SetXmlCommandType_Telecom(SYNC_LUNG);
	this->SetNetConfigType(TELECOM_XML);
	nRet = ExecuteXmlSession_Telecom(stringXmlContent);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::ECGUpload_Telecom(string stringPersonType, string stringPersonId, string stringDetectionTime, string stringEquCode, string stringRoomId, TELCOM_ECG ecgData)
{
	DateTime dt;
	dt += Timespan(Timezone::utcOffset(), 0);
	string stringDateTime = DateTimeFormatter::format(dt, "%Y-%m-%d %H:%M:%S");
	AutoPtr<Document> pDoc = new Document;
	AutoPtr<ProcessingInstruction> pi = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='utf-8'"); 
	AutoPtr<Element> pRoot = pDoc->createElement("root");
	AutoPtr<Element> pChildTimeStamp = pDoc->createElement("TimeStamp");
	AutoPtr<Element> pChildAccessCode = pDoc->createElement("AccessCode");
	AutoPtr<Element> pChildDiscernType = pDoc->createElement("UserDiscernType");
	AutoPtr<Element> pChildDiscernData = pDoc->createElement("UserDiscernData");
	AutoPtr<Element> pChildDetectionTime  = pDoc->createElement("DetectionTime");
	AutoPtr<Element> pChildDetectionResult = pDoc->createElement("DetectionResult");
	AutoPtr<Element> pChildEquCode = pDoc->createElement("EquCode");
	AutoPtr<Element> pChildId = pDoc->createElement("Id");
	AutoPtr<Element> pChildRoomId = pDoc->createElement("RoomId");
	AutoPtr<Element> pChildEcgLeadType = pDoc->createElement("EcgLeadType");
	AutoPtr<Element> pChildEcgLeadList = pDoc->createElement("EcgLeadList");
	AutoPtr<Text> pTextTimeStamp = pDoc->createTextNode(stringDateTime);
	AutoPtr<Text> pTextAccessCode = pDoc->createTextNode("CONTEC");
	AutoPtr<Text> pTextDiscernType = 0;
	pTextDiscernType = pDoc->createTextNode(stringPersonType);
// 	if(nCardType == 1)
// 	{
// 		pTextDiscernType = pDoc->createTextNode("01");
// 	}
// 	else
// 	{
// 		pTextDiscernType = pDoc->createTextNode("02");
// 	}
	AutoPtr<Text> pTextDiscernData = pDoc->createTextNode(stringPersonId);
	AutoPtr<Text> pTextDetectionTime = pDoc->createTextNode(stringDetectionTime);
	AutoPtr<Text> pTextDetectionResutl = pDoc->createTextNode(ecgData.result1);
	AutoPtr<Text> pTextEquCode = pDoc->createTextNode(stringEquCode);
	AutoPtr<Text> pTextId = pDoc->createTextNode("");
	AutoPtr<Text> pTextRoomId = pDoc->createTextNode(stringRoomId);
	AutoPtr<Text> pTextEcgLeadType = pDoc->createTextNode("ECG1ECG2ECG3ECG4ECG5ECG6ECG7ECG8ECG9ECG10ECG11ECG12");
	pChildTimeStamp->appendChild(pTextTimeStamp);
	pChildAccessCode->appendChild(pTextAccessCode);
	pChildDiscernType->appendChild(pTextDiscernType);
	pChildDiscernData->appendChild(pTextDiscernData);
	pChildDetectionTime->appendChild(pTextDetectionTime);
	pChildDetectionResult->appendChild(pTextDetectionResutl);
	pChildEquCode->appendChild(pTextEquCode);
	pChildId->appendChild(pTextId);
	pChildRoomId->appendChild(pTextRoomId);
	pChildEcgLeadType->appendChild(pTextEcgLeadType);
	for(int i=1; i<=ecgData.nArrayLength; i++)
	{
		AutoPtr<Element> pGrandChildEcgLeadInfo = pDoc->createElement("EcgLeadInfo");
		AutoPtr<Element> pGrand2ChildLeadId = pDoc->createElement("LeadId");
		AutoPtr<Element> pGrand2ChildLeadType = pDoc->createElement("LeadType");
		AutoPtr<Element> pGrand2ChildLeadData = pDoc->createElement("LeadData");
		AutoPtr<Element> pGrand2ChildAdRate = pDoc->createElement("AdRate");
		AutoPtr<Element> pGrand2ChildAdAccuracy = pDoc->createElement("AdAccuracy");
		AutoPtr<Element> pGrand2ChildRangeMin = pDoc->createElement("RangeMin");
		AutoPtr<Element> pGrand2ChildRangeMax = pDoc->createElement("RangeMax");
		AutoPtr<Element> pGrand2ChildHeartValue = pDoc->createElement("HeartValue");
		AutoPtr<Element> pGrand2ChildStLevel =  pDoc->createElement("StLevel");
		AutoPtr<Element> pGrand2ChildPrInterval = pDoc->createElement("PrInterval");
		AutoPtr<Element> pGrand2ChildQtInterval = pDoc->createElement("QtInterval");
		AutoPtr<Element> pGrand2ChildRrInterval = pDoc->createElement("RrInterval");
		AutoPtr<Element> pGrand2ChildPWave = pDoc->createElement("PWave");
		AutoPtr<Element> pGrand2ChildRWave = pDoc->createElement("RWave");
		AutoPtr<Element> pGrand2ChildTWave = pDoc->createElement("TWave");
		AutoPtr<Element> pGrand2ChildResult1 = pDoc->createElement("Result1");
		AutoPtr<Element> pGrand2ChildResult2 = pDoc->createElement("Result2");
		AutoPtr<Element> pGrand2ChildResult3 = pDoc->createElement("Result3");
		AutoPtr<Element> pGrand2ChildResult4 = pDoc->createElement("Result4");
		AutoPtr<Element> pGrand2ChildResult5 = pDoc->createElement("Result5");

		string stringLeadType = "ECG";
		stringLeadType += CUtil::ULongToString(i);
		string stringLeadId;
		stringLeadId = CUtil::GetMd5(stringLeadType.c_str(), stringLeadType.size());
		

		AutoPtr<Text> pTextLeadId = pDoc->createTextNode(stringLeadId.substr(0, 16));
		AutoPtr<Text> pTextLeadType = pDoc->createTextNode(stringLeadType);
		AutoPtr<Text> pTextLeadData = pDoc->createTextNode(ecgData.pArrayData[i-1]);
		AutoPtr<Text> pTextAdRate = pDoc->createTextNode(ecgData.adRate);
		AutoPtr<Text> pTextAdAccuracy = pDoc->createTextNode(ecgData.adAccuracy);
		AutoPtr<Text> pTextRangeMin = pDoc->createTextNode(ecgData.rangeMin);
		AutoPtr<Text> pTextRangeMax = pDoc->createTextNode(ecgData.rangeMax);
		AutoPtr<Text> pTextHeartValue = pDoc->createTextNode(ecgData.heartValue);
		AutoPtr<Text> pTextStLevel = pDoc->createTextNode(ecgData.stLevel);
		AutoPtr<Text> pTextPrInterval = pDoc->createTextNode(ecgData.prInterval);
		AutoPtr<Text> pTextQtInterval = pDoc->createTextNode(ecgData.qtInterval);
		AutoPtr<Text> pTextRrInterval = pDoc->createTextNode(ecgData.rrInterval);
		AutoPtr<Text> pTextPWave = pDoc->createTextNode(ecgData.pWave);
		AutoPtr<Text> pTextRWave = pDoc->createTextNode(ecgData.rWave);
		AutoPtr<Text> pTextTWave = pDoc->createTextNode(ecgData.tWave);
		AutoPtr<Text> pTextResult1 = pDoc->createTextNode(""/*ecgData.result1*/);
		AutoPtr<Text> pTextResult2 = pDoc->createTextNode(ecgData.result2);
		AutoPtr<Text> pTextResult3 = pDoc->createTextNode(ecgData.result3);
		AutoPtr<Text> pTextResult4 = pDoc->createTextNode(ecgData.result4);
		AutoPtr<Text> pTextResult5 = pDoc->createTextNode(ecgData.result5);
		pGrand2ChildLeadId->appendChild(pTextLeadId);
		pGrand2ChildLeadType->appendChild(pTextLeadType);
		pGrand2ChildLeadData->appendChild(pTextLeadData);
		pGrand2ChildAdRate->appendChild(pTextAdRate);
		pGrand2ChildAdAccuracy->appendChild(pTextAdAccuracy);
		pGrand2ChildRangeMin->appendChild(pTextRangeMin);
		pGrand2ChildRangeMax->appendChild(pTextRangeMax);
		pGrand2ChildHeartValue->appendChild(pTextHeartValue);
		pGrand2ChildStLevel->appendChild(pTextStLevel);
		pGrand2ChildPrInterval->appendChild(pTextPrInterval);
		pGrand2ChildQtInterval->appendChild(pTextQtInterval);
		pGrand2ChildRrInterval->appendChild(pTextRrInterval);
		pGrand2ChildPWave->appendChild(pTextPWave);
		pGrand2ChildRWave->appendChild(pTextRWave);
		pGrand2ChildTWave->appendChild(pTextTWave);
		pGrand2ChildResult1->appendChild(pTextResult1);
		pGrand2ChildResult2->appendChild(pTextResult2);
		pGrand2ChildResult3->appendChild(pTextResult3);
		pGrand2ChildResult4->appendChild(pTextResult4);
		pGrand2ChildResult5->appendChild(pTextResult5);
		pGrandChildEcgLeadInfo->appendChild(pGrand2ChildLeadId);
		pGrandChildEcgLeadInfo->appendChild(pGrand2ChildLeadType);
		pGrandChildEcgLeadInfo->appendChild(pGrand2ChildLeadData);
		pGrandChildEcgLeadInfo->appendChild(pGrand2ChildAdRate);
		pGrandChildEcgLeadInfo->appendChild(pGrand2ChildAdAccuracy);
		pGrandChildEcgLeadInfo->appendChild(pGrand2ChildRangeMin);
		pGrandChildEcgLeadInfo->appendChild(pGrand2ChildRangeMax);
		pGrandChildEcgLeadInfo->appendChild(pGrand2ChildHeartValue);
		pGrandChildEcgLeadInfo->appendChild(pGrand2ChildStLevel);
		pGrandChildEcgLeadInfo->appendChild(pGrand2ChildPrInterval);
		pGrandChildEcgLeadInfo->appendChild(pGrand2ChildQtInterval);
		pGrandChildEcgLeadInfo->appendChild(pGrand2ChildRrInterval);
		pGrandChildEcgLeadInfo->appendChild(pGrand2ChildPWave);
		pGrandChildEcgLeadInfo->appendChild(pGrand2ChildRWave);
		pGrandChildEcgLeadInfo->appendChild(pGrand2ChildTWave);
		pGrandChildEcgLeadInfo->appendChild(pGrand2ChildResult1);
		pGrandChildEcgLeadInfo->appendChild(pGrand2ChildResult2);
		pGrandChildEcgLeadInfo->appendChild(pGrand2ChildResult3);
		pGrandChildEcgLeadInfo->appendChild(pGrand2ChildResult4);
		pGrandChildEcgLeadInfo->appendChild(pGrand2ChildResult5);
		pChildEcgLeadList->appendChild(pGrandChildEcgLeadInfo);
	}
	pRoot->appendChild(pChildTimeStamp);
	pRoot->appendChild(pChildAccessCode);
	pRoot->appendChild(pChildDiscernType);
	pRoot->appendChild(pChildDiscernData);
	pRoot->appendChild(pChildDetectionTime);
	pRoot->appendChild(pChildDetectionResult);
	pRoot->appendChild(pChildEquCode);
	pRoot->appendChild(pChildId);
	pRoot->appendChild(pChildRoomId);
	pRoot->appendChild(pChildEcgLeadType);
	pRoot->appendChild(pChildEcgLeadList);
	//pDoc->appendChild(pi);
	pDoc->appendChild(pRoot);

	string stringXmlContent;
	int nRet = PHMS_SUCCESSFUL_RESULT;
	nRet = this->GetRequestXmlNoBase64(pDoc, stringXmlContent, true);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	this->SetXmlCommandType_Telecom(SYNC_ECG);
	this->SetNetConfigType(TELECOM_XML);
	nRet = ExecuteXmlSession_Telecom(stringXmlContent);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}


int CPhmsSession::GetToken_XiKang()
{
	string stringClientId;
	string stringClientSecret;
	if(CPhmsSession::m_stringClientId_XiKang.size()==0 || CPhmsSession::m_stringClientSecret_XiKang.size()==0)
	{
		string stringWorkingDir = CUtil::GetCurrentAppDir();
		string stringIniFilePath = stringWorkingDir+"PhmsConfig.ini";
		AutoPtr<IniFileConfiguration> iniFile = new IniFileConfiguration(stringIniFilePath, "gbk");
		stringClientId = iniFile->getString("XIKANG.ClientId", "");
		stringClientSecret = iniFile->getString("XIKANG.CliengSecret", "");
	}
	else
	{
		stringClientId = CPhmsSession::m_stringClientId_XiKang;
		stringClientSecret = CPhmsSession::m_stringClientSecret_XiKang;
	}
	map<string, string> mapParam;
	mapParam["grant_type"] = "client_credentials";
	mapParam["scope"] = "trust";
	mapParam["client_id"] = stringClientId;
	mapParam["client_secret"] = stringClientSecret;
	int nRet = PHMS_SUCCESSFUL_RESULT;
	this->SetXiKangCommandType(GET_TOKEN_XIKANG);
	this->SetNetConfigType(XIKANG_FORM);
	nRet = ExecuteXiKangSession(mapParam, "", "");
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	//处理token
	CPhmsSession::m_stringToken_XiKang = "";
	CPhmsSession::m_stringToken_XiKang = this->GetXiKangToken(m_nSaveType, m_stringContent, m_stringFilePath);
	if(m_stringToken_XiKang.size() == 0)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_GET_TOKEN_FAIL), __FUNCTION__, __FILE__, __LINE__);
		return COMMON_GET_TOKEN_FAIL;
	}
	CPhmsSession::m_stringClientId_XiKang = stringClientId;
	CPhmsSession::m_stringClientSecret_XiKang = stringClientSecret;
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::Login_XiKang(string stringUsername, string stringPassword)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	if(CPhmsSession::m_stringToken_XiKang.size() == 0)
	{
		nRet = this->GetToken_XiKang();
		if(nRet != PHMS_SUCCESSFUL_RESULT)
		{
			CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
			return nRet;
		}
	}
	map<string, string> mapParam;
	mapParam["userAccount"] = stringUsername;
	mapParam["password"] = stringPassword;
	this->SetXiKangCommandType(LOGIN_XIKANG);
	this->SetNetConfigType(XIKANG_FORM);
	nRet = ExecuteXiKangSession(mapParam, "", "");
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		if(nRet == COMMON_URL_NOT_FOUND)
		{
			//token失效
			nRet = this->GetToken_XiKang();
			if(nRet != PHMS_SUCCESSFUL_RESULT)
			{
				CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
				return nRet;
			}
			this->SetXiKangCommandType(LOGIN_XIKANG);
			this->SetNetConfigType(XIKANG_FORM);
			nRet = ExecuteXiKangSession(mapParam, "", "");
			if(nRet != PHMS_SUCCESSFUL_RESULT)
			{
				//写日志
				CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
				return nRet;
			}
		}
		else
		{
			//写日志
			CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
			return nRet;
		}
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::RegisterPatientId_XiKang(string stringName, string stringSex, string stringPhoneNum, string stringProofNum)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	if(CPhmsSession::m_stringToken_XiKang.size() == 0)
	{
		nRet = this->GetToken_XiKang();
		if(nRet != PHMS_SUCCESSFUL_RESULT)
		{
			CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
			return nRet;
		}
	}
	//网络库内部处理性别名称和生日
	string stringBirthday;
	string stringYear = stringProofNum.substr(6, 4);
	string stringMonth = stringProofNum.substr(10, 2);
	string stringDay = stringProofNum.substr(12, 2);
	stringBirthday = stringYear+"-"+stringMonth+"-"+stringDay+"T08:00:00";

	string stringSexName;
	if(stringSex == "1")
	{
		stringSexName  = "男";
	}
	else if(stringSex == "2")
	{
		stringSexName  = "女";
	}
	else
	{
		stringSexName = "未知";
	}
	map<string, string> mapParam;
	map<string, string> mapJson;
	string stringJson;
	mapJson["personName"] = stringName;
	mapJson["genderCode"] = stringSex;
	mapJson["genderName"] = stringSexName;
	mapJson["birthday"] = stringBirthday;
	mapJson["mobileNum"] = stringPhoneNum;
	mapJson["proofCode"] = "01";
	mapJson["proofNum"] = stringProofNum;
	stringJson = CUtil::MapToJson(mapJson);
	mapParam["memberBean"] = stringJson;

	this->SetXiKangCommandType(REGISTER_PATIENT_XIKANG);
	this->SetNetConfigType(XIKANG_FORM);
	nRet = ExecuteXiKangSession(mapParam, "", "");
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		if(nRet == COMMON_URL_NOT_FOUND)
		{
			//token失效
			nRet = this->GetToken_XiKang();
			if(nRet != PHMS_SUCCESSFUL_RESULT)
			{
				CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
				return nRet;
			}
			this->SetXiKangCommandType(REGISTER_PATIENT_XIKANG);
			this->SetNetConfigType(XIKANG_FORM);
			nRet = ExecuteXiKangSession(mapParam, "", "");
			if(nRet != PHMS_SUCCESSFUL_RESULT)
			{
				//写日志
				CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
				return nRet;
			}
		}
		else
		{
			//写日志
			CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
			return nRet;
		}
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::RegisterPatientInfo_XiKang(string stringPatientOpenId, string stringName, string stringSex, string stringPhoneNum, string stringProofNum, string stringPassword)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	if(CPhmsSession::m_stringToken_XiKang.size() == 0)
	{
		nRet = this->GetToken_XiKang();
		if(nRet != PHMS_SUCCESSFUL_RESULT)
		{
			CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
			return nRet;
		}
	}
	map<string, string> mapParam;
	map<string, string> mapJson;
	string stringJson;
	mapJson["personPHRCode"] = stringPatientOpenId;
	mapJson["memberCode"] = stringPatientOpenId;
	mapJson["userFullName"] = stringName;
	//mapJson["genderCode"] = stringSex;
	mapJson["mobileNum"] = stringPhoneNum;
	mapJson["userType"] = "0";
	//mapJson["proofCode"] = "01";
	mapJson["proofNum"] = stringProofNum;
	mapJson["userPassword"] = stringPassword;
	stringJson = CUtil::MapToJson(mapJson);
	mapParam["ManagerAccount"] = stringJson;

	this->SetXiKangCommandType(REGISTER_PATIENT_INFO_XIKANG);
	this->SetNetConfigType(XIKANG_FORM);
	nRet = ExecuteXiKangSession(mapParam, "", stringPatientOpenId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		if(nRet == COMMON_URL_NOT_FOUND)
		{
			//token失效
			nRet = this->GetToken_XiKang();
			if(nRet != PHMS_SUCCESSFUL_RESULT)
			{
				CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
				return nRet;
			}
			this->SetXiKangCommandType(REGISTER_PATIENT_INFO_XIKANG);
			this->SetNetConfigType(XIKANG_FORM);
			nRet = ExecuteXiKangSession(mapParam, "", stringPatientOpenId);
			if(nRet != PHMS_SUCCESSFUL_RESULT)
			{
				//写日志
				CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
				return nRet;
			}
		}
		else
		{
			//写日志
			CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
			return nRet;
		}
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::BindPatient_XiKang(string stringDoctorOpenId, string stringPatientOpenId)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	if(CPhmsSession::m_stringToken_XiKang.size() == 0)
	{
		nRet = this->GetToken_XiKang();
		if(nRet != PHMS_SUCCESSFUL_RESULT)
		{
			CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
			return nRet;
		}
	}
	map<string, string> mapParam;
	map<string, string> mapJson;
	string stringJson;
	mapJson["caregiverCode"] = stringDoctorOpenId;
	mapJson["memberCode"] = stringPatientOpenId;
	stringJson = CUtil::MapToJson(mapJson);
	mapParam["memberCaregiverBean"] = stringJson;

	this->SetXiKangCommandType(BIND_PATIENT_XIKANG);
	this->SetNetConfigType(XIKANG_FORM);
	nRet = ExecuteXiKangSession(mapParam, "", "");
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		if(nRet == COMMON_URL_NOT_FOUND)
		{
			//token失效
			nRet = this->GetToken_XiKang();
			if(nRet != PHMS_SUCCESSFUL_RESULT)
			{
				CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
				return nRet;
			}
			this->SetXiKangCommandType(BIND_PATIENT_XIKANG);
			this->SetNetConfigType(XIKANG_FORM);
			nRet = ExecuteXiKangSession(mapParam, "", "");
			if(nRet != PHMS_SUCCESSFUL_RESULT)
			{
				//写日志
				CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
				return nRet;
			}
		}
		else
		{
			//写日志
			CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
			return nRet;
		}
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::GetPatientIdList_XiKang(string stringDoctorOpenId)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	if(CPhmsSession::m_stringToken_XiKang.size() == 0)
	{
		nRet = this->GetToken_XiKang();
		if(nRet != PHMS_SUCCESSFUL_RESULT)
		{
			CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
			return nRet;
		}
	}
	map<string, string> mapParam;
	mapParam["caregiverCode"] = stringDoctorOpenId;
	mapParam["firstResult"] = "-1";
	mapParam["pageSize"] = "5000";

	this->SetXiKangCommandType(GET_PATIENT_ID_XIKANG);
	this->SetNetConfigType(XIKANG_FORM);
	nRet = ExecuteXiKangSession(mapParam, "", "");
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		if(nRet == COMMON_URL_NOT_FOUND)
		{
			//token失效
			nRet = this->GetToken_XiKang();
			if(nRet != PHMS_SUCCESSFUL_RESULT)
			{
				CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
				return nRet;
			}
			this->SetXiKangCommandType(GET_PATIENT_ID_XIKANG);
			this->SetNetConfigType(XIKANG_FORM);
			nRet = ExecuteXiKangSession(mapParam, "", "");
			if(nRet != PHMS_SUCCESSFUL_RESULT)
			{
				//写日志
				CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
				return nRet;
			}
		}
		else
		{
			//写日志
			CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
			return nRet;
		}
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::GetPatientInfo_XiKang(string stringPatientOpenId)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	if(CPhmsSession::m_stringToken_XiKang.size() == 0)
	{
		nRet = this->GetToken_XiKang();
		if(nRet != PHMS_SUCCESSFUL_RESULT)
		{
			CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
			return nRet;
		}
	}
	map<string, string> mapParam;
	//mapParam["openID"] = stringPatientOpenId;

	this->SetXiKangCommandType(GET_PATIENT_INFO_XIKANG);
	this->SetNetConfigType(XIKANG_FORM);
	nRet = ExecuteXiKangSession(mapParam, "", stringPatientOpenId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		if(nRet == COMMON_URL_NOT_FOUND)
		{
			//token失效
			nRet = this->GetToken_XiKang();
			if(nRet != PHMS_SUCCESSFUL_RESULT)
			{
				CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
				return nRet;
			}
			this->SetXiKangCommandType(GET_PATIENT_INFO_XIKANG);
			this->SetNetConfigType(XIKANG_FORM);
			nRet = ExecuteXiKangSession(mapParam, "", stringPatientOpenId);
			if(nRet != PHMS_SUCCESSFUL_RESULT)
			{
				//写日志
				CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
				return nRet;
			}
		}
		else
		{
			//写日志
			CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
			return nRet;
		}
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::SearchPatientId_XiKang(string stringKey, string stringValue)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	if(CPhmsSession::m_stringToken_XiKang.size() == 0)
	{
		nRet = this->GetToken_XiKang();
		if(nRet != PHMS_SUCCESSFUL_RESULT)
		{
			CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
			return nRet;
		}
	}
	map<string, string> mapParam;
	mapParam["queryItem"] = stringKey;
	mapParam["queryValue"] = stringValue;

	this->SetXiKangCommandType(SEARCH_PATIENT_ID_XIKANG);
	this->SetNetConfigType(XIKANG_FORM);
	nRet = ExecuteXiKangSession(mapParam, "", "");
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		if(nRet == COMMON_URL_NOT_FOUND)
		{
			//token失效
			nRet = this->GetToken_XiKang();
			if(nRet != PHMS_SUCCESSFUL_RESULT)
			{
				CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
				return nRet;
			}
			this->SetXiKangCommandType(SEARCH_PATIENT_ID_XIKANG);
			this->SetNetConfigType(XIKANG_FORM);
			nRet = ExecuteXiKangSession(mapParam, "", "");
			if(nRet != PHMS_SUCCESSFUL_RESULT)
			{
				//写日志
				CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
				return nRet;
			}
		}
		else
		{
			//写日志
			CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
			return nRet;
		}
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::GetCaseList_XiKang(vector<string> vPatientOpenId)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	if(CPhmsSession::m_stringToken_XiKang.size() == 0)
	{
		nRet = this->GetToken_XiKang();
		if(nRet != PHMS_SUCCESSFUL_RESULT)
		{
			CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
			return nRet;
		}
	}
	map<string, string> mapParam;
	map<string, string> mapJson;
	string stringParamValue;
	vector<string>::iterator iter = vPatientOpenId.begin();
	int i = 0;
	for(iter; iter!=vPatientOpenId.end(); iter++)
	{
		stringParamValue += *iter;
		i++;
		if(i != vPatientOpenId.size())
		{
			stringParamValue += ",";
		}
	}

	mapJson["memberCodes"] = stringParamValue;
	mapJson["states"] = "0,1,2,3";
	mapJson["startDate"] = "0";
	mapJson["endDate"] = "0";
	string stringJson = CUtil::MapToJson(mapJson);
	mapParam["FindDetailByMemberCodes"] = stringJson;

	this->SetXiKangCommandType(GET_CASE_LIST_XIKANG);
	this->SetNetConfigType(XIKANG_FORM);
	nRet = ExecuteXiKangSession(mapParam, "", "");
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		if(nRet == COMMON_URL_NOT_FOUND)
		{
			//token失效
			nRet = this->GetToken_XiKang();
			if(nRet != PHMS_SUCCESSFUL_RESULT)
			{
				CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
				return nRet;
			}
			this->SetXiKangCommandType(GET_CASE_LIST_XIKANG);
			this->SetNetConfigType(XIKANG_FORM);
			nRet = ExecuteXiKangSession(mapParam, "", "");
			if(nRet != PHMS_SUCCESSFUL_RESULT)
			{
				//写日志
				CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
				return nRet;
			}
		}
		else
		{
			//写日志
			CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
			return nRet;
		}
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::SubmitCase_XiKang(string stringPatientOpenId, string stringAdvice, string stringCaseType, string stringEcgXmlContent)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	if(CPhmsSession::m_stringToken_XiKang.size() == 0)
	{
		nRet = this->GetToken_XiKang();
		if(nRet != PHMS_SUCCESSFUL_RESULT)
		{
			CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
			return nRet;
		}
	}
	map<string, string> mapParam;
	map<string, string> mapJson;
	mapJson["memberCode"] = stringPatientOpenId;
	mapJson["advice"] = stringAdvice;
	mapJson["cnstType"] = stringCaseType;
	DateTime dt;
	//dt += Timespan(Timezone::utcOffset(), 0);
	mapJson["testTime"] = CUtil::Int64ToString(dt.timestamp().epochMicroseconds()/1000);

	ostringstream ostr;
	Base64Encoder encoderBase64(ostr);
	encoderBase64<<stringEcgXmlContent;
	encoderBase64.close();
	string stringBase64Content = ostr.str();
	CUtil::ReplaceString(stringBase64Content, "*", "+");
	mapJson["fileContent"] = stringBase64Content;

	string stringJson = CUtil::MapToJson(mapJson);
	mapParam["AddEcgInfo"] = stringJson;

	this->SetXiKangCommandType(SUBMIT_CASE_XIKANG);
	this->SetNetConfigType(XIKANG_FORM);
	nRet = ExecuteXiKangSession(mapParam, "", "");
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		if(nRet == COMMON_URL_NOT_FOUND)
		{
			//token失效
			nRet = this->GetToken_XiKang();
			if(nRet != PHMS_SUCCESSFUL_RESULT)
			{
				CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
				return nRet;
			}
			this->SetXiKangCommandType(SUBMIT_CASE_XIKANG);
			this->SetNetConfigType(XIKANG_FORM);
			nRet = ExecuteXiKangSession(mapParam, "", "");
			if(nRet != PHMS_SUCCESSFUL_RESULT)
			{
				//写日志
				CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
				return nRet;
			}
		}
		else
		{
			//写日志
			CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
			return nRet;
		}
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::GetReportFile_XiKang(string stringCaseId)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	if(CPhmsSession::m_stringToken_XiKang.size() == 0)
	{
		nRet = this->GetToken_XiKang();
		if(nRet != PHMS_SUCCESSFUL_RESULT)
		{
			CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
			return nRet;
		}
	}
	map<string, string> mapParam;

	this->SetXiKangCommandType(GET_REPORT_URL_XIKANG);
	this->SetNetConfigType(XIKANG_FORM);
	nRet = ExecuteXiKangSession(mapParam, "", stringCaseId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		if(nRet == COMMON_URL_NOT_FOUND)
		{
			//token失效
			nRet = this->GetToken_XiKang();
			if(nRet != PHMS_SUCCESSFUL_RESULT)
			{
				CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
				return nRet;
			}
			this->SetXiKangCommandType(GET_REPORT_URL_XIKANG);
			this->SetNetConfigType(XIKANG_FORM);
			nRet = ExecuteXiKangSession(mapParam, "", stringCaseId);
			if(nRet != PHMS_SUCCESSFUL_RESULT)
			{
				//写日志
				CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
				return nRet;
			}
		}
		else
		{
			//写日志
			CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
			return nRet;
		}
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::UploadFile_XiKang(string stringCaseId, string stringCaseFilePath)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	if(CPhmsSession::m_stringToken_XiKang.size() == 0)
	{
		nRet = this->GetToken_XiKang();
		if(nRet != PHMS_SUCCESSFUL_RESULT)
		{
			CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
			return nRet;
		}
	}
	map<string, string> mapParam;
	mapParam["ecgId"] = stringCaseId;
	mapParam["authority"] = "contec";

	this->SetXiKangCommandType(UPLOAD_FILE_XIKANG);
	this->SetNetConfigType(XIKANG_FORM);
	nRet = ExecuteXiKangSession(mapParam, stringCaseFilePath, "");
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		if(nRet == COMMON_URL_NOT_FOUND)
		{
			//token失效
			nRet = this->GetToken_XiKang();
			if(nRet != PHMS_SUCCESSFUL_RESULT)
			{
				CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
				return nRet;
			}
			this->SetXiKangCommandType(UPLOAD_FILE_XIKANG);
			this->SetNetConfigType(XIKANG_FORM);
			nRet = ExecuteXiKangSession(mapParam, stringCaseFilePath, "");
			if(nRet != PHMS_SUCCESSFUL_RESULT)
			{
				//写日志
				CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
				return nRet;
			}
		}
		else
		{
			//写日志
			CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
			return nRet;
		}
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::ExecuteGWService(string stringLastPath, map<string, string> mapParam, string stringFilePath, bool bMustSsl)
{
	CNetConfig netConfig;
	int nRet = PHMS_SUCCESSFUL_RESULT;
	//设置网络环境
	try
	{
		string stringWorkingDir = CUtil::GetCurrentAppDir();
		EnumNetConfigType enumNetConfigType = GW_FORM;
		nRet = this->GetNetConfig(stringWorkingDir+"PhmsConfig.ini", netConfig, bMustSsl, enumNetConfigType);
		netConfig.SetUrl(netConfig.GetUrl()+stringLastPath);
		if(nRet != PHMS_SUCCESSFUL_RESULT)
		{
			CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
			return nRet;
		}
		Mutex::ScopedLock lock(m_mutexAbortAndSslSwitch);
		if(bMustSsl)
		{
			if(!m_bSsl)
			{
				delete m_pHttpSession;
				m_pHttpSession = new CHttpSession(bMustSsl);
				m_bSsl = bMustSsl;
			}
		}
		else
		{
			if(m_bSsl != netConfig.GetSsl())
			{
				delete m_pHttpSession;
				m_pHttpSession = new CHttpSession(netConfig.GetSsl());
				m_bSsl = netConfig.GetSsl();
			}
		}
	}
	catch(FileNotFoundException& e)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(e, __FUNCTION__, __FILE__, __LINE__);
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_NO_NET_CONFIG_FILE), __FUNCTION__, __FILE__, __LINE__);
		return COMMON_NO_NET_CONFIG_FILE;
	}

	istream* pInputStream = NULL;
	nRet = m_pHttpSession->ExecuteFormHttpSession(netConfig, mapParam, stringFilePath, pInputStream);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = NoPhmsGetResult(pInputStream);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CPhmsSession::ExecuteCommonRESTful(string stringIp, string stringPort, string stringUrl, string stringContentType, string stringPayload)
{
	CNetConfig netConfig;
	int nRet = PHMS_SUCCESSFUL_RESULT;
	//设置网络环境
	try
	{
		netConfig.SetHost(stringIp);
		netConfig.SetAddr(stringIp);
		if(stringPort.size() == 0)
		{
			netConfig.SetPort(80);
		}
		else
		{
			netConfig.SetPort(CUtil::StringToULong(stringPort));
		}
		netConfig.SetUrl(stringUrl);
		netConfig.SetProxyType(NO_PROXY);
		netConfig.SetSsl(false);
	}
	catch(FileNotFoundException& e)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(e, __FUNCTION__, __FILE__, __LINE__);
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_NO_NET_CONFIG_FILE), __FUNCTION__, __FILE__, __LINE__);
		return COMMON_NO_NET_CONFIG_FILE;
	}

	istream* pInputStream = NULL;
	map<string, string> mapParam;
	nRet = m_pHttpSession->ExeceteCommonPostSession(netConfig, mapParam, stringContentType, stringPayload, pInputStream);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = NoPhmsGetResult(pInputStream);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}