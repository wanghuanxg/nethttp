#define PHMS_DLL_EXPORTS
#include "DoctorNetInterface.h"
#include "ExternalNetInterface.h"
#include "ErrorCode.h"
#include <map>
#include <string>
#include "NetThreadManager.h"

using namespace std;


int DoctorLogin(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringUsername, const char* stringPassword, const char* stringLanguage, const char* stringSignPath, 
				const char* stringHospitalInfoPath, const char* stringHospitalPhotoPath, const char* stringCliniqueListPath, const char* stringHospitalListPath, const char* stringDoctorListPath, char* stringIp)
{
	if(handle==NULL || stringLocalFilePath==NULL || stringUsername==NULL || stringPassword==NULL || stringLanguage==NULL || stringSignPath==NULL || stringCliniqueListPath==NULL || 
		stringHospitalListPath==NULL || stringDoctorListPath==NULL)
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
	int nRet = PHMS_SUCCESSFUL_RESULT;
	string stringServerIp;
	nRet = pNetInterface->DoctorLogin(stringLocalFilePath, stringUsername, stringPassword, stringLanguage, stringSignPath, stringHospitalInfoPath, stringHospitalPhotoPath, stringCliniqueListPath, 
		stringHospitalListPath, stringDoctorListPath, stringServerIp);
	if(stringServerIp.size() != 0)
	{
		strcpy(stringIp, stringServerIp.c_str());
	}
	return nRet;
}
int UploadSign(NET_HANDLE handle, const char* stringSignPath)
{
	if(handle==NULL || stringSignPath==NULL)
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
	return pNetInterface->UploadSign(stringSignPath);
}
int GetUnanalyzedCaseList(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringOwnerId, const char* stringOrderBy, unsigned int nPageTo, unsigned int nPageSize)
{
	if(handle==NULL || stringLocalFilePath==NULL || stringOwnerId==NULL || stringOrderBy==NULL)
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
	return pNetInterface->GetUnanalyzedCaseList(stringLocalFilePath, stringOwnerId, stringOrderBy, nPageTo, nPageSize);
}

int SearchCaseFromServer(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringCaseId, const char* stringCaseType, const char* stringCaseSource, 
						 const char* stringCaseState, const char* stringPatientName, const char* stringSenderId, const char* stringReceiverId, const char* stringStartDate, const char* stringEndDate, 
						 const char* stringCheckStartDate, const char* stringCheckEndDate, const char* stringSenderName, const char* stringDiagnostic, const char* stringCaseOrigin, const char* stringYzlb, const char* stringAId, 
						 const char* stringSex, const char* stringOrderBy, const char* stringThirdId, const char* stringLang, const char* stringIsFullName, unsigned int nPageTo, unsigned int nPageSize)
{
	if(handle==NULL || stringLocalFilePath==NULL || stringCaseId==NULL || stringCaseType==NULL ||stringCaseSource==NULL || stringCaseState==NULL ||
		stringPatientName==NULL || stringSenderId==NULL || stringReceiverId==NULL || stringStartDate==NULL || stringEndDate==NULL || stringCheckStartDate==NULL || stringCheckEndDate==NULL || 
		stringSenderName==NULL || stringDiagnostic==NULL || stringCaseOrigin==NULL || stringYzlb==NULL || stringAId==NULL || stringSex==NULL || stringOrderBy==NULL || stringThirdId==NULL || stringLang==NULL)
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
	return pNetInterface->SearchCaseFromServer(stringLocalFilePath, stringCaseId, stringCaseType, stringCaseSource, stringCaseState, stringPatientName, stringSenderId, 
		stringReceiverId, stringStartDate, stringEndDate, stringCheckStartDate, stringCheckEndDate, stringSenderName, stringDiagnostic, stringCaseOrigin, stringYzlb, stringAId, stringSex, 
		stringOrderBy, stringThirdId, stringLang, stringIsFullName, nPageTo, nPageSize);
}
int GetAutoCaseInfo(NET_HANDLE handle, const char* stringLocalFilePath, const char*  stringCaseType,const char*  stringSenderId,const char*  stringCaseOrigin)
{
	if(handle==NULL || stringLocalFilePath==NULL|| stringCaseType==NULL|| stringSenderId==NULL|| stringCaseOrigin==NULL)
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
	return pNetInterface->GetAutoCaseInfo(stringLocalFilePath,  stringCaseType, stringSenderId, stringCaseOrigin);
}
// int AutoDownloadCase(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringCaseId, const char* stringReceiverId, const char* stringUri, unsigned long nFileSize, 
// 					 pProgressAndSpeedCallback pCallback)
// {
// 	if(handle==NULL || stringLocalFilePath==NULL || stringCaseId==NULL || stringReceiverId==NULL || stringUri==NULL)
// 	{
// 		return COMMON_PARAMETER_NULL;
// 	}
// 	string stringUuid = handle;
// 	CExternalNetInterface* pNetInterface = NULL;
// 	pNetInterface = CNetThreadManager::GetNetThread(stringUuid);
// 	if(pNetInterface == NULL)
// 	{
// 		//写日志
// 		return COMMON_INVALID_HANDLE;
// 	}
// 	return pNetInterface->AutoDownloadCase(stringLocalFilePath, stringCaseId, stringReceiverId, stringUri, nFileSize, pCallback);
// }
int ManualDownloadCase(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringCaseId, const char* stringHospitalId, const char* stringReceiverId, const char* stringUri, unsigned long nFileSize, 
					   pProgressAndSpeedCallback pCallback, int nBDownloadNotSelfOwn)
{
	if(handle==NULL || stringLocalFilePath==NULL || stringCaseId==NULL || stringHospitalId==NULL || stringReceiverId==NULL || stringUri==NULL)
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
	bool bDownloadNotSelfOwn = false;
	if(nBDownloadNotSelfOwn == 1)
	{
		bDownloadNotSelfOwn = true;
	}
	return pNetInterface->ManualDownloadCase(stringLocalFilePath, stringCaseId, stringHospitalId, stringReceiverId, stringUri, nFileSize, pCallback, bDownloadNotSelfOwn);
}
int RollbackCase(NET_HANDLE handle, const char* stringCaseId, const char* stringReceiverId, const char* stringBackTo)
{
	if(handle==NULL || stringCaseId==NULL || stringReceiverId==NULL || stringBackTo==NULL)
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
	return pNetInterface->RollbackCase(stringCaseId, stringReceiverId, stringBackTo);
}
int TransmitCase(NET_HANDLE handle, const char* stringCaseId, const char* stringReceiverId, const char* stringForwardToH, const char* stringForwardToHospitalName,
				 const char* stringForwardToD, const char* stringForwardToDoctorName)
{
	if(handle==NULL || stringCaseId==NULL || stringReceiverId==NULL || stringForwardToH==NULL || stringForwardToHospitalName==NULL || 
		stringForwardToD==NULL || stringForwardToDoctorName==NULL)
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
	return pNetInterface->TransmitCase(stringCaseId, stringReceiverId, stringForwardToH, stringForwardToHospitalName, stringForwardToD, stringForwardToDoctorName);
}
int InvalidCase(NET_HANDLE handle, const char* stringCaseId, const char* stringReceiverId, const char* stringReason)
{
	if(handle==NULL || stringCaseId==NULL || stringReceiverId==NULL || stringReason==NULL)
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
	return pNetInterface->InvalidCase(stringCaseId, stringReceiverId, stringReason);
}
int AnalyzeBegin(NET_HANDLE handle, const char* stringCaseId, const char* stringReceiverId, const char* stringCaseType)
{
	if(handle==NULL || stringCaseId==NULL || stringReceiverId==NULL || stringCaseType==NULL)
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
	return pNetInterface->AnalyzeBegin(stringCaseId, stringReceiverId, stringCaseType);
}
int AnalyzeComplete(NET_HANDLE handle, const char* stringCaseId, const char* stringReceiverId, const char* stringCaseType)
{
	if(handle==NULL || stringCaseId==NULL || stringReceiverId==NULL || stringCaseType==NULL)
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
	return pNetInterface->AnalyzeComplete(stringCaseId, stringReceiverId, stringCaseType);
}
int UploadReport(NET_HANDLE handle, char* stringReportId, const char* stringCaseId, const char* stringReceiverId, const char* stringDiagnosis, const char* stringDigitalSign, const char* stringTimestampSign, const char* stringCaseType, const char* stringLocalReportPath, int nResultFlag, 
				 const char* stringFileType, pProgressAndSpeedCallback pCallback)
{
	if(handle==NULL || stringReportId==NULL || stringCaseId==NULL || stringReceiverId==NULL || stringDiagnosis==NULL || stringLocalReportPath==NULL || stringDigitalSign==NULL || stringTimestampSign==NULL || stringCaseType==NULL || stringFileType==NULL)
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
	string stringTempReportId = stringReportId;
	int nRet = PHMS_SUCCESSFUL_RESULT;
	nRet = pNetInterface->UploadReport(stringTempReportId, stringCaseId, stringReceiverId, stringDiagnosis, stringDigitalSign, stringTimestampSign, stringCaseType, stringLocalReportPath, nResultFlag, stringFileType, pCallback);
	if(stringTempReportId.size() != 0)
	{
		strcpy(stringReportId, stringTempReportId.c_str());
	}
	return nRet;
}
int DoctorDownloadReport(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringUri, unsigned long nFileSize, pProgressAndSpeedCallback pCallback)
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
	return pNetInterface->DoctorDownloadReport(stringLocalFilePath, stringUri, nFileSize, pCallback);
}
int UploadAdvice(NET_HANDLE handle, const char* stringUserId, const char* stringUsername, const char* stringMsg, const char* stringType, const char* stringIsSendMsg)
{
	if(handle==NULL || stringUserId==NULL || stringUsername==NULL || stringMsg==NULL || stringType==NULL || stringIsSendMsg==NULL)
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
	return pNetInterface->UploadAdvice(stringUserId, stringUsername, stringMsg, stringType, stringIsSendMsg);
}
int DoctorChangeInfo(NET_HANDLE handle, const char* stringName, const char* stringSex, const char* stringTel, const char* stringBirthyead, const char* stringPosition, const char* stringNotes)
{
	if(handle==NULL || stringName==NULL || stringSex==NULL || stringTel==NULL || stringBirthyead==NULL || stringPosition==NULL || stringNotes==NULL)
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
	return pNetInterface->DoctorChangeInfo(stringName, stringSex, stringTel, stringBirthyead, stringPosition, stringNotes);
}

int DoctorGetSelfInfo(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringSenderId)
{
	if(handle==NULL || stringLocalFilePath==NULL|| stringSenderId==NULL)
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
	return pNetInterface->DoctorGetSelfInfo(stringLocalFilePath, stringSenderId);
}

int UploadPhoto(NET_HANDLE handle, const char* stringPhotoPath)
{
	if(handle==NULL || stringPhotoPath==NULL)
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
	return pNetInterface->UploadPhoto(stringPhotoPath);
}

int GetReviewReportList(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringReportStartDate, const char* stringReportEndDate,const char* stringReviewStartDate,
						const char* stringReviewEndDate, const char* stringReviewState, const char* stringCaseType, const char* stringPtName, const char* stringSenderId, const char* stringYzlb, const char* stringAId, 
						const char* stringReviewerId, const char* stringReceiverId, const char* stringCaseId, unsigned int nPageTo, unsigned int nPageSize)
{
	if(handle==NULL || stringLocalFilePath==NULL || stringReportStartDate==NULL || stringReportEndDate==NULL || stringReviewStartDate==NULL || stringReviewEndDate==NULL || 
		stringReviewState==NULL || stringCaseType==NULL || stringPtName==NULL || stringSenderId==NULL || stringYzlb==NULL || stringAId==NULL || 
		stringReviewerId==NULL || stringReviewerId==NULL || stringCaseId==NULL)
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
	return pNetInterface->GetReviewReportList(stringLocalFilePath, stringReportStartDate, stringReportEndDate, stringReviewStartDate, stringReviewEndDate, stringReviewState, 
		stringCaseType, stringPtName, stringSenderId, stringYzlb, stringAId, stringReviewerId, stringReceiverId, stringCaseId, nPageTo, nPageSize);
}

int ReviewReport(NET_HANDLE handle, const char* stringReportKeyId, const char* stringReviewState, const char* stringPath, const char* stringDiagnostic, 
				 const char* stringDigitalSign, const char* stringTimeStampSign) 
{
	if(handle==NULL || stringReportKeyId==NULL || stringReviewState==NULL || stringPath == NULL || stringDiagnostic==NULL || stringDigitalSign==NULL || stringTimeStampSign==NULL)
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
	return pNetInterface->ReviewReport(stringReportKeyId, stringReviewState, stringPath, stringDiagnostic, stringDigitalSign, stringTimeStampSign);
}

int GetCardUserList(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringCardId, const char* stringSenderId, const char* stringPersonId, const char* stringTel, 
					 const char* stringName, const char* stringType, unsigned int nPageTo, unsigned int nPageSize)
{
	if(handle==NULL || stringLocalFilePath==NULL || stringCardId==NULL|| stringPersonId==NULL || stringTel==NULL|| stringName==NULL || stringType==NULL)
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
	return pNetInterface->GetCardUserList(stringLocalFilePath, stringCardId, stringSenderId, stringPersonId, stringTel, stringName, stringType, nPageTo, nPageSize);
}

int MarkCaseException(NET_HANDLE handle, const char* stringCaseId, const char* stringReceiverId, int nResultFlag)
{
	if(handle==NULL || stringCaseId==NULL || stringReceiverId==NULL)
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
	return pNetInterface->MarkCaseException(stringCaseId, stringReceiverId, nResultFlag);
}

int GetAllTeamList(NET_HANDLE handle, const char* stringLocalFilePath)
{
	if(handle==NULL || stringLocalFilePath==NULL)
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
	return pNetInterface->GetAllTeamList(stringLocalFilePath);
}

int GetDoctorListOfTeam(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringTeamId)
{
	if(handle==NULL || stringLocalFilePath==NULL || stringTeamId==NULL)
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
	return pNetInterface->GetDoctorListOfTeam(stringLocalFilePath, stringTeamId);
}


int DoctorHandledCaseStatistics(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringCaseType, const char* stringStartTime, const char* stringEndTime)
{
	if(handle==NULL || stringLocalFilePath==NULL || stringCaseType==NULL || stringStartTime==NULL || stringEndTime==NULL)
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
	return pNetInterface->DoctorHandledCaseStatistics(stringLocalFilePath, stringCaseType, stringStartTime, stringEndTime);
}

int DoctorDownloadCaseAndOneReportOfOldCm(NET_HANDLE handle, const char* stringLocalCasePath, const char* stringLocalReportPath, const char* stringCaseId, const char* stringReportUri, unsigned long nReportSize, pProgressAndSpeedCallback pCallback)
{
	if(handle==NULL || stringLocalCasePath==NULL || stringLocalReportPath==NULL || stringCaseId==NULL || stringReportUri==NULL)
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
	return pNetInterface->DoctorDownloadCaseAndOneReportOfOldCm(stringLocalCasePath, stringLocalReportPath, stringCaseId, stringReportUri, nReportSize, pCallback);
}

int GetTrendData(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringUId, const char* stringInterUId, const char* stringTrendType, const char* stringStartDate, const char* stringEndDate, const char* stringNextId)
{
	if(handle==NULL || stringLocalFilePath==NULL || stringUId==NULL || stringInterUId==NULL || stringTrendType==NULL || stringStartDate==NULL || stringEndDate==NULL || stringNextId==NULL)
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
	return pNetInterface->GetTrendData(stringLocalFilePath, stringUId, stringInterUId, stringTrendType, stringStartDate, stringEndDate, stringNextId);
}

int GetHealthDegree(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringUId, const char* stringInterUId, const char* stringDate)
{
	if(handle==NULL || stringLocalFilePath==NULL || stringUId==NULL || stringInterUId==NULL || stringDate==NULL)
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
	return pNetInterface->GetHealthDegree(stringLocalFilePath, stringUId, stringInterUId, stringDate);
}

int GetUserOfNoAdviceAndMostData(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringDays)
{
	if(handle==NULL || stringLocalFilePath==NULL || stringDays==NULL)
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
	return pNetInterface->GetUserOfNoAdviceAndMostData(stringLocalFilePath, stringDays);
}

int AdviceDelay(NET_HANDLE handle, const char* stringUId, const char* stringMinutes)
{
	if(handle==NULL || stringUId==NULL || stringMinutes==NULL)
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
	return pNetInterface->AdviceDelay(stringUId, stringMinutes);
}

int AdviceComplete(NET_HANDLE handle, const char* stringUId, const char* stringOptionType, const char* stringAdvice)
{
	if(handle==NULL || stringUId==NULL || stringOptionType==NULL || stringAdvice)
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
	return pNetInterface->AdviceComplete(stringUId, stringOptionType, stringAdvice);
}

int GetUserListOfNoAdvice(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringDays, unsigned int nPageTo, unsigned int nPageSize)
{
	if(handle==NULL || stringLocalFilePath==NULL || stringDays==NULL)
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
	return pNetInterface->GetUserListOfNoAdvice(stringLocalFilePath, stringDays, nPageTo, nPageSize);
}

int AdviceBegin(NET_HANDLE handle, const char* stringUId)
{
	if(handle==NULL || stringUId==NULL)
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
	return pNetInterface->AdviceBegin(stringUId);
}

int GetUserListOfAdvicing(NET_HANDLE handle, const char* stringLocalFilePath, unsigned int nPageTo, unsigned int nPageSize)
{
	if(handle==NULL || stringLocalFilePath==NULL)
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
	return pNetInterface->GetUserListOfAdvicing(stringLocalFilePath, nPageTo, nPageSize);
}

int GetAskList(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringAskType, const char* stringAskEnd, const char* stringDoctorId, const char* stringIsAsk, const char* stringUserId, const char* stringStartTime, const char* stringEndTime, unsigned int nPageTo, unsigned int nPageSize)
{
	if(handle==NULL || stringLocalFilePath==NULL || stringAskType==NULL || stringAskEnd==NULL || stringDoctorId==NULL || stringIsAsk==NULL || stringUserId==NULL || stringStartTime==NULL || stringEndTime==NULL)
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
	return pNetInterface->GetAskList(stringLocalFilePath, stringAskType, stringAskEnd, stringDoctorId, stringIsAsk, stringUserId, stringStartTime, stringEndTime, nPageTo, nPageSize);
}

int GetAskDetail(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringAskId)
{
	if(handle==NULL || stringLocalFilePath==NULL || stringAskId==NULL)
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
	return pNetInterface->GetAskDetail(stringLocalFilePath, stringAskId);
}

int SubmitQuestion(NET_HANDLE handle, const char* stringUserId, const char* stringAdvise, const char* stringReportType, const char* stringAskId, const char* stringAskedId, const char* stringDoctorPhotoPath)
{
	if(handle==NULL || stringUserId==NULL || stringAdvise==NULL || stringReportType==NULL || stringAskId==NULL || stringAskedId==NULL || stringDoctorPhotoPath==NULL)
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
	return pNetInterface->SubmitQuestion(stringUserId, stringAdvise, stringReportType, stringAskId, stringAskedId, stringDoctorPhotoPath);
}

int GetCliniqueListOfHospital(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringHospitalId)
{
	if(handle==NULL || stringLocalFilePath==NULL || stringHospitalId==NULL)
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
	return pNetInterface->GetCliniqueListOfHospital(stringLocalFilePath, stringHospitalId);
}

int GetStatisticsInfo(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringTypeId, const char* stringMonth, const char* stringMonthDay, unsigned int nPageTo, unsigned int nPageSize)
{
	if(handle==NULL || stringLocalFilePath==NULL || stringTypeId==NULL || stringMonth==NULL || stringMonthDay==NULL)
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
	return pNetInterface->GetStatisticsInfo(stringLocalFilePath, stringTypeId, stringMonth, stringMonthDay, nPageTo, nPageSize);
}

int NET_API GetUserHealthList(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringNextId, const char* stringTypeId, const char* stringSecondTypeId, const char* stringName, const char* stringSex, const char* stringThirdIdType, 
							  const char* stringThirdId, const char* stringHospitalId, const char* stringUnitSenderId, const char* stringIsData, const char* stringIntervalTime, const char* stringPopulations, const char* stringContractDoctor, const char* stringIsContract, const char* stringPageSize)
{
	if(handle==NULL || stringLocalFilePath==NULL || stringNextId==NULL || stringTypeId==NULL || stringSecondTypeId==NULL || stringName==NULL || stringSex==NULL || stringThirdIdType==NULL || stringThirdId==NULL || stringHospitalId==NULL 
		|| stringUnitSenderId==NULL ||  stringIsData==NULL || stringIntervalTime==NULL || stringPopulations==NULL || stringPageSize==NULL)
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
	return pNetInterface->GetUserHealthList(stringLocalFilePath, stringNextId, stringTypeId, stringSecondTypeId, stringName, stringSex, stringThirdIdType, stringThirdId, stringHospitalId, stringUnitSenderId, stringIsData, stringIntervalTime, 
		stringPopulations, stringContractDoctor, stringIsContract, stringPageSize);
}

int GetAdviceImplemention(NET_HANDLE handle, char* stringPercent, const char* stringThirdId, const char* stringHospitalId)
{
	if(handle==NULL || stringPercent==NULL || stringThirdId==NULL || stringHospitalId==NULL)
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
	string stringTempPercent;
	int nRet = PHMS_SUCCESSFUL_RESULT;
	nRet = pNetInterface->GetAdviceImplemention(stringTempPercent, stringThirdId, stringHospitalId);
	if(stringTempPercent.size() != 0)
	{
		strcpy(stringPercent, stringTempPercent.c_str());
	}
	return nRet;
}

int ChangePersonType(NET_HANDLE handle, const char* stringThirdId, const char* stringPeopleType, const char* stringHospitalId)
{
	if(handle==NULL || stringPeopleType==NULL || stringThirdId==NULL || stringHospitalId==NULL)
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
	return pNetInterface->ChangePersonType(stringThirdId, stringPeopleType, stringHospitalId);
}

int SetNextVisitTime(NET_HANDLE handle, const char* stringThirdId, const char* stringInterViewTime, const char* stringHospitalId)
{
	if(handle==NULL || stringInterViewTime==NULL || stringThirdId==NULL || stringHospitalId==NULL)
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
	return pNetInterface->SetNextVisitTime(stringThirdId, stringInterViewTime, stringHospitalId);
}

int GetPersonTypeList(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringLanguage)
{
	if(handle==NULL || stringLocalFilePath==NULL)
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
	return pNetInterface->GetPersonTypeList(stringLocalFilePath, stringLanguage);
}

int PublishHealthyKnowledge(NET_HANDLE handle, pHEALTHY_KNOWLEDGE pHealthKnowledge, unsigned int nArrayLen)
{
	if(handle==NULL || pHealthKnowledge==NULL || nArrayLen==0)
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
	vector<HEALTHY_KNOWLEDGE> vHealthyKnowledge;
	for(int i=0; i<nArrayLen; i++)
	{
		vHealthyKnowledge.push_back(pHealthKnowledge[i]);
	}
	return pNetInterface->PublishHealthyKnowledge(vHealthyKnowledge);
}

int GetTrendRangeValue(NET_HANDLE handle, const char* stringLocalFilePath)
{
	if(handle==NULL || stringLocalFilePath==NULL)
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
	return pNetInterface->GetTrendRangeValue(stringLocalFilePath);
}

int GetCliniqueReportList(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringCliniqueSenderId, const char* stringStartDate, 
						  const char* stringEndDate, const char* stringState, const char* stringName, const char* stringPatientSourceCode, 
						  const char* stringYzlb, const char* stringPIdType, const char* stringPId, const char* stringHb, const char* stringASectionId, 
						  const char* stringNextId, const char* stringPageSize)
{
	if(handle==NULL || stringLocalFilePath==NULL || stringCliniqueSenderId==NULL || stringStartDate==NULL || stringEndDate==NULL || stringState==NULL || 
		stringName==NULL || stringPatientSourceCode==NULL || stringYzlb==NULL || stringPIdType==NULL || stringPId==NULL || stringHb==NULL || stringASectionId==NULL || stringNextId==NULL || stringPageSize==NULL)
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
	return pNetInterface->GetCliniqueReportList(stringLocalFilePath, stringCliniqueSenderId, stringStartDate, stringEndDate, stringState, stringName, stringPatientSourceCode, stringYzlb, stringPIdType, stringPId, stringHb, stringASectionId, stringNextId, stringPageSize);
}


int GetUserListBindingOrNot(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringNextId, const char* stringPageSize, const char* stringIsBind, const char* stringIsActive)
{
	if(handle==NULL || stringLocalFilePath==NULL || stringNextId==NULL || stringPageSize==NULL || stringIsBind==NULL || stringIsActive==NULL)
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
	return pNetInterface->GetUserListBindingOrNot(stringLocalFilePath, stringNextId, stringPageSize, stringIsBind, stringIsActive);
}

int BindUser(NET_HANDLE handle, const char* stringUId, const char* stringUName)
{
	if(handle==NULL || stringUId==NULL || stringUName==NULL)
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
	return pNetInterface->BindUser(stringUId, stringUName);
}

int UnbindUser(NET_HANDLE handle, const char* stringUId)
{
	if(handle==NULL || stringUId==NULL)
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
	return pNetInterface->UnbindUser(stringUId);
}


int SearchCaseAdvanced(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringHospitalId, const char* stringDiagnostic, 
							   const char* stringName, const char* stringSex, QUERY_RANGE rangeAge, QUERY_RANGE rangeCreateTime, const char* stringMode, 
							   const char* stringCaseType, QUERY_ECG ecg, const char* stringFrom, const char* stringSize)
{
	if(handle==NULL || stringLocalFilePath==NULL || stringHospitalId==NULL || stringDiagnostic==NULL || stringName==NULL || stringSex==NULL || 
		rangeAge.upperLimit==NULL || rangeAge.lowerLimit==NULL || rangeCreateTime.upperLimit==NULL || rangeCreateTime.lowerLimit==NULL || stringMode==NULL || 
		stringCaseType==NULL || stringFrom==NULL || stringSize==NULL)
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
	return pNetInterface->SearchCaseAdvanced(stringLocalFilePath, stringHospitalId, stringDiagnostic, stringName, stringSex, rangeAge, rangeCreateTime, 
		stringMode, stringCaseType, ecg, stringFrom, stringSize);
}

int SearchCaseAdvancedString(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringHospitalId, const char* stringDiagnostic, 
									 const char* stringName, const char* stringSex, const char* stringJsonAge, const char* stringJsonCreateTime, const char* stringMode, 
									 const char* stringCaseType, const char* stringJsonEcg, const char* stringFrom, const char* stringSize)
{
	if(handle==NULL || stringLocalFilePath==NULL || stringHospitalId==NULL || stringDiagnostic==NULL || stringName==NULL || stringSex==NULL || 
		stringJsonAge==NULL || stringJsonCreateTime==NULL || stringJsonEcg==NULL || stringMode==NULL || 
		stringCaseType==NULL || stringFrom==NULL || stringSize==NULL)
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
	return pNetInterface->SearchCaseAdvancedString(stringLocalFilePath, stringHospitalId, stringDiagnostic, stringName, stringSex, stringJsonAge, stringJsonCreateTime, 
		stringMode, stringCaseType, stringJsonEcg, stringFrom, stringSize);
}

int SignDoctor(NET_HANDLE handle, const char* stringUId)
{
	if(handle==NULL || stringUId==NULL)
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
	return pNetInterface->SignDoctor(stringUId);
}

int OnlyDownloadCase(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringCaseId, const char* stringReceiverId, const char* stringUri, unsigned long nFileSize, pProgressAndSpeedCallback pCallback)
{
	if(handle==NULL || stringLocalFilePath==NULL || stringCaseId==NULL || stringReceiverId==NULL || stringUri==NULL)
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
	return pNetInterface->OnlyDownloadCase(stringLocalFilePath, stringCaseId, stringReceiverId, stringUri, nFileSize, pCallback);
}

int HoldPlaceAndAnalyzeBegin(NET_HANDLE handle, const char* stringCaseId, const char* stringHospitalId, const char* stringReceiverId, const char* stringCaseType)
{
	if(handle==NULL || stringCaseId==NULL || stringReceiverId==NULL || stringHospitalId==NULL)
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
	return pNetInterface->HoldPlaceAndAnalyzeBegin(stringCaseId, stringHospitalId, stringReceiverId, stringCaseType);
}

int HoldPlaceReview(NET_HANDLE handle, const char* stringReportId)
{
	if(handle==NULL || stringReportId==NULL)
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
	return pNetInterface->HoldPlaceReview(stringReportId);
}


int RollBackReview(NET_HANDLE handle, const char* stringReportId)
{
	if(handle==NULL || stringReportId==NULL)
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
	return pNetInterface->RollBackReview(stringReportId);
}


int ResetReviewHoldPlace(NET_HANDLE handle, const char* stringReportId, const char* stringOccupantId, const char* stringOccupantName, const char* stringSuperKey)
{
	if(handle==NULL || stringReportId==NULL || stringOccupantId==NULL || stringOccupantName==NULL || stringSuperKey==NULL)
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
	return pNetInterface->ResetReviewHoldPlace(stringReportId, stringOccupantId, stringOccupantName, stringSuperKey);
}


int ResetCaseHoldPlace(NET_HANDLE handle, const char* stringCaseId, const char* stringReceiverId, const char* stringOccupantId, const char* stringOccupantName, const char* stringSuperKey)
{
	if(handle==NULL || stringCaseId==NULL || stringReceiverId==NULL || stringOccupantId==NULL || stringOccupantName==NULL || stringSuperKey==NULL)
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
	return pNetInterface->ResetCaseHoldPlace(stringCaseId, stringReceiverId, stringOccupantId, stringOccupantName, stringSuperKey);
}

int ReviewReportWithFile(NET_HANDLE handle, const char* stringCaseId, const char* stringReceiverId, const char* stringReportKeyId, const char* stringReviewState, const char* stringFileLocalPath, const char* stringDiagnostic, 
						 const char* stringDigitalSign, const char* stringTimeStampSign, const char* stringResultFlag, const char* stringFileType, pProgressAndSpeedCallback pCallback)
{
	if(handle==NULL || stringCaseId==NULL || stringReceiverId==NULL || stringReportKeyId==NULL || stringReviewState==NULL || stringFileLocalPath == NULL || stringDiagnostic==NULL || 
		stringDigitalSign==NULL || stringTimeStampSign==NULL || stringResultFlag==NULL || stringFileType==NULL)
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
	return pNetInterface->ReviewReportWithFile(stringCaseId, stringReceiverId, stringReportKeyId, stringReviewState, stringFileLocalPath, stringDiagnostic, stringDigitalSign, stringTimeStampSign, stringResultFlag, stringFileType, pCallback);
}

int AddTag(NET_HANDLE handle, char* stringTagId, const char* stringTagName, const char* stringTagDescription)
{
	if(handle==NULL || stringTagId==NULL || stringTagName==NULL || stringTagDescription==NULL)
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
	string stringTempTagId;
	int nRet = PHMS_SUCCESSFUL_RESULT;
	nRet = pNetInterface->AddTag(stringTempTagId, stringTagName, stringTagDescription);
	if(stringTempTagId.size() != 0)
	{
		strcpy(stringTagId, stringTempTagId.c_str());
	}
	return nRet;
}

int GetTagList(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringSenderId)
{
	if(handle==NULL || stringLocalFilePath==NULL || stringSenderId==NULL)
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
	return pNetInterface->GetTagList(stringLocalFilePath, stringSenderId);
}

int AddCaseToTag(NET_HANDLE handle, const char* stringTagId, const char* stringTagName, const char* stringCaseId, const char* stringCaseType, const char* stringCaseTypeName, const char* stringPatientName)
{
	if(handle==NULL || stringTagId==NULL || stringTagName==NULL || stringCaseId==NULL || stringCaseType==NULL || stringCaseTypeName == NULL || stringPatientName==NULL)
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
	return pNetInterface->AddCaseToTag(stringTagId, stringTagName, stringCaseId, stringCaseType, stringCaseTypeName, stringPatientName);
}

int GetCaseListOfTag(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringTagId, const char* stringSenderId, const char* stringCaseType)
{
	if(handle==NULL || stringLocalFilePath==NULL || stringTagId==NULL || stringSenderId==NULL)
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
	return pNetInterface->GetCaseListOfTag(stringLocalFilePath, stringTagId, stringSenderId, stringCaseType);
}

