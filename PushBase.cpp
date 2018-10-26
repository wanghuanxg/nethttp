#include "PushBase.h"
#include "Poco/Thread.h"

CPushBase::CPushBase(pHttpPushCallback pCallback, pHttpPushFailCallback pFailCallback, int nPushType):m_pHttpPushCallback(pCallback), m_pHttpPushFailCallback(pFailCallback), m_nPushType(nPushType), m_bStopRetry(false), m_pPhmsPushSession(NULL)
{
	m_pPhmsPushSession = new CPhmsPushSession;//¿ÉÄÜÅ×Òì³£
	m_threadPush.start(*this);
}

CPushBase::~CPushBase(void)
{
}

void CPushBase::run()
{
	Poco::Thread::sleep(3000*2);
	ExecutePushSession();
	return;
}

void CPushBase::EndPush()
{
	m_bStopRetry = true;
	m_pPhmsPushSession->abort();
	m_threadPush.join();
	delete m_pPhmsPushSession;
	m_pPhmsPushSession = NULL;
}

string CPushBase::GetSessionId()
{
	return m_pPhmsPushSession->GetSessionId();
}

void CPushBase::SetSessionId(string stringSessionId)
{
	return m_pPhmsPushSession->SetSessionId(stringSessionId);
}

int CPushBase::GetPushType()
{
	return m_nPushType;
}
