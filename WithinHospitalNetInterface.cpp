#define PHMS_DLL_EXPORTS
#include "WithinHospitalNetInterface.h"
#include "ExternalNetInterface.h"
#include "ErrorCode.h"
#include <map>
#include <string>
#include "NetThreadManager.h"

using namespace std;


int SetCliniqueBusy(NET_HANDLE handle, const char* stringCliniqueId)
{
	if(handle==NULL || stringCliniqueId==NULL)
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
	return pNetInterface->SetCliniqueBusy(stringCliniqueId);
}


int SetCliniqueFree(NET_HANDLE handle, const char* stringCliniqueId)
{
	if(handle==NULL || stringCliniqueId==NULL)
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
	return pNetInterface->SetCliniqueFree(stringCliniqueId);
}


int NET_API SetCliniqueChecking(NET_HANDLE handle, const char* stringCliniqueId)
{
	if(handle==NULL || stringCliniqueId==NULL)
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
	return pNetInterface->SetCliniqueChecking(stringCliniqueId);
}


int NET_API SetCliniqueShoutComplete(NET_HANDLE handle, const char* stringCliniqueId)
{
	if(handle==NULL || stringCliniqueId==NULL)
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
	return pNetInterface->SetCliniqueShoutComplete(stringCliniqueId);
}



int GetPatientInfoByAppNum(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringHospitalId, const char* stringOrderType, const char* stringOrderId, 
								   const char* stringReqDepart, const char* stringOrderState, const char* stringCheckType, const char* stringStartDate, const char* stringEndDate,const char* stringClinicId)
{
	if(handle==NULL || stringLocalFilePath==NULL || stringHospitalId==NULL || stringOrderType==NULL || stringOrderId==NULL || stringReqDepart==NULL || stringOrderState==NULL || 
		stringCheckType==NULL || stringStartDate==NULL || stringEndDate==NULL || stringClinicId == NULL)
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
	return pNetInterface->GetPatientInfoByAppNum(stringLocalFilePath, stringHospitalId, stringOrderType, stringOrderId, stringReqDepart, stringOrderState, stringCheckType, stringStartDate, stringEndDate,stringClinicId);
}

int GetPatientInfoByAppNumToHis(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringHospitalId, const char* stringOrderType, const char* stringOrderId)
{
	if(handle==NULL || stringLocalFilePath==NULL || stringHospitalId==NULL || stringOrderType==NULL || stringOrderId==NULL)
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
	return pNetInterface->GetPatientInfoByAppNumToHis(stringLocalFilePath, stringHospitalId, stringOrderType, stringOrderId);
}

int GetPatientInfoByAppNumToPhmsAndHis(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringHospitalId, const char* stringOrderType, const char* stringOrderId)
{
	if(handle==NULL || stringLocalFilePath==NULL || stringHospitalId==NULL || stringOrderType==NULL || stringOrderId==NULL)
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
	return pNetInterface->GetPatientInfoByAppNumToPhmsAndHis(stringLocalFilePath, stringHospitalId, stringOrderType, stringOrderId);
}


int ApplyCheck(NET_HANDLE handle, const char* stringCliniqueId, const char* stringOrderId, const char* stringOrderType)
{
	if(handle==NULL || stringCliniqueId==NULL || stringOrderType==NULL || stringOrderId==NULL)
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
	return pNetInterface->ApplyCheck(stringCliniqueId, stringOrderId, stringOrderType);
}


int GetWaitingCheck(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringCliniqueId)
{
	if(handle==NULL || stringLocalFilePath==NULL || stringCliniqueId==NULL)
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
	return pNetInterface->GetWaitingCheck(stringLocalFilePath, stringCliniqueId);
}


int GetCliniqueState(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringCliniqueId)
{
	if(handle==NULL || stringLocalFilePath==NULL || stringCliniqueId==NULL)
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
	return pNetInterface->GetCliniqueState(stringLocalFilePath, stringCliniqueId);
}

int NotifyCaseState(NET_HANDLE handle, const char* stringHospitalId, const char* stringOrderType, const char* stringOrderId)
{
	if(handle==NULL || stringHospitalId==NULL || stringOrderType==NULL || stringOrderId==NULL)
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
	return pNetInterface->NotifyCaseState(stringHospitalId, stringOrderType, stringOrderId);
}

int NotifyReportState(NET_HANDLE handle, const char* stringReportId)
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
	return pNetInterface->NotifyReportState(stringReportId);
}

int SetDoctorAgentId(NET_HANDLE handle, const char* stringDoctorAgentId)
{
	if(handle==NULL || stringDoctorAgentId==NULL)
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
	return pNetInterface->SetDoctorAgentId(stringDoctorAgentId);
}

int NotifyAppState(NET_HANDLE handle, const char* stringUnitId, const char* stringOrderId, const char* stringOrderType, const char* stringState)
{
	if(handle==NULL || stringUnitId==NULL || stringOrderId==NULL || stringOrderType==NULL || stringState==NULL)
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
	return pNetInterface->NotifyAppState(stringUnitId, stringOrderId, stringOrderType, stringState);
}

int VerifyAppState(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringOrderId, const char* stringOrderType)
{
	if(handle==NULL || stringLocalFilePath==NULL || stringOrderId==NULL || stringOrderType==NULL)
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
	return pNetInterface->VerifyAppState(stringLocalFilePath, stringOrderId, stringOrderType);
}

int GetHisReportState(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringHospitalId, const char* stringOrderType, const char* stringOrderId)
{
	if(handle==NULL || stringHospitalId==NULL || stringOrderType==NULL || stringOrderId==NULL)
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
	return pNetInterface->GetHisReportState(stringLocalFilePath, stringHospitalId, stringOrderType, stringOrderId);
}

int SetHisCriticalValue(NET_HANDLE handle, const char* stringHospitalId, const char* stringCaseId, const char* stringCriticalValues)
{
	if(handle==NULL || stringHospitalId==NULL || stringCaseId==NULL || stringCriticalValues==NULL)
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
	return pNetInterface->SetHisCriticalValue(stringHospitalId, stringCaseId, stringCriticalValues);
}