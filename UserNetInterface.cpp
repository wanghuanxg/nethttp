#define PHMS_DLL_EXPORTS
#include "UserNetInterface.h"
#include "ExternalNetInterface.h"
#include "ErrorCode.h"
#include <map>
#include <string>
#include "NetThreadManager.h"
#include "PhmsLogger.h"
//#include "TempGuanXinDB.h"

using namespace std;




int UserLogin(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringUsername, const char* stringPassword, const char* stringRole, const char* stringToken, char* stringIp)
{
	if(handle==NULL || stringLocalFilePath==NULL || stringUsername==NULL || stringPassword==NULL || stringRole==NULL)
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
	nRet = pNetInterface->UserLogin(stringLocalFilePath, stringUsername, stringPassword, stringRole, stringToken, stringServerIp);
	if(stringServerIp.size() != 0)
	{
		strcpy(stringIp, stringServerIp.c_str());
	}
	return nRet;
}
int GetUserInfo(NET_HANDLE handle, const char* stringLocalFilePath)
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
	return pNetInterface->GetUserInfo(stringLocalFilePath);
}
int GetHospitalInfo(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringHospitalId)
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
	return pNetInterface->GetHospitalInfo(stringLocalFilePath, stringHospitalId);
}
int GetDoctorInfo(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringDoctorId)
{
	if(handle==NULL || stringLocalFilePath==NULL || stringDoctorId==NULL)
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
	return pNetInterface->GetDoctorInfo(stringLocalFilePath, stringDoctorId);
}
int UploadCase(NET_HANDLE handle, char* stringCaseId, const char* stringName, const char* stringThirdId, const char* stringDataType, const char* stringDataTypeName, const char* stringNotes, const char* stringHospitalId, 
			   const char* stringHospitalName, const char* stringDoctorId, const char* stringDoctorName, const char* stringCheckTime, const char* stringOtherParam, const char* stringDeviceId, const char* stringDeviceName, const char* stringCaseOrigin, const char* stringYzlb, const char* stringSex, 
			   const char* stringApplicationNo, const char* stringAutoMatic, const char* stringLocalCasePath, pProgressAndSpeedCallback pCallback)
{
	if(handle==NULL || stringCaseId==NULL || stringName==NULL || stringThirdId==NULL || stringDataType==NULL || stringDataTypeName==NULL || stringNotes==NULL || stringHospitalId==NULL || 
		stringHospitalName==NULL || stringDoctorId==NULL || stringDoctorName==NULL || stringCheckTime==NULL || stringOtherParam==NULL || stringDeviceId==NULL || stringDeviceName==NULL || stringCaseOrigin==NULL || stringYzlb==NULL || stringSex==NULL || 
		stringApplicationNo==NULL ||stringLocalCasePath==NULL)
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
	string stringTempCaseId = stringCaseId;
	int nRet = PHMS_SUCCESSFUL_RESULT;
	nRet = pNetInterface->UploadCase(stringTempCaseId, stringName, stringThirdId, stringDataType, stringDataTypeName, stringNotes, stringHospitalId, stringHospitalName, stringDoctorId, 
		stringDoctorName, stringCheckTime, stringOtherParam, stringDeviceId, stringDeviceName, stringCaseOrigin, stringYzlb, stringSex, stringApplicationNo, stringAutoMatic, stringLocalCasePath, pCallback);
	if(stringTempCaseId.size() != 0)
	{
		strcpy(stringCaseId, stringTempCaseId.c_str());
	}
	return nRet;
}
int UserDownloadReport(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringCaseId, const char* stringReportId, const char* stringUri, unsigned long nFileSize, 
					   pProgressAndSpeedCallback pCallback)
{
	if(handle==NULL || stringLocalFilePath==NULL || stringCaseId==NULL || stringReportId==NULL || stringUri==NULL)
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
	return pNetInterface->UserDownloadReport(stringLocalFilePath, stringCaseId, stringReportId, stringUri, nFileSize, pCallback);
}
int GetCaseReportState(NET_HANDLE handle, char* stringReportState, const char* stringCaseId)
{
	if(handle==NULL || stringReportState==NULL || stringCaseId==NULL)
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
	string stringTempReportState;
	int nRet = PHMS_SUCCESSFUL_RESULT;
	nRet = pNetInterface->GetCaseReportState(stringTempReportState, stringCaseId);
	if(stringTempReportState.size() != 0)
	{
		strcpy(stringReportState, stringTempReportState.c_str());
	}
	return nRet;
}
int UserGetReportList(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringReportId, const char* stringReportStartDate, const char* stringReportEndDate, 
					  const char* stringCaseType, const char* stringReportState, const char* stringOrderBy, unsigned long nPageTo, unsigned long nPageSize)
{
	if(handle==NULL || stringLocalFilePath==NULL || stringReportId==NULL || stringReportStartDate==NULL || stringReportEndDate==NULL || 
		stringCaseType==NULL || stringReportState==NULL || stringOrderBy==NULL)
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
	return pNetInterface->UserGetReportList(stringLocalFilePath, stringReportId, stringReportStartDate, stringReportEndDate, stringCaseType, stringReportState, stringOrderBy, nPageTo, nPageSize);
}
int UserGetCaseList(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringCaseId, const char* stringCaseType, const char* stringCaseState, const char* stringThirdId, const char* stringName, 
					const char* stringStartDate, const char* stringEndDate, const char* stringCheckStartDate, const char* stringCheckEndDate, const char* stringCaseOrigin, 
					const char* stringYzlb, const char* stringSex, const char* stringIsRead, const char* stringOrderBy, unsigned int nPageTo, unsigned int nPageSize)
{
	if(handle==NULL || stringLocalFilePath==NULL || stringCaseId==NULL || stringCaseType==NULL || stringCaseState==NULL || 
		stringThirdId==NULL || stringName==NULL || stringStartDate==NULL || stringEndDate==NULL || stringCheckStartDate==NULL || stringCheckEndDate==NULL || 
		stringCaseOrigin==NULL || stringYzlb==NULL || stringSex==NULL)
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
	return pNetInterface->UserGetCaseList(stringLocalFilePath, stringCaseId, stringCaseType, stringCaseState, stringThirdId, stringName, stringStartDate, stringEndDate, stringCheckStartDate, 
		stringCheckEndDate, stringCaseOrigin, stringYzlb, stringSex, stringIsRead, stringOrderBy, nPageTo, nPageSize);
}

int GetAdvice(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringSgId, const char* stringUserId, const char* stringDoctorId, const char* stringDownloadState, const char* stringType, 
					  const char* stringStartDate, const char* stringEndDate, unsigned int nPageTo, unsigned int nPageSize)
{
	if(handle==NULL || stringLocalFilePath==NULL || stringUserId==NULL || stringSgId==NULL || stringDoctorId==NULL || stringDownloadState==NULL || stringType==NULL || stringStartDate==NULL || 
		stringEndDate==NULL)
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
	return pNetInterface->GetAdvice(stringLocalFilePath, stringSgId, stringUserId, stringDoctorId, stringDownloadState, stringType, stringStartDate, stringEndDate, nPageTo, nPageSize);
}


int MarkReportRead(NET_HANDLE handle, const char* stringReportId)
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
	return pNetInterface->MarkReportRead(stringReportId);
}
int CardUserChangeInfo(NET_HANDLE handle, const char* stringPId, const char* stringName, const char* stringSex, const char* stringTel, const char* stringBirthday, const char* stringAddress, const char* stringLmp, const char* stringToken, 
					   const char* stringHeight, const char* stringWeight, const char* stringPhoto, const char* stringVCodeBeFollowed, const char* stringHospitalId, const char* stringOtherInfo)
{
	if(handle==NULL || stringPId==NULL || stringName==NULL || stringSex==NULL || stringTel==NULL || stringBirthday==NULL || stringAddress==NULL || stringLmp==NULL || 
		stringToken==NULL || stringHeight==NULL || stringWeight==NULL || stringPhoto==NULL || stringVCodeBeFollowed==NULL || stringHospitalId==NULL)
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
	return pNetInterface->CardUserChangeInfo(stringPId, stringName, stringSex, stringTel, stringBirthday, stringAddress, stringLmp, stringToken, stringHeight, stringWeight, stringPhoto, stringVCodeBeFollowed, stringHospitalId, stringOtherInfo);
}
int CliniqueChangeInfo(NET_HANDLE handle, const char* stringPId, const char* stringName, const char* stringTel, const char* stringNotes)
{
	if(handle==NULL || stringPId==NULL || stringName==NULL || stringTel==NULL || stringNotes==NULL)
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
	return pNetInterface->CliniqueChangeInfo(stringPId, stringName, stringTel, stringNotes);
}

int UploadTrendData(NET_HANDLE handle, const char* stringCardId, const char* stringDeviceId, const char* stringDeviceName, const char* stringCaseId, const RECORD_DATA* pRecordData, int nArrayLen)
{
	if(handle==NULL || stringCardId==NULL || stringDeviceId==NULL || stringDeviceName==NULL || 
		stringCaseId==NULL || pRecordData==NULL || nArrayLen==0 || pRecordData->urineData.urineValue==NULL || pRecordData->hrConclusion.conclusion==NULL || pRecordData->checkTime==NULL)
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
	vector<RECORD_DATA> vRecordData;
	for(int i=0; i<nArrayLen; i++)
	{
		vRecordData.push_back(pRecordData[i]);
	}
	return pNetInterface->UploadTrendData(stringCardId, stringDeviceId, stringDeviceName, stringCaseId, vRecordData);
}

int NET_API UploadTrendDataString(NET_HANDLE handle, const char* stringCardId, const char* stringDeviceId, const char* stringDeviceName, const char* stringCaseId, const RECORD_DATA_STRING* pRecordData, int nArrayLen)
{
	if(handle==NULL || stringCardId==NULL || stringDeviceId==NULL || stringDeviceName==NULL || 
		stringCaseId==NULL || pRecordData==NULL || nArrayLen==0)
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
	vector<RECORD_DATA_STRING> vRecordData;
	for(int i=0; i<nArrayLen; i++)
	{
		vRecordData.push_back(pRecordData[i]);
	}
	return pNetInterface->UploadTrendDataString(stringCardId, stringDeviceId, stringDeviceName, stringCaseId, vRecordData);
}

int UploadTrendDataFile(NET_HANDLE handle, const char* stringFilePath)
{
	if(handle==NULL || stringFilePath==NULL)
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
	return pNetInterface->UploadTrendDataFile(stringFilePath);
}


int NET_API VerifyThirdId(NET_HANDLE handle, char* stringCardId, const char* stringUId, const char* stringType)
{
	if(handle==NULL || stringCardId==NULL || stringUId==NULL || stringType==NULL)
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
	string stringTempCardId;
	int nRet = PHMS_SUCCESSFUL_RESULT;
	nRet = pNetInterface->VerifyThirdId(stringTempCardId, stringUId, stringType);
	if(stringTempCardId.size() != 0)
	{
		strcpy(stringCardId, stringTempCardId.c_str());
	}
	return nRet;
}

int GetDoctorList_999120(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringKtCliniqueId, const char* stringCaseTypeId)
{
	if(handle==NULL || stringLocalFilePath==NULL || stringKtCliniqueId==NULL || stringCaseTypeId==NULL)
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
	string stringTempCardId;
	int nRet = PHMS_SUCCESSFUL_RESULT;
	nRet = pNetInterface->GetDoctorList_999120(stringLocalFilePath, stringKtCliniqueId, stringCaseTypeId);
	return nRet;
}

int ValidatePay_999120(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringConsumeId, const char* stringKtCliniqueId, const char* stringCardNumber, const char* stringKtDoctorId, const char* stringConsumeMoney, const char* stringCaseTypeId)
{
	if(handle==NULL || stringLocalFilePath==NULL || stringConsumeId==NULL || stringKtCliniqueId==NULL || stringCardNumber==NULL || stringKtDoctorId==NULL || stringConsumeMoney==NULL || stringCaseTypeId==NULL)
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
	string stringTempCardId;
	int nRet = PHMS_SUCCESSFUL_RESULT;
	nRet = pNetInterface->ValidatePay_999120(stringLocalFilePath, stringConsumeId, stringKtCliniqueId, stringCardNumber, stringKtDoctorId, stringConsumeMoney, stringCaseTypeId);
	return nRet;
}

int UpdateCaseForConsumerRecord_999120(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringConsumeId, const char* stringKtCaseId, const char* stringCaseTypeId)
{
	if(handle==NULL || stringLocalFilePath==NULL || stringConsumeId==NULL || stringKtCaseId==NULL || stringCaseTypeId==NULL)
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
	string stringTempCardId;
	int nRet = PHMS_SUCCESSFUL_RESULT;
	nRet = pNetInterface->UpdateCaseForConsumerRecord_999120(stringLocalFilePath, stringConsumeId, stringKtCaseId, stringCaseTypeId);
	return nRet;
}

int PersonIdRegister(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringType, const char* stringRegFrom, const char* stringPersonId, const char* stringName, const char* stringSex, const char* stringBirthday, const char* stringAddress, 
					 const char* stringPhone, const char* stringHeigth, const char* stringUploadUnitId, const char* stringHospitalId, const char* stringHGroupId, const char* stringScode, const char* stringWxUId, const char* stringLmp,const char*  stringWeight, const char* stringPassword, 
					 const char* stringThirdCode, const char* stringIsRegister)
{
	if(handle==NULL || stringLocalFilePath==NULL || stringType==NULL || stringRegFrom==NULL || stringPersonId==NULL || stringName==NULL || stringSex==NULL || stringBirthday==NULL || stringAddress==0 || 
		stringPhone==NULL || stringHeigth==NULL || stringUploadUnitId==NULL || stringHospitalId==NULL || stringHGroupId==NULL || stringScode==NULL || stringWxUId==NULL || stringLmp==NULL || stringWeight ==NULL || stringPassword==NULL || stringThirdCode==NULL || stringIsRegister==NULL)
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
	return pNetInterface->PersonIdRegister(stringLocalFilePath, stringType, stringRegFrom, stringPersonId, stringName, stringSex, stringBirthday, stringAddress, stringPhone, stringHeigth, stringUploadUnitId, stringHospitalId, stringHGroupId, stringScode, stringWxUId, stringLmp, stringWeight, stringPassword, stringThirdCode, stringIsRegister);
}

int GetDataDictionary(NET_HANDLE handle, const char* stringLocalFilePath)
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
	return pNetInterface->GetDataDictionary(stringLocalFilePath);
}

int LoggingHealthBaseInfo(NET_HANDLE handle, const char* stringUserId, const char* stringWeblock, const char* stringNation, const char* stringProfression, const char* stringCrowd, const char* stringBloodType, 
								  const char* stringJwsJiBing, const char* stringJwsShouShu, const char* stringJwsWaiShang, const char* stringJwsShuXue, const char* stringJwsBaoLuShi, const char* stringJwsBaoLuShiBz, 
								  const char* stringJwsYiChuanJiBing, const char* stringGmsGuoMinShi, const char* stringGmsGuoMinShiBz, const char* stringJzsFuQin, const char* stringJzsFuQinBz, const char* stringJzsMuQin, 
								  const char* stringJzsMuQinBz, const char* stringJzsXdjm, const char* stringJzsXdjmBz, const char* stringJzsZiNv, const char* stringJzsZiNvBz, const char* stringJzsQiTa)
{
	if(handle==NULL || stringUserId==NULL || stringWeblock==NULL || stringNation==NULL || stringProfression==NULL || stringCrowd==NULL || stringBloodType==NULL || 
		stringJwsJiBing==NULL || stringJwsShouShu==NULL || stringJwsWaiShang==NULL || stringJwsShuXue==NULL || stringJwsBaoLuShi==NULL || stringJwsBaoLuShiBz==NULL ||
		stringJwsYiChuanJiBing==NULL || stringGmsGuoMinShi==NULL || stringGmsGuoMinShiBz==NULL || stringJzsFuQin==NULL || stringJzsFuQinBz==NULL || stringJzsMuQin==NULL || 
		stringJzsMuQinBz==NULL || stringJzsXdjm==NULL || stringJzsXdjmBz==NULL || stringJzsZiNv==NULL || stringJzsZiNvBz==NULL || stringJzsQiTa==NULL)
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
	return pNetInterface->LoggingHealthBaseInfo(stringUserId, stringWeblock, stringNation, stringProfression, stringCrowd, stringBloodType, stringJwsJiBing, stringJwsShouShu, stringJwsWaiShang, stringJwsShuXue,
		stringJwsBaoLuShi, stringJwsBaoLuShiBz, stringJwsYiChuanJiBing, stringGmsGuoMinShi, stringGmsGuoMinShiBz, stringJzsFuQin, stringJzsFuQinBz, stringJzsMuQin, stringJzsMuQinBz, stringJzsXdjm, stringJzsXdjmBz,
		stringJzsZiNv, stringJzsZiNvBz, stringJzsQiTa);
}

int LoggingHealthCurrentInfo(NET_HANDLE handle, const char* stringUserId, const char* stringShfsYinShi, const char* stringShfsXlzk, const char* stringShfsXlzkBz, const char* stringShfsXltz, const char* stringShfsXiYan, 
									 const char* stringShfsYinJiu, const char* stringShfsSyqk, const char* stringShfsYunDong, const char* stringShfsYunDongShiJian, const char* stringSysjcDanGuChun, const char* stringSysjcXueChangGui, 
									 const char* stringSysjcXueChangGuiBz, const char* stringSysjcNiaoChangGui, const char* stringSysjcNiaoChangGuiBz, const char* stringSysjcQiTa, const char* stringYongYao, const char* stringZhuYuan, 
									 const char* stringJkwtNaoXueGuan, const char* stringJkwtNaoXueGuanBz, const char* stringJkwtShenZang, const char* stringJkwtShenZangBz, const char* stringJkwtXinZang, const char* stringJkwtXinZangBz, 
									 const char* stringJkwtXueGuan, const char* stringJkwtXueGuanBz, const char* stringJkwtYanBu, const char* stringJkwtYanBuBz, const char* stringJkwtShenJing, const char* stringJkwtQiTa, const char* stringDate)
{
	if(handle==NULL || stringUserId==NULL || stringShfsYinShi==NULL || stringShfsXlzk==NULL || stringShfsXlzkBz==NULL || stringShfsXltz==NULL || stringShfsXiYan==NULL || 
		stringShfsYinJiu==NULL || stringShfsSyqk==NULL || stringShfsYunDong==NULL || stringShfsYunDongShiJian==NULL || stringSysjcDanGuChun==NULL || stringSysjcXueChangGui==NULL ||
		stringSysjcXueChangGuiBz==NULL || stringSysjcNiaoChangGui==NULL || stringSysjcNiaoChangGuiBz==NULL || stringSysjcQiTa==NULL || stringYongYao==NULL || stringZhuYuan==NULL || 
		stringJkwtNaoXueGuan==NULL || stringJkwtNaoXueGuanBz==NULL || stringJkwtShenZang==NULL || stringJkwtShenZangBz==NULL || stringJkwtXinZang==NULL || stringJkwtXinZangBz==NULL || 
		stringJkwtXueGuan==NULL|| stringJkwtXueGuanBz==NULL || stringJkwtYanBu==NULL || stringJkwtYanBuBz==NULL || stringJkwtShenJing==NULL || stringJkwtQiTa==NULL || stringDate==NULL)
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
	return pNetInterface->LoggingHealthCurrentInfo(stringUserId, stringShfsYinShi, stringShfsXlzk, stringShfsXlzkBz, stringShfsXltz, stringShfsXiYan, stringShfsYinJiu, stringShfsSyqk, stringShfsYunDong, stringShfsYunDongShiJian, 
		stringSysjcDanGuChun, stringSysjcXueChangGui, stringSysjcXueChangGuiBz, stringSysjcNiaoChangGui, stringSysjcNiaoChangGuiBz, stringSysjcQiTa, stringYongYao, stringZhuYuan, stringJkwtNaoXueGuan, stringJkwtNaoXueGuanBz, 
		stringJkwtShenZang, stringJkwtShenZangBz, stringJkwtXinZang, stringJkwtXinZangBz, stringJkwtXueGuan, stringJkwtXueGuanBz, stringJkwtYanBu, stringJkwtYanBuBz, stringJkwtShenJing, stringJkwtQiTa, stringDate);
}

int UpdateCaseInfo(NET_HANDLE handle, char* stringDoctorId, const char* stringHospitalId, const char* stringCaseId, const char* stringOtherParam, const char* stringFlag, const char* stringNotes, const char* stringReceiver)
{
	if(handle==NULL || stringDoctorId==NULL || stringHospitalId==NULL || stringCaseId==NULL || stringOtherParam==NULL || stringFlag==NULL || stringNotes==NULL || stringReceiver==NULL)
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
	string stringTemp;
	nRet = pNetInterface->UpdateCaseInfo(stringTemp, stringHospitalId, stringCaseId, stringOtherParam, stringFlag, stringNotes, stringReceiver);
	if(nRet == PHMS_SUCCESSFUL_RESULT)
	{
		strcpy(stringDoctorId, stringTemp.c_str());
	}
	return nRet;
}

int PersonIdRegisterAndLogin(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringRole, const char* stringPId, const char* stringName, const char* stringSex, const char* stringBirthday, const char* stringAddress, const char* stringDeviceId, const char* stringHospitalId, 
							 const char* stringLang, const char* stringRegFrom)
{
	if(handle==NULL || stringLocalFilePath==NULL || stringRole==NULL || stringPId==NULL || stringName==NULL || stringSex==NULL || stringBirthday==NULL || stringAddress==NULL || stringDeviceId==NULL || stringHospitalId==NULL || stringLang==NULL || stringRegFrom==NULL)
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
	return pNetInterface->PersonIdRegisterAndLogin(stringLocalFilePath, stringRole, stringPId, stringName, stringSex, stringBirthday, stringAddress, stringDeviceId, stringHospitalId, stringLang, stringRegFrom);
}

int UploadTrendData2(NET_HANDLE handle, const char* stringTargetPath)
{
	if(handle==NULL || stringTargetPath==NULL)
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
	return pNetInterface->UploadTrendData2(stringTargetPath);
}

int CardBindPhone(NET_HANDLE handle, const char* stringPhone, const char* stringScode)
{
	if(handle==NULL || stringPhone==NULL || stringScode==NULL)
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
	return pNetInterface->CardBindPhone(stringPhone, stringScode);
}

int ResetPassword(NET_HANDLE handle, const char* stringPhone, const char* stringScode, const char* stringNewPwd)
{
	if(handle==NULL || stringPhone==NULL || stringScode==NULL || stringNewPwd==NULL)
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
	return pNetInterface->ResetPassword(stringPhone, stringScode, stringNewPwd);
}

int UploadDaily(NET_HANDLE handle, const char* stringType, const char* stringCreateTime, const char* stringFiles)
{
	if(handle==NULL || stringType==NULL || stringCreateTime==NULL || stringFiles==NULL)
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
	return pNetInterface->UploadDaily(stringType, stringCreateTime, stringFiles);
}

int GetDateListOfDaily(NET_HANDLE handle, const char* stringLocalPath, const char* stringYear, const char* stringMonth)
{
	if(handle==NULL || stringLocalPath==NULL || stringYear==NULL || stringMonth==NULL)
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
	return pNetInterface->GetDateListOfDaily(stringLocalPath, stringYear, stringMonth);
}


int GetDailyOfDate(NET_HANDLE handle, const char* stringLocalPath, const char* stringDate, const char* stringType)
{
	if(handle==NULL || stringLocalPath==NULL || stringDate==NULL || stringType==NULL)
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
	return pNetInterface->GetDailyOfDate(stringLocalPath, stringDate, stringType);
}

int ShareDaily(NET_HANDLE handle, const char* stringLocalPath, const char* stringType, const char* stringCreateTime)
{
	if(handle==NULL || stringLocalPath==NULL || stringType==NULL || stringCreateTime==NULL)
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
	return pNetInterface->ShareDaily(stringLocalPath, stringType, stringCreateTime);
}

int DeleteDaily(NET_HANDLE handle, const char* stringType, const char* stringDate)
{
	if(handle==NULL || stringType==NULL || stringDate==NULL)
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
	return pNetInterface->DeleteDaily(stringType, stringDate);
}


int PhoneScanQR(NET_HANDLE handle, char* stringSence, const char* stringQRCode)
{
	if(handle==NULL || stringSence==NULL || stringQRCode==NULL)
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
	string stringTempSence = stringSence;
	int nRet = pNetInterface->PhoneScanQR(stringTempSence, stringQRCode);
	if(stringTempSence.size() != 0)
	{
		strcpy(stringSence, stringTempSence.c_str());
	}
	return nRet;
}

int PhoneScanQRVerify(NET_HANDLE handle, const char* stringQRCode)
{
	if(handle==NULL || stringQRCode==NULL)
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
	return pNetInterface->PhoneScanQRVerify(stringQRCode);
}

int RegisterBluetoothDevice(NET_HANDLE handle, const char* stringUniqueSN, const char* stringSerial, const char* stringBluetoothAddress, const char* stringBluetoothName, const char* stringDeviceType)
{
	if(handle==NULL || stringUniqueSN==NULL || stringSerial==NULL || stringBluetoothAddress==NULL || stringBluetoothName==NULL || stringDeviceType==NULL)
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
	return pNetInterface->RegisterBluetoothDevice(stringUniqueSN, stringSerial, stringBluetoothAddress, stringBluetoothName, stringDeviceType);
}


int GetBluetoothInfoOfDevice(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringUniqueSN)
{
	if(handle==NULL || stringUniqueSN==NULL || stringLocalFilePath==NULL)
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
	return pNetInterface->GetBluetoothInfoOfDevice(stringLocalFilePath, stringUniqueSN);
}


int ChangeDeviceType(NET_HANDLE handle, const char* stringUniqueSN, const char* stringDeviceType)
{
	if(handle==NULL || stringUniqueSN==NULL || stringDeviceType==NULL)
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
	return pNetInterface->ChangeDeviceType(stringUniqueSN, stringDeviceType);
}


int UnregisterBluetoothDevice(NET_HANDLE handle, const char* stringUniqueSN)
{
	if(handle==NULL || stringUniqueSN==NULL)
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
	return pNetInterface->UnregisterBluetoothDevice(stringUniqueSN);
}

int ValidatePay_XinJie(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringRrn, const char* stringLocalDate)
{
	if(handle==NULL || stringRrn==NULL || stringLocalDate==NULL || stringLocalFilePath==NULL)
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
	return pNetInterface->ValidatePay_XinJie(stringLocalFilePath, stringRrn, stringLocalDate);
}

int NotifyCheck_XinJie(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringRrn, const char* stringLocalDate)
{
	if(handle==NULL || stringRrn==NULL || stringLocalDate==NULL || stringLocalFilePath==NULL)
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
	return pNetInterface->NotifyCheck_XinJie(stringLocalFilePath, stringRrn, stringLocalDate);
}

int GetHospitalListOfArea(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringAreaId, const char* stringRank, const char* stringBeta)
{
	if(handle==NULL || stringLocalFilePath==NULL || stringAreaId==NULL || stringRank==NULL || stringBeta==NULL)
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
	return pNetInterface->GetHospitalListOfArea(stringLocalFilePath, stringAreaId, stringRank, stringBeta);
}



int SearchSomebody(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringUserId)
{
	if(handle==NULL || stringLocalFilePath==NULL || stringUserId==NULL)
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
	return pNetInterface->SearchSomebody(stringLocalFilePath, stringUserId);
}

int FollowSomebody(NET_HANDLE handle, const char* stringUserId, const char* stringUsername, const char* stringNickname, const char* stringVCode)
{
	if(handle==NULL || stringUserId==NULL || stringUsername==NULL || stringNickname==NULL || stringVCode==NULL)
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
	return pNetInterface->FollowSomebody(stringUserId, stringUsername, stringNickname, stringVCode);
}

int MyFollowing(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringUserId, const char* stringNextId, const char* stringFType)
{
	if(handle==NULL || stringLocalFilePath==NULL || stringUserId==NULL || stringNextId==NULL || stringFType==NULL)
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
	return pNetInterface->MyFollowing(stringLocalFilePath, stringUserId, stringNextId, stringFType);
}


int ChangeNicknameOfFollowed(NET_HANDLE handle, const char* stringUserId, const char* stringNickname)
{
	if(handle==NULL || stringUserId==NULL || stringNickname==NULL)
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
	return pNetInterface->ChangeNicknameOfFollowed(stringUserId, stringNickname);
}

int CancelFollowing(NET_HANDLE handle, const char* stringUserId)
{
	if(handle==NULL || stringUserId==NULL)
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
	return pNetInterface->CancelFollowing(stringUserId);
}

int GenerateOrDeletePngZlibReport(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringServerPath, const char* stringFlag)
{
	if(handle==NULL || stringLocalFilePath==NULL || stringServerPath==NULL || stringFlag==NULL)
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
	return pNetInterface->GenerateOrDeletePngZlibReport(stringLocalFilePath, stringServerPath, stringFlag);
}

int GetDailyList(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringType, const char* stringStartDate, const char* stringEndDate, const char* stringOrderBy, const char* stringNextId, const char* stringPageSize)
{
	if(handle==NULL || stringLocalFilePath==NULL || stringType==NULL || stringStartDate==NULL || stringEndDate==NULL || stringOrderBy==NULL || stringNextId==NULL || stringPageSize==NULL)
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
	return pNetInterface->GetDailyList(stringLocalFilePath, stringType, stringStartDate, stringEndDate, stringOrderBy, stringNextId, stringPageSize);
}

int ChangeViewLikeCollectCount(NET_HANDLE handle, const char* stringHealthTipId, const char* stringUri, const char* stringTitle, const char* stringLike, const char* stringRead, const char* stringCollect)
{
	if(handle==NULL || stringHealthTipId==NULL || stringUri==NULL || stringTitle==NULL || stringLike==NULL || stringRead==NULL || stringCollect==NULL)
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
	return pNetInterface->ChangeViewLikeCollectCount(stringHealthTipId, stringUri, stringTitle, stringLike, stringRead, stringCollect);
}

int ChangeAdviceExecutiveState(NET_HANDLE handle, const char* stringSgId, const char* stringState)
{
	if(handle==NULL || stringSgId==NULL || stringState==NULL)
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
	return pNetInterface->ChangeAdviceExecutiveState(stringSgId, stringState);
}

// int CreateSession_GuanXin(NET_HANDLE handle, const char* stringUsername, const char* stringPassword)
// {
// 	if(handle==NULL || stringUsername==NULL || stringPassword==NULL)
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
// 	return pNetInterface->CreateSession_GuanXin(stringUsername, stringPassword);
// }

int GetAccessToken_GuanXin(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringUsername, const char* stringPassword)
{
	if(handle==NULL || stringLocalFilePath==NULL || stringUsername==NULL || stringPassword==NULL)
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
	return pNetInterface->GetAccessToken_GuanXin(stringLocalFilePath, stringUsername, stringPassword);
}

int GetPersonInfo_GuanXin(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringAccessToken, const char* stringPersonName, const char* stringPersonGender, const char* stringPersonIdentity, const char* stringPersonBirthDate)
{
	if(handle==NULL || stringLocalFilePath==NULL || stringAccessToken==NULL || stringPersonName==NULL || stringPersonGender==NULL || stringPersonIdentity==NULL || stringPersonBirthDate==NULL)
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
	return pNetInterface->GetPersonInfo_GuanXin(stringLocalFilePath, stringAccessToken, stringPersonName, stringPersonGender, stringPersonIdentity, stringPersonBirthDate);
}

int UploadECGData_GuanXin(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringECGXmlPath, const char* stringECGDataPath)
{
	if(handle==NULL || stringLocalFilePath==NULL || stringECGXmlPath==NULL || stringECGDataPath==NULL)
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
	return pNetInterface->UploadECGData_GuanXin(stringLocalFilePath, stringECGXmlPath, stringECGDataPath);
}

int UploadOtherData_GuanXin(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringOtherXmlPath)
{
	if(handle==NULL || stringLocalFilePath==NULL || stringOtherXmlPath==NULL)
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
	return pNetInterface->UploadOtherData_GuanXin(stringLocalFilePath, stringOtherXmlPath);
}

int UploadCheckData_GuanXin(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringCheckXmlPath)
{
	if(handle==NULL || stringLocalFilePath==NULL || stringCheckXmlPath==NULL)
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
	return pNetInterface->UploadCheckData_GuanXin(stringLocalFilePath, stringCheckXmlPath);
}

int UploadPersonInfo_GuanXin(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringPersonInfoXmlPath)
{
	if(handle==NULL || stringLocalFilePath==NULL || stringPersonInfoXmlPath==NULL)
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
	return pNetInterface->UploadPersonInfo_GuanXin(stringLocalFilePath, stringPersonInfoXmlPath);
}

int Login_XiKang(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringUsername, const char* stringPassword)
{
	if(handle==NULL || stringLocalFilePath==NULL || stringUsername==NULL || stringPassword==NULL)
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
	return pNetInterface->Login_XiKang(stringLocalFilePath, stringUsername, stringPassword);
}

int RegisterPatientId_XiKang(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringName, const char* stringSex, const char* stringPhoneNum, const char* stringProofNum)
{
	if(handle==NULL || stringLocalFilePath==NULL || stringName==NULL || stringSex==NULL || stringPhoneNum==NULL || stringProofNum==NULL)
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
	return pNetInterface->RegisterPatientId_XiKang(stringLocalFilePath, stringName, stringSex, stringPhoneNum, stringProofNum);
}

int RegisterPatientInfo_XiKang(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringPatientOpenId, const char* stringName, const char* stringSex, const char* stringPhoneNum, const char* stringProofNum, const char* stringPassword)
{
	if(handle==NULL || stringLocalFilePath==NULL || stringPatientOpenId==NULL || stringName==NULL || stringSex==NULL || stringPhoneNum==NULL || stringProofNum==NULL || stringPassword==NULL)
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
	return pNetInterface->RegisterPatientInfo_XiKang(stringLocalFilePath, stringPatientOpenId, stringName, stringSex, stringPhoneNum, stringProofNum, stringPassword);
}

int BindPatient_XiKang(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringDoctorOpenId, const char* stringPatientOpenId)
{
	if(handle==NULL || stringLocalFilePath==NULL || stringPatientOpenId==NULL || stringDoctorOpenId==NULL)
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
	return pNetInterface->BindPatient_XiKang(stringLocalFilePath, stringDoctorOpenId, stringPatientOpenId);
}


int GetPatientIdList_XiKang(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringDoctorOpenId)
{
	if(handle==NULL || stringLocalFilePath==NULL || stringDoctorOpenId==NULL)
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
	return pNetInterface->GetPatientIdList_XiKang(stringLocalFilePath, stringDoctorOpenId);
}

int SearchPatientId_XiKang(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringKey, const char* stringValue)
{
	if(handle==NULL || stringLocalFilePath==NULL || stringKey==NULL || stringValue==NULL)
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
	return pNetInterface->SearchPatientId_XiKang(stringLocalFilePath, stringKey, stringValue);
}


int GetPatientInfo_XiKang(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringPatientOpenId)
{
	if(handle==NULL || stringLocalFilePath==NULL || stringPatientOpenId==NULL)
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
	return pNetInterface->GetPatientInfo_XiKang(stringLocalFilePath, stringPatientOpenId);
}

int GetCaseList_XiKang(NET_HANDLE handle, const char* stringLocalFilePath, const char** arrPatientOpenId, int nLength)
{
	if(handle==NULL || stringLocalFilePath==NULL || arrPatientOpenId==NULL || nLength==0)
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
	vector<string> vPatientOpenId;
	for(int i=0; i<nLength; i++)
	{
		vPatientOpenId.push_back(arrPatientOpenId[i]);
	}
	return pNetInterface->GetCaseList_XiKang(stringLocalFilePath, vPatientOpenId);
}

int SubmitCase_XiKang(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringPatientOpenId, const char* stringAdvice, const char* stringCaseType, const char* stringEcgXmlContent)
{
	if(handle==NULL || stringLocalFilePath==NULL || stringPatientOpenId==NULL || stringAdvice==NULL || stringCaseType==NULL || stringEcgXmlContent==NULL)
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
	return pNetInterface->SubmitCase_XiKang(stringLocalFilePath, stringPatientOpenId, stringAdvice, stringCaseType, stringEcgXmlContent);
}

int GetReportFile_XiKang(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringCaseId)
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
	return pNetInterface->GetReportFile_XiKang(stringLocalFilePath, stringCaseId);
}

int UploadFile_XiKang(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringCaseId, const char* stringCaseFilePath)
{
	if(handle==NULL || stringLocalFilePath==NULL || stringCaseId==NULL || stringCaseFilePath==NULL)
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
	return pNetInterface->UploadFile_XiKang(stringLocalFilePath, stringCaseId, stringCaseFilePath);
}

int GetCardUserListOfCustomer(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringThirdId, const char* stringName, const char* stringCaseType, 
									  const char* stringStartDate, const char* stringEndDate, const char* stringNextId, unsigned int nPageSize)
{
	if(handle==NULL || stringLocalFilePath==NULL || stringThirdId==NULL || stringName==NULL || stringCaseType==NULL || stringStartDate==NULL || stringEndDate==NULL ||
		stringNextId==NULL)
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
	return pNetInterface->GetCardUserListOfCustomer(stringLocalFilePath, stringThirdId, stringName, stringCaseType, stringStartDate, stringEndDate, stringNextId, nPageSize);
}

int RefundFee(NET_HANDLE handle, const char* stringType, const char* stringCaseId, const char* stringRefundFee)
{
	if(handle==NULL || stringType==NULL || stringCaseId==NULL || stringRefundFee==NULL)
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
	return pNetInterface->RefundFee(stringType, stringCaseId, stringRefundFee);
}