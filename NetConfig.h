#pragma once

#include <string>

using namespace std;


enum EnumProxyType
{
	NO_PROXY=0,
	HTTP_PROXY,
	SOCK_PROXY
};

class CNetConfig
{
public:
	CNetConfig(void);
	CNetConfig(bool bSsl, string stringAddr, unsigned short nPort, EnumProxyType enumProxyType, string stringProxyAddr, unsigned short nProxyPort,
		string stringProxyUsername, string stringProxyPassword, string stringUrl);
	~CNetConfig(void);
private:
	bool m_bSsl;
	string m_stringAddr;
	string m_stringHost;
	unsigned short m_nPort;
	EnumProxyType m_enumProxyType;
	string m_stringProxyAddr;
	unsigned short m_nProxyPort;
	string m_stringProxyUsername;
	string m_stringProxyPassword;
	string m_stringUrl;
	string m_stringIp;
	
public:
	//Setter
	void SetSsl(bool bSsl);
	void SetAddr(string stringAddr);
	void SetHost(string stringHost);
	void SetPort(unsigned short nPort);
	void SetProxyType(EnumProxyType enumProxyType);
	void SetProxyAddr(string stringProxyAddr);
	void SetProxyPort(unsigned short nProxyPort);
	void SetProxyUsername(string stringProxyUsername);
	void SetProxyPassword(string stringProxyPassword);
	void SetUrl(string stringUrl);
	void SetIp(string stringIp);

	//Getter
	bool GetSsl();
	string GetAddr();
	string GetHost();
	unsigned short GetPort();
	EnumProxyType GetPrpxyType();
	string GetProxyAddr();
	unsigned short GetProxyPort();
	string GetProxyUsername();
	string GetProxyPassword();
	string GetUrl();
	string GetIp();

	bool ValidatePort();
	bool ValidAddrAndUrl();
	bool ValidateProxyInfo();
};
