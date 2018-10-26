#include "PhmsResponseHead.h"

CPhmsResponseHead::CPhmsResponseHead(void)
{
}

CPhmsResponseHead::CPhmsResponseHead(string stringPhmsResponseHead)
{
	this->m_stringSign = stringPhmsResponseHead.substr(0, SIGN_LENGTH);
	this->m_stringVersion = stringPhmsResponseHead.substr(SIGN_LENGTH, VERSION_LENGTH);
	this->m_stringReturnCode = stringPhmsResponseHead.substr(SIGN_LENGTH+VERSION_LENGTH, RETURNCODE_LENGTH);
	this->m_stringMessageFormat = stringPhmsResponseHead.substr(SIGN_LENGTH+VERSION_LENGTH+RETURNCODE_LENGTH, MESSAGEFORMAT_LENGTH);
}
CPhmsResponseHead::CPhmsResponseHead(string stringSign, string stringVersion, string stringReturnCode, string stringMessageFormat)
{
	this->m_stringSign = stringSign;
	this->m_stringVersion = stringVersion;
	this->m_stringReturnCode = stringReturnCode;
	this->m_stringMessageFormat = stringMessageFormat;
}
CPhmsResponseHead::CPhmsResponseHead(CPhmsResponseHead& phmsResponseHead)
{
	this->m_stringSign = phmsResponseHead.m_stringSign;
	this->m_stringVersion = phmsResponseHead.m_stringVersion;
	this->m_stringReturnCode = phmsResponseHead.m_stringReturnCode;
	this->m_stringMessageFormat = phmsResponseHead.m_stringMessageFormat;
}
CPhmsResponseHead& CPhmsResponseHead::operator=(CPhmsResponseHead& phmsResponseHead)
{
	this->m_stringSign = phmsResponseHead.m_stringSign;
	this->m_stringVersion = phmsResponseHead.m_stringVersion;
	this->m_stringReturnCode = phmsResponseHead.m_stringReturnCode;
	this->m_stringMessageFormat = phmsResponseHead.m_stringMessageFormat;
	return *this;
}

CPhmsResponseHead::~CPhmsResponseHead(void)
{
}


//Getter
string CPhmsResponseHead::GetSign()
{
	return this->m_stringSign;
}
string CPhmsResponseHead::GetVersion()
{
	return this->m_stringVersion;
}
string CPhmsResponseHead::GetReturnCode()
{
	return this->m_stringReturnCode;
}
string CPhmsResponseHead::GetMessageFormat()
{
	return this->m_stringMessageFormat;
}

//Setter
void CPhmsResponseHead::SetSign(string stringSign)
{
	this->m_stringSign = stringSign;
}
void CPhmsResponseHead::SetVersion(string stringVersion)
{
	this->m_stringVersion = stringVersion;
}
void CPhmsResponseHead::SetReturnCode(string stringReturnCode)
{
	this->m_stringReturnCode = stringReturnCode;
}
void CPhmsResponseHead::SetMessageFormat(string stringMessageFormat)
{
	this->m_stringMessageFormat = stringMessageFormat;
}


EnumPhmsResponseMessageFormat CPhmsResponseHead::GetPhmsResponseMessageFormat()
{
	if(this->m_stringMessageFormat == "10")
	{
		return NO_PHMS_RESPONSE_BODY;
	}
	if(this->m_stringMessageFormat == "11")
	{
		return XML_PHMS_RESPONSE_BODY;
	}
	if(this->m_stringMessageFormat == "12")
	{
		return FILE_OR_ECHORSTRING_PHMS_RESPONSE_BODY;
	}
	if(this->m_stringMessageFormat == "13")
	{
		return JSON_PHMS_RESPONSE_BODY;
	}
	return WRONG_PHMS_RESPONSE_MESSAGE_FORMAT;
}

EnumPhmsResponseReturnCode CPhmsResponseHead::GetPhmsResponseReturnCode()
{
	if(this->m_stringReturnCode == "100000")
	{
		return SUCCESS_RETURN_CODE;
	}
	return FAIL_RETURN_CODE;
}

int CPhmsResponseHead::GetPhmsResposeHeadTotalLength()
{
	return TOTAL_HEAD_LENGTH;
}

void CPhmsResponseHead::SetPhmsResponseHead(string stringPhmsResponseHead)
{
	this->m_stringSign = stringPhmsResponseHead.substr(0, SIGN_LENGTH);
	this->m_stringVersion = stringPhmsResponseHead.substr(SIGN_LENGTH, VERSION_LENGTH);
	this->m_stringReturnCode = stringPhmsResponseHead.substr(SIGN_LENGTH+VERSION_LENGTH, RETURNCODE_LENGTH);
	this->m_stringMessageFormat = stringPhmsResponseHead.substr(SIGN_LENGTH+VERSION_LENGTH+RETURNCODE_LENGTH, MESSAGEFORMAT_LENGTH);
}
