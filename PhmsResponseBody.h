#pragma once
#include <string>
using namespace std;

class CPhmsResponseBody
{
public:
	CPhmsResponseBody(void);
	CPhmsResponseBody(istream* pInputStream, void* pPhmsSession);
	CPhmsResponseBody(CPhmsResponseBody&);
	CPhmsResponseBody& operator=(CPhmsResponseBody&);
	~CPhmsResponseBody(void);
private:
	istream* m_pInputStream;//该变量用于获得Http返回内容
	void*  m_pPhmsSession;//用于将来的长连接扩展
public:
	//Getter
	istream*  GetInputStreamPointer();
	void* GetPhmsSessionPointer();

	//Setter
	void SetInputStreamPointer(istream* pInputStream);
	void SetPhmsSessionPointer(void* pPhmsSession);

	int InputStreamToString(string& stringContent);
	int InputStreamToFile(string& stringFilePath, bool bFileClearBeforeWrite=true);
};
