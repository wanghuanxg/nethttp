#pragma once
#include <string>

using namespace std;


enum EnumPhmsResponseMessageFormat
{
	WRONG_PHMS_RESPONSE_MESSAGE_FORMAT=-1,
	NO_PHMS_RESPONSE_BODY,
	XML_PHMS_RESPONSE_BODY,
	FILE_OR_ECHORSTRING_PHMS_RESPONSE_BODY,
	JSON_PHMS_RESPONSE_BODY
};

enum EnumPhmsResponseReturnCode
{
	SUCCESS_RETURN_CODE=0,
	FAIL_RETURN_CODE
};

class CPhmsResponseHead
{
public:
	CPhmsResponseHead(void);
	CPhmsResponseHead(string stringPhmsResponseHead);
	CPhmsResponseHead(string stringSign, string stringVersion, string stringReturnCode, string stringMessageFormat);
	CPhmsResponseHead(CPhmsResponseHead& phmsResponseHead);
	CPhmsResponseHead& operator=(CPhmsResponseHead& phmsResponseHead);
	~CPhmsResponseHead(void);
private:
	string m_stringSign;
	string m_stringVersion;
	string m_stringReturnCode;
	string m_stringMessageFormat;
	enum EnumPhmsResponseHeadStringLength
	{
		SIGN_LENGTH=32,
		VERSION_LENGTH=2,
		RETURNCODE_LENGTH=6,
		MESSAGEFORMAT_LENGTH=2,
		TOTAL_HEAD_LENGTH=42
	};
public:
	//Getter
	string GetSign();
	string GetVersion();
	string GetReturnCode();
	string GetMessageFormat();

	//Setter
	void SetSign(string stringSign);
	void SetVersion(string stringVersion);
	void SetReturnCode(string stringReturnCode);
	void SetMessageFormat(string stringMessageFormat);

	void SetPhmsResponseHead(string stringPhmsResponseHead);


	EnumPhmsResponseMessageFormat GetPhmsResponseMessageFormat();
	EnumPhmsResponseReturnCode GetPhmsResponseReturnCode();
	static int GetPhmsResposeHeadTotalLength();
};
