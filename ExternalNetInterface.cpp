#include "ExternalNetInterface.h"
#include "Util.h"
#include "ErrorCode.h"
#include "PhmsLogger.h"
#include "Poco/File.h"
#include "Poco/Util/IniFileConfiguration.h"
#include "Poco/AutoPtr.h"
#include "Poco/Exception.h"
#include <sstream>
#include "AllObjectManagerUtil.h"

using Poco::File;
using Poco::Util::IniFileConfiguration;
using Poco::AutoPtr;
using Poco::FileNotFoundException;



CExternalNetInterface::CExternalNetInterface(void)
{
	m_pInternalNetInterface = new CInternalNetIntetface;
}

CExternalNetInterface::~CExternalNetInterface(void)
{
	delete m_pInternalNetInterface;
}

void CExternalNetInterface::SetSessionId(string stringSessionId)
{
	m_pInternalNetInterface->SetSessionId(stringSessionId);
}
string CExternalNetInterface::GetSessionId()
{
	return m_pInternalNetInterface->GetSessionId();
}
void CExternalNetInterface::SetGWSessionId(string stringSessionId)
{
	m_pInternalNetInterface->SetGWSessionId(stringSessionId);
}
string CExternalNetInterface::GetGWSessionId()
{
	return m_pInternalNetInterface->GetGWSessionId();
}

//专家端接口
int CExternalNetInterface::DoctorLogin(string stringLocalFilePath, string stringUsername, string stringPassword, string stringLanguage, string stringSignPath, 
									   string stringHospitalInfoPath, string stringHospitalPhotoPath, string stringCliniqueListPath, string stringHospitalListPath, string stringDoctorListPath, string& stringIp)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	//登录和下载签名
	m_pInternalNetInterface->SetSaveType(2);
	m_pInternalNetInterface->SetFilePath(stringLocalFilePath);
	nRet = m_pInternalNetInterface->Login("2", stringUsername, stringPassword, stringLanguage, "", true);
	if(nRet==COMMON_SEND_FAIL || nRet==COMMON_SEND_TIMEOUT || nRet==COMMON_RECV_FAIL || nRet==COMMON_RECV_TIMEOUT || nRet==COMMON_URL_NOT_FOUND)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		nRet = m_pInternalNetInterface->Login("2", stringUsername, stringPassword, stringLanguage, "", false);
		if(nRet != PHMS_SUCCESSFUL_RESULT)
		{
			CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
			return nRet;
		}
	}
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	stringIp = m_pInternalNetInterface->GetIp();
	//最新解析出的ip是否与上一次的ip一致，若不一致，设置优先ip
	string stringWorkingDir = CUtil::GetCurrentAppDir();
	try
	{
		AutoPtr<IniFileConfiguration> iniFile = new IniFileConfiguration(stringWorkingDir+"PhmsConfig.ini", "gbk");
		if(iniFile->has("NET.IP"))
		{
			string stringLastIp = iniFile->getString("NET.IP", "");
			if(stringLastIp.size()!=0 && stringLastIp!=stringIp && stringIp.size()!=0)
			{
				CAllObjectManagerUtil::SetPriorIp(stringIp);
			}
		}
	}
	catch(FileNotFoundException& e)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(e, __FUNCTION__, __FILE__, __LINE__);
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_NO_NET_CONFIG_FILE), __FUNCTION__, __FILE__, __LINE__);
		throw;
	}
	string stringSignUri;
	string stringSignSize;
	string stringHospitalId;
	bool bHaveSign = true;
	nRet = CUtil::GetXmlFieldVaule(2, "", stringLocalFilePath, "signature", stringSignUri);
	nRet = CUtil::GetXmlFieldVaule(2, "", stringLocalFilePath, "signsize", stringSignSize);
	nRet = CUtil::GetXmlFieldVaule(2, "", stringLocalFilePath, "hospitalid", stringHospitalId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(stringHospitalId.size() == 0)
	{
		return LOGIC_NO_HOSPITAL_ID;//hospitalid必须得有，没有算错误
	}
	m_pInternalNetInterface->SetSaveType(2);
	m_pInternalNetInterface->SetFilePath(stringCliniqueListPath);
	nRet = m_pInternalNetInterface->GetCliniqueListOfHospital(stringHospitalId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(stringSignUri.size()==0 || stringSignSize.size()==0 || stringSignSize=="0")
	{
		bHaveSign = false;//doctorsign可以没有，没有的话最后返回LOGIC_NO_DOCTOR_SIGN，不影响其他文件的获得
	}
	else
	{
		if(CUtil::StringToULong(stringSignSize)!=CUtil::GetFileSize(stringSignPath) && CUtil::GetFileSize(stringSignPath)!=0)
		{
			File fileSign(stringSignPath, "gbk");
			fileSign.remove(true);
		}
		m_pInternalNetInterface->SetFilePath(stringSignPath);
		nRet = m_pInternalNetInterface->DownloadFile(1, stringSignUri, CUtil::StringToULong(stringSignSize), stringSignPath, NULL);
		if(nRet != PHMS_SUCCESSFUL_RESULT)
		{
			CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
			return nRet;
		}
	}

	//获得医院信息和下载医院照片
	m_pInternalNetInterface->SetSaveType(2);
	m_pInternalNetInterface->SetFilePath(stringHospitalInfoPath);
	nRet = m_pInternalNetInterface->GetHospitalInfo(stringHospitalId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	string stringPhotoUri;
	string stringPhotoSize;
	nRet = CUtil::GetXmlFieldVaule(2, "", stringHospitalInfoPath, "photo", stringPhotoUri);
	nRet = CUtil::GetXmlFieldVaule(2, "", stringHospitalInfoPath, "photosize", stringPhotoSize);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(stringPhotoUri.size()==0 || stringPhotoSize.size()==0)
	{
		//return PHMS_SUCCESSFUL_RESULT;//HospitalPhoto可以没有
	}
	else
	{
		if(CUtil::StringToULong(stringPhotoSize)!=CUtil::GetFileSize(stringHospitalPhotoPath) && CUtil::GetFileSize(stringHospitalPhotoPath)!=0)
		{
			File fileHospitalPhoto(stringHospitalPhotoPath, "gbk");
			fileHospitalPhoto.remove(true);
		}
		m_pInternalNetInterface->SetFilePath(stringHospitalPhotoPath);
		nRet = m_pInternalNetInterface->DownloadFile(1, stringPhotoUri, CUtil::StringToULong(stringPhotoSize), stringHospitalPhotoPath, NULL);
		if(nRet != PHMS_SUCCESSFUL_RESULT)
		{
			CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
			return nRet;
		}
	}

	//获得医院所属医院组织信息
	string stringGroupId;
	string stringSuggestType;
	nRet = CUtil::GetXmlFieldVaule(2, "", stringLocalFilePath, "groupid", stringGroupId);
	nRet = CUtil::GetXmlFieldVaule(2, "", stringLocalFilePath, "suggesttype", stringSuggestType);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(stringSuggestType=="2" && stringGroupId.size()!=0)
	{
		m_pInternalNetInterface->SetSaveType(2);
		m_pInternalNetInterface->SetFilePath(stringHospitalListPath);
		nRet = m_pInternalNetInterface->GetHospitalListOfGroup(stringGroupId, "1", "", 1, 300);//300家医院应该足够了
		if(nRet != PHMS_SUCCESSFUL_RESULT)
		{
			CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
			return nRet;
		}
	}

	//获得医院内医生列表
	m_pInternalNetInterface->SetSaveType(2);
	m_pInternalNetInterface->SetFilePath(stringDoctorListPath);
	nRet = m_pInternalNetInterface->GetDoctorListOfHospital(stringHospitalId, "", "", "", 1, 200);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(!bHaveSign)
	{
		return LOGIC_NO_DOCTOR_SIGN;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CExternalNetInterface::UploadSign(string stringSignPath)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pInternalNetInterface->SetSaveType(0);
	nRet = m_pInternalNetInterface->SetDoctorSign(stringSignPath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}

int CExternalNetInterface::UploadPhoto(string stringPhotoPath)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pInternalNetInterface->SetSaveType(0);
	nRet = m_pInternalNetInterface->SetDoctorPhoto(stringPhotoPath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}

int CExternalNetInterface::GetUnanalyzedCaseList(string stringLocalFilePath, string stringOwnerId, string stringOrderBy, unsigned int nPageTo, unsigned int nPageSize)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pInternalNetInterface->SetSaveType(2);
	m_pInternalNetInterface->SetFilePath(stringLocalFilePath);
	nRet = m_pInternalNetInterface->GetCaseListOfDoctor("", "", "", "2,3,4,5", "", "", stringOwnerId, "", "", "", "", "", "", "", "", "", "", stringOrderBy, "", "", "", nPageTo, nPageSize);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}


int CExternalNetInterface::SearchCaseFromServer(string stringLocalFilePath, string stringCaseId, string stringCaseType, string stringCaseSource, 
						 string stringCaseState, string stringPatientName, string stringSenderId, string stringReceiverId, string stringStartDate, string stringEndDate, 
						 string stringCheckStartDate, string stringCheckEndDate, string stringSenderName, string stringDiagnostic, string stringCaseOrigin, string stringYzlb, string stringAId, 
						 string stringSex, string stringOrderBy, string stringThirdId, string stringLang, string stringIsFullName, unsigned int nPageTo, unsigned int nPageSize)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pInternalNetInterface->SetSaveType(2);
	m_pInternalNetInterface->SetFilePath(stringLocalFilePath);
	nRet = m_pInternalNetInterface->GetCaseListOfDoctor(stringCaseId, stringCaseType, stringCaseSource, stringCaseState, stringPatientName, 
		stringSenderId, stringReceiverId, stringStartDate, stringEndDate, stringCheckStartDate, stringCheckEndDate, stringSenderName, stringDiagnostic, 
		stringCaseOrigin, stringYzlb, stringAId, stringSex, stringOrderBy, stringThirdId, stringLang, stringIsFullName, nPageTo, nPageSize);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}

int CExternalNetInterface::GetAutoCaseInfo(string stringLocalFilePath,string stringCaseType,string stringSenderId,string stringCaseOrigin)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pInternalNetInterface->SetSaveType(2);
	m_pInternalNetInterface->SetFilePath(stringLocalFilePath);
	nRet = m_pInternalNetInterface->GetAutoDownloadCase( stringCaseType, stringSenderId, stringCaseOrigin);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}

// int CExternalNetInterface::AutoDownloadCase(string stringLocalFilePath, string stringCaseId, string stringReceiverId, string stringUri, unsigned long nFileSize, 
// 											pProgressAndSpeedCallback pCallback)
// {
// 	int nRet = PHMS_SUCCESSFUL_RESULT;
// 	m_pInternalNetInterface->SetSaveType(0);
// 	nRet = m_pInternalNetInterface->DownloadCaseBegin(stringCaseId, stringReceiverId);
// 
// 	if(nRet != PHMS_SUCCESSFUL_RESULT)
// 	{
// 		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
// 		return nRet;
// 	}
// 
// 	m_pInternalNetInterface->SetFilePath(stringLocalFilePath);
// 	nRet = m_pInternalNetInterface->DownloadFile(3, stringUri, nFileSize, stringLocalFilePath, pCallback);
// 	if(nRet != PHMS_SUCCESSFUL_RESULT)
// 	{
// 		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
// 		return nRet;
// 	}
// 
// 	m_pInternalNetInterface->SetSaveType(0);
// 	nRet = m_pInternalNetInterface->DownloadCaseComplete(stringCaseId, stringReceiverId);
// 	if(nRet != PHMS_SUCCESSFUL_RESULT)
// 	{
// 		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
// 		return nRet;
// 	}
// 	
// 	
// 	return nRet;
// }

int CExternalNetInterface::ManualDownloadCase(string stringLocalFilePath, string stringCaseId, string stringHospitalId, string stringReceiverId, string stringUri, unsigned long nFileSize, 
											  pProgressAndSpeedCallback pCallback, bool bDownloadNotSelfOwn)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	//获得病例在服务器端的状态，从而决定是否需要进行占位或开始下载或下载完成
	string stringServerReceiverId;
	string stringServerCaseState;
	string stringCaseType;
	int nCaseState;
	bool bHoldPlace = true;
	bool bBeginDownload = true;
	bool bCompleteDownload = true;
	m_pInternalNetInterface->SetSaveType(1);
	nRet = m_pInternalNetInterface->GetCaseListOfDoctor(stringCaseId, "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 1, 1);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = CUtil::GetXmlFieldVaule(1, m_pInternalNetInterface->GetContent(), "", "receiverid", stringServerReceiverId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = CUtil::GetXmlFieldVaule(1, m_pInternalNetInterface->GetContent(), "", "casestate", stringServerCaseState);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = CUtil::GetXmlFieldVaule(1, m_pInternalNetInterface->GetContent(), "", "casetype", stringCaseType);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	try
	{
		istringstream iStringStream(stringServerCaseState);
		iStringStream.exceptions(ios_base::failbit|ios_base::badbit);
		iStringStream>>nCaseState;
	}
	catch(const ios::failure& e)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(e.what(), __FUNCTION__, __FILE__, __LINE__);
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_GET_CASE_STATE_ERROR), __FUNCTION__, __FILE__, __LINE__);
		return COMMON_GET_CASE_STATE_ERROR;
	}
	if(stringHospitalId.compare(stringServerReceiverId) != 0)
	{
		//已占位病例
		if(stringReceiverId.compare(stringServerReceiverId) != 0)
		{
			//他人占位
			if(bDownloadNotSelfOwn)
			{
				//可以继续下载，但是不执行改变病例状态的操作
				bHoldPlace = false;
				bBeginDownload = false;
				bCompleteDownload = false;
			}
			else
			{
				CPhmsLogger::GetPhmsLogger()->WriteLog(stringReceiverId+":"+stringServerReceiverId, __FUNCTION__, __FILE__, __LINE__);
				CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(NOT_HODL_BY_YOURSELF), __FUNCTION__, __FILE__, __LINE__);
				return NOT_HODL_BY_YOURSELF;
			}
		}
		else
		{
			//自己占位
			if(nCaseState>CASE_STAT_DOWNLOADING)
			{
				bHoldPlace = false;
				bBeginDownload = false;
				bCompleteDownload = false;
			}
			if(nCaseState==CASE_STAT_DOWNLOADING)
			{
				bHoldPlace = false;
				bBeginDownload = false;
				bCompleteDownload = true;
			}
			if(nCaseState<CASE_STAT_DOWNLOADING)
			{
				bHoldPlace = false;
				bBeginDownload = true;
				bCompleteDownload = true;
			}
		}
	}
	if(bHoldPlace)
	{
		//只有公共病例才执行占位
		m_pInternalNetInterface->SetSaveType(0);
		nRet = m_pInternalNetInterface->CaseHoldPlace(stringCaseId, stringHospitalId);
		if(nRet != PHMS_SUCCESSFUL_RESULT)
		{
			CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
			return nRet;
		}
	}
	if(bBeginDownload)
	{
		m_pInternalNetInterface->SetSaveType(0);
		nRet = m_pInternalNetInterface->DownloadCaseBegin(stringCaseId, stringReceiverId);
		if(nRet != PHMS_SUCCESSFUL_RESULT)
		{
			CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
			return nRet;
		}
	}
	m_pInternalNetInterface->SetFilePath(stringLocalFilePath);
	nRet = m_pInternalNetInterface->DownloadFile(3, stringUri, nFileSize, stringLocalFilePath, pCallback);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(bCompleteDownload)
	{
		m_pInternalNetInterface->SetSaveType(0);
		nRet = m_pInternalNetInterface->DownloadCaseComplete(stringCaseId, stringReceiverId);
		if(nRet != PHMS_SUCCESSFUL_RESULT)
		{
			CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
			return nRet;
		}
	}
	if(stringCaseType == "23")
	{
		//图文咨询病例
		vector<string> vImagePath;
		nRet = CUtil::GetOrReplaceImagePathVector(2, "", stringLocalFilePath, vImagePath);
		if(nRet != PHMS_SUCCESSFUL_RESULT)
		{
			CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
			return nRet;
		}
		string stringImgDir = stringLocalFilePath + "_IMG";
		File fileDir(stringImgDir, "gbk");
		try
		{
			fileDir.createDirectory();
		}
		catch(const Exception& e)
		{
			CPhmsLogger::GetPhmsLogger()->WriteLog(e.what(), __FUNCTION__, __FILE__, __LINE__);
			CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_CREATE_DIR_FAIL), __FUNCTION__, __FILE__, __LINE__);
			return COMMON_CREATE_DIR_FAIL;
		}
		vector<string> vImageLocalPath;
		vector<string>::iterator iter;
		for(iter=vImagePath.begin(); iter!=vImagePath.end(); iter++)
		{
			string stringLocalImagePath = stringImgDir + "/" + CUtil::GetMd5(iter->c_str(), iter->size());
			nRet = DownloadPhoto(stringLocalImagePath, *iter, 0, 0);
			if(nRet != PHMS_SUCCESSFUL_RESULT)
			{
				CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
				return nRet;
			}
			/**iter = stringLocalImagePath;*/
			vImageLocalPath.push_back(stringLocalImagePath);
			
		}
		for(int i=0; i<vImageLocalPath.size(); i++)
		{
			CUtil::ReplaceStringInFile(stringLocalFilePath, vImagePath[i], vImageLocalPath[i]);
		}
// 		nRet = CUtil::GetOrReplaceImagePathVector(2, "", stringLocalFilePath, vImagePath);
// 		if(nRet != PHMS_SUCCESSFUL_RESULT)
// 		{
// 			CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
// 			return nRet;
// 		}
	}
	return nRet;
}

int CExternalNetInterface::RollbackCase(string stringCaseId, string stringReceiverId, string stringBackTo)
{
	m_pInternalNetInterface->SetSaveType(0);
	int nRet = PHMS_SUCCESSFUL_RESULT;
	nRet = m_pInternalNetInterface->RollbackCase(stringCaseId, stringReceiverId, stringBackTo);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}

int CExternalNetInterface::TransmitCase(string stringCaseId, string stringReceiverId, string stringForwardToH, string stringForwardToHospitalName, 
										string stringForwardToD, string stringForwardToDoctorName)
{
	m_pInternalNetInterface->SetSaveType(0);
	int nRet = PHMS_SUCCESSFUL_RESULT;
	nRet = m_pInternalNetInterface->TransmitCase(stringCaseId, stringReceiverId, stringForwardToH, stringForwardToHospitalName, stringForwardToD, stringForwardToDoctorName);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}

int CExternalNetInterface::InvalidCase(string stringCaseId, string stringReceiverId, string stringReason)
{
	m_pInternalNetInterface->SetSaveType(0);
	int nRet = PHMS_SUCCESSFUL_RESULT;
	nRet = m_pInternalNetInterface->InvalidCase(stringCaseId, stringReceiverId, stringReason);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}

int CExternalNetInterface::AnalyzeBegin(string stringCaseId, string stringReceiverId, string stringCaseType)
{
	m_pInternalNetInterface->SetSaveType(0);
	int nRet = PHMS_SUCCESSFUL_RESULT;
	nRet = m_pInternalNetInterface->AnalyzeBegin(stringCaseId, stringReceiverId, stringCaseType);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}

int CExternalNetInterface::AnalyzeComplete(string stringCaseId, string stringReceiverId, string stringCaseType)
{
	m_pInternalNetInterface->SetSaveType(0);
	int nRet = PHMS_SUCCESSFUL_RESULT;
	nRet = m_pInternalNetInterface->AnalyzeComplete(stringCaseId, stringReceiverId, stringCaseType);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}

int CExternalNetInterface::UploadReport(string& stringReportId, string stringCaseId, string stringReceiverId, string stringDiagnosis, string stringDigitalSign, string stringTimestampSign, string stringCaseType, string stringLocalReportPath, int nResultFlag, 
										string stringFileType, pProgressAndSpeedCallback pCallback)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pInternalNetInterface->SetSaveType(0);
	//判断诊断结论中是否有非法的GBK编码字符
	string stringWorkingDir = CUtil::GetCurrentAppDir();
	string stringIniFilePath = stringWorkingDir+"PhmsConfig.ini";
	AutoPtr<IniFileConfiguration> iniFile = new IniFileConfiguration(stringIniFilePath, "gbk");
	string stringEncoding = iniFile->getString("OTHER.Encoding", "GBK");
	if(stringEncoding=="GBK" || stringEncoding=="gbk")
	{
		if(!CUtil::IsLegalGBK(stringDiagnosis))
		{
			CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_ILLEGAL_GBK_ENCODING), __FUNCTION__, __FILE__, __LINE__);
			CPhmsLogger::GetPhmsLogger()->WriteLog(stringDiagnosis, __FUNCTION__, __FILE__, __LINE__);
			return COMMON_ILLEGAL_GBK_ENCODING;
		}
	}
	/*if(bExceptionCase == 1)*/
	{
		nRet = m_pInternalNetInterface->MarkCaseException(stringCaseId, stringReceiverId, nResultFlag);
		if(nRet != PHMS_SUCCESSFUL_RESULT)
		{
			CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
			return nRet;
		}
	}
	m_pInternalNetInterface->SetSaveType(1);
	if(stringReportId.size() == 0)
	{
		string stringMd5 = CUtil::GetMd5(stringLocalReportPath.c_str(), stringLocalReportPath.size());
		nRet = m_pInternalNetInterface->ApplyNewReport(stringCaseId, stringReceiverId, stringDiagnosis, stringMd5, stringDigitalSign, stringTimestampSign, stringCaseType);
		if(nRet != PHMS_SUCCESSFUL_RESULT)
		{
			CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
			return nRet;
		}
		stringReportId = m_pInternalNetInterface->GetContent();
	}
	nRet = m_pInternalNetInterface->UploadReport(stringCaseId, stringReceiverId, stringCaseType, stringReportId, stringLocalReportPath, stringFileType, pCallback);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}

int CExternalNetInterface::DoctorDownloadReport(string stringLocalFilePath, string stringUri, unsigned long nFileSize, pProgressAndSpeedCallback pCallback)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	nRet = m_pInternalNetInterface->DownloadFile(3, stringUri, nFileSize, stringLocalFilePath, pCallback);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}

int CExternalNetInterface::UploadAdvice(string stringUserId, string stringUsername, string stringMsg, string stringType, string stringIsSendMsg)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pInternalNetInterface->SetSaveType(0);
	nRet = m_pInternalNetInterface->UploadAdvice(stringUserId, stringUsername, stringMsg, stringType, stringIsSendMsg);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		return nRet;
	}
	return nRet;
}


int CExternalNetInterface::DoctorChangeInfo(string stringName, string stringSex, string stringTel, string stringBirthyead, string stringPosition, string stringNotes)
{
	m_pInternalNetInterface->SetSaveType(0);
	int nRet = PHMS_SUCCESSFUL_RESULT;
	nRet = m_pInternalNetInterface->DoctorChangeInfo(stringName, stringSex, stringTel, stringBirthyead, stringPosition, stringNotes);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}

//接口有变动，加了一个参数senderid，表示要查找信息的人，如为空表示自己
int CExternalNetInterface::DoctorGetSelfInfo(string stringLocalFilePath, string stringSenderId)
{
	m_pInternalNetInterface->SetSaveType(2);
	m_pInternalNetInterface->SetFilePath(stringLocalFilePath);
	int nRet = PHMS_SUCCESSFUL_RESULT;
	nRet = m_pInternalNetInterface->GetSelfInfo(stringSenderId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}

int CExternalNetInterface::GetReviewReportList(string stringLocalFilePath, string stringReportStartDate, string stringReportEndDate,string stringReviewStartDate,
											   string stringReviewEndDate, string stringReviewState, string stringCaseType, string stringPtName, string stringSenderId, string stringYzlb, string stringAId, 
											   string stringReviewerId, string stringReceiverId, string stringCaseId, unsigned int nPageTo, unsigned int nPageSize)
{
	m_pInternalNetInterface->SetSaveType(2);
	m_pInternalNetInterface->SetFilePath(stringLocalFilePath);
	int nRet = PHMS_SUCCESSFUL_RESULT;
	nRet = m_pInternalNetInterface->GetReviewReportList(stringReportStartDate, stringReportEndDate, stringReviewStartDate, stringReviewEndDate, stringReviewState, 
		stringCaseType, stringPtName, stringSenderId, stringYzlb, stringAId, stringReviewerId, stringReceiverId, stringCaseId, nPageTo, nPageSize);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}

int CExternalNetInterface::ReviewReport(string stringReportKeyId, string stringReviewState, string stringPath, string stringDiagnostic, 
										string stringDigitalSign, string stringTimeStampSign)
{
	m_pInternalNetInterface->SetSaveType(0);
	int nRet = PHMS_SUCCESSFUL_RESULT;
	nRet = m_pInternalNetInterface->ReviewReport(stringReportKeyId, stringReviewState, stringPath, stringDiagnostic, stringDigitalSign, stringTimeStampSign);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}

int CExternalNetInterface::GetCardUserList (string stringLocalFilePath, string stringCardId, string stringSenderId, string stringPersonId, string stringTel, string stringName,
					 string stringType, unsigned int nPageTo, unsigned int nPageSize)
{
	m_pInternalNetInterface->SetSaveType(2);
	m_pInternalNetInterface->SetFilePath(stringLocalFilePath);
	int nRet = PHMS_SUCCESSFUL_RESULT;
	nRet = m_pInternalNetInterface->GetCardUserList(stringCardId, stringSenderId, stringPersonId, stringTel, stringName, stringType, nPageTo, nPageSize);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}

int CExternalNetInterface::MarkCaseException(string stringCaseId, string stringReceiverId, int nResultFlag)
{
	m_pInternalNetInterface->SetSaveType(0);
	int nRet = PHMS_SUCCESSFUL_RESULT;
	nRet = m_pInternalNetInterface->MarkCaseException(stringCaseId, stringReceiverId, nResultFlag);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}

int CExternalNetInterface::GetAllTeamList(string stringLocalFilePath)
{
	m_pInternalNetInterface->SetSaveType(2);
	m_pInternalNetInterface->SetFilePath(stringLocalFilePath);
	int nRet = PHMS_SUCCESSFUL_RESULT;
	nRet = m_pInternalNetInterface->GetAllTeamList();
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}

int CExternalNetInterface::GetDoctorListOfTeam(string stringLocalFilePath, string stringTeamId)
{
	m_pInternalNetInterface->SetSaveType(2);
	m_pInternalNetInterface->SetFilePath(stringLocalFilePath);
	int nRet = PHMS_SUCCESSFUL_RESULT;
	nRet = m_pInternalNetInterface->GetDoctorListOfTeam(stringTeamId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}
int CExternalNetInterface::DoctorHandledCaseStatistics(string stringLocalFilePath, string stringCaseType, string stringStartTime, string stringEndTime)
{
	m_pInternalNetInterface->SetSaveType(2);
	m_pInternalNetInterface->SetFilePath(stringLocalFilePath);
	int nRet = PHMS_SUCCESSFUL_RESULT;
	nRet = m_pInternalNetInterface->DoctorHandledCaseStatistics(stringCaseType, stringStartTime, stringEndTime);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}
int CExternalNetInterface::DoctorDownloadCaseAndOneReportOfOldCm(string stringLocalCasePath, string stringLocalReportPath, string stringCaseId, string stringReportUri, unsigned long nReportSize, pProgressAndSpeedCallback pCallback)
{
	m_pInternalNetInterface->SetSaveType(1);
	int nRet = PHMS_SUCCESSFUL_RESULT;
	nRet = m_pInternalNetInterface->GetCaseListOfDoctor(stringCaseId, "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 1, 1);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	string stringCaseUri;
	string stringCaseSize;
	unsigned long nCaseSize = 0;
	nRet = CUtil::GetXmlFieldVaule(1, m_pInternalNetInterface->GetContent(), "", "path", stringCaseUri);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = CUtil::GetXmlFieldVaule(1, m_pInternalNetInterface->GetContent(), "", "size", stringCaseSize);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	try
	{
		istringstream iStringStream(stringCaseSize);
		iStringStream.exceptions(ios_base::failbit|ios_base::badbit);
		iStringStream>>nCaseSize;
	}
	catch(const ios::failure& e)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(e.what(), __FUNCTION__, __FILE__, __LINE__);
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_GET_CASE_SIZE_ERROR), __FUNCTION__, __FILE__, __LINE__);
		return COMMON_GET_CASE_SIZE_ERROR;
	}
	m_pInternalNetInterface->SetSaveType(2);
	m_pInternalNetInterface->SetFilePath(stringLocalCasePath);
	nRet = m_pInternalNetInterface->DownloadFile(1, stringCaseUri, nCaseSize, stringLocalCasePath, pCallback);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = m_pInternalNetInterface->DownloadFile(1, stringReportUri, nReportSize, stringLocalReportPath, pCallback);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}
int CExternalNetInterface::GetTrendData(string stringLocalFilePath, string stringUId, string stringInterUId, string stringTrendType, string stringStartDate, string stringEndDate, string stringNextId)
{
	m_pInternalNetInterface->SetSaveType(2);
	m_pInternalNetInterface->SetFilePath(stringLocalFilePath);
	int nRet = PHMS_SUCCESSFUL_RESULT;
	nRet = m_pInternalNetInterface->GetTrendData(stringUId, stringInterUId, stringTrendType, stringStartDate, stringEndDate, stringNextId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}
int CExternalNetInterface::GetHealthDegree(string stringLocalFilePath, string stringUId, string stringInterUId, string stringDate)
{
	m_pInternalNetInterface->SetSaveType(2);
	m_pInternalNetInterface->SetFilePath(stringLocalFilePath);
	int nRet = PHMS_SUCCESSFUL_RESULT;
	nRet = m_pInternalNetInterface->GetHealthDegree(stringUId, stringInterUId, stringDate);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}
int CExternalNetInterface::GetUserOfNoAdviceAndMostData(string stringLocalFilePath, string stringDays)
{
	m_pInternalNetInterface->SetSaveType(2);
	m_pInternalNetInterface->SetFilePath(stringLocalFilePath);
	int nRet = PHMS_SUCCESSFUL_RESULT;
	nRet = m_pInternalNetInterface->GetUserOfNoAdviceAndMostData(stringDays);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}
int CExternalNetInterface::AdviceDelay(string stringUId, string stringMinutes)
{
	m_pInternalNetInterface->SetSaveType(0);
	int nRet = PHMS_SUCCESSFUL_RESULT;
	nRet = m_pInternalNetInterface->AdviceDelay(stringUId, stringMinutes);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}
int CExternalNetInterface::AdviceComplete(string stringUId, string stringOptionType, string stringAdvice)
{
	m_pInternalNetInterface->SetSaveType(0);
	int nRet = PHMS_SUCCESSFUL_RESULT;
	nRet = m_pInternalNetInterface->AdviceComplete(stringUId, stringOptionType, stringAdvice);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}
int CExternalNetInterface::GetUserListOfNoAdvice(string stringLocalFilePath, string stringDays, unsigned int nPageTo, unsigned int nPageSize)
{
	m_pInternalNetInterface->SetSaveType(2);
	m_pInternalNetInterface->SetFilePath(stringLocalFilePath);
	int nRet = PHMS_SUCCESSFUL_RESULT;
	nRet = m_pInternalNetInterface->GetUserListOfNoAdvice(stringDays, nPageTo, nPageSize);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}
int CExternalNetInterface::AdviceBegin(string stringUId)
{
	m_pInternalNetInterface->SetSaveType(0);
	int nRet = PHMS_SUCCESSFUL_RESULT;
	nRet = m_pInternalNetInterface->AdviceBegin(stringUId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}
int CExternalNetInterface::GetUserListOfAdvicing(string stringLocalFilePath, unsigned int nPageTo, unsigned int nPageSize)
{
	m_pInternalNetInterface->SetSaveType(2);
	m_pInternalNetInterface->SetFilePath(stringLocalFilePath);
	int nRet = PHMS_SUCCESSFUL_RESULT;
	nRet = m_pInternalNetInterface->GetUserListOfAdvicing(nPageTo, nPageSize);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}
int CExternalNetInterface::GetAskList(string stringLocalFilePath, string stringAskType, string stringAskEnd, string stringDoctorId, string stringIsAsk, string stringUserId, string stringStartTime, string stringEndTime, unsigned int nPageTo, unsigned int nPageSize)
{
	m_pInternalNetInterface->SetSaveType(2);
	m_pInternalNetInterface->SetFilePath(stringLocalFilePath);
	int nRet = PHMS_SUCCESSFUL_RESULT;
	nRet = m_pInternalNetInterface->GetAskList(stringAskType, stringAskEnd, stringDoctorId, stringIsAsk, stringUserId, stringStartTime, stringEndTime, nPageTo, nPageSize);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}
int CExternalNetInterface::GetAskDetail(string stringLocalFilePath, string stringAskId)
{
	m_pInternalNetInterface->SetSaveType(2);
	m_pInternalNetInterface->SetFilePath(stringLocalFilePath);
	int nRet = PHMS_SUCCESSFUL_RESULT;
	nRet = m_pInternalNetInterface->GetAskDetail(stringAskId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}
int CExternalNetInterface::SubmitQuestion(string stringUserId, string stringAdvise, string stringReportType, string stringAskId, string stringAskedId, string stringDoctorPhotoPath)
{
	m_pInternalNetInterface->SetSaveType(0);
	int nRet = PHMS_SUCCESSFUL_RESULT;
	nRet = m_pInternalNetInterface->SubmitQuestion(stringUserId, stringAdvise, stringReportType, stringAskId, stringAskedId, stringDoctorPhotoPath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}
int CExternalNetInterface::GetCliniqueListOfHospital(string stringLocalFilePath, string stringHospitalId)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pInternalNetInterface->SetSaveType(2);
	m_pInternalNetInterface->SetFilePath(stringLocalFilePath);
	nRet = m_pInternalNetInterface->GetCliniqueListOfHospital(stringHospitalId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}
int CExternalNetInterface::GetStatisticsInfo(string stringLocalFilePath, string stringTypeId, string stringMonth, string stringMonthDay, unsigned int nPageTo, unsigned int nPageSize)
{
	m_pInternalNetInterface->SetSaveType(2);
	m_pInternalNetInterface->SetFilePath(stringLocalFilePath);
	int nRet = PHMS_SUCCESSFUL_RESULT;
	nRet = m_pInternalNetInterface->GetStatisticsInfo(stringTypeId, stringMonth, stringMonthDay, nPageTo, nPageSize);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}
int CExternalNetInterface::GetUserHealthList(string stringLocalFilePath, string stringNextId, string stringTypeId, string stringSecondTypeId, string stringName, string stringSex, string stringThirdIdType, 
					  string stringThirdId, string stringHospitalId, string stringUnitSenderId, string stringIsData, string stringIntervalTime, string stringPopulations, string stringContractDoctor, string stringIsContract, string stringPageSize)
{
	m_pInternalNetInterface->SetSaveType(2);
	m_pInternalNetInterface->SetFilePath(stringLocalFilePath);
	int nRet = PHMS_SUCCESSFUL_RESULT;
	nRet = m_pInternalNetInterface->GetUserHealthList(stringNextId, stringTypeId, stringSecondTypeId, stringName, stringSex, stringThirdIdType, stringThirdId, stringHospitalId, stringUnitSenderId, stringIsData, stringIntervalTime, 
		stringPopulations, stringContractDoctor, stringIsContract, stringPageSize);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}
int CExternalNetInterface::GetAdviceImplemention(string& stringPercent, string stringThirdId, string stringHospitalId)
{
	m_pInternalNetInterface->SetSaveType(1);
	int nRet = PHMS_SUCCESSFUL_RESULT;
	nRet = m_pInternalNetInterface->GetAdviceImplemention(stringThirdId, stringHospitalId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	stringPercent = m_pInternalNetInterface->GetContent();
	return nRet;
}
int CExternalNetInterface::ChangePersonType(string stringThirdId, string stringPeopleType, string stringHospitalId)
{
	m_pInternalNetInterface->SetSaveType(0);
	int nRet = PHMS_SUCCESSFUL_RESULT;
	nRet = m_pInternalNetInterface->ChangePersonType(stringThirdId, stringPeopleType, stringHospitalId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}
int CExternalNetInterface::SetNextVisitTime(string stringThirdId, string stringInterViewTime, string stringHospitalId)
{
	m_pInternalNetInterface->SetSaveType(0);
	int nRet = PHMS_SUCCESSFUL_RESULT;
	nRet = m_pInternalNetInterface->SetNextVisitTime(stringThirdId, stringInterViewTime, stringHospitalId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}
int CExternalNetInterface::GetPersonTypeList(string stringLocalFilePath, string stringLanguage)
{
	m_pInternalNetInterface->SetSaveType(2);
	m_pInternalNetInterface->SetFilePath(stringLocalFilePath);
	int nRet = PHMS_SUCCESSFUL_RESULT;
	nRet = m_pInternalNetInterface->GetPersonTypeList(stringLanguage);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}
int CExternalNetInterface::GetTrendRangeValue(string stringLocalFilePath)
{
	m_pInternalNetInterface->SetSaveType(2);
	m_pInternalNetInterface->SetFilePath(stringLocalFilePath);
	int nRet = PHMS_SUCCESSFUL_RESULT;
	nRet = m_pInternalNetInterface->GetTrendRangeValue();
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}
int CExternalNetInterface::OnlyDownloadCase(string stringLocalFilePath, string stringCaseId, string stringReceiverId, string stringUri, unsigned long nFileSize, pProgressAndSpeedCallback pCallback)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pInternalNetInterface->SetSaveType(0);
	nRet = m_pInternalNetInterface->DownloadCaseBegin(stringCaseId, stringReceiverId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
	}
	
	m_pInternalNetInterface->SetSaveType(2);
	m_pInternalNetInterface->SetFilePath(stringLocalFilePath);
	nRet = m_pInternalNetInterface->DownloadFile(3, stringUri, nFileSize, stringLocalFilePath, pCallback);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}

	m_pInternalNetInterface->SetSaveType(0);
	m_pInternalNetInterface->DownloadCaseComplete(stringCaseId, stringReceiverId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
	}
	return nRet;
}
int CExternalNetInterface::HoldPlaceAndAnalyzeBegin(string stringCaseId, string stringHospitalId, string stringReceiverId, string stringCaseType)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pInternalNetInterface->SetSaveType(0);
	nRet = m_pInternalNetInterface->CaseHoldPlace(stringCaseId, stringHospitalId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}

	m_pInternalNetInterface->SetSaveType(0);
	nRet = m_pInternalNetInterface->AnalyzeBegin(stringCaseId, stringReceiverId, stringCaseType);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}

int CExternalNetInterface::ReviewReportWithFile(string stringCaseId, string stringReceiverId, string stringReportKeyId, string stringReviewState, string stringLocalFilePath, string stringDiagnostic, 
										string stringDigitalSign, string stringTimeStampSign, string stringResultFlag, string stringFileType, pProgressAndSpeedCallback pCallback)
{
	m_pInternalNetInterface->SetSaveType(0);
	int nRet = PHMS_SUCCESSFUL_RESULT;
	nRet = m_pInternalNetInterface->MarkCaseException(stringCaseId, stringReceiverId, CUtil::StringToULong(stringResultFlag));
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	m_pInternalNetInterface->SetSaveType(1);
	string stringUri;
	nRet = m_pInternalNetInterface->UploadCommonFile(stringUri, "2", stringReportKeyId, stringLocalFilePath, stringFileType, pCallback);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	m_pInternalNetInterface->SetSaveType(0);
	nRet = m_pInternalNetInterface->ReviewReport(stringReportKeyId, stringReviewState, stringUri, stringDiagnostic, stringDigitalSign, stringTimeStampSign);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}
//公共接口
int CExternalNetInterface::KeepSession()
{
	m_pInternalNetInterface->SetSaveType(0);
	int nRet = PHMS_SUCCESSFUL_RESULT;
	nRet = m_pInternalNetInterface->KeepSession();
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}
int CExternalNetInterface::Logout()
{
	m_pInternalNetInterface->SetSaveType(0);
	int nRet = PHMS_SUCCESSFUL_RESULT;
	nRet = m_pInternalNetInterface->Logout();
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}

int CExternalNetInterface::UploadIm(string stringSenderId, string stringSenderName, string stringReceiverId, string stringMsgDirection, string stringMsgType, 
			 string stringMsgContent, string stringAudioFilePath, string stringCaseId, string stringCaseType)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	if(stringMsgType=="1" || stringMsgType=="2" || stringMsgType=="3")
	{
		m_pInternalNetInterface->SetSaveType(1);
		string stringUri;
		nRet = m_pInternalNetInterface->UploadCommonFile(stringUri, "FTYPE_MSG", "", stringAudioFilePath, "", NULL);
		if(nRet != PHMS_SUCCESSFUL_RESULT)
		{
			CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
			return nRet;
		}
		stringMsgContent = stringUri;
	}
	m_pInternalNetInterface->SetSaveType(0);
	nRet = m_pInternalNetInterface->UploadIm(stringSenderId, stringSenderName, stringReceiverId, stringMsgDirection, stringMsgType, stringMsgContent, stringCaseId, stringCaseType);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	
// 	nRet = m_pInternalNetInterface->UploadIm(stringSenderId, stringSenderName, stringReceiverId, stringMsgDirection, stringMsgType, stringMsgContent, stringCaseId, stringCaseType);
// 	if(nRet != PHMS_SUCCESSFUL_RESULT)
// 	{
// 		return nRet;
// 	}
// 	string stringMsgId = m_pInternalNetInterface->GetContent();
// 	if(stringMsgType == "1" || stringMsgType == "2")
// 	{
// 		nRet = m_pInternalNetInterface->UploadAudio(stringMsgId, stringAudioFilePath, NULL);
// 		if(nRet != PHMS_SUCCESSFUL_RESULT)
// 		{
// 			CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
// 			return nRet;
// 		}
// 	}
	return nRet;
}

int CExternalNetInterface::GetImList(string stringLocalFilePath, string stringSenderId, string stringCaseId, string stringCaseType, string stringMsgDirection, string stringMsgType)
{
	m_pInternalNetInterface->SetSaveType(2);
	m_pInternalNetInterface->SetFilePath(stringLocalFilePath);
	int nRet = PHMS_SUCCESSFUL_RESULT;
	nRet = m_pInternalNetInterface->GetImList(stringSenderId, stringCaseId, stringCaseType, stringMsgDirection, stringMsgType);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}

int CExternalNetInterface::GetImList2(string stringLocalFilePath, string stringNextId)
{
	m_pInternalNetInterface->SetSaveType(2);
	m_pInternalNetInterface->SetFilePath(stringLocalFilePath);
	int nRet = PHMS_SUCCESSFUL_RESULT;
	nRet = m_pInternalNetInterface->GetImList2(stringNextId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}

int CExternalNetInterface::DownloadIm(string stringLocalFilePath, string stringMsgId, string stringUri, unsigned long nFileSize)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	nRet = m_pInternalNetInterface->DownloadFile(3, stringUri, nFileSize, stringLocalFilePath, NULL);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	m_pInternalNetInterface->SetSaveType(0);
	nRet = m_pInternalNetInterface->DownloadImComplete(stringMsgId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}

int CExternalNetInterface::DownloadImComplete(string stringMsgId)
{
	m_pInternalNetInterface->SetSaveType(0);
	int nRet = PHMS_SUCCESSFUL_RESULT;
	nRet = m_pInternalNetInterface->DownloadImComplete(stringMsgId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}

int CExternalNetInterface::ChangePwd(string stringOldPwd, string stringNewPwd)
{
	m_pInternalNetInterface->SetSaveType(0);
	int nRet = PHMS_SUCCESSFUL_RESULT;
	nRet = m_pInternalNetInterface->ChangePwd(stringOldPwd, stringNewPwd);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}

int CExternalNetInterface::Feedback(string stringTitle, string stringNotes, string stringPhone, string stringPicPath)
{
	m_pInternalNetInterface->SetSaveType(0);
	int nRet = PHMS_SUCCESSFUL_RESULT;
	nRet = m_pInternalNetInterface->Feedback(stringTitle, stringNotes, stringPhone, stringPicPath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}

int CExternalNetInterface::GetReportListOfCase(string stringLocalFilePath, string stringCaseId)
{
	m_pInternalNetInterface->SetSaveType(2);
	m_pInternalNetInterface->SetFilePath(stringLocalFilePath);
	int nRet = PHMS_SUCCESSFUL_RESULT;
	nRet = m_pInternalNetInterface->GetReportListOfCase(stringCaseId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}

int CExternalNetInterface::GetHospitalListOfGroup(string stringLocalFilePath, string stringGroupId, string stringType, string stringCaseType, unsigned int nPageTo, unsigned int nPageSize)
{
	m_pInternalNetInterface->SetSaveType(2);
	m_pInternalNetInterface->SetFilePath(stringLocalFilePath);
	int nRet = PHMS_SUCCESSFUL_RESULT;
	nRet = m_pInternalNetInterface->GetHospitalListOfGroup(stringGroupId, stringType, stringCaseType, nPageTo, nPageSize);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}

int CExternalNetInterface::GetDoctorListOfHospital(string stringLocalFilePath, string stringHospitalId, string stringDoctorId, string stringCaseType, string stringDoctorType, unsigned int nPageTo, unsigned int nPageSize)
{
	m_pInternalNetInterface->SetSaveType(2);
	m_pInternalNetInterface->SetFilePath(stringLocalFilePath);
	int nRet = PHMS_SUCCESSFUL_RESULT;
	nRet = m_pInternalNetInterface->GetDoctorListOfHospital(stringHospitalId, stringDoctorId, stringCaseType, stringDoctorType, nPageTo, nPageSize);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}

int CExternalNetInterface::AbortSession()
{
	return m_pInternalNetInterface->AbortSession();
}

int CExternalNetInterface::GetEventStream(string stringLocalFilePath, string stringCaseId, string stringOriginalHospital)
{
	m_pInternalNetInterface->SetSaveType(2);
	m_pInternalNetInterface->SetFilePath(stringLocalFilePath);
	int nRet = PHMS_SUCCESSFUL_RESULT;
	nRet = m_pInternalNetInterface->GetEventStream(stringCaseId, stringOriginalHospital);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}
int CExternalNetInterface::GetCaseTypeList(string stringLocalFilePath, string stringLanguage)
{
	m_pInternalNetInterface->SetSaveType(2);
	m_pInternalNetInterface->SetFilePath(stringLocalFilePath);
	int nRet = PHMS_SUCCESSFUL_RESULT;
	nRet = m_pInternalNetInterface->GetCaseTypeList(stringLanguage);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}
int CExternalNetInterface::DownloadPhoto(string stringLocalFilePath, string stringUri, unsigned long nFileSize, pProgressAndSpeedCallback pCallback)
{
	m_pInternalNetInterface->SetSaveType(2);
	m_pInternalNetInterface->SetFilePath(stringLocalFilePath);
	int nRet = PHMS_SUCCESSFUL_RESULT;
	nRet = m_pInternalNetInterface->DownloadFile(1, stringUri, nFileSize, stringLocalFilePath, pCallback);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}
int CExternalNetInterface::GetAlreadyUploadFileSize(string stringUri, int& nFileSize)
{
	m_pInternalNetInterface->SetSaveType(1);
	int nRet = PHMS_SUCCESSFUL_RESULT;
	nRet = m_pInternalNetInterface->GetAlreadyUploadFileSize(stringUri);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	string stringFileSize = m_pInternalNetInterface->GetContent();
	if(stringFileSize.size()==0 || stringFileSize.compare("0")==0)
	{
		nFileSize = 0;
	}
	else
	{
		nFileSize = CUtil::StringToULong(stringFileSize);
	}
	return nRet;
}
int CExternalNetInterface::SendPhoneMsg(string stringCardId, string stringPhone, string stringMsg)
{
	m_pInternalNetInterface->SetSaveType(0);
	int nRet = PHMS_SUCCESSFUL_RESULT;
	nRet = m_pInternalNetInterface->SendPhoneMsg(stringCardId, stringPhone, stringMsg);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}
int CExternalNetInterface::GetFileWithHttp(string stringLocalFilePath, string stringUrl, int bContinue)
{
	m_pInternalNetInterface->SetSaveType(2);
	m_pInternalNetInterface->SetFilePath(stringLocalFilePath);
	int nRet = PHMS_SUCCESSFUL_RESULT;
	nRet = m_pInternalNetInterface->GetFileWithHttp(stringUrl, bContinue);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}
int CExternalNetInterface::PostForm(string& stringResponse, string stringHost, string stringUrl, map<string, string> mapParams)
{
	m_pInternalNetInterface->SetSaveType(1);
	int nRet = PHMS_SUCCESSFUL_RESULT;
	nRet = m_pInternalNetInterface->PostForm(stringHost, stringUrl, mapParams);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	stringResponse = m_pInternalNetInterface->GetContent();
	return nRet;
}
int CExternalNetInterface::GetCardUserReportList(string stringLocalFilePath, string stringReportId, string stringReportStartDate, string stringReportEndDate, string stringCaseType, string stringState, string stringOrderBy, 
												 string stringThirdIdType, string stringThirdId, string stringNextId, string stringPageSize)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pInternalNetInterface->SetSaveType(2);
	m_pInternalNetInterface->SetFilePath(stringLocalFilePath);
	nRet = m_pInternalNetInterface->GetCardUserReportList(stringReportId, stringReportStartDate, stringReportEndDate, stringCaseType, stringState, stringOrderBy, stringThirdIdType, stringThirdId, stringNextId, stringPageSize);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}

int CExternalNetInterface::SearchCaseAdvanced(string stringLocalFilePath, string stringHospitalId, string stringDiagnostic, 
					   string stringName, string stringSex, QUERY_RANGE rangeAge, QUERY_RANGE rangeCreateTime, string stringMode, 
					   string stringCaseType, QUERY_ECG ecg, string stringFrom, string stringSize)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pInternalNetInterface->SetSaveType(2);
	m_pInternalNetInterface->SetFilePath(stringLocalFilePath);
	nRet = m_pInternalNetInterface->SearchCaseAdvanced(stringHospitalId, stringDiagnostic, stringName, stringSex, rangeAge,
		rangeCreateTime, stringMode, stringCaseType, ecg, stringFrom, stringSize);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}

int CExternalNetInterface::SearchCaseAdvancedString(string stringLocalFilePath, string stringHospitalId, string stringDiagnostic, 
							 string stringName, string stringSex, string stringJsonAge, string stringJsonCreateTime, string stringMode, 
							 string stringCaseType, string stringJsonEcg, string stringFrom, string stringSize)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pInternalNetInterface->SetSaveType(2);
	m_pInternalNetInterface->SetFilePath(stringLocalFilePath);
	nRet = m_pInternalNetInterface->SearchCaseAdvancedString(stringHospitalId, stringDiagnostic, stringName, stringSex, stringJsonAge,
		stringJsonCreateTime, stringMode, stringCaseType, stringJsonEcg, stringFrom, stringSize);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}

int CExternalNetInterface::GetPayCode(string stringLocalFilePath, string stringType, string stringCaseId)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pInternalNetInterface->SetSaveType(2);
	m_pInternalNetInterface->SetFilePath(stringLocalFilePath);
	nRet = m_pInternalNetInterface->GetPayCode(stringType, stringCaseId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}

int CExternalNetInterface::GetOrderInfo(string stringLocalFilePath, string stringType, string stringCaseId)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pInternalNetInterface->SetSaveType(2);
	m_pInternalNetInterface->SetFilePath(stringLocalFilePath);
	nRet = m_pInternalNetInterface->GetOrderInfo(stringType, stringCaseId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}


//客户端接口
int CExternalNetInterface::UserLogin(string stringLocalFilePath, string stringUsername, string stringPassword, string stringRole, string stringToken, string& stringIp)
{
	m_pInternalNetInterface->SetSaveType(2);
	m_pInternalNetInterface->SetFilePath(stringLocalFilePath);
	int nRet = PHMS_SUCCESSFUL_RESULT;
	nRet = m_pInternalNetInterface->Login(stringRole, stringUsername, stringPassword, "", stringToken, true);
	if(nRet==COMMON_SEND_FAIL || nRet==COMMON_SEND_TIMEOUT || nRet==COMMON_RECV_FAIL || nRet==COMMON_RECV_TIMEOUT || nRet==COMMON_URL_NOT_FOUND)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		nRet = m_pInternalNetInterface->Login(stringRole, stringUsername, stringPassword, "", stringToken, false);
		if(nRet != PHMS_SUCCESSFUL_RESULT)
		{
			CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
			return nRet;
		}
	}
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	stringIp = m_pInternalNetInterface->GetIp();
	return nRet;
}

int CExternalNetInterface::GetUserInfo(string stringLocalFilePath)
{
	m_pInternalNetInterface->SetSaveType(2);
	m_pInternalNetInterface->SetFilePath(stringLocalFilePath);
	int nRet = PHMS_SUCCESSFUL_RESULT;
	nRet = m_pInternalNetInterface->GetSelfInfo("");
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}

int CExternalNetInterface::GetHospitalInfo(string stringLocalFilePath, string stringHospitalId)
{
	m_pInternalNetInterface->SetSaveType(2);
	m_pInternalNetInterface->SetFilePath(stringLocalFilePath);
	int nRet = PHMS_SUCCESSFUL_RESULT;
	nRet = m_pInternalNetInterface->GetHospitalInfo(stringHospitalId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}

int CExternalNetInterface::GetDoctorInfo(string stringLocalFilePath, string stringDoctorId)
{
	m_pInternalNetInterface->SetSaveType(2);
	m_pInternalNetInterface->SetFilePath(stringLocalFilePath);
	int nRet = PHMS_SUCCESSFUL_RESULT;
	nRet = m_pInternalNetInterface->GetDoctorListOfHospital("", stringDoctorId, "", "", 1, 1);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}

int CExternalNetInterface::UploadCase(string& stringCaseId, string stringName, string stringThirdId, string stringDataType, string stringDataTypeName, string stringNotes, string stringHospitalId, 
			   string stringHospitalName, string stringDoctorId, string stringDoctorName, string stringCheckTime, string stringOtherParam, string stringDeviceId, string stringDeviceName, string stringCaseOrigin, string stringYzlb, string stringSex, 
			   string stringApplicationNo, string stringAutoMatic, string stringLocalCasePath, pProgressAndSpeedCallback pCallback)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pInternalNetInterface->SetSaveType(1);
	//判断otherparam是否为空
	if(stringOtherParam.size() == 0)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_PARAM_SIZE_0), __FUNCTION__, __FILE__, __LINE__);
		CPhmsLogger::GetPhmsLogger()->WriteLog(stringOtherParam, __FUNCTION__, __FILE__, __LINE__);
		return COMMON_PARAM_SIZE_0;
	}
	//判断诊断结论中是否有非法的GBK编码字符
	string stringWorkingDir = CUtil::GetCurrentAppDir();
	string stringIniFilePath = stringWorkingDir+"PhmsConfig.ini";
	AutoPtr<IniFileConfiguration> iniFile = new IniFileConfiguration(stringIniFilePath, "gbk");
	string stringEncoding = iniFile->getString("OTHER.Encoding", "GBK");
	if(stringEncoding=="GBK" || stringEncoding=="gbk")
	{
		if(!CUtil::IsLegalGBK(stringOtherParam))
		{
			CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_ILLEGAL_GBK_ENCODING), __FUNCTION__, __FILE__, __LINE__);
			CPhmsLogger::GetPhmsLogger()->WriteLog(stringOtherParam, __FUNCTION__, __FILE__, __LINE__);
			return COMMON_ILLEGAL_GBK_ENCODING;
		}
	}
// 	//判断otherparam参数中是否有非法的GBK编码字符
// 	if(!CUtil::IsLegalGBK(stringOtherParam))
// 	{
// 		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_ILLEGAL_GBK_ENCODING), __FUNCTION__, __FILE__, __LINE__);
// 		CPhmsLogger::GetPhmsLogger()->WriteLog(stringOtherParam, __FUNCTION__, __FILE__, __LINE__);
// 		return COMMON_ILLEGAL_GBK_ENCODING;
// 	}
	if(stringCaseId.size() == 0)
	{
		string stringMd5Src = stringLocalCasePath+stringHospitalId+stringDoctorId+stringDeviceId;
		string stringMd5 = CUtil::GetMd5(stringMd5Src.c_str(), stringMd5Src.size());
		nRet = m_pInternalNetInterface->ApplyNewCase(stringName, stringThirdId, stringDataType, stringDataTypeName, stringNotes, stringHospitalId, stringHospitalName, stringDoctorId, 
			stringDoctorName, stringCheckTime, stringOtherParam, stringDeviceId, stringDeviceName, stringMd5, stringCaseOrigin, stringYzlb, stringSex, stringApplicationNo, stringAutoMatic);
		if(nRet != PHMS_SUCCESSFUL_RESULT)
		{
			return nRet;
		}
		stringCaseId = m_pInternalNetInterface->GetContent();
	}
	nRet = m_pInternalNetInterface->UploadCase(stringCaseId, stringLocalCasePath, pCallback);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}

int CExternalNetInterface::UserDownloadReport(string stringLocalFilePath, string stringCaseId, string stringReportId, string stringUri, unsigned long nFileSize, pProgressAndSpeedCallback pCallback)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pInternalNetInterface->SetSaveType(0);
	nRet = m_pInternalNetInterface->DownloadReportBegin(stringCaseId, stringReportId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		return nRet;
	}
	nRet = m_pInternalNetInterface->DownloadFile(3, stringUri, nFileSize, stringLocalFilePath, pCallback);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		return nRet;
	}
	m_pInternalNetInterface->SetSaveType(0);
	nRet = m_pInternalNetInterface->DownloadReportComplete(stringCaseId, stringReportId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}

int CExternalNetInterface::GetCaseReportState(string& stringReportState, string stringCaseId)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pInternalNetInterface->SetSaveType(1);
	nRet = m_pInternalNetInterface->GetReportStateOfCase(stringCaseId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		return nRet;
	}
	stringReportState = m_pInternalNetInterface->GetContent();
	return PHMS_SUCCESSFUL_RESULT;
}

int CExternalNetInterface::UserGetReportList(string stringLocalFilePath, string stringReportId, string stringReportStartDate, string stringReportEndDate, 
					  string stringCaseType, string stringReportState, string stringOrderBy, unsigned long nPageTo, unsigned long nPageSize)
{
	m_pInternalNetInterface->SetSaveType(2);
	m_pInternalNetInterface->SetFilePath(stringLocalFilePath);
	int nRet = m_pInternalNetInterface->GetReportListOfUser(stringReportId, stringReportStartDate, stringReportEndDate, stringCaseType, stringReportState, 
		stringOrderBy, nPageTo, nPageSize);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}

int CExternalNetInterface::UserGetCaseList(string stringLocalFilePath, string stringCaseId, string stringCaseType, string stringCaseState, string stringThirdId, string stringName, 
										   string stringStartDate, string stringEndDate, string stringCheckStartDate, string stringCheckEndDate, string stringCaseOrigin, 
										   string stringYzlb, string stringSex, string stringIsRead, string stringOrderBy, unsigned int nPageTo, unsigned int nPageSize)
{
	m_pInternalNetInterface->SetSaveType(2);
	m_pInternalNetInterface->SetFilePath(stringLocalFilePath);
	int nRet = m_pInternalNetInterface->GetCaseListOfUser(stringCaseId, stringCaseType, stringCaseState, stringThirdId, stringName, stringStartDate, stringEndDate, 
		stringCheckStartDate, stringCheckEndDate, stringCaseOrigin, stringYzlb, stringSex, stringIsRead, stringOrderBy, nPageTo, nPageSize);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}

int CExternalNetInterface::GetAdvice(string stringLocalFilePath, string stringSgId, string stringUserId, string stringDoctorId, string stringDownloadState, string stringType, 
									 string stringStartDate, string stringEndDate, unsigned int nPageTo, unsigned int nPageSize)
{
	m_pInternalNetInterface->SetSaveType(2);
	m_pInternalNetInterface->SetFilePath(stringLocalFilePath);
	int nRet = PHMS_SUCCESSFUL_RESULT;
	nRet = m_pInternalNetInterface->GetAdvice(stringSgId, stringUserId, stringDoctorId, stringDownloadState, stringType, stringStartDate, stringEndDate, nPageTo, nPageSize);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}

int CExternalNetInterface::MarkReportRead(string stringReportId)
{
	m_pInternalNetInterface->SetSaveType(0);
	int nRet = m_pInternalNetInterface->MarkReportRead(stringReportId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}

int CExternalNetInterface::CardUserChangeInfo(string stringPId, string stringName, string stringSex, string stringTel, string stringBirthday, string stringAddress, string stringLmp, string stringToken, string stringHeight, string stringWeight, string stringPhoto, string stringVCodeBeFollowed, string stringHospitalId, string stringOtherInfo)
{
	m_pInternalNetInterface->SetSaveType(0);
	int nRet = m_pInternalNetInterface->UserChangeInfo(stringPId, stringName, stringSex, stringTel, stringBirthday, stringAddress, stringLmp, stringToken, stringHeight, stringWeight, stringPhoto, stringVCodeBeFollowed, stringHospitalId, stringOtherInfo);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}

int CExternalNetInterface::CliniqueChangeInfo(string stringPId, string stringName, string stringTel, string stringNotes)
{
	m_pInternalNetInterface->SetSaveType(0);
	int nRet = m_pInternalNetInterface->CliniqueChangeInfo(stringPId, stringName, stringTel, stringNotes);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}

int CExternalNetInterface::UploadTrendData(string stringCardId, string stringDeviceId, string stringDeviceName, string stringCaseId, vector<RECORD_DATA> vRecordData)
{
	m_pInternalNetInterface->SetSaveType(0);
	int nRet = m_pInternalNetInterface->UploadTrendData(stringCardId, stringDeviceId, stringDeviceName, stringCaseId, vRecordData);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}

int CExternalNetInterface::UploadTrendDataString(string stringCardId, string stringDeviceId, string stringDeviceName, string stringCaseId, vector<RECORD_DATA_STRING> vRecordData)
{
	m_pInternalNetInterface->SetSaveType(0);
	int nRet = m_pInternalNetInterface->UploadTrendDataString(stringCardId, stringDeviceId, stringDeviceName, stringCaseId, vRecordData);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}

int CExternalNetInterface::UploadTrendDataFile(string stringFilePath)
{
	m_pInternalNetInterface->SetSaveType(0);
	int nRet = m_pInternalNetInterface->UploadTrendDataFile(stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}

int CExternalNetInterface::VerifyThirdId(string& stringCardId, string stringUId, string stringType)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pInternalNetInterface->SetSaveType(1);
	nRet = m_pInternalNetInterface->VerifyThirdId(stringUId, stringType);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	stringCardId = m_pInternalNetInterface->GetContent();
	return nRet;
}

int CExternalNetInterface::PersonIdRegister(string stringLocalFilePath, string stringType, string stringRegFrom, string stringPersonId, string stringName, string stringSex, string stringBirthday, string stringAddress, 
					 string stringPhone, string stringHeigth, string stringUploadUnitId, string stringHospitalId, string stringHGroupId, string stringScode, string stringWxUId, string stringLmp, string stringWeight, string stringPassword, 
					 string stringThirdCode, string stringIsRegister)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	if(stringLocalFilePath.size() == 0)
	{
		m_pInternalNetInterface->SetSaveType(0);
	}
	else
	{
		m_pInternalNetInterface->SetSaveType(2);
		m_pInternalNetInterface->SetFilePath(stringLocalFilePath);
	}
	nRet = m_pInternalNetInterface->PersonIdRegister(stringType, stringRegFrom, stringPersonId, stringName, stringSex, stringBirthday, stringAddress, stringPhone, stringHeigth,
		stringUploadUnitId, stringHospitalId, stringHGroupId, stringScode, stringWxUId, stringLmp, stringWeight, stringPassword, stringThirdCode, stringIsRegister);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}

int CExternalNetInterface::GetVerificationCode(string stringType, string stringPhone)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pInternalNetInterface->SetSaveType(0);
	nRet = m_pInternalNetInterface->GetVerificationCode(stringType, stringPhone);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}

int CExternalNetInterface::GetDataDictionary(string stringLocalFilePath)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pInternalNetInterface->SetSaveType(2);
	m_pInternalNetInterface->SetFilePath(stringLocalFilePath);
	nRet = m_pInternalNetInterface->GetDataDictionary();
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}

int CExternalNetInterface::LoggingHealthBaseInfo(string stringUserId, string stringWeblock, string stringNation, string stringProfression, string stringCrowd, string stringBloodType, 
						  string stringJwsJiBing, string stringJwsShouShu, string stringJwsWaiShang, string stringJwsShuXue, string stringJwsBaoLuShi, string stringJwsBaoLuShiBz, 
						  string stringJwsYiChuanJiBing, string stringGmsGuoMinShi, string stringGmsGuoMinShiBz, string stringJzsFuQin, string stringJzsFuQinBz, string stringJzsMuQin, 
						  string stringJzsMuQinBz, string stringJzsXdjm, string stringJzsXdjmBz, string stringJzsZiNv, string stringJzsZiNvBz, string stringJzsQiTa)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pInternalNetInterface->SetSaveType(0);
	nRet = m_pInternalNetInterface->LoggingHealthBaseInfo(stringUserId, stringWeblock, stringNation, stringProfression, stringCrowd, stringBloodType, stringJwsJiBing, stringJwsShouShu, 
		stringJwsWaiShang, stringJwsShuXue, stringJwsBaoLuShi, stringJwsBaoLuShiBz, stringJwsYiChuanJiBing, stringGmsGuoMinShi, stringGmsGuoMinShiBz, stringJzsFuQin, stringJzsFuQinBz, 
		stringJzsMuQin, stringJzsMuQinBz, stringJzsXdjm, stringJzsXdjmBz, stringJzsZiNv, stringJzsZiNvBz, stringJzsQiTa);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}

int CExternalNetInterface::LoggingHealthCurrentInfo(string stringUserId, string stringShfsYinShi, string stringShfsXlzk, string stringShfsXlzkBz, string stringShfsXltz, string stringShfsXiYan, 
							 string stringShfsYinJiu, string stringShfsSyqk, string stringShfsYunDong, string stringShfsYunDongShiJian, string stringSysjcDanGuChun, string stringSysjcXueChangGui, 
							 string stringSysjcXueChangGuiBz, string stringSysjcNiaoChangGui, string stringSysjcNiaoChangGuiBz, string stringSysjcQiTa, string stringYongYao, string stringZhuYuan, 
							 string stringJkwtNaoXueGuan, string stringJkwtNaoXueGuanBz, string stringJkwtShenZang, string stringJkwtShenZangBz, string stringJkwtXinZang, string stringJkwtXinZangBz, 
							 string stringJkwtXueGuan, string stringJkwtXueGuanBz, string stringJkwtYanBu, string stringJkwtYanBuBz, string stringJkwtShenJing, string stringJkwtQiTa, string stringDate)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pInternalNetInterface->SetSaveType(0);
	nRet = m_pInternalNetInterface->LoggingHealthCurrentInfo(stringUserId, stringShfsYinShi, stringShfsXlzk, stringShfsXlzkBz, stringShfsXltz, stringShfsXiYan, stringShfsYinJiu, stringShfsSyqk, 
		stringShfsYunDong, stringShfsYunDongShiJian, stringSysjcDanGuChun, stringSysjcXueChangGui, stringSysjcXueChangGuiBz, stringSysjcNiaoChangGui, stringSysjcNiaoChangGuiBz, stringSysjcQiTa, 
		stringYongYao, stringZhuYuan, stringJkwtNaoXueGuan, stringJkwtNaoXueGuanBz, stringJkwtShenZang, stringJkwtShenZangBz, stringJkwtXinZang, stringJkwtXinZangBz, stringJkwtXueGuan, stringJkwtXueGuanBz, 
		stringJkwtYanBu, stringJkwtYanBuBz, stringJkwtShenJing, stringJkwtQiTa, stringDate);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}

int CExternalNetInterface::UpdateCaseInfo(string& stringDoctorId, string stringHospitalId, string stringCaseId, string stringOtherParam, string stringFlag, string stringNotes, string stringReceiverId)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pInternalNetInterface->SetSaveType(1);
	nRet = m_pInternalNetInterface->UpdateCaseInfo(stringHospitalId, stringCaseId, stringOtherParam, stringFlag, stringNotes, stringReceiverId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	stringDoctorId = m_pInternalNetInterface->GetContent();
	return nRet;
}

int CExternalNetInterface::PersonIdRegisterAndLogin(string stringLocalFilePath, string stringRole, string stringPId, string stringName, string stringSex, string stringBirthday, string stringAddress, string stringDeviceId, string stringHospitalId, 
							 string stringLang, string stringRegFrom)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pInternalNetInterface->SetSaveType(2);
	m_pInternalNetInterface->SetFilePath(stringLocalFilePath);
	string stringMd5Src = stringHospitalId + stringDeviceId + stringPId;
	string stringMd5 = CUtil::GetMd5(stringMd5Src.c_str(), stringMd5Src.size());
	nRet = m_pInternalNetInterface->PersonIdRegisterAndLogin(stringRole, stringPId, stringName, stringSex, stringBirthday, stringAddress, stringDeviceId, stringMd5, stringLang, stringRegFrom);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}

int CExternalNetInterface::UploadTrendData2(string stringTargetPath)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pInternalNetInterface->SetSaveType(0);
	nRet = m_pInternalNetInterface->UploadTrendData2(stringTargetPath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}

int CExternalNetInterface::CardBindPhone(string stringPhone, string stringScode)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pInternalNetInterface->SetSaveType(0);
	nRet = m_pInternalNetInterface->CardBindPhone(stringPhone, stringScode);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}

int CExternalNetInterface::ResetPassword(string stringPhone, string stringScode, string stringNewPwd)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pInternalNetInterface->SetSaveType(0);
	nRet = m_pInternalNetInterface->ResetPassword(stringPhone, stringScode, stringNewPwd);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}

int CExternalNetInterface::UploadDaily(string stringType, string stringCreateTime, string stringFiles)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pInternalNetInterface->SetSaveType(0);
	nRet = m_pInternalNetInterface->UploadDaily(stringType, stringCreateTime, stringFiles);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}

int CExternalNetInterface::GetDateListOfDaily(string stringLocalFilePath, string stringYear, string stringMonth)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pInternalNetInterface->SetSaveType(2);
	m_pInternalNetInterface->SetFilePath(stringLocalFilePath);
	nRet = m_pInternalNetInterface->GetDateListOfDaily(stringYear, stringMonth);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}

int CExternalNetInterface::GetDailyOfDate(string stringLocalFilePath, string stringDate, string stringType)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pInternalNetInterface->SetSaveType(2);
	m_pInternalNetInterface->SetFilePath(stringLocalFilePath);
	nRet = m_pInternalNetInterface->GetDailyOfDate(stringDate, stringType);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}

int CExternalNetInterface::ShareDaily(string stringLocalFilePath, string stringType, string stringCreateTime)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pInternalNetInterface->SetSaveType(2);
	m_pInternalNetInterface->SetFilePath(stringLocalFilePath);
	nRet = m_pInternalNetInterface->ShareDaily(stringType, stringCreateTime);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}

int CExternalNetInterface::DeleteDaily(string stringType, string stringDate)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pInternalNetInterface->SetSaveType(0);
	nRet = m_pInternalNetInterface->DeleteDaily(stringType, stringDate);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}

int CExternalNetInterface::PhoneScanQR(string& stringSence, string stringQRCode)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pInternalNetInterface->SetSaveType(1);
	nRet = m_pInternalNetInterface->PhoneScanQR(stringQRCode);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	stringSence = m_pInternalNetInterface->GetContent();
	return nRet;
}

int CExternalNetInterface::PhoneScanQRVerify(string stringQRCode)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pInternalNetInterface->SetSaveType(0);
	nRet = m_pInternalNetInterface->PhoneScanQRVerify(stringQRCode);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}

int CExternalNetInterface::ApplyQRCode(string stringLocalFilePath, string stringType, string stringScene)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pInternalNetInterface->SetSaveType(2);
	m_pInternalNetInterface->SetFilePath(stringLocalFilePath);
	nRet = m_pInternalNetInterface->ApplyQRCode(stringType, stringScene);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}
int CExternalNetInterface::ValidateUserAccount(string stringLocalFilePath, string stringLoginId, string stringPassword, string stringRole)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pInternalNetInterface->SetSaveType(2);
	m_pInternalNetInterface->SetFilePath(stringLocalFilePath);
	nRet = m_pInternalNetInterface->ValidateUserAccount(stringLoginId, stringPassword, stringRole);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}
int CExternalNetInterface::RegisterBluetoothDevice(string stringUniqueSN, string stringSerial, string stringBluetoothAddress, string stringBluetoothName, string stringDeviceType)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pInternalNetInterface->SetSaveType(0);
	nRet = m_pInternalNetInterface->RegisterBluetoothDevice(stringUniqueSN, stringSerial, stringBluetoothAddress, stringBluetoothName, stringDeviceType);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}
int CExternalNetInterface::GetBluetoothInfoOfDevice(string stringLocalFilePath, string stringUniqueSN)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pInternalNetInterface->SetSaveType(2);
	m_pInternalNetInterface->SetFilePath(stringLocalFilePath);
	nRet = m_pInternalNetInterface->GetBluetoothInfoOfDevice(stringUniqueSN);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}
int CExternalNetInterface::ChangeDeviceType(string stringUniqueSN, string stringDeviceType)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pInternalNetInterface->SetSaveType(0);
		nRet = m_pInternalNetInterface->ChangeDeviceType(stringUniqueSN, stringDeviceType);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}
int CExternalNetInterface::UnregisterBluetoothDevice(string stringUniqueSN)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pInternalNetInterface->SetSaveType(0);
	nRet = m_pInternalNetInterface->UnregisterBluetoothDevice(stringUniqueSN);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}
int CExternalNetInterface::GetHospitalListOfArea(string stringLocalFilePath, string stringAreaId, string stringRank, string stringBeta)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pInternalNetInterface->SetSaveType(2);
	m_pInternalNetInterface->SetFilePath(stringLocalFilePath);
	nRet = m_pInternalNetInterface->GetHospitalListOfArea(stringAreaId, stringRank, stringBeta);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}
int CExternalNetInterface::UploadCommonFile(string& stringUri, string stringType, string stringExtra, string stringLocalFilePath, string stringFileType, pProgressAndSpeedCallback pCallbcak)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pInternalNetInterface->SetSaveType(1);
	nRet = m_pInternalNetInterface->UploadCommonFile(stringUri, stringType, stringExtra, stringLocalFilePath, stringFileType, pCallbcak);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}
int CExternalNetInterface::SearchSomebody(string stringLocalFilePath, string stringUserId)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pInternalNetInterface->SetSaveType(2);
	m_pInternalNetInterface->SetFilePath(stringLocalFilePath);
	nRet = m_pInternalNetInterface->SearchSomebody(stringUserId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}
int CExternalNetInterface::FollowSomebody(string stringUserId, string stringUsername, string stringNickname, string stringVCode)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pInternalNetInterface->SetSaveType(0);
	nRet = m_pInternalNetInterface->FollowSomebody(stringUserId, stringUsername, stringNickname, stringVCode);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}
int CExternalNetInterface::MyFollowing(string stringLocalFilePath, string stringUserId, string stringNextId, string stringFType)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pInternalNetInterface->SetSaveType(2);
	m_pInternalNetInterface->SetFilePath(stringLocalFilePath);
	nRet = m_pInternalNetInterface->MyFollowing(stringUserId, stringNextId, stringFType);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}
int CExternalNetInterface::MyFans(string stringLocalFilePath, string stringUserId, string stringNextId)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pInternalNetInterface->SetSaveType(2);
	m_pInternalNetInterface->SetFilePath(stringLocalFilePath);
	nRet = m_pInternalNetInterface->MyFans(stringUserId, stringNextId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}
int CExternalNetInterface::ChangeNicknameOfFollowed(string stringUserId, string stringNickname)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pInternalNetInterface->SetSaveType(0);
	nRet = m_pInternalNetInterface->ChangeNicknameOfFollowed(stringUserId, stringNickname);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}
int CExternalNetInterface::CancelFollowing(string stringUserId)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pInternalNetInterface->SetSaveType(0);
	nRet = m_pInternalNetInterface->CancelFollowing(stringUserId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}
int CExternalNetInterface::PublishHealthyKnowledge(vector<HEALTHY_KNOWLEDGE> vHealthKnowledge)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pInternalNetInterface->SetSaveType(0);
	nRet = m_pInternalNetInterface->PublishHealthyKnowledge(vHealthKnowledge);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}
int CExternalNetInterface::GetListOfHealthyKnowledge(string stringLocalFilePath, string stringSenderId, string stringNextId, string stringIds, string stringPageSize, string stringOrderBy)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pInternalNetInterface->SetSaveType(2);
	m_pInternalNetInterface->SetFilePath(stringLocalFilePath);
	nRet = m_pInternalNetInterface->GetListOfHealthyKnowledge(stringSenderId, stringNextId, stringIds, stringPageSize, stringOrderBy);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}
int CExternalNetInterface::GenerateOrDeletePngZlibReport(string stringLocalFilePath, string stringServerPath, string stringFlag)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	if(stringFlag=="1")
	{
		m_pInternalNetInterface->SetSaveType(2);
	}
	else
	{
		m_pInternalNetInterface->SetSaveType(0);
	}
	m_pInternalNetInterface->SetFilePath(stringLocalFilePath);
	nRet = m_pInternalNetInterface->GenerateOrDeletePngZlibReport(stringServerPath, stringFlag);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}

int CExternalNetInterface::GetDailyList(string stringLocalFilePath, string stringType, string stringStartDate, string stringEndDate, string stringOrderBy, string stringNextId, string stringPageSize)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pInternalNetInterface->SetSaveType(2);
	m_pInternalNetInterface->SetFilePath(stringLocalFilePath);
	nRet = m_pInternalNetInterface->GetDailyList(stringType, stringStartDate, stringEndDate, stringOrderBy, stringNextId, stringPageSize);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}

int CExternalNetInterface::ChangeViewLikeCollectCount(string stringHealthTipId, string stringUri, string stringTitle, string stringLike, string stringRead, string stringCollect)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pInternalNetInterface->SetSaveType(0);
	nRet = m_pInternalNetInterface->ChangeViewLikeCollectCount(stringHealthTipId, stringUri, stringTitle, stringLike, stringRead, stringCollect);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}

int CExternalNetInterface::ChangeAdviceExecutiveState(string stringSgId, string stringState)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pInternalNetInterface->SetSaveType(0);
	nRet = m_pInternalNetInterface->ChangeAdviceExecutiveState(stringSgId, stringState);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}

int CExternalNetInterface::GetCliniqueReportList(string stringLocalFilePath, string stringCliniqueSenderId, string stringStartDate, 
												 string stringEndDate, string stringState, string stringName, string stringPatientSourceCode, string stringYzlb, string stringPIdType, string stringPId, string stringHb, string stringASectionId, string stringNextId, string stringPageSize)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pInternalNetInterface->SetSaveType(2);
	m_pInternalNetInterface->SetFilePath(stringLocalFilePath);
	nRet = m_pInternalNetInterface->GetCliniqueReportList(stringCliniqueSenderId, stringStartDate, stringEndDate, stringState, stringName, stringPatientSourceCode, stringYzlb, stringPIdType, stringPId, stringHb, stringASectionId, 
		stringNextId, stringPageSize);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}

int CExternalNetInterface::GetUserListBindingOrNot(string stringLocalFilePath, string stringNextId, string stringPageSize, string stringIsBind, string stringIsActive)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pInternalNetInterface->SetSaveType(2);
	m_pInternalNetInterface->SetFilePath(stringLocalFilePath);
	nRet = m_pInternalNetInterface->GetUserListBindingOrNot(stringNextId, stringPageSize, stringIsBind, stringIsActive);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}
int CExternalNetInterface::BindUser(string stringUId, string stringUName)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pInternalNetInterface->SetSaveType(0);
	nRet = m_pInternalNetInterface->BindUser(stringUId, stringUName);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}
int CExternalNetInterface::UnbindUser(string stringUId)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pInternalNetInterface->SetSaveType(0);
	nRet = m_pInternalNetInterface->UnbindUser(stringUId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}
int CExternalNetInterface::GetCardUserListOfCustomer(string stringLocalFilePath, string stringThirdId, string stringName, string stringCaseType, string stringStartDate, string stringEndDate, string stringNextId, unsigned int nPageSize)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pInternalNetInterface->SetSaveType(2);
	m_pInternalNetInterface->SetFilePath(stringLocalFilePath);
	nRet = m_pInternalNetInterface->GetCardUserListOfCustomer(stringThirdId, stringName, stringCaseType, stringStartDate, stringEndDate, stringNextId, nPageSize);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}
int CExternalNetInterface::SignDoctor(string stringUId)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pInternalNetInterface->SetSaveType(0);
	nRet = m_pInternalNetInterface->SignDoctor(stringUId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}
int CExternalNetInterface::RefundFee(string stringType, string stringCaseId, string stringRefundFee)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pInternalNetInterface->SetSaveType(0);
	nRet = m_pInternalNetInterface->RefundFee(stringType, stringCaseId, stringRefundFee);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}
int CExternalNetInterface::HoldPlaceReview(string stringReportId)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pInternalNetInterface->SetSaveType(0);
	nRet = m_pInternalNetInterface->HoldPlaceReview(stringReportId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}
int CExternalNetInterface::RollBackReview(string stringReportId)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pInternalNetInterface->SetSaveType(0);
	nRet = m_pInternalNetInterface->RollBackReview(stringReportId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}
int CExternalNetInterface::ResetReviewHoldPlace(string stringReportId, string stringOccupantId, string stringOccupantName, string stringSuperKey)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pInternalNetInterface->SetSaveType(0);
	nRet = m_pInternalNetInterface->ResetReviewHoldPlace(stringReportId, stringOccupantId, stringOccupantName, stringSuperKey);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}
int CExternalNetInterface::ResetCaseHoldPlace(string stringCaseId, string stringReceiverId, string stringOccupantId, string stringOccupantName, string stringSuperKey)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	string stringServerReceiverId = stringReceiverId;
	if(stringReceiverId.size() == 0)
	{
		m_pInternalNetInterface->SetSaveType(1);
		nRet = m_pInternalNetInterface->GetCaseListOfDoctor(stringCaseId, "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 1, 1);
		if(nRet != PHMS_SUCCESSFUL_RESULT)
		{
			CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
			return nRet;
		}
		nRet = CUtil::GetXmlFieldVaule(1, m_pInternalNetInterface->GetContent(), "", "receiverid", stringServerReceiverId);
		if(nRet != PHMS_SUCCESSFUL_RESULT)
		{
			CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
			return nRet;
		}
	}
	m_pInternalNetInterface->SetSaveType(0);
	nRet = m_pInternalNetInterface->ResetCaseHoldPlace(stringCaseId, stringServerReceiverId, stringOccupantId, stringOccupantName, stringSuperKey);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}

int CExternalNetInterface::AddTag(string& stringTagId, string stringTagName, string stringTagDescription)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pInternalNetInterface->SetSaveType(1);
	nRet = m_pInternalNetInterface->AddTag(stringTagName, stringTagDescription);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	stringTagId = m_pInternalNetInterface->GetContent();
	return nRet;
}
int CExternalNetInterface::GetTagList(string stringLocalFilePath, string stringSenderId)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pInternalNetInterface->SetSaveType(2);
	m_pInternalNetInterface->SetFilePath(stringLocalFilePath);
	nRet = m_pInternalNetInterface->GetTagList(stringSenderId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}
int CExternalNetInterface::AddCaseToTag(string stringTagId, string stringTagName, string stringCaseId, string stringCaseType, string stringCaseTypeName, string stringPatientName)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pInternalNetInterface->SetSaveType(0);
	nRet = m_pInternalNetInterface->AddCaseToTag(stringTagId, stringTagName, stringCaseId, stringCaseType, stringCaseTypeName, stringPatientName);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}
int CExternalNetInterface::GetCaseListOfTag(string stringLocalFilePath, string stringTagId, string stringSenderId, string stringCaseType)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pInternalNetInterface->SetSaveType(2);
	m_pInternalNetInterface->SetFilePath(stringLocalFilePath);
	nRet = m_pInternalNetInterface->GetCaseListOfTag(stringTagId, stringSenderId, stringCaseType);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}

// int CExternalNetInterface::CreateSession_GuanXin(string stringUsername, string stringPassword)
// {
// 	int nRet = PHMS_SUCCESSFUL_RESULT;
// 	m_pInternalNetInterface->SetSaveType(0);
// 	nRet = m_pInternalNetInterface->CreateSession_GuanXin(stringUsername, stringPassword);
// 	if(nRet != PHMS_SUCCESSFUL_RESULT)
// 	{
// 		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
// 		return nRet;
// 	}
// 	return nRet;
// }

int CExternalNetInterface::GetAccessToken_GuanXin(string stringLocalFilePath, string stringUsername, string stringPassword)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pInternalNetInterface->SetSaveType(2);
	m_pInternalNetInterface->SetFilePath(stringLocalFilePath);
	nRet = m_pInternalNetInterface->GetAccessToken_GuanXin(stringUsername, stringPassword);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}
int CExternalNetInterface::GetPersonInfo_GuanXin(string stringLocalFilePath, string stringAccessToken, string stringPersonName, string stringPersonGender, string stringPersonIdentity, string stringPersonBirthDate)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pInternalNetInterface->SetSaveType(2);
	m_pInternalNetInterface->SetFilePath(stringLocalFilePath);
	nRet = m_pInternalNetInterface->GetPersonInfo_GuanXin(stringAccessToken, stringPersonName, stringPersonGender, stringPersonIdentity, stringPersonBirthDate);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}
int CExternalNetInterface::UploadECGData_GuanXin(string stringLocalFilePath, string stringECGXmlPath, string stringECGDataPath)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pInternalNetInterface->SetSaveType(2);
	m_pInternalNetInterface->SetFilePath(stringLocalFilePath);
	nRet = m_pInternalNetInterface->UploadECGData_GuanXin(stringECGXmlPath, stringECGDataPath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}
int CExternalNetInterface::UploadOtherData_GuanXin(string stringLocalFilePath, string stringOtherXmlPath)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pInternalNetInterface->SetSaveType(2);
	m_pInternalNetInterface->SetFilePath(stringLocalFilePath);
	nRet = m_pInternalNetInterface->UploadOtherData_GuanXin(stringOtherXmlPath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}
int CExternalNetInterface::UploadCheckData_GuanXin(string stringLocalFilePath, string stringCheckXmlPath)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pInternalNetInterface->SetSaveType(2);
	m_pInternalNetInterface->SetFilePath(stringLocalFilePath);
	nRet = m_pInternalNetInterface->UploadCheckData_GuanXin(stringCheckXmlPath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}
int CExternalNetInterface::UploadPersonInfo_GuanXin(string stringLocalFilePath, string stringPersonInfoXmlPath)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pInternalNetInterface->SetSaveType(2);
	m_pInternalNetInterface->SetFilePath(stringLocalFilePath);
	nRet = m_pInternalNetInterface->UploadPersonInfo_GuanXin(stringPersonInfoXmlPath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}


//院内使用接口
int CExternalNetInterface::SetCliniqueBusy(string stringCliniqueId)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pInternalNetInterface->SetSaveType(0);
	nRet = m_pInternalNetInterface->SetCliniqueState(stringCliniqueId, "3");
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}
int CExternalNetInterface::SetCliniqueFree(string stringCliniqueId)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pInternalNetInterface->SetSaveType(0);
	nRet = m_pInternalNetInterface->SetCliniqueState(stringCliniqueId, "0");
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}
int CExternalNetInterface::SetCliniqueChecking(string stringCliniqueId)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pInternalNetInterface->SetSaveType(0);
	nRet = m_pInternalNetInterface->SetCliniqueState(stringCliniqueId, "3");
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}
int CExternalNetInterface::SetCliniqueShoutComplete(string stringCliniqueId)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pInternalNetInterface->SetSaveType(0);
	nRet = m_pInternalNetInterface->SetCliniqueState(stringCliniqueId, "2");
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}
int CExternalNetInterface::GetPatientInfoByAppNum(string stringLocalFilePath, string stringHospitalId, string stringOrderType, string stringOrderId, string stringReqDepart, string stringOrderState, string stringCheckType, string stringStartDate, string stringEndDate, string stringClinicId)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pInternalNetInterface->SetSaveType(2);
	m_pInternalNetInterface->SetFilePath(stringLocalFilePath);
	nRet = m_pInternalNetInterface->GetPatientInfoByAppNum(stringHospitalId, stringOrderType, stringOrderId, stringReqDepart, stringOrderState, stringCheckType, stringStartDate, stringEndDate, stringClinicId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}
int CExternalNetInterface::GetPatientInfoByAppNumToHis(string stringLocalFilePath, string stringHospitalId, string stringOrderType, string stringOrderId)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pInternalNetInterface->SetSaveType(2);
	m_pInternalNetInterface->SetFilePath(stringLocalFilePath);
	nRet = m_pInternalNetInterface->GetPatientInfoByAppNumToHis(stringHospitalId, stringOrderType, stringOrderId); 
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}
int CExternalNetInterface::GetPatientInfoByAppNumToPhmsAndHis(string stringLocalFilePath, string stringHospitalId, string stringOrderType, string stringOrderId)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pInternalNetInterface->SetSaveType(2);
	m_pInternalNetInterface->SetFilePath(stringLocalFilePath);
	nRet = m_pInternalNetInterface->GetPatientInfoByAppNumToPhmsAndHis(stringHospitalId, stringOrderType, stringOrderId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}
int CExternalNetInterface::ApplyCheck(string stringCliniqueId, string stringOrderId, string stringOrderType)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pInternalNetInterface->SetSaveType(0);
	nRet = m_pInternalNetInterface->ApplyCheck(stringCliniqueId, stringOrderId, stringOrderType);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		if(nRet != 220303)
		{
			CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		}
		return nRet;
	}
	return nRet;
}
int CExternalNetInterface::GetWaitingCheck(string stringLocalFilePath, string stringCliniqueId)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pInternalNetInterface->SetSaveType(2);
	m_pInternalNetInterface->SetFilePath(stringLocalFilePath);
	nRet = m_pInternalNetInterface->GetWaitingCheck(stringCliniqueId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		if(nRet!=220403 && nRet!=220405)
		{
			CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		}
		return nRet;
	}
	return nRet;
}
int CExternalNetInterface::GetCliniqueState(string stringLocalFilePath, string stringCliniqueId)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pInternalNetInterface->SetSaveType(2);
	m_pInternalNetInterface->SetFilePath(stringLocalFilePath);
	nRet = m_pInternalNetInterface->GetCliniqueState(stringCliniqueId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}
int CExternalNetInterface::NotifyCaseState(string stringHospitalId, string stringOrderType, string stringOrderId)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pInternalNetInterface->SetSaveType(0);
	nRet = m_pInternalNetInterface->NotifyCaseState(stringHospitalId, stringOrderType, stringOrderId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}
int CExternalNetInterface::NotifyReportState(string stringReportId)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pInternalNetInterface->SetSaveType(0);
	nRet = m_pInternalNetInterface->NotifyReportState(stringReportId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}

int CExternalNetInterface::SetDoctorAgentId(string stringDoctorAgentId)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pInternalNetInterface->SetSaveType(0);
	nRet = m_pInternalNetInterface->SetDoctorAgentId(stringDoctorAgentId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}

int CExternalNetInterface::NotifyAppState(string stringUnitId, string stringOrderId, string stringOrderType, string stringState)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pInternalNetInterface->SetSaveType(0);
	nRet = m_pInternalNetInterface->NotifyAppState(stringUnitId, stringOrderId, stringOrderType, stringState);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}

int CExternalNetInterface::VerifyAppState(string stringLocalFilePath, string stringOrderId, string stringOrderType)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pInternalNetInterface->SetSaveType(2);
	m_pInternalNetInterface->SetFilePath(stringLocalFilePath);
	nRet = m_pInternalNetInterface->VerifyAppState(stringOrderId, stringOrderType);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}

int CExternalNetInterface::GetHisReportState(string stringLocalFilePath, string stringHospitalId, string stringOrderType, string stringOrderId)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pInternalNetInterface->SetSaveType(2);
	m_pInternalNetInterface->SetFilePath(stringLocalFilePath);
	nRet = m_pInternalNetInterface->GetHisReportState(stringHospitalId, stringOrderType, stringOrderId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}

int CExternalNetInterface::SetHisCriticalValue(string stringHospitalId, string stringCaseId, string stringCriticalValues)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pInternalNetInterface->SetSaveType(0);
	nRet = m_pInternalNetInterface->SetHisCriticalValue(stringHospitalId, stringCaseId, stringCriticalValues);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}

//东亚医讯接口
int CExternalNetInterface::GetDoctorList_999120(string stringLocalFilePath, string stringKtCliniqueId, string stringCaseTypeId)
{
	m_pInternalNetInterface->SetSaveType(2);
	m_pInternalNetInterface->SetFilePath(stringLocalFilePath);
	int nRet = m_pInternalNetInterface->GetDoctorList_999120(stringKtCliniqueId, stringCaseTypeId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}

int CExternalNetInterface::ValidatePay_999120(string stringLocalFilePath, string stringConsumeId, string stringKtCliniqueId, string stringCardNumber, string stringKtDoctorId, string stringConsumeMoney, string stringCaseTypeId)
{
	m_pInternalNetInterface->SetSaveType(2);
	m_pInternalNetInterface->SetFilePath(stringLocalFilePath);
	int nRet = m_pInternalNetInterface->ValidatePay_999120(stringConsumeId, stringKtCliniqueId, stringCardNumber, stringKtDoctorId, stringConsumeMoney, stringCaseTypeId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}

int CExternalNetInterface::UpdateCaseForConsumerRecord_999120(string stringLocalFilePath, string stringConsumeId, string stringKtCaseId, string stringCaseTypeId)
{
	m_pInternalNetInterface->SetSaveType(2);
	m_pInternalNetInterface->SetFilePath(stringLocalFilePath);
	int nRet = m_pInternalNetInterface->UpdateCaseForConsumerRecord_999120(stringConsumeId, stringKtCaseId, stringCaseTypeId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}

int CExternalNetInterface::ValidatePay_XinJie(string stringLocalFilePath, string stringRrn, string stringLocalDate)
{
	m_pInternalNetInterface->SetSaveType(1);//此处因为心界返回的信息需要单独处理，所以SaveType是1，保存到内存后再处理，再写到文件
	m_pInternalNetInterface->SetFilePath(stringLocalFilePath);
	int nRet = m_pInternalNetInterface->ValidatePay_XinJie(stringRrn, stringLocalDate);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}

int CExternalNetInterface::NotifyCheck_XinJie(string stringLocalFilePath, string stringRrn, string stringLocalDate)
{
	m_pInternalNetInterface->SetSaveType(1);//此处因为心界返回的信息需要单独处理，所以SaveType是1，保存到内存后再处理，再写到文件
	m_pInternalNetInterface->SetFilePath(stringLocalFilePath);
	int nRet = m_pInternalNetInterface->NotifyCheck_XinJie(stringRrn, stringLocalDate);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}

int CExternalNetInterface::UserAuthorize_Telecom(string stringLocalFilePath, string stringPersonId, string stringName, string stringNation, string stringAddress, string stringRoomId)
{
	m_pInternalNetInterface->SetSaveType(2);
	m_pInternalNetInterface->SetFilePath(stringLocalFilePath);
	int nRet = m_pInternalNetInterface->UserAuthorize_Telecom(stringPersonId, stringName, stringNation, stringAddress, stringRoomId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}

int CExternalNetInterface::BloodPressureUpload_Telecom(string stringLocalFilePath, string stringPersonType, string stringPersonId, string stringSBP, string stringDBP, string stringAverageValue, string stringPulseRate, string stringDetectionTime, 
								string stringEquCode, string stringRoomId)
{
	m_pInternalNetInterface->SetSaveType(2);
	m_pInternalNetInterface->SetFilePath(stringLocalFilePath);
	int nRet = m_pInternalNetInterface->BloodPressureUpload_Telecom(stringPersonType, stringPersonId, stringSBP, stringDBP, stringAverageValue, stringPulseRate, stringDetectionTime, stringEquCode, stringRoomId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}

int CExternalNetInterface::BloodOxygenUpload_Telecom(string stringLocalFilePath, string stringPersonType, string stringPersonId, string stringBloodOxygen, string stringPulseRate, string stringDetectionTime, 
							  string stringEquCode, string stringRoomId)
{
	m_pInternalNetInterface->SetSaveType(2);
	m_pInternalNetInterface->SetFilePath(stringLocalFilePath);
	int nRet = m_pInternalNetInterface->BloodOxygenUpload_Telecom(stringPersonType, stringPersonId, stringBloodOxygen, stringPulseRate, stringDetectionTime, stringEquCode, stringRoomId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}

int CExternalNetInterface::BloodSugarUpload_Telecom(string stringLocalFilePath, string stringPersonType, string stringPersonId, string stringBloodSugar, string stringDetectionTime, 
													string stringEquCode, string stringRoomId)
{
	m_pInternalNetInterface->SetSaveType(2);
	m_pInternalNetInterface->SetFilePath(stringLocalFilePath);
	int nRet = m_pInternalNetInterface->BloodSugarUpload_Telecom(stringPersonType, stringPersonId, stringBloodSugar, stringDetectionTime, stringEquCode, stringRoomId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}

int CExternalNetInterface::HeightWeightUpload_Telecom(string stringLocalFilePath, string stringPersonType, string stringPersonId, string stringHeight, string stringWeight, string stringBMI, string stringDetectionTime, 
							   string stringEquCode, string stringRoomId)
{
	m_pInternalNetInterface->SetSaveType(2);
	m_pInternalNetInterface->SetFilePath(stringLocalFilePath);
	int nRet = m_pInternalNetInterface->HeightWeightUpload_Telecom(stringPersonType, stringPersonId, stringHeight, stringWeight, stringBMI, stringDetectionTime, stringEquCode, stringRoomId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}

int CExternalNetInterface::LungUpload_Telecom(string stringLocalFilePath, string stringPersonType, string stringPersonId, string stringM_B1, string stringBSA, string stringBSA1, string stringMMF, string stringMefSeventyFive, string stringMefFifty, 
					   string stringMefTwentyFive, string stringMefFVST, string stringIC, string stringIRV, string stringERV, string stringFevOne, string stringFevTwo, string stringFevThree, 
					   string stringFevOnePer, string stringFevTwoPer, string stringFevThreePer, string stringRR, string stringFVC, string stringPEF, string stringTV, string stringVC, string stringMVV, 
					   string stringMV, string stringVSTHeight, string stringVSMvvBsa, string stringVSMvvOne, string stringSVC, string stringFevOneZeroFive, string stringFevOneZeroFivePer, string stringPefOne, 
					   string stringPefTwo, string stringPefThree, string stringFive, string stringFiveOne, string stringFiveTwo, string stringFiveThree, string stringPIF, string stringMifFifty, 
					   string stringDetectionTime, string stringEquCode, string stringRoomId)
{
	m_pInternalNetInterface->SetSaveType(2);
	m_pInternalNetInterface->SetFilePath(stringLocalFilePath);
	int nRet = m_pInternalNetInterface->LungUpload_Telecom(stringPersonType, stringPersonId, stringM_B1, stringBSA, stringBSA1, stringMMF, stringMefSeventyFive, stringMefFifty, stringMefTwentyFive, 
		stringMefFVST, stringIC, stringIRV, stringERV, stringFevOne, stringFevTwo, stringFevThree, stringFevOnePer, stringFevTwoPer, stringFevThreePer, stringRR, stringFVC, stringPEF, 
		stringTV, stringVC, stringMVV, stringMV, stringVSTHeight, stringVSMvvBsa, stringVSMvvOne, stringSVC, stringFevOneZeroFive, stringFevOneZeroFivePer, stringPefOne, stringPefTwo, 
		stringPefThree, stringFive, stringFiveOne, stringFiveTwo, stringFiveThree, stringPIF, stringMifFifty, stringDetectionTime, stringEquCode, stringRoomId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}

int CExternalNetInterface::ECGUpload_Telecom(string stringLocalFilePath, string stringPersonType, string stringPersonId, string stringDetectionTime, 
					  string stringEquCode, string stringRoomId, TELCOM_ECG ecgData)
{
	m_pInternalNetInterface->SetSaveType(2);
	m_pInternalNetInterface->SetFilePath(stringLocalFilePath);
	int nRet = m_pInternalNetInterface->ECGUpload_Telecom(stringPersonType, stringPersonId, stringDetectionTime, stringEquCode, stringRoomId, ecgData);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}

int CExternalNetInterface::Login_XiKang(string stringLocalFilePath, string stringUsername, string stringPassword)
{
	m_pInternalNetInterface->SetSaveType(2);
	m_pInternalNetInterface->SetFilePath(stringLocalFilePath);
	int nRet = m_pInternalNetInterface->Login_XiKang(stringUsername, stringPassword);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}

int CExternalNetInterface::RegisterPatientId_XiKang(string stringLocalFilePath, string stringName, string stringSex, string stringPhoneNum, string stringProofNum)
{
	m_pInternalNetInterface->SetSaveType(2);
	m_pInternalNetInterface->SetFilePath(stringLocalFilePath);
	int nRet = m_pInternalNetInterface->RegisterPatientId_XiKang(stringName, stringSex, stringPhoneNum, stringProofNum);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}

int CExternalNetInterface::RegisterPatientInfo_XiKang(string stringLocalFilePath, string stringPatientOpenId, string stringName, string stringSex, string stringPhoneNum, string stringProofNum, string stringPassword)
{
	m_pInternalNetInterface->SetSaveType(2);
	m_pInternalNetInterface->SetFilePath(stringLocalFilePath);
	int nRet = m_pInternalNetInterface->RegisterPatientInfo_XiKang(stringPatientOpenId, stringName, stringSex, stringPhoneNum, stringProofNum, stringPassword);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}

int CExternalNetInterface::BindPatient_XiKang(string stringLocalFilePath, string stringDoctorOpenId, string stringPatientOpenId)
{
	m_pInternalNetInterface->SetSaveType(2);
	m_pInternalNetInterface->SetFilePath(stringLocalFilePath);
	int nRet = m_pInternalNetInterface->BindPatient_XiKang(stringDoctorOpenId, stringPatientOpenId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}

int CExternalNetInterface::GetPatientIdList_XiKang(string stringLocalFilePath, string stringDoctorOpenId)
{
	m_pInternalNetInterface->SetSaveType(2);
	m_pInternalNetInterface->SetFilePath(stringLocalFilePath);
	int nRet = m_pInternalNetInterface->GetPatientIdList_XiKang(stringDoctorOpenId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}

int CExternalNetInterface::SearchPatientId_XiKang(string stringLocalFilePath, string stringKey, string stringValue)
{
	m_pInternalNetInterface->SetSaveType(2);
	m_pInternalNetInterface->SetFilePath(stringLocalFilePath);
	int nRet = m_pInternalNetInterface->SearchPatientId_XiKang(stringKey, stringValue);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}

int CExternalNetInterface::GetPatientInfo_XiKang(string stringLocalFilePath, string stringPatientOpenId)
{
	m_pInternalNetInterface->SetSaveType(2);
	m_pInternalNetInterface->SetFilePath(stringLocalFilePath);
	int nRet = m_pInternalNetInterface->GetPatientInfo_XiKang(stringPatientOpenId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}

int CExternalNetInterface::GetCaseList_XiKang(string stringLocalFilePath, vector<string> vPatientOpenId)
{
	m_pInternalNetInterface->SetSaveType(2);
	m_pInternalNetInterface->SetFilePath(stringLocalFilePath);
	int nRet = m_pInternalNetInterface->GetCaseList_XiKang(vPatientOpenId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}

int CExternalNetInterface::SubmitCase_XiKang(string stringLocalFilePath, string stringPatientOpenId, string stringAdvice, string stringCaseType, string stringEcgXmlContent)
{
	m_pInternalNetInterface->SetSaveType(2);
	m_pInternalNetInterface->SetFilePath(stringLocalFilePath);
	int nRet = m_pInternalNetInterface->SubmitCase_XiKang(stringPatientOpenId, stringAdvice, stringCaseType, stringEcgXmlContent);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}

int CExternalNetInterface::GetReportFile_XiKang(string stringLocalFilePath, string stringCaseId)
{
	m_pInternalNetInterface->SetSaveType(1);
	m_pInternalNetInterface->SetFilePath(stringLocalFilePath);
	int nRet = m_pInternalNetInterface->GetReportFile_XiKang(stringCaseId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}

int CExternalNetInterface::UploadFile_XiKang(string stringLocalFilePath, string stringCaseId, string stringCaseFilePath)
{
	m_pInternalNetInterface->SetSaveType(2);
	m_pInternalNetInterface->SetFilePath(stringLocalFilePath);
	int nRet = m_pInternalNetInterface->UploadFile_XiKang(stringCaseId, stringCaseFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}

int CExternalNetInterface::ExecuteGWService(string stringLocalFilePath, string stringLastPath, string stringJson, string stringFilePath)
{
	m_pInternalNetInterface->SetSaveType(2);
	m_pInternalNetInterface->SetFilePath(stringLocalFilePath);
	int nRet = m_pInternalNetInterface->ExecuteGWService(stringLastPath, stringJson, stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}

int CExternalNetInterface::ExecuteCommonRESTful(string stringLocalFilePath, string stringIp, string stringPort, string stringUrl, string stringContentType, string stringPayload)
{
	m_pInternalNetInterface->SetSaveType(2);
	m_pInternalNetInterface->SetFilePath(stringLocalFilePath);
	int nRet = m_pInternalNetInterface->ExecuteCommonRESTful(stringIp, stringPort, stringUrl, stringContentType, stringPayload);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return nRet;
}