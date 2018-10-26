#include "AllObjectManagerUtil.h"
#include "PhmsLogger.h"

Mutex CAllObjectManagerUtil::m_mutex;
string CAllObjectManagerUtil::m_stringPriorIp;

void CAllObjectManagerUtil::InitializeSSL()
{
	Mutex::ScopedLock lock(m_mutex);
	if(CNetThreadManager::m_map.size() == 0 && CPushObjectManager::m_map.size()==0)
	{
		Poco::Net::initializeSSL();
	}
}

void CAllObjectManagerUtil::UnInitializeSSL()
{
	Mutex::ScopedLock lock(m_mutex);
	if(CNetThreadManager::m_map.size() == 0 && CPushObjectManager::m_map.size()==0)
	{
		Poco::Net::uninitializeSSL();
	}
}

void CAllObjectManagerUtil::InitializeLocale()
{
	Mutex::ScopedLock lock(m_mutex);
	if(CNetThreadManager::m_map.size() == 0 && CPushObjectManager::m_map.size()==0)
	{
		std::locale::global(locale(locale("C"), "", locale::ctype));
	}
}

void CAllObjectManagerUtil::UnInitializeLocale()
{
	Mutex::ScopedLock lock(m_mutex);
	if(CNetThreadManager::m_map.size() == 0 && CPushObjectManager::m_map.size()==0)
	{
		std::locale::global(locale("C"));
	}
}

void CAllObjectManagerUtil::ReleasePhmsLogger()
{
	Mutex::ScopedLock lock(m_mutex);
	if(CNetThreadManager::m_map.size()==0 && CPushObjectManager::m_map.size()==0)
	{
		//释放日志对象
		CPhmsLogger::ReleasePhmsLogger();
	}
}

void CAllObjectManagerUtil::SetPriorIp(string stringIp)
{
	Mutex::ScopedLock lock(m_mutex);
	m_stringPriorIp = stringIp;
}

string CAllObjectManagerUtil::GetPriorIp()
{
	return m_stringPriorIp;
}
