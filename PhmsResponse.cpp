#include "PhmsResponse.h"
#include "Poco/Buffer.h"
#include <fstream>
#include <sstream>
#include "ErrorCode.h"
#include "Poco/Exception.h"
#include "Poco/MD5Engine.h"
#include "Poco/Path.h"
#include "Poco/AutoPtr.h"
#include "Poco/Util/IniFileConfiguration.h"
#include "Poco/TextEncoding.h"
#include "Poco/StreamConverter.h"
#include "Poco/StreamCopier.h"
#include "PhmsSession.h"
#include "PhmsLogger.h"
#include "Util.h"
#include "GlobalTerminalLogoString.h"

using Poco::MD5Engine;
using Poco::DigestEngine;
using Poco::Buffer;
using Poco::Exception;
using Poco::TimeoutException;
using Poco::Path;
using Poco::AutoPtr;
using Poco::Util::IniFileConfiguration;
using Poco::TextEncoding;
using Poco::InputStreamConverter;
using Poco::StreamCopier;



CPhmsResponse::CPhmsResponse()
{
	
}

CPhmsResponse::CPhmsResponse(CPhmsResponse& phmsResponse)
{
	this->m_phmsResponseHead = phmsResponse.GetPhmsResponseHead();
	this->m_phmsResponseBody = phmsResponse.GetPhmsResponseBody();
}

CPhmsResponse& CPhmsResponse::operator=(CPhmsResponse& phmsResponse)
{
	this->m_phmsResponseHead = phmsResponse.GetPhmsResponseHead();
	this->m_phmsResponseBody = phmsResponse.GetPhmsResponseBody();
	return *this;
}

CPhmsResponse::~CPhmsResponse(void)
{
}


//Getter
CPhmsResponseHead& CPhmsResponse::GetPhmsResponseHead()
{
	return m_phmsResponseHead;
}
CPhmsResponseBody& CPhmsResponse::GetPhmsResponseBody()
{
	return m_phmsResponseBody;
}


//Setter
void CPhmsResponse::SetPhmsResponseHead(CPhmsResponseHead phmsResponseHead)
{
	m_phmsResponseHead = phmsResponseHead;
}
void CPhmsResponse::SetPhmsResponseBody(CPhmsResponseBody phmsResponseBody)
{
	m_phmsResponseBody = phmsResponseBody;
}

//pInputStream:���ڻ��HttpSession�з��������ص�����
//pPhmsSession:Ӧ����CPhmsSession*�����������쳣��״̬����»��Http�쳣��Ϣ����д����־��ͬʱҲ��Ϊ������չʹ��Http������
//ʱ���������http���쳣����HttpSession::reset����ʹ��

int CPhmsResponse::SetPhmsResponse(istream* pInputStream, void* pPhmsSession)
{
	int nPhmsResponseHeadTotalLength = CPhmsResponseHead::GetPhmsResposeHeadTotalLength();
	Buffer<char> bufPhmsResponseHead(nPhmsResponseHeadTotalLength+1);
	memset(bufPhmsResponseHead.begin(), 0, nPhmsResponseHeadTotalLength+1);
	pInputStream->exceptions(ios::failbit);
	try
	{
		pInputStream->read(bufPhmsResponseHead.begin(), nPhmsResponseHeadTotalLength);
	}
	catch(const std::ios::failure& e)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(bufPhmsResponseHead.begin(), __FUNCTION__, __FILE__, __LINE__);
		CPhmsLogger::GetPhmsLogger()->WriteLog(e.what(), __FUNCTION__, __FILE__, __LINE__);
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_RESPONSE_FORMAT_ERROR), __FUNCTION__, __FILE__, __LINE__);
		return COMMON_RESPONSE_FORMAT_ERROR;
	}
	catch(const TimeoutException& e)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(e, __FUNCTION__, __FILE__, __LINE__);
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_RECV_TIMEOUT), __FUNCTION__, __FILE__, __LINE__);
		return COMMON_RECV_TIMEOUT;
	}
	catch(Exception& e)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(e, __FUNCTION__, __FILE__, __LINE__);
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_RECV_FAIL), __FUNCTION__, __FILE__, __LINE__);
		return COMMON_RECV_FAIL;
	}
	string stringPhmsResponseHead = bufPhmsResponseHead.begin();
	m_phmsResponseHead.SetPhmsResponseHead(stringPhmsResponseHead);
	m_phmsResponseBody.SetInputStreamPointer(pInputStream);
	m_phmsResponseBody.SetPhmsSessionPointer(pPhmsSession);
	return PHMS_SUCCESSFUL_RESULT;
}

//������Ϣһ��ͨ��stringMsgContent����
int CPhmsResponse::GetResult(int nSaveType, string& stringMsgContent, string& stringFilePath, bool bFileClearBeforeWrite, bool bNeedConvertEncoding)
{
	EnumPhmsResponseReturnCode enumReturnCode = FAIL_RETURN_CODE;
	enumReturnCode = m_phmsResponseHead.GetPhmsResponseReturnCode();

	int nRet = PHMS_SUCCESSFUL_RESULT;
	if(enumReturnCode != SUCCESS_RETURN_CODE)
	{
		//������Ϣһ��ͨ��stringMsgContent����
		nRet = m_phmsResponseBody.InputStreamToString(stringMsgContent);
		if(nRet != PHMS_SUCCESSFUL_RESULT)
		{
			CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
			return nRet;
		}
		else
		{
			//���������ش�����Ϣд��־
			int nResultErrorCode = atoi(m_phmsResponseHead.GetReturnCode().c_str());
			if(nResultErrorCode!=220403 && nResultErrorCode!=220405 && nResultErrorCode!=220303)
			{
				CPhmsLogger::GetPhmsLogger()->WriteLog(stringMsgContent, __FUNCTION__, __FILE__, __LINE__);
			}
			nSaveType = 1;//������Ϣ�����ַ������棬��д����־������nSaveTypeΪ1��Ϊ�˺�����ȷ����ǩ��
		}
	}
	else
	{
		switch(nSaveType)
		{
		case 0:
			//�������Ӧ����Ҳ�ջ�������ֹӰ���´ν���
			nRet = m_phmsResponseBody.InputStreamToString(stringMsgContent);
			break;
		case 1:
			nRet = m_phmsResponseBody.InputStreamToString(stringMsgContent);
			if(nRet != PHMS_SUCCESSFUL_RESULT)
			{
				CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
				return nRet;
			}
			break;
		case 2:
			nRet = m_phmsResponseBody.InputStreamToFile(stringFilePath, bFileClearBeforeWrite);
			if(nRet != PHMS_SUCCESSFUL_RESULT)
			{
				CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
				return nRet;
			}
			break;
		default:
			break;
		}
	}
	//��istream��֮ǰ��������ȵĻ�����֪�������������Ƿ���ȷ�����԰�ǩ����֤�ŵ������ȷ��md5Դ��size
	//�ŵ���Ҳ��Ϊ��������������ȫ�����������ˡ�
	//���Ƕ��ڱ��浽�ļ�����д�ļ����������ʱ��֧��ǩ����֤��������˵��������CPhmsResponseBody�м�һ����Ա���������棩
	if(!(nSaveType==2&&bFileClearBeforeWrite==false))
	{
		if(this->ValidateSign(nSaveType, stringMsgContent, stringFilePath) == false)
		{
			CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_RESPONSE_SIGN_VALIDATE_FAIL), __FUNCTION__, __FILE__, __LINE__);
			return COMMON_RESPONSE_SIGN_VALIDATE_FAIL;
		}
	}
	
	if(bNeedConvertEncoding && (nSaveType==1||nSaveType==2) )
	{
		nRet = ConvertEncodingFromUtf8(nSaveType, stringMsgContent, stringFilePath);
		if(nRet != PHMS_SUCCESSFUL_RESULT)
		{
			CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
			return nRet;
		}
	}
	return atoi(m_phmsResponseHead.GetReturnCode().c_str());
}

bool CPhmsResponse::ValidateSign(int nSaveType, string& stringMsgContent, string& stringFilePath)
{
	//�����ų�������֤�����
	if(m_phmsResponseHead.GetReturnCode() == "100001" || m_phmsResponseHead.GetReturnCode() == "100002" || 
		m_phmsResponseHead.GetReturnCode() == "100005" || m_phmsResponseHead.GetReturnCode() == "100006" || 
		m_phmsResponseHead.GetReturnCode() == "100007")
	{
		return true;
	}
	if(m_phmsResponseHead.GetSign() == "00000000000000000000000000000000")//ֻΪ1068\1099�������
	{
		return true;
	}
	//���ɵ�һ��md5
	string username;
	string password;
	//����û���������
	{
		username = CPhmsRequest::GetUsername();
		password = CPhmsRequest::GetPassword();
		if(username.size()==0 || password.size()==0)
		{
			username = g_stringUsername;
			password = g_stringPassword;
		}
	}
	string stringSrc1 = username+password;
	string md5_1;
	md5_1= CUtil::GetMd5(stringSrc1.c_str(), stringSrc1.size());

	//���buf�ܳ���
	int nLength = 0;
	int nHeadLengthExceptSign = 0;
	nHeadLengthExceptSign = m_phmsResponseHead.GetPhmsResposeHeadTotalLength()-(int)m_phmsResponseHead.GetSign().size();
	int nIStreamLength = 0;
	ifstream ifLocalTargetFile;
	streampos pos = 0;
	
	switch(nSaveType)
	{
	case 0:
		nIStreamLength = 0;
		break;
	case 1:
		nIStreamLength = (int)stringMsgContent.size();
		break;
	case 2:
		ifLocalTargetFile.exceptions(ios::failbit|ios::badbit);
		try
		{
			ifLocalTargetFile.open(stringFilePath.c_str(), ios::in|ios::binary);
		}
		catch(const ios::failure& error)
		{
			//д��־
			CPhmsLogger::GetPhmsLogger()->WriteLog(error.what(), __FUNCTION__, __FILE__, __LINE__);
			return false;
		}
		pos = ifLocalTargetFile.tellg();
		ifLocalTargetFile.seekg(0,ios::end);   
		nIStreamLength = ifLocalTargetFile.tellg();   
		ifLocalTargetFile.seekg(pos);
		break;
	default:
		nIStreamLength = 0;
		break;
	}
	//����ܳ��Ȳ�����bufSrc
	char* bufSrc = NULL;
	nLength = (int)md5_1.size()+nHeadLengthExceptSign+nIStreamLength;
	bufSrc = new char[nLength+1];
	memset(bufSrc, 0, nLength+1);
	//md5_1����
	memcpy(bufSrc, md5_1.c_str(), md5_1.size());
	//PhmsResponseHead����
	string stringHeadExceptSign;
	stringHeadExceptSign = m_phmsResponseHead.GetVersion()+m_phmsResponseHead.GetReturnCode()+m_phmsResponseHead.GetMessageFormat();
	memcpy(bufSrc+md5_1.size(), stringHeadExceptSign.c_str(), nHeadLengthExceptSign);
	//PhmsResponseBody����
	switch(nSaveType)
	{
	case 0:
		break;
	case 1:
		memcpy(bufSrc+md5_1.size()+nHeadLengthExceptSign, stringMsgContent.c_str(), stringMsgContent.size());
		break;
	case 2:
		//int nTempLength = 0;
		try
		{
			ifLocalTargetFile.read(bufSrc+md5_1.size()+nHeadLengthExceptSign, nIStreamLength);
			ifLocalTargetFile.close();
		}
		catch (const ios::failure& error)
		{
			//д��־
			ifLocalTargetFile.close();
			delete bufSrc;
			CPhmsLogger::GetPhmsLogger()->WriteLog(error.what(), __FUNCTION__, __FILE__, __LINE__);
			CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_FILE_READ_FAIL), __FUNCTION__, __FILE__, __LINE__);
			return false;
		}
		break;
	}
	//�������յ�md5��У��
	string md5_2 ;
	md5_2 = CUtil::GetMd5(bufSrc, nLength);
	if(md5_2 != m_phmsResponseHead.GetSign())
	{
		delete bufSrc;
		return false;
	}
	delete bufSrc;
	return true;
}

int CPhmsResponse::ConvertEncodingFromUtf8(int nSaveType, string& stringMsgContent, string& stringFilePath)
{
	string stringTempXmlContent;
	string stringWorkingDir = CUtil::GetCurrentAppDir();
	string stringIniFilePath = stringWorkingDir+"PhmsConfig.ini";
	AutoPtr<IniFileConfiguration> iniFile = new IniFileConfiguration(stringIniFilePath, "gbk");
	string stringEncoding = iniFile->getString("OTHER.Encoding", "GBK");
	TextEncoding* pEnvEncoding = TextEncoding::find(stringEncoding);
	TextEncoding* pUtf8Encoding = TextEncoding::find("UTF-8");

	ostringstream ostr;
	if(nSaveType == 1)
	{
		istringstream istr(stringMsgContent);
		istr.exceptions(ios::badbit);
		InputStreamConverter converter(istr, *pUtf8Encoding, *pEnvEncoding);
		try
		{
			StreamCopier::copyStream(converter, ostr);
		}
		catch(const std::ios::failure& e)
		{
			CPhmsLogger::GetPhmsLogger()->WriteLog(e.what(), __FUNCTION__, __FILE__, __LINE__);
			CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_INPUT_STREAM_FAIL), __FUNCTION__, __FILE__, __LINE__);
			return COMMON_INPUT_STREAM_FAIL;
		}
		stringTempXmlContent = ostr.str();
	}
	if(nSaveType == 2)
	{
		ifstream istr;
		istr.exceptions(ios::badbit);
		try
		{
			istr.open(stringFilePath.c_str());
		}
		catch(const ios::failure& error)
		{
			//д��־
			CPhmsLogger::GetPhmsLogger()->WriteLog(error.what(), __FUNCTION__, __FILE__, __LINE__);
			CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_FILE_OPEN_FAIL), __FUNCTION__, __FILE__, __LINE__);
			return COMMON_FILE_OPEN_FAIL;
		}
		InputStreamConverter converter(istr, *pUtf8Encoding, *pEnvEncoding);
		try
		{
			StreamCopier::copyStream(converter, ostr);
		}
		catch(const std::ios::failure& e)
		{
			istr.close();
			CPhmsLogger::GetPhmsLogger()->WriteLog(e.what(), __FUNCTION__, __FILE__, __LINE__);
			CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_INPUT_STREAM_FAIL), __FUNCTION__, __FILE__, __LINE__);
			return COMMON_INPUT_STREAM_FAIL;
		}
		istr.close();
		stringTempXmlContent = ostr.str();
	}



	string stringSrc = "encoding=\"UTF-8\"?>";
	string stringDst = "encoding=\"";
	stringDst += stringEncoding;
	stringDst += "\"?>";
	CUtil::ReplaceString(stringTempXmlContent, stringSrc, stringDst);
	if(nSaveType == 1)
	{
		stringMsgContent = stringTempXmlContent;
	}
	if(nSaveType == 2)
	{
		ofstream ofs;
		ofs.exceptions(ios::failbit | ios::badbit);
		try
		{
			ofs.open(stringFilePath.c_str(), ios::out|ios::trunc);
			ofs.write(stringTempXmlContent.c_str(), stringTempXmlContent.size());
			ofs.close();
		}
		catch(const std::ios::failure& e)
		{
			ofs.close();
			CPhmsLogger::GetPhmsLogger()->WriteLog(e.what(), __FUNCTION__, __FILE__, __LINE__);
			CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_INPUT_STREAM_FAIL), __FUNCTION__, __FILE__, __LINE__);
			return COMMON_INPUT_STREAM_FAIL;
		}

	}
	return PHMS_SUCCESSFUL_RESULT;
}
