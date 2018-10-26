#pragma once
#include "PhmsDataType.h"
#include "Poco/Thread.h"
#include "Poco/Runnable.h"
#include "PhmsPushSession.h"

using Poco::Thread;
using Poco::Runnable;

#define MSG_LIST_PUSH	(0)
class CPushBase:public Runnable
{
public:
	CPushBase(pHttpPushCallback pCallback, pHttpPushFailCallback pFailCallback, int nPushType);
	virtual ~CPushBase(void);
	void run();
	void EndPush();
	string GetSessionId();
	void SetSessionId(string stringSessionId);
	int GetPushType();
protected:
	virtual void ExecutePushSession()=0;
protected:
	pHttpPushCallback m_pHttpPushCallback;
	pHttpPushFailCallback m_pHttpPushFailCallback;
	CPhmsPushSession* m_pPhmsPushSession;
	bool m_bStopRetry;
private:
	Thread m_threadPush;
	int m_nPushType;
};
