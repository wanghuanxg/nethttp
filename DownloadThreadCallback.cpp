#include "DownloadThreadCallback.h"
//#include "Poco/Thread.h"

CDownloadThreadCallback::CDownloadThreadCallback(void)
	:m_nStart(0), m_nEnd(0)
{
}

CDownloadThreadCallback::CDownloadThreadCallback(string stringUri, string stringLocalFile, unsigned long nStart, unsigned long nEnd, CInternalNetIntetface* pNet)
:m_stringUri(stringUri), m_stringLocalFile(stringLocalFile), m_nStart(nStart), m_nEnd(nEnd), m_pNet(pNet)
{
}

CDownloadThreadCallback::~CDownloadThreadCallback(void)
{
}

void CDownloadThreadCallback::SetUri(string stringUri)
{
	m_stringUri = stringUri;
}

void CDownloadThreadCallback::SetLocalFile(string stringLocalFile)
{
	m_stringLocalFile = stringLocalFile;
}

void CDownloadThreadCallback::SetStart(unsigned long nStart)
{
	m_nStart = nStart;
}

void CDownloadThreadCallback::SetEnd(unsigned long nEnd)
{
	m_nEnd = nEnd;
}

void CDownloadThreadCallback::SetNet(CInternalNetIntetface* pNet)
{
	m_pNet = pNet;
}

unsigned long CDownloadThreadCallback::GetStart()
{
	return m_nStart;
}

unsigned long CDownloadThreadCallback::GetEnd()
{
	return m_nEnd;
}

void CDownloadThreadCallback::run()
{
	//Poco::Thread::sleep(1000*2);
 	m_pNet->SetSaveType(2);
 	m_pNet->SetFilePath(m_stringLocalFile);
 	m_pNet->DownloadFileSegment(m_stringUri, m_nStart, m_nEnd);
}
