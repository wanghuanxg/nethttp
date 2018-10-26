#pragma once
#include "PhmsRequestBody.h"
#include <string>
using namespace std;


enum EnumPhmsRequestMessageFormat{
		WRONG_PHMS_REQUEST_MESSAGE_FORMAT=-1,
		NO_PHMS_REQUEST_BODY,
		HAVE_PHMS_REQUEST_BODY
	};

class CPhmsRequestHead
{
public:
	CPhmsRequestHead(void);
	CPhmsRequestHead(string version, string operationCode, string sessionId, string messageFormat);
	CPhmsRequestHead(CPhmsRequestHead& phmsRequestHead);
	CPhmsRequestHead& operator=(CPhmsRequestHead& phmsRequestHead);
	~CPhmsRequestHead(void);
	
	//Getter
	string GetSign();
	string GetVersion();
	string GetOperationCode();
	string GetSessionId();
	string GetMessageFormat();
	//Setter
	void SetSign(string sign);
	void SetVersion(string version);
	void SetOperationCode(string operationCode);
	void SetSessionId(string sessionId);
	void SetMessageFormat(string messageFormat);

	string ToString();
	static int GetHeadLength();
	bool ValidatePhmsRequestHead();
	bool HavePhmsBody();
	EnumPhmsRequestMessageFormat GetPhmsRequestMessageFormat();
private:
	string m_sign;
	string m_version;
	string m_operationCode;
	string m_sessionId;
	string m_messageFormat;
	//EnumHeadStringLength enumHeadStringLength;
	enum EnumPhmsRequestHeadStringLength
	{
		SIGN_LENGTH=32,
		VERSION_LENGTH=2,
		OPERATION_LENGTH=4,
		SESSIONID_LENGTH=32,
		MESSAGEFORMAT_LENGTH=2,
		TOTAL_HEAD_LENGTH=72
	};

};
