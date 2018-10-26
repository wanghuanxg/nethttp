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
	istream* m_pInputStream;//�ñ������ڻ��Http��������
	void*  m_pPhmsSession;//���ڽ����ĳ�������չ
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
