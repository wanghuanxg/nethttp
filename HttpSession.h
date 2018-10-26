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
	//�ýӿ����ڷ�PhmsЭ���Http��������ֱ������ĳ��Դ������httpֱ������xml���������ļ��������ļ�����ͨ��php
	int ExecuteNoPhmsHttpSession(CNetConfig& netConfig, istream* & pInputStream, int bContinue, string stringLocalFilePath="");
	//�ýӿ����ڷ�PhmsЭ���Http���󣬱���ʹ��Post�ύForm����ͨ��php(��ʱ��֧�ָ����ļ��ı����Ժ��������������)
	int	ExecuteFormHttpSession(CNetConfig& netConfig, map<string, string> mapParam, istream* & pInputStream);
	//�ýӿ����ڷ�PhmsЭ���Http���󣬱���ʹ��Post�ύForm������ѡ�������߲������ļ�,ͨ��php�Ƚű�
	int	ExecuteFormHttpSession(CNetConfig& netConfig, map<string, string> mapParam, string stringLocalFilePath, istream* & pInputStream);
	//�ýӿ����ڷ�PhmsЭ���Http���󣬱����ύxml�����ڶ���ҽѶ�ӿڻ������Ľ�Ľӿ�
	int ExecuteXmlHttpSession(CNetConfig& netConfig, string stringXmlContent, istream* & pInputStream);
	//�ýӿ����ڷ�PhmsЭ���Http��������ʹ��GET��ʽ�ύ����
	int ExeceteGetHttpSession(CNetConfig& netConfig, map<string, string> mapParam, istream* & pInputStream);
	//�ýӿ�����ͨ��POST���󣬲����о���Content-Type��HttpBody�Լ�URL Param
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
