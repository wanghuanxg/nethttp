#define PHMS_DLL_EXPORTS
#include "CommonNetInterface.h"
#include "Poco/UUIDGenerator.h"
#include "Poco/UUID.h"
#include "Poco/Exception.h"
#include "Poco/Thread.h"
#include "Util.h"
#include "PhmsLogger.h"
#include "ExternalNetInterface.h"
#include "ErrorCode.h"
#include <map>
#include "NetThreadManager.h"
#include "SessionIdManager.h"
#include "HandleMutexManager.h"
#include "PushObjectManager.h"
#include "AllObjectManagerUtil.h"
#include "GlobalTerminalLogoString.h"


using Poco::UUID;
using Poco::UUIDGenerator;
using Poco::Thread;
using namespace std;


string g_stringTerminalType;
string g_stringContecServerAddr;
string g_stringContecServerPort;
string g_stringUsername;
string g_stringPassword;

void SetContecServerInfo(const char* stringContecServerAddr, const char* stringContecServerPort)
{
	g_stringContecServerAddr = stringContecServerAddr;
	g_stringContecServerPort = stringContecServerPort;
}

void SetAppDir(const char* stringWorkDir)
{
	return CUtil::SetAppDir(stringWorkDir);
}

int SetTerminalType(const char* stringTerminalType)
{
	if(stringTerminalType == NULL)
	{
		return COMMON_PARAMETER_NULL;
	}
	g_stringTerminalType = stringTerminalType;
	return PHMS_SUCCESSFUL_RESULT;
}

NET_HANDLE InitializeNetLibObject()
{
	CAllObjectManagerUtil::InitializeSSL();
	CAllObjectManagerUtil::InitializeLocale();
	//初始化日志对象Logger
	NET_HANDLE handle = new char[128];
	memset(handle, 0, 128);
	string stringUuid;
	stringUuid = CNetThreadManager::GetHandleByThreadId(Thread::currentTid());
	if(stringUuid.size() != 0)
	{
		strcpy(handle, stringUuid.c_str());
		return handle;
	}
	try
	{
		CPhmsLogger::GetPhmsLogger();
	}
	catch(Exception& error)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(error.what(), __FUNCTION__, __FILE__, __LINE__);
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_GENERATE_LOG_FILE_ERROR), __FUNCTION__, __FILE__, __LINE__);
		delete handle;
		return NULL;
	}
	if(g_stringTerminalType.size() == 0)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_NO_SET_TERMINAL_TYPE), __FUNCTION__, __FILE__, __LINE__);
		delete handle;
		return NULL;
	}
	stringUuid = UUIDGenerator::defaultGenerator().createOne().toString();
	int nRet = PHMS_SUCCESSFUL_RESULT;
	nRet = CNetThreadManager::InsertNetThread(stringUuid);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		delete handle;
		return NULL;
	}
	nRet = CHandleMutexManager::InsertHandleMutex(stringUuid);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		delete handle;
		return NULL;
	}
	strcpy(handle, stringUuid.c_str());
	return handle;
}
int DeleteNetLibObject(NET_HANDLE handle)
{
	if(handle == NULL)
	{
		return COMMON_PARAMETER_NULL;
	}
	string stringUuid = handle;
	int nRet = PHMS_SUCCESSFUL_RESULT;
	//DeleteNetLibObject接口也不允许handle跨线程
	CExternalNetInterface* pNetInterface = NULL;
	pNetInterface = CNetThreadManager::GetNetThread(stringUuid);
	if(pNetInterface == NULL)
	{
		//写日志
		return COMMON_INVALID_HANDLE;
	}
	Thread::yield();
	Mutex* pMutex = CHandleMutexManager::GetHandleMutex(handle);
	if(pMutex == NULL)
	{
		return COMMON_INVALID_HANDLE;
	}
	Thread::yield();
	pMutex->lock();
	nRet = CNetThreadManager::DeleteNetThread(stringUuid);
	if(nRet == PHMS_SUCCESSFUL_RESULT)
	{
		CAllObjectManagerUtil::UnInitializeSSL();
		CAllObjectManagerUtil::UnInitializeLocale();
		delete handle;
	}
	pMutex->unlock();
	nRet = CHandleMutexManager::DeleteHandleMutex(stringUuid);
	CAllObjectManagerUtil::ReleasePhmsLogger();
	return nRet;
}
int SetSessionId(const char* stringSessionId)
{
	if(stringSessionId == NULL)
	{
		return COMMON_PARAMETER_NULL;
	}
	string stringTempSessionId = stringSessionId;
	int nRet = PHMS_SUCCESSFUL_RESULT;
	nRet = CSessionIdManager::SetSessionId(stringTempSessionId);
	return nRet;
}

int GetSessionId(char* bufSessionId, unsigned int nBufMaxLen)
{
	if(bufSessionId == NULL)
	{
		return COMMON_PARAMETER_NULL;
	}
	string stringSessionId;
	int nRet = PHMS_SUCCESSFUL_RESULT;
	nRet = CSessionIdManager::GetSessionId(stringSessionId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		return nRet;
	}
	if(stringSessionId.size() <= nBufMaxLen)
	{
		strcpy(bufSessionId, stringSessionId.c_str());
	}
	else
	{
		return COMMON_BUF_SMALL;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int SetUsernameAndPassword(const char* stringUsername, const char* stringPassword)
{
	if(stringUsername==NULL || stringPassword==NULL)
	{
		return COMMON_PARAMETER_NULL;
	}
	g_stringUsername = stringUsername;
	g_stringPassword = stringPassword;
	return PHMS_SUCCESSFUL_RESULT;
}

int KeepSession(NET_HANDLE handle)
{
	if(handle == NULL)
	{
		return COMMON_PARAMETER_NULL;
	}
	string stringUuid = handle;
	CExternalNetInterface* pNetInterface = NULL;
	pNetInterface = CNetThreadManager::GetNetThread(stringUuid);
	if(pNetInterface == NULL)
	{
		//写日志
		return COMMON_INVALID_HANDLE;
	}
	return pNetInterface->KeepSession();
}

int Logout(NET_HANDLE handle)
{
	if(handle == NULL)
	{
		return COMMON_PARAMETER_NULL;
	}
	string stringUuid = handle;
	CExternalNetInterface* pNetInterface = NULL;
	pNetInterface = CNetThreadManager::GetNetThread(stringUuid);
	if(pNetInterface == NULL)
	{
		//写日志
		return COMMON_INVALID_HANDLE;
	}
	return pNetInterface->Logout();
}
int UploadIm(NET_HANDLE handle, const char* stringSenderId, const char* stringSenderName, const char* stringReceiverId, const char* stringMsgDirection, const char* stringMsgType, 
			 const char* stringMsgContent, const char* stringAudioFilePath, const char* stringCaseId, const char* stringCaseType)
{
	if(handle==NULL || stringSenderId==NULL || stringSenderName==NULL || stringReceiverId==NULL || stringMsgDirection==NULL || stringMsgType==NULL || 
		stringMsgContent==NULL || stringAudioFilePath==NULL || stringCaseId==NULL || stringCaseType==NULL)
	{
		return COMMON_PARAMETER_NULL;
	}
	string stringUuid = handle;
	CExternalNetInterface* pNetInterface = NULL;
	pNetInterface = CNetThreadManager::GetNetThread(stringUuid);
	if(pNetInterface == NULL)
	{
		//写日志
		return COMMON_INVALID_HANDLE;
	}
	return pNetInterface->UploadIm(stringSenderId, stringSenderName, stringReceiverId, stringMsgDirection, stringMsgType, stringMsgContent, stringAudioFilePath, stringCaseId, stringCaseType);
}
int GetImList(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringSenderId, const char* stringCaseId, const char* stringCaseType, const char* stringMsgDirection, const char* stringMsgType)
{
	if(handle==NULL || stringLocalFilePath==NULL || stringSenderId==NULL || stringCaseId==NULL || stringCaseType==NULL || stringMsgDirection==NULL || stringMsgType==NULL)
	{
		return COMMON_PARAMETER_NULL;
	}
	string stringUuid = handle;
	CExternalNetInterface* pNetInterface = NULL;
	pNetInterface = CNetThreadManager::GetNetThread(stringUuid);
	if(pNetInterface == NULL)
	{
		//写日志
		return COMMON_INVALID_HANDLE;
	}
	return pNetInterface->GetImList(stringLocalFilePath, stringSenderId, stringCaseId, stringCaseType, stringMsgDirection, stringMsgType);
}


int GetImList2(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringNextId)
{
	if(handle==NULL || stringLocalFilePath==NULL || stringNextId==NULL)
	{
		return COMMON_PARAMETER_NULL;
	}
	string stringUuid = handle;
	CExternalNetInterface* pNetInterface = NULL;
	pNetInterface = CNetThreadManager::GetNetThread(stringUuid);
	if(pNetInterface == NULL)
	{
		//写日志
		return COMMON_INVALID_HANDLE;
	}
	return pNetInterface->GetImList2(stringLocalFilePath, stringNextId);
}

int DownloadIm(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringMsgId, const char* stringUri, unsigned long nFileSize)
{
	if(handle==NULL || stringLocalFilePath==NULL || stringMsgId==NULL || stringUri==NULL)
	{
		return COMMON_PARAMETER_NULL;
	}
	string stringUuid = handle;
	CExternalNetInterface* pNetInterface = NULL;
	pNetInterface = CNetThreadManager::GetNetThread(stringUuid);
	if(pNetInterface == NULL)
	{
		//写日志
		return COMMON_INVALID_HANDLE;
	}
	return pNetInterface->DownloadIm(stringLocalFilePath, stringMsgId, stringUri, nFileSize);
}
int DownloadImComplete(NET_HANDLE handle, const char* stringMsgId)
{
	if(handle==NULL || stringMsgId==NULL)
	{
		return COMMON_PARAMETER_NULL;
	}
	string stringUuid = handle;
	CExternalNetInterface* pNetInterface = NULL;
	pNetInterface = CNetThreadManager::GetNetThread(stringUuid);
	if(pNetInterface == NULL)
	{
		//写日志
		return COMMON_INVALID_HANDLE;
	}
	return pNetInterface->DownloadImComplete(stringMsgId);
}
int ChangePwd(NET_HANDLE handle, const char* stringOldPwd, const char* stringNewPwd)
{
	if(handle==NULL || stringOldPwd==NULL || stringNewPwd==NULL)
	{
		return COMMON_PARAMETER_NULL;
	}
	string stringUuid = handle;
	CExternalNetInterface* pNetInterface = NULL;
	pNetInterface = CNetThreadManager::GetNetThread(stringUuid);
	if(pNetInterface == NULL)
	{
		//写日志
		return COMMON_INVALID_HANDLE;
	}
	return pNetInterface->ChangePwd(stringOldPwd, stringNewPwd);
}
int Feedback(NET_HANDLE handle, const char* stringTitle, const char* stringNotes, const char* stringPhone, const char* stringPicPath)
{
	if(handle==NULL || stringTitle==NULL || stringNotes==NULL || stringPhone==NULL || stringPicPath==NULL)
	{
		return COMMON_PARAMETER_NULL;
	}
	string stringUuid = handle;
	CExternalNetInterface* pNetInterface = NULL;
	pNetInterface = CNetThreadManager::GetNetThread(stringUuid);
	if(pNetInterface == NULL)
	{
		//写日志
		return COMMON_INVALID_HANDLE;
	}
	return pNetInterface->Feedback(stringTitle, stringNotes, stringPhone, stringPicPath);
}
int GetReportListOfCase(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringCaseId)
{
	if(handle==NULL || stringLocalFilePath==NULL || stringCaseId==NULL)
	{
		return COMMON_PARAMETER_NULL;
	}
	string stringUuid = handle;
	CExternalNetInterface* pNetInterface = NULL;
	pNetInterface = CNetThreadManager::GetNetThread(stringUuid);
	if(pNetInterface == NULL)
	{
		//写日志
		return COMMON_INVALID_HANDLE;
	}
	return pNetInterface->GetReportListOfCase(stringLocalFilePath, stringCaseId);
}
int GetHospitalListOfGroup(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringGroupId, const char* stringType, const char* stringCaseType, unsigned int nPageTo, unsigned int nPageSize)
{
	if(handle==NULL || stringLocalFilePath==NULL || stringGroupId==NULL || stringType==NULL)
	{
		return COMMON_PARAMETER_NULL;
	}
	string stringUuid = handle;
	CExternalNetInterface* pNetInterface = NULL;
	pNetInterface = CNetThreadManager::GetNetThread(stringUuid);
	if(pNetInterface == NULL)
	{
		//写日志
		return COMMON_INVALID_HANDLE;
	}
	return pNetInterface->GetHospitalListOfGroup(stringLocalFilePath, stringGroupId, stringType, stringCaseType, nPageTo, nPageSize);
}
int GetDoctorListOfHospital(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringHospitalId, const char* stringDoctorId, const char* stringCaseType, const char* stringDoctorType, unsigned int nPageTo, unsigned int nPageSize)
{
	if(handle==NULL || stringLocalFilePath==NULL || stringHospitalId==NULL || stringDoctorId==NULL)
	{
		return COMMON_PARAMETER_NULL;
	}
	string stringUuid = handle;
	CExternalNetInterface* pNetInterface = NULL;
	pNetInterface = CNetThreadManager::GetNetThread(stringUuid);
	if(pNetInterface == NULL)
	{
		//写日志
		return COMMON_INVALID_HANDLE;
	}
	return pNetInterface->GetDoctorListOfHospital(stringLocalFilePath, stringHospitalId, stringDoctorId, stringCaseType, stringDoctorType, nPageTo, nPageSize);
}

int AbortSession(NET_HANDLE handle)
{
	if(handle==NULL)
	{
		return COMMON_PARAMETER_NULL;
	}
	Mutex* pMutex = CHandleMutexManager::GetHandleMutex(handle);
	if(pMutex == NULL)
	{
		return COMMON_INVALID_HANDLE;
	}
	Mutex::ScopedLock lock(*pMutex);
	string stringUuid = handle;
	CExternalNetInterface* pNetInterface = NULL;
	pNetInterface = CNetThreadManager::GetNetThread(stringUuid, true);
	if(pNetInterface == NULL)
	{
		//写日志
		return COMMON_INVALID_HANDLE;
	}
	return pNetInterface->AbortSession();
}

int GetEventStream(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringCaseId, const char* stringOriginalHospital)
{
	if(handle==NULL || stringLocalFilePath==NULL || stringCaseId==NULL || stringOriginalHospital==NULL)
	{
		return COMMON_PARAMETER_NULL;
	}
	string stringUuid = handle;
	CExternalNetInterface* pNetInterface = NULL;
	pNetInterface = CNetThreadManager::GetNetThread(stringUuid);
	if(pNetInterface == NULL)
	{
		//写日志
		return COMMON_INVALID_HANDLE;
	}
	return pNetInterface->GetEventStream(stringLocalFilePath, stringCaseId, stringOriginalHospital);
}

int GetCaseTypeList(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringLanguage)
{
	if(handle==NULL || stringLocalFilePath==NULL || stringLanguage==NULL)
	{
		return COMMON_PARAMETER_NULL;
	}
	string stringUuid = handle;
	CExternalNetInterface* pNetInterface = NULL;
	pNetInterface = CNetThreadManager::GetNetThread(stringUuid);
	if(pNetInterface == NULL)
	{
		//写日志
		return COMMON_INVALID_HANDLE;
	}
	return pNetInterface->GetCaseTypeList(stringLocalFilePath, stringLanguage);
}

int DownloadPhoto(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringUri, unsigned long nFileSize, pProgressAndSpeedCallback pCallback)
{
	if(handle==NULL || stringLocalFilePath==NULL || stringUri==NULL)
	{
		return COMMON_PARAMETER_NULL;
	}
	string stringUuid = handle;
	CExternalNetInterface* pNetInterface = NULL;
	pNetInterface = CNetThreadManager::GetNetThread(stringUuid);
	if(pNetInterface == NULL)
	{
		//写日志
		return COMMON_INVALID_HANDLE;
	}
	return pNetInterface->DownloadPhoto(stringLocalFilePath, stringUri, nFileSize, pCallback);
}

int GetServerFileSize(NET_HANDLE handle, const char* stringUri, int& nFileSize)
{
	if(handle==NULL || stringUri==NULL)
	{
		return COMMON_PARAMETER_NULL;
	}
	string stringUuid = handle;
	CExternalNetInterface* pNetInterface = NULL;
	pNetInterface = CNetThreadManager::GetNetThread(stringUuid);
	if(pNetInterface == NULL)
	{
		//写日志
		return COMMON_INVALID_HANDLE;
	}
	return pNetInterface->GetAlreadyUploadFileSize(stringUri, nFileSize);
}

int SendPhoneMsg(NET_HANDLE handle, const char* stringCardId, const char* stringPhone, const char* stringMsg)
{
	if(handle==NULL || stringCardId==NULL || stringPhone==NULL || stringMsg==NULL)
	{
		return COMMON_PARAMETER_NULL;
	}
	string stringUuid = handle;
	CExternalNetInterface* pNetInterface = NULL;
	pNetInterface = CNetThreadManager::GetNetThread(stringUuid);
	if(pNetInterface == NULL)
	{
		//写日志
		return COMMON_INVALID_HANDLE;
	}
	return pNetInterface->SendPhoneMsg(stringCardId, stringPhone, stringMsg);
}

int GetFileWithHttp(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringUrl, int bContinue)
{
	if(handle==NULL || stringLocalFilePath==NULL || stringUrl==NULL)
	{
		return COMMON_PARAMETER_NULL;
	}
	string stringUuid = handle;
	CExternalNetInterface* pNetInterface = NULL;
	pNetInterface = CNetThreadManager::GetNetThread(stringUuid);
	if(pNetInterface == NULL)
	{
		//写日志
		return COMMON_INVALID_HANDLE;
	}
	return pNetInterface->GetFileWithHttp(stringLocalFilePath, stringUrl, bContinue);
}


int PostForm(NET_HANDLE handle, char* stringResponse, unsigned int nResponseSize, const char* stringHost, const char* stringUrl, const char* stringKey[], const char* stringValue[], int nSize)
{
	if(handle==NULL || stringResponse==NULL || stringHost==NULL || stringUrl==NULL || stringKey==NULL || stringValue==NULL || nSize==0 || nResponseSize==0)
	{
		return COMMON_PARAMETER_NULL;
	}
	map<string, string> mapParams;
	for(int i=0; i<nSize; i++)
	{
		mapParams[stringKey[i]] = stringValue[i];
	}
	string stringUuid = handle;
	CExternalNetInterface* pNetInterface = NULL;
	pNetInterface = CNetThreadManager::GetNetThread(stringUuid);
	if(pNetInterface == NULL)
	{
		//写日志
		return COMMON_INVALID_HANDLE;
	}
	int nRet = PHMS_SUCCESSFUL_RESULT;
	string stringTempResponse;
	nRet = pNetInterface->PostForm(stringTempResponse, stringHost, stringUrl, mapParams);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		return nRet;
	}
	if(stringTempResponse.size() != 0)
	{
		if(stringTempResponse.size() > nResponseSize)
		{
			strncpy(stringResponse, stringTempResponse.c_str(), nResponseSize);
		}
		else
		{
			strcpy(stringResponse, stringTempResponse.c_str());
		}
	}
	return nRet;
}

int GetVerificationCode(NET_HANDLE handle, const char* stringType, const char* stringPhone)
{
	if(handle==NULL || stringType==NULL || stringPhone==NULL)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(handle, __FUNCTION__, __FILE__, __LINE__);
		CPhmsLogger::GetPhmsLogger()->WriteLog(stringType, __FUNCTION__, __FILE__, __LINE__);
		CPhmsLogger::GetPhmsLogger()->WriteLog(stringPhone, __FUNCTION__, __FILE__, __LINE__);
		return COMMON_PARAMETER_NULL;
	}
	string stringUuid = handle;
	CExternalNetInterface* pNetInterface = NULL;
	pNetInterface = CNetThreadManager::GetNetThread(stringUuid);
	if(pNetInterface == NULL)
	{
		//写日志
		return COMMON_INVALID_HANDLE;
	}
	return pNetInterface->GetVerificationCode(stringType, stringPhone);
}

int ApplyQRCode(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringType, const char* stringScene)
{
	if(handle==NULL || stringLocalFilePath==NULL || stringType==NULL || stringScene==NULL)
	{
		return COMMON_PARAMETER_NULL;
	}
	string stringUuid = handle;
	CExternalNetInterface* pNetInterface = NULL;
	pNetInterface = CNetThreadManager::GetNetThread(stringUuid);
	if(pNetInterface == NULL)
	{
		//写日志
		return COMMON_INVALID_HANDLE;
	}
	return pNetInterface->ApplyQRCode(stringLocalFilePath, stringType, stringScene);
}


int ValidateUserAccount(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringLoginId, const char* stringPassword, const char* stringRole)
{
	if(handle==NULL || stringLocalFilePath==NULL || stringLoginId==NULL || stringPassword==NULL || stringRole==NULL)
	{
		return COMMON_PARAMETER_NULL;
	}
	string stringUuid = handle;
	CExternalNetInterface* pNetInterface = NULL;
	pNetInterface = CNetThreadManager::GetNetThread(stringUuid);
	if(pNetInterface == NULL)
	{
		//写日志
		return COMMON_INVALID_HANDLE;
	}
	return pNetInterface->ValidateUserAccount(stringLocalFilePath, stringLoginId, stringPassword, stringRole);
}

int UploadCommonFile(NET_HANDLE handle, char* stringUri, const char* stringType, const char* stringExtra, const char* stringLocalFilePath, const char* stringFileType, pProgressAndSpeedCallback pCallbcak)
{
	if(handle==NULL || stringType==NULL || stringExtra==NULL || stringLocalFilePath==NULL)
	{
		return COMMON_PARAMETER_NULL;
	}
	string stringUuid = handle;
	CExternalNetInterface* pNetInterface = NULL;
	pNetInterface = CNetThreadManager::GetNetThread(stringUuid);
	if(pNetInterface == NULL)
	{
		//写日志
		return COMMON_INVALID_HANDLE;
	}
	string stringTempUri;
	int nRet = pNetInterface->UploadCommonFile(stringTempUri, stringType, stringExtra, stringLocalFilePath, stringFileType, pCallbcak);
	if(stringTempUri.size() != 0)
	{
		strcpy(stringUri, stringTempUri.c_str());
	}
	return nRet;
}

int GetListOfHealthyKnowledge(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringSenderId, const char* stringNextId, const char* stringIds, const char* stringPageSize, const char* stringOrderBy)
{
	if(handle==NULL || stringSenderId==NULL || stringNextId==NULL || stringIds==NULL || stringLocalFilePath==NULL || stringPageSize==NULL || stringOrderBy==NULL)
	{
		return COMMON_PARAMETER_NULL;
	}
	string stringUuid = handle;
	CExternalNetInterface* pNetInterface = NULL;
	pNetInterface = CNetThreadManager::GetNetThread(stringUuid);
	if(pNetInterface == NULL)
	{
		//写日志
		return COMMON_INVALID_HANDLE;
	}
	return pNetInterface->GetListOfHealthyKnowledge(stringLocalFilePath, stringSenderId, stringNextId, stringIds, stringPageSize, stringOrderBy);
}

int GetCardUserReportList(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringReportId, const char* stringReportStartDate, const char* stringReportEndDate, const char* stringCaseType, const char* stringState, const char* stringOrderBy, 
		const char* stringThirdIdType, const char* stringThirdId, const char* stringNextId, const char* stringPageSize)
{
	if(handle==NULL || stringLocalFilePath==NULL || stringReportId==NULL || stringReportStartDate==NULL || stringReportEndDate==NULL || stringCaseType==NULL || stringState==NULL || stringOrderBy==NULL ||
		stringThirdIdType==NULL || stringThirdId==NULL || stringNextId==NULL || stringPageSize==NULL)
	{
		return COMMON_PARAMETER_NULL;
	}
	string stringUuid = handle;
	CExternalNetInterface* pNetInterface = NULL;
	pNetInterface = CNetThreadManager::GetNetThread(stringUuid);
	if(pNetInterface == NULL)
	{
		//写日志
		return COMMON_INVALID_HANDLE;
	}
	return pNetInterface->GetCardUserReportList(stringLocalFilePath, stringReportId, stringReportStartDate, stringReportEndDate, stringCaseType, stringState, stringOrderBy, stringThirdIdType, stringThirdId, stringNextId, stringPageSize);
}

int MyFans(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringUserId, const char* stringNextId)
{
	if(handle==NULL || stringLocalFilePath==NULL || stringUserId==NULL || stringNextId==NULL)
	{
		return COMMON_PARAMETER_NULL;
	}
	string stringUuid = handle;
	CExternalNetInterface* pNetInterface = NULL;
	pNetInterface = CNetThreadManager::GetNetThread(stringUuid);
	if(pNetInterface == NULL)
	{
		//写日志
		return COMMON_INVALID_HANDLE;
	}
	return pNetInterface->MyFans(stringLocalFilePath, stringUserId, stringNextId);
}

int GetPayCode(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringType, const char* stringCaseId)
{
	if(handle==NULL || stringLocalFilePath==NULL || stringType==NULL || stringCaseId==NULL)
	{
		return COMMON_PARAMETER_NULL;
	}
	string stringUuid = handle;
	CExternalNetInterface* pNetInterface = NULL;
	pNetInterface = CNetThreadManager::GetNetThread(stringUuid);
	if(pNetInterface == NULL)
	{
		//写日志
		return COMMON_INVALID_HANDLE;
	}
	return pNetInterface->GetPayCode(stringLocalFilePath, stringType, stringCaseId);
}

int GetOrderInfo(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringType, const char* stringCaseId)
{
	if(handle==NULL || stringLocalFilePath==NULL || stringType==NULL || stringCaseId==NULL)
	{
		return COMMON_PARAMETER_NULL;
	}
	string stringUuid = handle;
	CExternalNetInterface* pNetInterface = NULL;
	pNetInterface = CNetThreadManager::GetNetThread(stringUuid);
	if(pNetInterface == NULL)
	{
		//写日志
		return COMMON_INVALID_HANDLE;
	}
	return pNetInterface->GetOrderInfo(stringLocalFilePath, stringType, stringCaseId);
}

int AbortSessionByThreadId(THREAD_ID threadId)
{
#if(POCO_OS == POCO_OS_MAC_OS_X)
	return PHMS_SUCCESSFUL_RESULT;
#else
	string handle = CNetThreadManager::GetHandleByThreadId(threadId);
	if(handle.size() == 0)
	{
		return COMMON_INVALID_THREAD_ID;
	}
	return AbortSession((char*)handle.c_str());
#endif
}

NET_HANDLE NET_API BeginMsgListPush(const char* stringSenderId, const char* stringCaseId, const char* stringCaseType, const char* stringMsgType, const char* stringMsgDirection, const char* stringVersion, 
									pHttpPushCallback pCallback, pHttpPushFailCallback pFailCallback)
{
	CAllObjectManagerUtil::InitializeSSL();
	CAllObjectManagerUtil::InitializeLocale();
	//初始化日志对象Logger
	string stringUuid;
	try
	{
		CPhmsLogger::GetPhmsLogger();
	}
	catch(Exception& error)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(error.what(), __FUNCTION__, __FILE__, __LINE__);
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_GENERATE_LOG_FILE_ERROR), __FUNCTION__, __FILE__, __LINE__);
		return NULL;
	}
	if(stringSenderId==NULL || stringCaseId==NULL || stringCaseType==NULL || stringMsgType==NULL ||stringMsgDirection==NULL || 
		pCallback==NULL || pFailCallback==NULL)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_PARAMETER_NULL), __FUNCTION__, __FILE__, __LINE__);
		return NULL;
	}
	if(g_stringTerminalType.size() == 0)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_NO_SET_TERMINAL_TYPE), __FUNCTION__, __FILE__, __LINE__);
		return NULL;
	}
	if(CPushObjectManager::SomeTypePushIsRun(MSG_LIST_PUSH))
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_SAME_PUSH_IS_RUN), __FUNCTION__, __FILE__, __LINE__);
		return NULL;
	}
	stringUuid = UUIDGenerator::defaultGenerator().createOne().toString();
	int nRet = PHMS_SUCCESSFUL_RESULT;
	nRet = CPushObjectManager::InsertPushMsgListObject(stringUuid, stringSenderId, stringCaseId, stringCaseType, stringMsgType, stringMsgDirection, stringVersion, pCallback, pFailCallback);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		return NULL;
	}
	NET_HANDLE handle = new char[128];
	memset(handle, 0, 128);
	strcpy(handle, stringUuid.c_str());
	return handle;
}

int NET_API EndPush(NET_HANDLE handle)
{
	if(handle == NULL)
	{
		return COMMON_PARAMETER_NULL;
	}
	string stringUuid = handle;
	int nRet = PHMS_SUCCESSFUL_RESULT;
	nRet = CPushObjectManager::DeletePushObject(stringUuid);
	if(nRet == PHMS_SUCCESSFUL_RESULT)
	{
		CAllObjectManagerUtil::UnInitializeSSL();
		CAllObjectManagerUtil::UnInitializeLocale();
		delete handle;
	}
	CAllObjectManagerUtil::ReleasePhmsLogger();
	return nRet;
}

int ExecuteGWService(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringLastPath, const char* stringJson, const char* stringUploadFilePath)
{
	if(handle==NULL || stringLocalFilePath==NULL || stringLastPath==NULL || stringJson==NULL || stringUploadFilePath==NULL)
	{
		return COMMON_PARAMETER_NULL;
	}
	string stringUuid = handle;
	CExternalNetInterface* pNetInterface = NULL;
	pNetInterface = CNetThreadManager::GetNetThread(stringUuid);
	if(pNetInterface == NULL)
	{
		//写日志
		return COMMON_INVALID_HANDLE;
	}
	return pNetInterface->ExecuteGWService(stringLocalFilePath, stringLastPath, stringJson, stringUploadFilePath);
}

int ExecuteCommonRESTful(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringIp, const char* stringPort, const char* stringUrl, const char* stringContentType, const char* stringPayload)
{
	if(handle==NULL || stringLocalFilePath==NULL || stringIp==NULL || stringPort==NULL || stringUrl==NULL || stringContentType==NULL || stringPayload==NULL)
	{
		return COMMON_PARAMETER_NULL;
	}
	string stringUuid = handle;
	CExternalNetInterface* pNetInterface = NULL;
	pNetInterface = CNetThreadManager::GetNetThread(stringUuid);
	if(pNetInterface == NULL)
	{
		//写日志
		return COMMON_INVALID_HANDLE;
	}
	return pNetInterface->ExecuteCommonRESTful(stringLocalFilePath, stringIp, stringPort, stringUrl, stringContentType, stringPayload);
}

