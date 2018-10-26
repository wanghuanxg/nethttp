#pragma once
#include "PhmsRequestHead.h"
#include "PhmsRequestBody.h"

class CPhmsRequest
{
public:
	CPhmsRequest(void);
	CPhmsRequest(CPhmsRequest& phmsRequest);
	CPhmsRequest(CPhmsRequestHead& phmsRequestHead, CPhmsRequestBody& phmsRequestBody, string stringFilePath="");
	CPhmsRequest& operator=(CPhmsRequest& phmsRequest);
	~CPhmsRequest(void);
private:
	CPhmsRequestHead m_phmsRequestHead;
	CPhmsRequestBody m_phmsRequestBody;
	string m_stringFilePath;

	//用于保存当前在线的用户名和密码
	//限制：同一时刻一个应用程序只能有一个在线账户
	static string m_stringUsername;
	static string m_stringPassword;
public:
	//Getter
	CPhmsRequestHead& GetPhmsRequestHead();
	CPhmsRequestBody& GetPhmsRequestBody();
	string GetFilePath();
	static string GetUsername();
	static string GetPassword();

	//Setter
	void SetPhmsRequestHead(CPhmsRequestHead& phmsRequestHead);
	void SetPhmsRequestBody(CPhmsRequestBody& phmsRequestBody);
	void SetFilePath(string stringFilePath);
	static void SetUsername(string stringUsername);
	static void SetPassword(string stringPassword);

	string GenerateAndSetSign();
	bool ValidataPhmsRequest();
	string ToString();
};
