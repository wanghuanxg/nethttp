#pragma once
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPSClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/Context.h"
#include <istream>
#include <ostream>

#include "PhmsRequest.h"
#include "PhmsResponse.h"
#include "NetConfig.h"
#include "PhmsDataType.h"

using Poco::Net::HTTPClientSession;
using Poco::Net::HTTPSClientSession;
using Poco::Net::HTTPRequest;
using Poco::Net::HTTPResponse;
using Poco::Net::HTTPMessage;
using Poco::Net::Context;
class CHttpPushSession
{
public:
	CHttpPushSession(void);
	CHttpPushSession(bool bSsl);
	CHttpPushSession(CNetConfig netConfig);
	~CHttpPushSession(void);
private:
	bool m_bSsl;
	HTTPClientSession* m_pSession;
	Context::Ptr m_pContext;
public:
	//Getter
	bool GetSsl();
	HTTPClientSession* GetHttpClientSession();

	int ExecuteHttpPushSession(CNetConfig& netConfig, CPhmsRequest& phmsRequest, pHttpPushCallback pCallback);
	int GetHttpExceptionInfo(int& nCode, string& msgName, string& msgText);
	int abort();
private:
	int SetNetWork(CNetConfig& netConfig);
	int GenerateHttpRequest(CNetConfig& netConfig, CPhmsRequest& phmsRequest, string stringBoundary, HTTPRequest& httpRequest);
	int SendPhms(ostream& outputStream, string stringBoundary, CPhmsRequest& phmsRequest);
	int HandleHttpResponse(HTTPResponse& httpResponse);
};
