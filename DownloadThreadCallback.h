#pragma once
#include "Poco/Runnable.h"
#include "InternalNetIntetface.h"

using Poco::Runnable;


class CDownloadThreadCallback :
	public Runnable
{
public:
	CDownloadThreadCallback(void);
	CDownloadThreadCallback(string, string, unsigned long, unsigned long, CInternalNetIntetface*);
	~CDownloadThreadCallback(void);
public:
	void SetUri(string);
	void SetLocalFile(string);
	void SetStart(unsigned long);
	void SetEnd(unsigned long);
	void SetNet(CInternalNetIntetface*);

	unsigned long GetStart();
	unsigned long GetEnd();
protected:
	void run();
private:
	string m_stringUri;
	string m_stringLocalFile;
	unsigned long m_nStart;
	unsigned long m_nEnd;
	CInternalNetIntetface* m_pNet;
};
