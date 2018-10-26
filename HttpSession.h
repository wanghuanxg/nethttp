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

using Poco::Net::HTTPClientSession;
using Poco::Net::HTTPSClientSession;
using Poco::Net::HTTPRequest;
using Poco::Net::HTTPResponse;
using Poco::Net::HTTPMessage;
using Poco::Net::Context;


class CHttpSession
{
public:
	CHttpSession(void);
	CHttpSession(bool bSsl);
	CHttpSession(CNetConfig netConfig);
	~CHttpSession(void);
private:
	bool m_bSsl;
	HTTPClientSession* m_pSession;
	Context::Ptr m_pContext;
public:
	//Getter
	bool GetSsl();
	HTTPClientSession* GetHttpClientSession();

	int ExecuteHttpSession(CNetConfig& netConfig, CPhmsRequest& phmsRequest, istream* & pInputStream);
	//该接口用于非Phms协议的Http请求，用于直接请求某资源，比如http直接请求xml升级配置文件或其他文件，不通过php
	int ExecuteNoPhmsHttpSession(CNetConfig& netConfig, istream* & pInputStream, int bContinue, string stringLocalFilePath="");
	//该接口用于非Phms协议的Http请求，比如使用Post提交Form表单，通过php(暂时不支持附带文件的表单，以后有需求可以扩充)
	int	ExecuteFormHttpSession(CNetConfig& netConfig, map<string, string> mapParam, istream* & pInputStream);
	//该接口用于非Phms协议的Http请求，比如使用Post提交Form表单，可选附带或者不附带文件,通过php等脚本
	int	ExecuteFormHttpSession(CNetConfig& netConfig, map<string, string> mapParam, string stringLocalFilePath, istream* & pInputStream);
	//该接口用于非Phms协议的Http请求，比如提交xml，用于东亚医讯接口或用于心界的接口
	int ExecuteXmlHttpSession(CNetConfig& netConfig, string stringXmlContent, istream* & pInputStream);
	//该接口用于非Phms协议的Http请求，用于使用GET方式提交参数
	int ExeceteGetHttpSession(CNetConfig& netConfig, map<string, string> mapParam, istream* & pInputStream);
	//该接口用于通用POST请求，参数中决定Content-Type、HttpBody以及URL Param
	int ExeceteCommonPostSession(CNetConfig& netConfig, map<string, string> mapParam, string stringContentType, string stringHttpBody, istream* & pInputStream);
	
	
	int GetHttpExceptionInfo(int& nCode, string& msgName, string& msgText);
	int AbortSession();
private:
	int SetNetWork(CNetConfig& netConfig);
	int GenerateHttpRequest(CNetConfig& netConfig, CPhmsRequest& phmsRequest, string stringBoundary, HTTPRequest& httpRequest);
	int GenerateNoPhmsHttpRequest(CNetConfig& netConfig, HTTPRequest& httpRequest, int bContinue, string stringLocalFilePath="");
	int GenerateFormHttpRequest(CNetConfig& netConfig, map<string, string> mapParam, string& stringParam, HTTPRequest& httpRequest);
	int GenerateFormHttpRequest(CNetConfig& netConfig, map<string, string> mapParam, string stringLocalFilePath, string stringBoundary, HTTPRequest& httpRequest);
	int GeneratePostHttpRequest(CNetConfig& netConfig, map<string, string> mapParam, string stringContentType, string stringHttpBody, HTTPRequest& httpRequest);
	int GenerateXmlHttpRequest(CNetConfig& netConfig, string stringXmlContent, HTTPRequest& httpRequest);
	int GenerateGetHttpRequest(CNetConfig& netConfig, map<string, string> mapParam, HTTPRequest& httpRequest);
	int SendPhms(ostream& outputStream, string stringBoundary, CPhmsRequest& phmsRequest);
	int SendPostMapAndFile(ostream& outputStream, string stringBoundary, map<string, string> mapParam, string stringLocalFilePath);
	int HandleHttpResponse(HTTPResponse& httpResponse);
};
