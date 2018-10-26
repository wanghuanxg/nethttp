#pragma once
#include <string>
#include "HttpPushSession.h"
#include "PhmsRequest.h"
#include "NetConfig.h"

#include "Poco/AutoPtr.h"
#include "Poco/DOM/Document.h"
#include "Poco/Mutex.h"


//���͵ķ���˵����
//������ͳɹ������óɹ��ص�����
//�������ʧ�ܣ�����ʧ�ܻص�����

//����ʧ���е�����ʧ������Ƚϸ��ӡ�����ʧ�ܴ��·��ࣺ��ʱ�����������رջ��쳣�رա������������رջ��쳣�رա�����
//���У���Ϊ�����޷��ͺͽ��ܳ�ʱ�����Գ�ʱֻ�����ӹ����вŻ���֡�
//��Ŀǰ�ĳ���ṹ����������������󶼲���Ҫ����ʧ�ܻص���ֻ�������߼�����ŵ���ʧ�ܻص�����Ӧ�ó������������
//������󣺲�����ʧ�ܻص�������û�δ�����ƶ��˳����̻߳�������ԣ��ٷ�һ����������û������������˳����߳��˳�
//�߼����󣺵���ʧ�ܻص���֪ͨ�û�������
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
