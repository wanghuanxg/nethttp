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

	//���ڱ��浱ǰ���ߵ��û���������
	//���ƣ�ͬһʱ��һ��Ӧ�ó���ֻ����һ�������˻�
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
