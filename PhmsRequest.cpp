#include "PhmsRequest.h"
#include "Poco/MD5Engine.h"
#include "Util.h"
#include "GlobalTerminalLogoString.h"

using namespace Poco;

string CPhmsRequest::m_stringUsername = "";
string CPhmsRequest::m_stringPassword = "";
CPhmsRequest::CPhmsRequest(void)
{
}

CPhmsRequest::~CPhmsRequest(void)
{
}

CPhmsRequest::CPhmsRequest(CPhmsRequest& phmsRequest)
{
	this->m_phmsRequestHead = phmsRequest.GetPhmsRequestHead();
	this->m_phmsRequestBody = phmsRequest.GetPhmsRequestBody();
	this->m_stringFilePath =  phmsRequest.GetFilePath();
}
CPhmsRequest::CPhmsRequest(CPhmsRequestHead& phmsRequestHead, CPhmsRequestBody& phmsRequestBody, string stringFilePath)
				:m_phmsRequestHead(phmsRequestHead), m_phmsRequestBody(phmsRequestBody), m_stringFilePath(stringFilePath)
{

}

CPhmsRequest& CPhmsRequest::operator=(CPhmsRequest& phmsRequest)
{
	this->m_phmsRequestHead = phmsRequest.GetPhmsRequestHead();
	this->m_phmsRequestBody = phmsRequest.GetPhmsRequestBody();
	this->m_stringFilePath =  phmsRequest.GetFilePath();
	return *this;
}


CPhmsRequestHead& CPhmsRequest::GetPhmsRequestHead()
{
	return this->m_phmsRequestHead;
}
CPhmsRequestBody& CPhmsRequest::GetPhmsRequestBody()
{
	return this->m_phmsRequestBody;
}
string CPhmsRequest::GetFilePath()
{
	return this->m_stringFilePath;
}
string CPhmsRequest::GetUsername()
{
	return CPhmsRequest::m_stringUsername;
}
string CPhmsRequest::GetPassword()
{
	return CPhmsRequest::m_stringPassword;
}


//Setter
void CPhmsRequest::SetPhmsRequestHead(CPhmsRequestHead& phmsRequestHead)
{
	m_phmsRequestHead = phmsRequestHead;
}
void CPhmsRequest::SetPhmsRequestBody(CPhmsRequestBody& phmsRequestBody)
{
	m_phmsRequestBody = phmsRequestBody;
}
void CPhmsRequest::SetFilePath(string stringFilePath)
{
	m_stringFilePath = stringFilePath;
}
void CPhmsRequest::SetUsername(string stringUsername)
{
	CPhmsRequest::m_stringUsername = stringUsername;
}
void CPhmsRequest::SetPassword(string stringPassword)
{
	CPhmsRequest::m_stringPassword = stringPassword;
}

//用户名和密码具体保存到哪里以后再说
string CPhmsRequest::GenerateAndSetSign()
{
	string username;
	string password;
	//获得用户名和密码
	{
		username = CPhmsRequest::GetUsername();
		password = CPhmsRequest::GetPassword();
		if(username.size()==0 || password.size()==0)
		{
			username = g_stringUsername;
			password = g_stringPassword;
		}
	}
	
	string md5_1;
	string stringSrc1 = username+password;
	md5_1 = CUtil::GetMd5(stringSrc1.c_str(), stringSrc1.size());
	string stringSrc2;
	stringSrc2 = md5_1 + m_phmsRequestHead.GetVersion() + m_phmsRequestHead.GetOperationCode() + m_phmsRequestHead.GetSessionId() + 
		m_phmsRequestHead.GetMessageFormat() + m_phmsRequestBody.GetXmlContent();
	
	string md5_2 ;
	md5_2 = CUtil::GetMd5(stringSrc2.c_str(), stringSrc2.size());
	m_phmsRequestHead.SetSign(md5_2);

// 	cout<<username<<":"<<password<<endl;
// 	cout<<m_phmsRequestHead.GetSessionId()<<endl;
// 	cout<<m_phmsRequestHead.GetVersion()<<endl;
// 	cout<<m_phmsRequestHead.GetOperationCode()<<endl;
// 	cout<<m_phmsRequestHead.GetMessageFormat()<<endl;
// 	cout<<m_phmsRequestBody.GetXmlContent()<<endl;
// 	cout<<"md5_1:"<<md5_1<<endl;
// 	cout<<"md5_2:"<<md5_2<<endl;
	return md5_2;
}


bool CPhmsRequest::ValidataPhmsRequest()
{
	if(!m_phmsRequestHead.ValidatePhmsRequestHead())
	{
		return false;
	}
	if(m_phmsRequestHead.GetPhmsRequestMessageFormat()==WRONG_PHMS_REQUEST_MESSAGE_FORMAT)
	{
		return false;
	}
	if(m_phmsRequestHead.GetPhmsRequestMessageFormat()==HAVE_PHMS_REQUEST_BODY && m_phmsRequestBody.GetXmlContent().size()==0)
	{
		return false;
	}
	if(m_phmsRequestHead.GetPhmsRequestMessageFormat()==NO_PHMS_REQUEST_BODY && m_phmsRequestBody.GetXmlContent().size()!=0)
	{
		return false;
	}
	return true;
}


string CPhmsRequest::ToString()
{
	return m_phmsRequestHead.ToString()+m_phmsRequestBody.ToString();
}