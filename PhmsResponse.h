#pragma once
#include <istream>
#include <string>
#include "PhmsResponseHead.h"
#include "PhmsResponseBody.h"


using namespace std;

class CPhmsResponse
{
public:
	CPhmsResponse();
	CPhmsResponse(CPhmsResponse& phmsResponse);
	CPhmsResponse& operator=(CPhmsResponse& phmsResponse);
	~CPhmsResponse(void);
private:
	CPhmsResponseHead m_phmsResponseHead;
	CPhmsResponseBody m_phmsResponseBody;
public:
	//Getter
	CPhmsResponseHead& GetPhmsResponseHead();
	CPhmsResponseBody& GetPhmsResponseBody();
	//Setter
	void SetPhmsResponseHead(CPhmsResponseHead phmsResponseHead);
	void SetPhmsResponseBody(CPhmsResponseBody phmsResponseBody);

	int SetPhmsResponse(istream* pInputStream, void* pPhmsSession=NULL);
	int GetResult(int nSaveType, string& stringMsgContent, string& stringFilePath, bool bFileClearBeforeWrite=true, bool bNeedConvertEncoding=true);
	bool ValidateSign(int nSaveType, string& stringMsgContent, string& stringFilePath);
private:
	int ConvertEncodingFromUtf8(int nSaveType, string& stringMsgContent, string& stringFilePath);
};
