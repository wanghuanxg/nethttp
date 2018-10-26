#include "NetConfig.h"
#include "ErrorCode.h"
#include "PhmsLogger.h"
#include "Util.h"

CNetConfig::CNetConfig(void):m_nPort(0), m_enumProxyType(NO_PROXY), m_nProxyPort(0)
{
}

CNetConfig::~CNetConfig(void)
{
}

CNetConfig::CNetConfig(bool bSsl, string stringAddr, unsigned short nPort, EnumProxyType enumProxyType, string stringProxyAddr, unsigned short nProxyPort,
					string stringProxyUsername, string stringProxyPassword, string stringUrl):m_bSsl(bSsl), m_stringAddr(stringAddr), m_nPort(nPort),
					m_enumProxyType(enumProxyType), m_stringProxyAddr(stringProxyAddr), m_nProxyPort(nProxyPort), m_stringProxyUsername(stringProxyUsername),
					m_stringProxyPassword(stringProxyPassword), m_stringUrl(stringUrl)
{
}

//Setter
void CNetConfig::SetSsl(bool bSsl)
{
	m_bSsl = bSsl;
}
void CNetConfig::SetAddr(string stringAddr)
{
	m_stringAddr = stringAddr;
}
void CNetConfig::SetHost(string stringHost)
{
	m_stringHost = stringHost;
}
void CNetConfig::SetPort(unsigned short nPort)
{
	m_nPort = nPort;
}
void CNetConfig::SetProxyType(EnumProxyType nProxyType)
{
	m_enumProxyType = nProxyType;
}
void CNetConfig::SetProxyAddr(string stringProxyAddr)
{
	m_stringProxyAddr = stringProxyAddr;
}
void CNetConfig::SetProxyPort(unsigned short nProxyPort)
{
	m_nProxyPort = nProxyPort;
}
void CNetConfig::SetProxyUsername(string stringProxyUsername)
{
	m_stringProxyUsername = stringProxyUsername;
}
void CNetConfig::SetProxyPassword(string stringProxyPassword)
{
	m_stringProxyPassword = stringProxyPassword;
}
void CNetConfig::SetUrl(string stringUrl)
{
	m_stringUrl = stringUrl;
}
void CNetConfig::SetIp(string stringIp)
{
	m_stringIp = stringIp;
}

//Getter
bool CNetConfig::GetSsl()
{
	return m_bSsl;
}
string CNetConfig::GetAddr()
{
	return m_stringAddr;
}
string CNetConfig::GetHost()
{
	return m_stringHost;
}
unsigned short CNetConfig::GetPort()
{
	return m_nPort;
}
EnumProxyType CNetConfig::GetPrpxyType()
{
	return m_enumProxyType;
}
string CNetConfig::GetProxyAddr()
{
	return m_stringProxyAddr;
}
unsigned short CNetConfig::GetProxyPort()
{
	return m_nProxyPort;
}
string CNetConfig::GetProxyUsername()
{
	return m_stringProxyUsername;
}
string CNetConfig::GetProxyPassword()
{
	return m_stringProxyPassword;
}
string CNetConfig::GetUrl()
{
	return m_stringUrl;
}
string CNetConfig::GetIp()
{
	return m_stringIp;
}

bool CNetConfig::ValidatePort()
{
	if(m_nPort>=1 && m_nPort<=65535)
	{
		if(m_enumProxyType == HTTP_PROXY || m_enumProxyType == SOCK_PROXY)
		{
			if(m_nProxyPort>1 && m_nProxyPort<=65535)
			{
				return true;
			}
			else
			{
				CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_NO_PORT), __FUNCTION__, __FILE__, __LINE__);
				return false;
			}
		}

	}
	return true;
}
bool CNetConfig::ValidAddrAndUrl()
{
	if(m_stringAddr.size() == 0)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_NO_IP), __FUNCTION__, __FILE__, __LINE__);
		return false;
	}
	if(m_stringHost.size() == 0)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_NO_IP), __FUNCTION__, __FILE__, __LINE__);
		return false;
	}
	if(m_stringUrl.size() == 0)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_NO_URL), __FUNCTION__, __FILE__, __LINE__);
		return false;
	}
	if(m_enumProxyType == HTTP_PROXY || m_enumProxyType == SOCK_PROXY)
	{
		if(m_stringProxyAddr.size() == 0)
		{
			CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_NO_IP), __FUNCTION__, __FILE__, __LINE__);
			return false;
		}
	}
	return true;
}
bool CNetConfig::ValidateProxyInfo()
{
	if(m_enumProxyType != HTTP_PROXY && m_enumProxyType != SOCK_PROXY)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_PROXY_INFO_ERROR), __FUNCTION__, __FILE__, __LINE__);
		return false;
	}
	if(m_stringProxyAddr.size() == 0)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_PROXY_INFO_ERROR), __FUNCTION__, __FILE__, __LINE__);
		return false;
	}
	if(m_nProxyPort>1 && m_nProxyPort<=65535)
	{
		return true;
	}
	CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_PROXY_INFO_ERROR), __FUNCTION__, __FILE__, __LINE__);
	return false;
}
