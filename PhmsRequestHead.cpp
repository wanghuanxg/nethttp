#include "PhmsRequestHead.h"
#include "PhmsLogger.h"



CPhmsRequestHead::CPhmsRequestHead(void)
{
}

CPhmsRequestHead::CPhmsRequestHead(string version, string operationCode, string sessionId, string messageFormat)
	:m_sign("00000000000000000000000000000000"), m_version(version),m_operationCode(operationCode),m_sessionId(sessionId),m_messageFormat(messageFormat)
{
	
}
CPhmsRequestHead::CPhmsRequestHead(CPhmsRequestHead& phmsRequestHead)
{
	m_sign = phmsRequestHead.GetSign();
	m_version = phmsRequestHead.GetVersion();
	m_sessionId = phmsRequestHead.GetSessionId();
	m_operationCode = phmsRequestHead.GetOperationCode();
	m_messageFormat = phmsRequestHead.GetMessageFormat();
}

CPhmsRequestHead& CPhmsRequestHead::operator=(CPhmsRequestHead& phmsRequestHead)
{
	m_sign = phmsRequestHead.GetSign();
	m_version = phmsRequestHead.GetVersion();
	m_sessionId = phmsRequestHead.GetSessionId();
	m_operationCode = phmsRequestHead.GetOperationCode();
	m_messageFormat = phmsRequestHead.GetMessageFormat();
	return *this;
}

CPhmsRequestHead::~CPhmsRequestHead(void)
{
}


//Getter
string CPhmsRequestHead::GetSign()
{
	return m_sign;
}
string CPhmsRequestHead::GetVersion()
{
	return m_version;
}
string CPhmsRequestHead::GetOperationCode()
{
	return m_operationCode;
}
string CPhmsRequestHead::GetSessionId()
{
	return m_sessionId;
}
string CPhmsRequestHead::GetMessageFormat()
{
	return m_messageFormat;
}
//Setter
void CPhmsRequestHead::SetSign(string sign)
{
	m_sign = sign;
}
void CPhmsRequestHead::SetVersion(string version)
{
	m_version = version;
}
void CPhmsRequestHead::SetOperationCode(string operationCode)
{
	m_operationCode = operationCode;
}
void CPhmsRequestHead::SetSessionId(string sessionId)
{
	m_sessionId = sessionId;
}
void CPhmsRequestHead::SetMessageFormat(string messageFormat)
{
	m_messageFormat = messageFormat;
}


bool CPhmsRequestHead::ValidatePhmsRequestHead()
{
	if(m_sign.size() != this->SIGN_LENGTH || m_version.size() != this->VERSION_LENGTH ||
		m_operationCode.size() != this->OPERATION_LENGTH || m_sessionId.size() != this->SESSIONID_LENGTH ||
		m_messageFormat.size() != this->MESSAGEFORMAT_LENGTH)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(m_sign, __FUNCTION__, __FILE__, __LINE__);
		CPhmsLogger::GetPhmsLogger()->WriteLog(m_version, __FUNCTION__, __FILE__, __LINE__);
		CPhmsLogger::GetPhmsLogger()->WriteLog(m_operationCode, __FUNCTION__, __FILE__, __LINE__);
		CPhmsLogger::GetPhmsLogger()->WriteLog(m_sessionId, __FUNCTION__, __FILE__, __LINE__);
		CPhmsLogger::GetPhmsLogger()->WriteLog(m_messageFormat, __FUNCTION__, __FILE__, __LINE__);
		return false;
	}
	return true;
}

string CPhmsRequestHead::ToString()
{
	string phmsHead;
	phmsHead = m_sign + m_version + m_operationCode + m_sessionId + m_messageFormat;
	return phmsHead;
}


int CPhmsRequestHead::GetHeadLength()
{
	return TOTAL_HEAD_LENGTH;
}

EnumPhmsRequestMessageFormat CPhmsRequestHead::GetPhmsRequestMessageFormat()
{
	if(m_messageFormat == "10")
	{
		return NO_PHMS_REQUEST_BODY;
	}
	if(m_messageFormat=="11" || m_messageFormat=="12" || m_messageFormat=="13")
	{
		return HAVE_PHMS_REQUEST_BODY;
	}else
	{
		return WRONG_PHMS_REQUEST_MESSAGE_FORMAT;
	}
}