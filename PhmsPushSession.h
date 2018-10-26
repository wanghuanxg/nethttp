#pragma once
#include <string>
#include "HttpPushSession.h"
#include "PhmsRequest.h"
#include "NetConfig.h"

#include "Poco/AutoPtr.h"
#include "Poco/DOM/Document.h"
#include "Poco/Mutex.h"


//推送的返回说明：
//如果推送成功，调用成功回调函数
//如果推送失败，调用失败回调函数

//其中失败中的网络失败情况比较复杂。网络失败大致分类：超时、本地正常关闭或异常关闭、服务器正常关闭或异常关闭、其他
//其中，因为推送无发送和接受超时，所以超时只有连接过程中才会出现。
//以目前的程序结构来看，所有网络错误都不需要调用失败回调，只有其他逻辑错误才调用失败回调，由应用程序决定来处理。
//网络错误：不调用失败回调；如果用户未调用推动退出，线程会继续重试，再发一次请求；如果用户调用了推送退出，线程退出
//逻辑错误：调用失败回调，通知用户错误码
using namespace std;
using Poco::XML::Document;
using Poco::AutoPtr;
using Poco::Mutex;
class CPhmsPushSession
{
public:
	CPhmsPushSession(void);
	~CPhmsPushSession(void);

	void abort();
	string GetSessionId();
	void SetSessionId(string stringSessionId);
	void SetPushCallback(pHttpPushCallback pCallback);
	int PushMsg(string stringSenderId, string stringCaseId, string stringCaseType, string stringMsgType, string stringMsgDirection, string stringVersion);
private:
	int ExecutePhmsPushSession(CPhmsRequest& phmsRequest);
	int GetNetConfig(string stringFilePath, CNetConfig& netConfig);
	int GetRequestXml(AutoPtr<Document> pDoc, string& stringXmlContent);
private:
	CHttpPushSession* m_pHttpPushSession;
	bool m_bSsl;
	string m_stringSessionId;
	pHttpPushCallback m_pCallback;
	Mutex m_mutexAbortAndSslSwitch;
};
