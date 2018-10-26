#define PHMS_DLL_EXPORTS
#include "TelecomHealthInterface.h"
#include "ExternalNetInterface.h"
#include "ErrorCode.h"
#include <map>
#include <string>
#include "NetThreadManager.h"

using namespace std;

int UserAuthorize_Telecom(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringPersonId, const char* stringName, const char* stringNation, const char* stringAddress, const char* stringRoomId)
{
	if(handle==NULL || stringLocalFilePath==NULL || stringPersonId==NULL)
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
	return pNetInterface->UserAuthorize_Telecom(stringLocalFilePath, stringPersonId, stringName, stringNation, stringAddress, stringRoomId);
}

int BloodPressureUpload_Telecom(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringPersonType, const char* stringPersonId, const char* stringSBP, const char* stringDBP, const char* stringAverageValue, const char* stringPulseRate, const char* stringDetectionTime, 
								const char* stringEquCode, const char* stringRoomId)
{
	if(handle==NULL || stringLocalFilePath==NULL || stringPersonId==NULL || stringSBP==NULL || stringDBP==NULL || stringAverageValue==NULL || stringPulseRate==NULL || stringDetectionTime==NULL || stringEquCode==NULL || stringRoomId==NULL)
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
	return pNetInterface->BloodPressureUpload_Telecom(stringLocalFilePath, stringPersonType, stringPersonId, stringSBP, stringDBP, stringAverageValue, stringPulseRate, stringDetectionTime, stringEquCode, stringRoomId);
}

int BloodOxygenUpload_Telecom(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringPersonType, const char* stringPersonId, const char* stringBloodOxygen, const char* stringPulseRate, const char* stringDetectionTime, 
									  const char* stringEquCode, const char* stringRoomId)
{
	if(handle==NULL || stringLocalFilePath==NULL || stringPersonId==NULL || stringBloodOxygen==NULL || stringPulseRate==NULL || stringDetectionTime==NULL || stringEquCode==NULL || stringRoomId==NULL)
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
	return pNetInterface->BloodOxygenUpload_Telecom(stringLocalFilePath, stringPersonType, stringPersonId, stringBloodOxygen, stringPulseRate, stringDetectionTime, stringEquCode, stringRoomId);
}

int BloodSugarUpload_Telecom(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringPersonType, const char* stringPersonId, const char* stringBloodSugar, const char* stringDetectionTime, 
									 const char* stringEquCode, const char* stringRoomId)
{
	if(handle==NULL || stringLocalFilePath==NULL || stringPersonId==NULL || stringBloodSugar==NULL || stringDetectionTime==NULL || stringEquCode==NULL || stringRoomId==NULL)
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
	return pNetInterface->BloodSugarUpload_Telecom(stringLocalFilePath, stringPersonType, stringPersonId, stringBloodSugar, stringDetectionTime, stringEquCode, stringRoomId);
}

int HeightWeightUpload_Telecom(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringPersonType, const char* stringPersonId, const char* stringHeight, const char* stringWeight, const char* stringBMI, const char* stringDetectionTime, 
							 const char* stringEquCode, const char* stringRoomId)
{
	if(handle==NULL || stringLocalFilePath==NULL || stringPersonId==NULL || stringHeight==NULL || stringWeight==NULL || stringBMI==NULL || stringDetectionTime==NULL || stringEquCode==NULL || stringRoomId==NULL)
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
	return pNetInterface->HeightWeightUpload_Telecom(stringLocalFilePath, stringPersonType, stringPersonId, stringHeight, stringWeight, stringBMI, stringDetectionTime, stringEquCode, stringRoomId);
}


int LungUpload_Telecom(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringPersonType, const char* stringPersonId, const char* stringM_B1, const char* stringBSA, const char* stringBSA1, const char* stringMMF, const char* stringMefSeventyFive, const char* stringMefFifty, 
							   const char* stringMefTwentyFive, const char* stringMefFVST, const char* stringIC, const char* stringIRV, const char* stringERV, const char* stringFevOne, const char* stringFevTwo, const char* stringFevThree, 
							   const char* stringFevOnePer, const char* stringFevTwoPer, const char* stringFevThreePer, const char* stringRR, const char* stringFVC, const char* stringPEF, const char* stringTV, const char* stringVC, const char* stringMVV, 
							   const char* stringMV, const char* stringVSTHeight, const char* stringVSMvvBsa, const char* stringVSMvvOne, const char* stringSVC, const char* stringFevOneZeroFive, const char* stringFevOneZeroFivePer, const char* stringPefOne, 
							   const char* stringPefTwo, const char* stringPefThree, const char* stringFive, const char* stringFiveOne, const char* stringFiveTwo, const char* stringFiveThree, const char* stringPIF, const char* stringMifFifty, 
							   const char* stringDetectionTime, const char* stringEquCode, const char* stringRoomId)
{
	if(handle==NULL || stringLocalFilePath==NULL || stringPersonId==NULL || stringM_B1==NULL || stringBSA==NULL || stringBSA1==NULL || stringMMF==NULL || stringMefSeventyFive==NULL || stringMefFifty==NULL || stringMefTwentyFive==NULL || 
		stringMefFVST==NULL || stringIC==NULL || stringIRV==NULL || stringERV==NULL || stringFevOne==NULL || stringFevTwo==NULL || stringFevThree==NULL || stringFevOnePer==NULL || stringFevTwoPer==NULL || stringFevThreePer==NULL || 
		stringRR==NULL || stringFVC==NULL || stringPEF==NULL || stringTV==NULL || stringVC==NULL || stringMVV==NULL || stringMV==NULL || stringVSTHeight==NULL || stringVSMvvBsa==NULL || stringVSMvvOne==NULL || stringSVC==NULL || 
		stringFevOneZeroFive==NULL || stringFevOneZeroFivePer==NULL || stringPefOne==NULL || stringPefTwo==NULL || stringPefThree==NULL || stringFive==NULL || stringFiveOne==NULL || stringFiveTwo==NULL || stringFiveThree==NULL || 
		stringPIF==NULL || stringMifFifty==NULL || stringDetectionTime==NULL || stringEquCode==NULL || stringRoomId==NULL)
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
	return pNetInterface->LungUpload_Telecom(stringLocalFilePath, stringPersonType, stringPersonId, stringM_B1, stringBSA, stringBSA1, stringMMF, stringMefSeventyFive, stringMefFifty, stringMefTwentyFive, stringMefFVST, stringIC, 
		stringIRV, stringERV, stringFevOne, stringFevTwo, stringFevThree, stringFevOnePer, stringFevTwoPer, stringFevThreePer, stringRR, stringFVC, stringPEF, stringTV, stringVC, stringMVV, stringMV, 
		stringVSTHeight, stringVSMvvBsa, stringVSMvvOne, stringSVC, stringFevOneZeroFive, stringFevOneZeroFivePer, stringPefOne, stringPefTwo, stringPefThree, stringFive, stringFiveOne, stringFiveTwo, stringFiveThree, 
		stringPIF, stringMifFifty, stringDetectionTime, stringEquCode, stringRoomId);
}


int ECGUpload_Telecom(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringPersonType, const char* stringPersonId, const char* stringDetectionTime, 
							  const char* stringEquCode, const char* stringRoomId, TELCOM_ECG ecgData)
{
	if(handle==NULL || stringLocalFilePath==NULL || stringPersonId==NULL || stringDetectionTime==NULL || stringEquCode==NULL || stringRoomId==NULL || 
		ecgData.pArrayData==NULL || ecgData.adRate==NULL || ecgData.adAccuracy==NULL || ecgData.rangeMin==NULL || ecgData.rangeMax==NULL || ecgData.heartValue==NULL || 
		ecgData.stLevel==NULL || ecgData.prInterval==NULL || ecgData.qtInterval==NULL || ecgData.rrInterval==NULL || ecgData.pWave==NULL || ecgData.rWave==NULL || 
		ecgData.tWave==NULL || ecgData.result1==NULL || ecgData.result2==NULL || ecgData.result3==NULL || ecgData.result4==NULL || ecgData.result5==NULL)
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
	return pNetInterface->ECGUpload_Telecom(stringLocalFilePath, stringPersonType, stringPersonId, stringDetectionTime, stringEquCode, stringRoomId, ecgData);
}

