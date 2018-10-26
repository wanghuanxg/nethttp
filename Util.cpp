#include "Util.h"
#include <algorithm>
#include <functional>
#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>
#include "Poco/Exception.h"
#include "Poco/MD5Engine.h"
#include "Poco/File.h"
#include <fstream>
#include "ErrorCode.h"
#include "Poco/DOM/Document.h"
#include "Poco/DOM/Text.h"
#include "Poco/DOM/Element.h"
#include "Poco/DOM/ProcessingInstruction.h"
#include "Poco/DOM/DOMWriter.h"
#include "Poco/DOM/DOMParser.h"
#include "Poco/DOM/NodeIterator.h"
#include "Poco/SAX/InputSource.h"
#include "Poco/DOM/Node.h"
#include "Poco/DOM/NodeFilter.h"
#include "Poco/XML/XMLWriter.h"
#include "Poco/AutoPtr.h"
#include "Poco/TextConverter.h"
#include "Poco/TextEncoding.h"
#include "Poco/GBKEncoding.h"
#include "Poco/KOI8REncoding.h"
#include "Poco/UTF8Encoding.h"
#include "Poco/TextIterator.h"
#include "Poco/StreamCopier.h"
#include "PhmsLogger.h"
#include "Poco/Buffer.h"
#include "Poco/Util/Application.h"
#include "Poco/Path.h"
#include "Poco/Util/IniFileConfiguration.h"
#include <iomanip>
#include "json/json.h"
#include "json/value.h"

using Poco::MD5Engine;
using Poco::Exception;
using Poco::DigestEngine;
using Poco::File;
using Poco::XML::Document;
using Poco::XML::Text;
using Poco::XML::Element;
using Poco::XML::ProcessingInstruction;
using Poco::XML::DOMWriter;
using Poco::XML::DOMParser;
using Poco::XML::NodeIterator;
using Poco::XML::InputSource;
using Poco::XML::Node;
using Poco::XML::NodeFilter;
using Poco::XML::XMLWriter;
using Poco::AutoPtr;
using Poco::TextConverter;
using Poco::TextEncoding;
using Poco::GBKEncoding;
using Poco::KOI8REncoding;
using Poco::UTF8Encoding;
using Poco::StreamCopier;
using Poco::Util::Application;
using Poco::Path;
using Poco::TextIterator;
using Poco::Util::IniFileConfiguration;

using namespace std;


string CUtil::m_stringAppDir="";

/**********************************************************
*
*功能：去前空格
*
*str：源字符串
*
*返回值：去除前空格后的字符串
*
***********************************************************/
string& CUtil::Ltrim(string &str)
{
	str.erase(str.begin(), std::find_if(str.begin(), str.end(),
		std::not1(std::ptr_fun(::isspace))));
	return str;

}

/**********************************************************
*
*功能：去后空格
*
*str：源字符串
*
*返回值：去除后空格后的字符串
*
***********************************************************/
string& CUtil::Rtrim(string &str)
{
	str.erase(std::find_if(str.rbegin(), str.rend(),
		std::not1(std::ptr_fun(::isspace))).base(),
		str.end());
	return str;

}

/**********************************************************
*
*功能：去前后空格
*
*str：源字符串
*
*返回值：去除前后空格后的字符串
*
***********************************************************/
string& CUtil::Trim(string &str)
{
	return Rtrim(Ltrim(str));
}


/**********************************************************
*
*功能：double转string
*
*dValut：源double数值
*decplaces：欲保留的小数点后位数（包括小数点）
*
*返回值：转换后的string
*
***********************************************************/
#define DECIMAL_POINT "."
string CUtil::DoubleToString(long double dValut, int decplaces)
{
	if((long)(dValut*100)%100 == 0)
	{
		return CUtil::ULongToString(dValut);
	}
	if(dValut == 0.0)
	{
		return "";
	}
	ostringstream out;
	//out.ios
	//int prec=numeric_limits<double>::digits10;//使用最大精度18
	//out.precision(prec);//覆盖默认精度
	out<<setiosflags(ios::fixed)<<setprecision(2)<<dValut;//四舍五入并保留小数点后两位
	string str= out.str(); //从流中取出字符串
	size_t n=str.find(DECIMAL_POINT);
	if ((n!=string::npos) && (str.size()> n+decplaces)) //后面至少还有decplaces位吗？
	{
		str[n+decplaces]='\0';//覆盖第一个多余的数,得到小数点后decplaces-1位
	}
	string stringTemp = str.c_str();
	str.swap(stringTemp);//自交换删除nul之后的多余字符，因为string对象的数据可以包含nul字符；而字符串的实际长度由size()的返回值决定。
	return str;
}


/**********************************************************
*
*功能：string转unsigned long
*
*nValue：源string值
*
*返回值：转换后的unsigned long
*
***********************************************************/
unsigned long CUtil::StringToULong(string stringValue)
{
	unsigned long nValue;
	istringstream iStringStream(stringValue);
	iStringStream>>nValue;
	return nValue;
}

/**********************************************************
*
*功能：unsigned long转string
*
*nValue：源unsigned long数值
*
*返回值：转换后的string
*
***********************************************************/
string CUtil::ULongToString(unsigned long nValue)
{
	string stringDst;
	ostringstream oStringStream;
	oStringStream<<nValue;
	stringDst = oStringStream.str();
	return stringDst;
}
/**********************************************************
*
*功能：unsigned int转string
*
*nValue：源unsigned int数值
*
*返回值：转换后的string
*
***********************************************************/
string CUtil::UIntToString(unsigned int nValue)
{
	string stringDst;
	ostringstream oStringStream;
	oStringStream<<nValue;
	stringDst = oStringStream.str();
	return stringDst;
}

string CUtil::Int64ToString(Poco::Int64 nValue)
{
	string stringDst;
	ostringstream oStringStream;
	oStringStream<<nValue;
	stringDst = oStringStream.str();
	return stringDst;
}


/**********************************************************
*
*功能：求Md5码
*
*bufSrc：源buf
*nLength：buf长度
*
*返回值：计算出的md5码
*
***********************************************************/
string CUtil::GetMd5(const char* bufSrc, int nLength)
{
	string stringMd5;
	MD5Engine engine;
	engine.update(bufSrc, nLength);
	stringMd5 = DigestEngine::digestToHex(engine.digest());
	return stringMd5;
}

/**********************************************************
*
*功能：求Md5码
*
*stringFilePath：源文件路径
*stringMd5：输出参数，计算出的md5码
*
*返回值：正确或错误码
*
***********************************************************/
int CUtil::GetMd5(string stringFilePath, string& stringMd5)
{
	//计算md5
	char* bufSrc = NULL;
	unsigned long nLength = 0;
	int nRet = PHMS_SUCCESSFUL_RESULT;
	nRet = CUtil::GetFileSize(stringFilePath, nLength);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	bufSrc = new char[nLength+1];
	memset(bufSrc, 0, nLength+1);
	ifstream iFStream;
	try
	{
		iFStream.exceptions(ios::eofbit|ios::failbit|ios::badbit);
		iFStream.open(stringFilePath.c_str(), ios_base::binary);
		iFStream.read(bufSrc, nLength);
		iFStream.close();
	}
	catch (ios_base::failure& e)
	{
		//写日志
		delete bufSrc;
		CPhmsLogger::GetPhmsLogger()->WriteLog(e.what(), __FUNCTION__, __FILE__, __LINE__);
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_FILE_READ_FAIL), __FUNCTION__, __FILE__, __LINE__);
		return COMMON_FILE_READ_FAIL;
		
	}
	stringMd5 = CUtil::GetMd5(bufSrc, nLength);
	delete bufSrc;
	return PHMS_SUCCESSFUL_RESULT;
}


/**********************************************************
*
*功能：获得文件长度
*
*stringFilePath：源文件路径
*nFileSize：输出参数，文件长度
*
*返回值：文件长度
*
***********************************************************/
int CUtil::GetFileSize(string stringFilePath, unsigned long& nFileSize)
{
	ifstream ifLocalTargetFile;
	unsigned long nPos=0;
	try
	{
		ifLocalTargetFile.exceptions(ios::badbit);
		ifLocalTargetFile.open(stringFilePath.c_str(), ios::in|ios::binary);
		nPos = ifLocalTargetFile.tellg();
		ifLocalTargetFile.seekg(0,ios::end);   
		nFileSize = ifLocalTargetFile.tellg();   
		ifLocalTargetFile.seekg(nPos);
		ifLocalTargetFile.close();
	}
	catch(const ios::failure& error)
	{
		//写日志
		nFileSize = 0;
		CPhmsLogger::GetPhmsLogger()->WriteLog(error.what(), __FUNCTION__, __FILE__, __LINE__);
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_FILE_READ_FAIL), __FUNCTION__, __FILE__, __LINE__);
		return COMMON_FILE_READ_FAIL;
	}
	return PHMS_SUCCESSFUL_RESULT;
}


/**********************************************************
*
*功能：获得文件长度
*
*stringFilePath：源文件路径
*
*返回值：文件长度
*
***********************************************************/
unsigned long CUtil::GetFileSize(string stringFilePath)
{
	File fileTarget(stringFilePath, "gbk");
	if(fileTarget.exists())
		return (unsigned long)fileTarget.getSize();
	else
		return 0;
}
//该函数只适合于只有一个Text Node子节点的Element Node
int CUtil::GetXmlFieldVaule(int nSaveType, string stringXmlContent, string stringXmlPath, string stringField, string& stringVaule)
{
	DOMParser parser;//创建一个分析者 
	AutoPtr<Document> pDoc = NULL;//开始分析，并返回指向分析后数据的指针 
	string stringContent;
	string stringUtf8NodeVaule;


	string stringWorkingDir = CUtil::GetCurrentAppDir();
	string stringIniFilePath = stringWorkingDir+"PhmsConfig.ini";
	AutoPtr<IniFileConfiguration> iniFile = new IniFileConfiguration(stringIniFilePath, "gbk");
	string stringEncoding = iniFile->getString("OTHER.Encoding", "GBK");
	TextEncoding* pEnvEncoding = TextEncoding::find(stringEncoding);
	TextEncoding* pUtf8Encoding = TextEncoding::find("UTF-8");

	TextConverter converter(*pUtf8Encoding, *pEnvEncoding);
	//int nError = 0;
	try
	{
		if(nSaveType == 1)
		{
			//xml内容保存到了stringContent
			stringContent = stringXmlContent;
			parser.setEncoding(pEnvEncoding->canonicalName());
			pDoc = parser.parseString(stringContent);//开始分析，并返回指向分析后数据的指针 
		}
		else
		{
			if(nSaveType == 2)
			{
				//xml内容保存到了stringFilePath
				ifstream iFStream;
				ostringstream oStringStream;
				iFStream.open(stringXmlPath.c_str());
				StreamCopier::copyStream(iFStream, oStringStream);
				iFStream.close();
				stringContent = oStringStream.str();
				parser.setEncoding(pEnvEncoding->canonicalName());
				pDoc = parser.parseString(stringContent);//开始分析，并返回指向分析后数据的指针 
			}
			else
			{
				return PHMS_SUCCESSFUL_RESULT;
			}
		}

		NodeIterator it(pDoc, NodeFilter::SHOW_ALL);//创建一个节点迭代器，默认迭代显示全部node  
		Node* pNode = it.nextNode();//此时指向第一个  
		while (pNode)  
		{
			if(pNode->nodeName() == stringField)
			{
				if(pNode->hasChildNodes())
				{
					pNode = pNode->firstChild();
					if(pNode->nodeType() == Node::TEXT_NODE)
					{
						stringUtf8NodeVaule = pNode->nodeValue();
						converter.convert(stringUtf8NodeVaule, stringVaule);
						break;
					}
					else
					{
						stringVaule = "";
						break;
					}
				}
				else
				{
					stringVaule = "";
					break;
				}
			}
			//std::cout<<pNode->nodeName()<<"	"<<pNode->nodeName().size()<<"	"<<pNode->nodeValue()<<"	"<<pNode->nodeValue().size()<<endl;
			pNode = it.nextNode();//指向下一个node
		}
		if(!pNode)
		{
			//写日志
			//返回xml中没有sid
			stringVaule = "";
			return PHMS_SUCCESSFUL_RESULT;
		}
	}
	catch(Exception& e)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(e, __FUNCTION__, __FILE__, __LINE__);
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_PARSE_XML_FAIL), __FUNCTION__, __FILE__, __LINE__);
		return COMMON_PARSE_XML_FAIL;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CUtil::GetOrReplaceImagePathVector(int nSaveType, string stringXmlContent, string stringXmlPath, vector<string>& vImagePath)
{
	DOMParser parser;//创建一个分析者 
	AutoPtr<Document> pDoc = NULL;//开始分析，并返回指向分析后数据的指针 
	string stringContent;
	string stringUtf8NodeVaule;

	string stringWorkingDir = CUtil::GetCurrentAppDir();
	string stringIniFilePath = stringWorkingDir+"PhmsConfig.ini";
	AutoPtr<IniFileConfiguration> iniFile = new IniFileConfiguration(stringIniFilePath, "gbk");
	string stringEncoding = iniFile->getString("OTHER.Encoding", "GBK");
	TextEncoding* pEnvEncoding = TextEncoding::find(stringEncoding);
	TextEncoding* pUtf8Encoding = TextEncoding::find("UTF-8");

	TextConverter converter(*pUtf8Encoding, *pEnvEncoding);
	//int nError = 0;
	try
	{
		if(nSaveType == 1)
		{
			//xml内容保存到了stringContent
			stringContent = stringXmlContent;
			parser.setEncoding(pEnvEncoding->canonicalName());
			pDoc = parser.parseString(stringContent);//开始分析，并返回指向分析后数据的指针 
		}
		else
		{
			if(nSaveType == 2)
			{
				//xml内容保存到了stringFilePath
				ifstream iFStream;
				ostringstream oStringStream;
				iFStream.open(stringXmlPath.c_str());
				StreamCopier::copyStream(iFStream, oStringStream);
				iFStream.close();
				stringContent = oStringStream.str();
				parser.setEncoding(pEnvEncoding->canonicalName());
				pDoc = parser.parseString(stringContent);//开始分析，并返回指向分析后数据的指针 
			}
			else
			{
				return PHMS_SUCCESSFUL_RESULT;
			}
		}
		NodeIterator it(pDoc, NodeFilter::SHOW_ALL);//创建一个节点迭代器，默认迭代显示全部node  
		Node* pNode = it.nextNode();//此时指向第一个  
		Node* pChildNode = NULL;
		Node* pGrandChildNode = NULL;
		while (pNode)  
		{
			if(pNode->nodeName() == "image")
			{
				pChildNode = pNode->firstChild();
				do
				{
					if(pChildNode->nodeName() != "path")
					{
						pChildNode = pChildNode->nextSibling();
						continue;
					}
					pGrandChildNode = pChildNode->firstChild();
					if(pGrandChildNode && pGrandChildNode->nodeType()==Node::TEXT_NODE)
					{
						if(pGrandChildNode->nodeValue().size() != 0)
						{
							vImagePath.push_back(pGrandChildNode->nodeValue());
						}
						else
						{
							CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_PARSE_XML_FAIL), __FUNCTION__, __FILE__, __LINE__);
							return COMMON_PICTURE_CONSULTATION_ERROR;
						}
					}
					break;
				}while(pChildNode && pChildNode->nodeType()==Node::ELEMENT_NODE);
			}
			//std::cout<<pNode->nodeName()<<"	"<<pNode->nodeName().size()<<"	"<<pNode->nodeValue()<<"	"<<pNode->nodeValue().size()<<endl;
			pNode = it.nextNode();//指向下一个node
		}
	}
	catch(Exception& e)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(e, __FUNCTION__, __FILE__, __LINE__);
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_PARSE_XML_FAIL), __FUNCTION__, __FILE__, __LINE__);
		return COMMON_PARSE_XML_FAIL;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CUtil::GetMapFromRequestXml(string stringXmlContent, string stringKey, map<string, string>& mapParam)
{//未设置字符编码
	DOMParser parser;//创建一个分析者 
	AutoPtr<Document> pDoc = NULL;//开始分析，并返回指向分析后数据的指针 
	string stringNodeKey;
	string stringNodeVaule;
	//GBKEncoding encodingGbk;
	//int nError = 0;
	try
	{
		//parser.setEncoding(encodingGbk.canonicalName());
		pDoc = parser.parseString(stringXmlContent);//开始分析，并返回指向分析后数据的指针 
		NodeIterator it(pDoc, NodeFilter::SHOW_ALL);//创建一个节点迭代器，默认迭代显示全部node  
		Node* pNode = it.nextNode();//此时指向第一个  
		while (pNode)  
		{
			if(pNode->nodeName() == stringKey)
			{
				if(pNode->hasChildNodes())
				{
					pNode = pNode->firstChild();
					while(pNode)
					{
						if(pNode->hasChildNodes())
						{
							if(pNode->firstChild()->nodeType()==Node::TEXT_NODE)
							{
								stringNodeKey = pNode->nodeName();
								stringNodeVaule = pNode->firstChild()->nodeValue();
								//string stringGBKNodeValue;
								//CUtil::UTF8ToGBK(stringNodeVaule, stringGBKNodeValue);
								mapParam[stringNodeKey] = stringNodeVaule/*stringGBKNodeValue*/;
							}
						}
						else
						{
							if(pNode->nodeType() == Node::ELEMENT_NODE)
							{
								stringNodeKey = pNode->nodeName();
								stringNodeVaule = "";
								mapParam[stringNodeKey] = stringNodeVaule;
							}
						}
						pNode = pNode->nextSibling();
					}
				}
				break;
			}
			pNode = it.nextNode();//指向下一个node
		}
		if(!pNode)
		{
			//写日志
			//返回xml中没有sid
			return PHMS_SUCCESSFUL_RESULT;
		}
	}
	catch(Exception& e)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(e, __FUNCTION__, __FILE__, __LINE__);
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_PARSE_XML_FAIL), __FUNCTION__, __FILE__, __LINE__);
		return COMMON_PARSE_XML_FAIL;
	}
	return PHMS_SUCCESSFUL_RESULT;
}


string CUtil::GetErrorMsg(int nError)
{
	string stringErrorMsg;
	switch(nError)
	{
	case COMMON_PARAM_VALIDATE_FAIL:
		stringErrorMsg = "COMMON_PARAM_VALIDATE_FAIL";
		break;
	case COMMON_NO_IP:
		stringErrorMsg = "COMMON_NO_IP";
		break;
	case COMMON_NO_PORT:
		stringErrorMsg = "COMMON_NO_PORT";
		break;
	case COMMON_DNS_FAIL:
		stringErrorMsg = "COMMON_DNS_FAIL";
		break;
	case COMMON_CONNECT_FAIL:
		stringErrorMsg = "COMMON_CONNECT_FAIL";
		break;
	case COMMON_SEND_FAIL:
		stringErrorMsg = "COMMON_SEND_FAIL";
		break;
	case COMMON_SEND_TIMEOUT:
		stringErrorMsg = "COMMON_SEND_TIMEOUT";
		break;
	case COMMON_RECV_FAIL:
		stringErrorMsg = "COMMON_RECV_FAIL";
		break;
	case COMMON_RECV_TIMEOUT:
		stringErrorMsg = "COMMON_RECV_TIMEOUT";
		break;
	case COMMON_OFF_NETWORK:
		stringErrorMsg = "COMMON_OFF_NETWORK";
		break;
	case COMMON_FILE_OPEN_FAIL:
		stringErrorMsg = "COMMON_FILE_OPEN_FAIL";
		break;
	case COMMON_FILE_WRITE_FAIL:
		stringErrorMsg = "COMMON_FILE_WRITE_FAIL";
		break;
	case COMMON_FILE_READ_FAIL:
		stringErrorMsg = "COMMON_FILE_READ_FAIL";
		break;
	case COMMON_CREATE_DIR_FAIL:
		stringErrorMsg = "COMMON_CREATE_DIR_FAIL";
		break;
	case COMMON_MEMORY_FAIL:
		stringErrorMsg = "COMMON_MEMORY_FAIL";
		break;
	case COMMON_BUF_SMALL:
		stringErrorMsg = "COMMON_BUF_SMALL";
		break;
	case COMMON_DISK_FULL:
		stringErrorMsg = "COMMON_DISK_FULL";
		break;
	case COMMON_CALCULATE_MD5_FAIL:
		stringErrorMsg = "COMMON_CALCULATE_MD5_FAIL";
		break;
	case COMMON_VALIDATE_MD5_FAIL:
		stringErrorMsg = "COMMON_VALIDATE_MD5_FAIL";
		break;
	case COMMON_BINARY_TO_STRING:
		stringErrorMsg = "COMMON_BINARY_TO_STRING";
		break;
	case COMMON_RECV_CONTENT_FAIL:
		stringErrorMsg = "COMMON_RECV_CONTENT_FAIL";
		break;
	case COMMON_SSL_CONFLICT_ERROR:
		stringErrorMsg = "COMMON_SSL_CONFLICT_ERROR";
		break;
	case COMMON_OUTPUT_STREAM_FAIL:
		stringErrorMsg = "COMMON_OUTPUT_STREAM_FAIL";
		break;
	case COMMON_INPUT_STREAM_FAIL:
		stringErrorMsg = "COMMON_INPUT_STREAM_FAIL";
		break;
	case COMMON_NET_CONFIG_ERROR:
		stringErrorMsg = "COMMON_NET_CONFIG_ERROR";
		break;
	case COMMON_RESPONSE_SIGN_VALIDATE_FAIL:
		stringErrorMsg = "COMMON_RESPONSE_SIGN_VALIDATE_FAIL";
		break;
	case COMMON_NO_SAVE_SESSIONID:
		stringErrorMsg = "COMMON_NO_SAVE_SESSIONID";
		break;
	case COMMON_PARSE_XML_FAIL:
		stringErrorMsg = "COMMON_PARSE_XML_FAIL";
		break;
	case COMMON_NO_SID:
		stringErrorMsg = "COMMON_NO_SID";
		break;
	case COMMON_WRITE_XML_ERROR:
		stringErrorMsg = "COMMON_WRITE_XML_ERROR";
		break;
	case COMMON_UPLOAD_INI_SIZE_ERROR:
		stringErrorMsg = "COMMON_UPLOAD_INI_SIZE_ERROR";
		break;
	case COMMON_DOWNLOAD_THREAD_COUNT_TOO_LARGE:
		stringErrorMsg = "COMMON_DOWNLOAD_THREAD_COUNT_TOO_LARGE";
		break;
	case COMMON_LOCAL_FILE_MORE_THAN_SERVER_FILE:
		stringErrorMsg = "COMMON_LOCAL_FILE_MORE_THAN_SERVER_FILE";
		break;
	case COMMON_LOCAL_TEMP_TOO_LARGE:
		stringErrorMsg = "COMMON_LOCAL_TEMP_TOO_LARGE";
		break;
	case COMMON_DOWNLOAD_SEGMENT_FAIL_FOR_FILESIEZE_CHANGE:
		stringErrorMsg = "COMMON_DOWNLOAD_SEGMENT_FAIL_FOR_FILESIEZE_CHANGE";
		break;
	case COMMON_COMBILE_FILE_FAIL:
		stringErrorMsg = "COMMON_COMBILE_FILE_FAIL";
		break;
	case COMMON_INVALID_HANDLE:
		stringErrorMsg = "COMMON_INVALID_HANDLE";
		break;
	case COMMON_SESSION_BUF_TOO_SMALL:
		stringErrorMsg = "COMMON_SESSION_BUF_TOO_SMALL";
		break;
	case COMMON_PHMS_REQUEST_VALIDATE_FAIL:
		stringErrorMsg = "COMMON_PHMS_REQUEST_VALIDATE_FAIL";
		break;
	case COMMON_NO_URL:
		stringErrorMsg = "COMMON_NO_URL";
		break;
	case COMMON_PROXY_INFO_ERROR:
		stringErrorMsg = "COMMON_PROXY_INFO_ERROR";
		break;
	case COMMON_RESPONSE_FORMAT_ERROR:
		stringErrorMsg = "COMMON_RESPONSE_FORMAT_ERROR";
		break;
	case COMMON_CONVERT_ENCODE_ERROR:
		stringErrorMsg = "COMMON_CONVERT_ENCODE_ERROR";
		break;
	case COMMON_UPLOAD_LESS_3_BYTES:
		stringErrorMsg = "COMMON_UPLOAD_LESS_3_BYTES";
		break;
	case COMMON_NO_NET_CONFIG_FILE:
		stringErrorMsg = "COMMON_NO_NET_CONFIG_FILE";
		break;
	case COMMON_NET_CONFIG_READ_ERROR:
		stringErrorMsg = "COMMON_NET_CONFIG_READ_ERROR";
		break;
	case COMMON_NO_VALID_SESSIONID:
		stringErrorMsg = "COMMON_NO_VALID_SESSIONID";
		break;
	case COMMON_HANDLE_THREAD_MISMATCH:
		stringErrorMsg = "COMMON_HANDLE_THREAD_MISMATCH";
		break;
	case COMMON_GET_CASE_STATE_ERROR:
		stringErrorMsg = "COMMON_GET_CASE_STATE_ERROR";
		break;
	case COMMON_GENERATE_LOG_FILE_ERROR:
		stringErrorMsg = "COMMON_GENERATE_LOG_FILE_ERROR";
		break;
	case COMMON_DOWNLOAD_FILE_SIZE_0:
		stringErrorMsg = "COMMON_DOWNLOAD_FILE_SIZE_0";
		break;
	case COMMON_SAME_PUSH_IS_RUN:
		stringErrorMsg = "COMMON_SAME_PUSH_IS_RUN";
		break;
	case COMMON_SESSION_INVALID:
		stringErrorMsg = "COMMON_SESSION_INVALID";
		break;
	case COMMON_LOGIN_ANOTHER_PLACE:
		stringErrorMsg = "COMMON_LOGIN_ANOTHER_PLACE";
		break;
	case COMMON_SERVER_NO_SUPPORT:
		stringErrorMsg = "COMMON_SERVER_NO_SUPPORT";
		break;
	case COMMON_INVALID_CHARSET:
		stringErrorMsg = "COMMON_INVALID_CHARSET";
		break;
	case COMMON_NO_SET_TERMINAL_TYPE:
		stringErrorMsg = "COMMON_NO_SET_TERMINAL_TYPE";
		break;
	case COMMON_URL_NOT_FOUND:
		stringErrorMsg = "COMMON_URL_NOT_FOUND";
		break;
	case COMMON_GET_CASE_SIZE_ERROR:
		stringErrorMsg = "COMMON_GET_CASE_SIZE_ERROR";
		break;
	case COMMON_COMBILE_FILE_FAIL_BUT_CAN_RETRY:
		stringErrorMsg = "COMMON_COMBILE_FILE_FAIL_BUT_CAN_RETRY";
		break;
	case COMMON_INVALID_THREAD_ID:
		stringErrorMsg = "COMMON_INVALID_THREAD_ID";
		break;
	case COMMON_ILLEGAL_GBK_ENCODING:
		stringErrorMsg = "COMMON_ILLEGAL_GBK_ENCODING";
		break;
	case COMMON_PARAM_SIZE_0:
		stringErrorMsg = "COMMON_PARAM_SIZE_0";
		break;
	case COMMON_PICTURE_CONSULTATION_ERROR:
		stringErrorMsg = "COMMON_PICTURE_CONSULTATION_ERROR";
		break;
	case COMMON_XIKANG_URL_OPENID_NULL:
		stringErrorMsg = "COMMON_XIKANG_URL_OPENID_NULL";
		break;
	case COMMON_GET_TOKEN_FAIL:
		stringErrorMsg = "COMMON_GET_TOKEN_FAIL";
		break;
	case COMMON_INVALID_URL:
		stringErrorMsg = "COMMON_INVALID_URL";
		break;
	default:
		stringErrorMsg = "SERVER_RETURN_INTERFACE_ERROR";
		break;
	}
	return stringErrorMsg;
}


int CUtil::GBKToUTF8(string stringGbk, string& stringUtf8)
{
	GBKEncoding encodingGBK;
	UTF8Encoding encodingUtf8;
	TextConverter converter(encodingGBK, encodingUtf8);
	converter.convert(stringGbk, stringUtf8);
	return PHMS_SUCCESSFUL_RESULT;
}

int CUtil::UTF8ToGBK(string stringUtf8, string& stringGbk)
{
	GBKEncoding encodingGBK;
	UTF8Encoding encodingUtf8;
	TextConverter converter(encodingUtf8, encodingGBK);
	converter.convert(stringUtf8, stringGbk);
	return PHMS_SUCCESSFUL_RESULT;
}

int CUtil::EnvToUtf8(string stringEnv, string& stringUtf8)
{
	string stringWorkingDir = CUtil::GetCurrentAppDir();
	string stringIniFilePath = stringWorkingDir+"PhmsConfig.ini";
	AutoPtr<IniFileConfiguration> iniFile = new IniFileConfiguration(stringIniFilePath, "gbk");
	string stringEncoding = iniFile->getString("OTHER.Encoding", "GBK");
	TextEncoding* pEnvEncoding = TextEncoding::find(stringEncoding);
	TextEncoding* pUtf8Encoding = TextEncoding::find("UTF-8");
	TextConverter converter(*pEnvEncoding, *pUtf8Encoding);
	converter.convert(stringEnv, stringUtf8);
	return PHMS_SUCCESSFUL_RESULT;
}
string CUtil::EnvToUtf8(string stringEnv)
{
	string stringUtf8;
	string stringWorkingDir = CUtil::GetCurrentAppDir();
	string stringIniFilePath = stringWorkingDir+"PhmsConfig.ini";
	AutoPtr<IniFileConfiguration> iniFile = new IniFileConfiguration(stringIniFilePath, "gbk");
	string stringEncoding = iniFile->getString("OTHER.Encoding", "GBK");
	TextEncoding* pEnvEncoding = TextEncoding::find(stringEncoding);
	TextEncoding* pUtf8Encoding = TextEncoding::find("UTF-8");
	TextConverter converter(*pEnvEncoding, *pUtf8Encoding);
	converter.convert(stringEnv, stringUtf8);
	return stringUtf8;
}
int CUtil::UTF8ToEnv(string stringUtf8, string& stringEnv)
{
	string stringWorkingDir = CUtil::GetCurrentAppDir();
	string stringIniFilePath = stringWorkingDir+"PhmsConfig.ini";
	AutoPtr<IniFileConfiguration> iniFile = new IniFileConfiguration(stringIniFilePath, "gbk");
	string stringEncoding = iniFile->getString("OTHER.Encoding", "GBK");
	TextEncoding* pEnvEncoding = TextEncoding::find(stringEncoding);
	TextEncoding* pUtf8Encoding = TextEncoding::find("UTF-8");
	TextConverter converter(*pUtf8Encoding, *pEnvEncoding);
	converter.convert(stringUtf8, stringEnv);
	return PHMS_SUCCESSFUL_RESULT;
}


string CUtil::GetCurrentAppDir()
{
	if(m_stringAppDir.size() != 0)
	{
		return m_stringAppDir;
	}
	string stringPathEnv;
	string stringPathUtf8;
	stringPathUtf8 = Path::current();
	CUtil::UTF8ToGBK(stringPathUtf8, stringPathEnv);
#if(POCO_OS == POCO_OS_MAC_OS_X)
	return stringPathEnv + "Documents/";
#else
	return stringPathEnv;
#endif
}

bool CUtil::IsLegalGBK(string stringSrc)
{
	GBKEncoding inEncoding;
	TextIterator it(stringSrc, inEncoding);
	TextIterator end(stringSrc);
	for(; it!=end; it++)
	{
		if(*it == -1)
			return false;
	}
	return true;
}
bool CUtil::IsLegalKOI8R(string stringSrc)
{
	KOI8REncoding inEncoding;
	TextIterator it(stringSrc, inEncoding);
	TextIterator end(stringSrc);
	for(; it!=end; it++)
	{
		if(*it == -1)
			return false;
	}
	return true;
}
bool CUtil::IsLegalUTF8(string stringSrc)
{
	UTF8Encoding inEncoding;
	TextIterator it(stringSrc, inEncoding);
	TextIterator end(stringSrc);
	for(; it!=end; it++)
	{
		if(*it == -1)
			return false;
	}
	return true;
}

void CUtil::SetAppDir(string stringWorkDir)
{
	if(m_stringAppDir.size()==0)
	{
		m_stringAppDir = stringWorkDir;
	}
}

void CUtil::ReplaceStringInFile(string stringPath, string stringSrc, string stringDst)
{
	string stringContent;
	ifstream iFStream;
	ostringstream oStringStream;
	iFStream.open(stringPath.c_str());
	StreamCopier::copyStream(iFStream, oStringStream);
	iFStream.close();
	stringContent = oStringStream.str();
	string::size_type nPos = stringContent.find(stringSrc);
	if(nPos != string::npos)
	{
		stringContent.replace(nPos, stringSrc.size(), stringDst);
	}
	ofstream oFStream;
	istringstream iStringStream(stringContent);
	oFStream.open(stringPath.c_str(), ios_base::out|ios_base::trunc);
	StreamCopier::copyStream(iStringStream, oFStream);
	oFStream.close();
	return;
}

void CUtil::ReplaceString(string& stringContent, string stringSrc, string stringDst)
{
	if(stringSrc == stringDst)
	{
		return;
	}
	string::size_type nPos = stringContent.find(stringSrc);
	while(nPos != string::npos)
	{
		stringContent.replace(nPos, stringSrc.size(), stringDst);
		nPos = stringContent.find(stringSrc);
	}
}

string CUtil::GetFileNameFromPath(string stringPath)
{
	string stringFileName = stringPath;
	if(stringPath.size() == 0)
	{
		return stringFileName;
	}
	string::size_type nPosSlash = stringPath.rfind("\\");
	if(nPosSlash == string::npos)
	{
		nPosSlash = stringPath.rfind("/");
		if(nPosSlash == string::npos)
		{
			return stringFileName;
		}
	}
	stringFileName = stringPath.substr(nPosSlash+1, stringPath.size()-nPosSlash-1);
	return stringFileName;
}

void CUtil::ParseJsonObjectWithoutArray(int nSaveType, string stringJsonContent, string stringJsonPath, map<string, string>& mapResult)
{
	string stringContent;
	if(nSaveType == 1)
	{
		//xml内容保存到了stringContent
		stringContent = stringJsonContent;
	}
	else
	{
		if(nSaveType == 2)
		{
			//xml内容保存到了stringFilePath
			ifstream iFStream;
			ostringstream oStringStream;
			iFStream.open(stringJsonPath.c_str());
			StreamCopier::copyStream(iFStream, oStringStream);
			iFStream.close();
			stringContent = oStringStream.str();
		}
		else
		{
			return;
		}
	}
	try
	{
		Json::Reader jsonReader;
		Json::Value jsonRoot;
		//处理两层json对象到map
		if(jsonReader.parse(stringContent, jsonRoot, false))
		{
			Json::Value::iterator iter = jsonRoot.begin();
			for(iter; iter!=jsonRoot.end(); iter++)
			{
				if(jsonRoot[iter.memberName()].isString())
				{
					mapResult[iter.memberName()] = jsonRoot[iter.memberName()].asString();
				}
				if(jsonRoot[iter.memberName()].isObject())
				{
					Json::Value jsonObject = jsonRoot[iter.memberName()];
					Json::Value::iterator iter2 = jsonObject.begin();
					for(iter2; iter2!=jsonObject.end(); iter2++)
					{
						if(jsonObject[iter2.memberName()].isString())
						{
							mapResult[iter2.memberName()] = jsonObject[iter2.memberName()].asString();
						}
					}
				}
			}
		}
	}
	catch(...)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog("Json Handle Exception", __FUNCTION__, __FILE__, __LINE__);
	}
	return;
}

string CUtil::MapToJson(map<string, string> mapParam)
{
	Json::Value jRoot;
	Json::FastWriter jWriter;
	map<string, string>::iterator iter = mapParam.begin();
	for(iter; iter!=mapParam.end(); iter++)
	{
		jRoot[iter->first] = iter->second;
	}
	string stringJson = jWriter.write(jRoot);
	return stringJson;
}

int CUtil::ParseStandardQueryString(string stringQuery, string& stringId, string& stringIdType)
{
	if(stringQuery.size() != 0)
	{
		string stringFirstLetter = stringQuery.substr(0, 1);
		string stringRealId = stringQuery.substr(1);
		if(stringFirstLetter=="S" || stringFirstLetter=="s")
		{
			stringIdType = "4";
		}
		else if(stringFirstLetter=="M" || stringFirstLetter=="m")
		{
			stringIdType = "1";
		}
		else if(stringFirstLetter=="Z" || stringFirstLetter=="z")
		{
			stringIdType = "2";
		}
		else if(stringFirstLetter=="T" || stringFirstLetter=="t")
		{
			stringIdType = "3";
		}
		else if(stringFirstLetter=="W" || stringFirstLetter=="w")
		{
			stringIdType = "0";
		}
		else if(stringFirstLetter=="K" || stringFirstLetter=="k")
		{
			stringIdType = "5";
		}
		else
		{
			return COMMON_PARAM_VALIDATE_FAIL;
		}
		stringId = stringRealId;
		if(stringId.size() == 0)
		{
			return COMMON_PARAM_VALIDATE_FAIL;
		}
		return PHMS_SUCCESSFUL_RESULT;
	}
	return COMMON_PARAM_VALIDATE_FAIL;
}