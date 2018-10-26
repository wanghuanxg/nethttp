#pragma once
#include "InternalNetIntetface.h"
#include <string>
#include <map>


using namespace std;

class CExternalNetInterface
{
public:
	CExternalNetInterface(void);
	~CExternalNetInterface(void);
private:
	CInternalNetIntetface* m_pInternalNetInterface;
public:
	void SetSessionId(string stringSessionId);
	string GetSessionId();
	void SetGWSessionId(string stringSessionId);
	string GetGWSessionId();
public:
	//专家端接口
	int DoctorLogin(string stringLocalFilePath, string stringUsername, string stringPassword, string stringLanguage, string stringSignPath, 
		string stringHospitalInfoPath, string stringHospitalPhotoPath, string stringCliniqueListPath, string stringHospitalListPath, 
		string stringDoctorListPath, string& stringIp);
	int UploadSign(string stringSignPath);
	int UploadPhoto(string stringPhotoPath);
	int GetUnanalyzedCaseList(string stringLocalFilePath, string stringOwnerId, string stringOrderBy, unsigned int nPageTo, unsigned int nPageSize);
	int SearchCaseFromServer(string stringLocalFilePath, string stringCaseId, string stringCaseType, string stringCaseSource, 
		string stringCaseState, string stringPatientName, string stringSenderId, string stringReceiverId, string stringStartDate, string stringEndDate, 
		string stringCheckStartDate, string stringCheckEndDate, string stringSenderName, string stringDiagnostic, string stringCaseOrigin, string stringYzlb, string stringAId, 
		string stringSex, string stringOrderBy, string stringThirdId, string stringLang, string stringIsFullName, unsigned int nPageTo, unsigned int nPageSize);
	int GetAutoCaseInfo(string stringLocalFilePath, string stringCaseType,string stringSenderId,string stringCaseOrigin);
	int ManualDownloadCase(string stringLocalFilePath, string stringCaseId, string stringHospitalId, string stringReceiverId, string stringUri, unsigned long nFileSize, 
		pProgressAndSpeedCallback pCallback, bool bDownloadNotSelfOwn);
	int RollbackCase(string stringCaseId, string stringReceiverId, string stringBackTo);
	int TransmitCase(string stringCaseId, string stringReceiverId, string stringForwardToH, string stringForwardToHospitalName, 
		string stringForwardToD, string stringForwardToDoctorName);
	int InvalidCase(string stringCaseId, string stringReceiverId, string stringReason);
	int AnalyzeBegin(string stringCaseId, string stringReceiverId, string stringCaseType);
	int AnalyzeComplete(string stringCaseId, string stringReceiverId, string stringCaseType);
	int UploadReport(string& stringReportId, string stringCaseId, string stringReceiverId, string stringDiagnosis, string stringDigitalSign, string stringTimestampSign, string stringCaseType, string stringLocalReportPath, int nResultFlag, 
		string stringFileType, pProgressAndSpeedCallback pCallbcak);
	int DoctorDownloadReport(string stringLocalFilePath, string stringUri, unsigned long nFileSize, pProgressAndSpeedCallback pCallback);
	int UploadAdvice(string stringUserId, string stringUsername, string stringMsg, string stringType, string stringIsSendMsg);	
	int DoctorChangeInfo(string stringName, string stringSex, string stringTel, string stringBirthyead, string stringPosition, string stringNotes);
	int DoctorGetSelfInfo(string stringLocalFilePath, string stringSenderId);//接口有变动，加了一个参数senderid，表示要查找信息的人，如为空表示自己
	int GetReviewReportList(string stringLocalFilePath, string stringReportStartDate, string stringReportEndDate,string stringReviewStartDate,
		string stringReviewEndDate, string stringReviewState, string stringCaseType, string stringPtName, string stringSenderId, string stringYzlb, string stringAId, 
		string stringReviewerId, string stringReceiverId, string stringCaseId, unsigned int nPageTo, unsigned int nPageSize);
	int ReviewReport(string stringReportKeyId, string stringReviewState, string stringPath, string stringDiagnostic, 
		string stringDigitalSign, string stringTimeStampSign);
	int GetCardUserList (string stringLocalFilePath, string stringCardId, string stringSenderId, string stringPersonId, string stringTel, string stringName,
		string stringType, unsigned int nPageTo, unsigned int nPageSize);
	int MarkCaseException(string stringCaseId, string stringReceiverId, int nResultFlag);
	int GetAllTeamList(string stringLocalFilePath);
	int GetDoctorListOfTeam(string stringLocalFilePath, string stringTeamId);
	int DoctorHandledCaseStatistics(string stringLocalFilePath, string stringCaseType, string stringStartTime, string stringEndTime);
	int DoctorDownloadCaseAndOneReportOfOldCm(string stringLocalCasePath, string stringLocalReportPath, string stringCaseId, string stringReportUri, unsigned long nReportSize, pProgressAndSpeedCallback pCallback);
	int GetTrendData(string stringLocalFilePath, string stringUId, string stringInterUId, string stringTrendType, string stringStartDate, string stringEndDate, string stringNextId);
	int GetHealthDegree(string stringLocalFilePath, string stringUId, string stringInterUId, string stringDate);
	int GetUserOfNoAdviceAndMostData(string stringLocalFilePath, string stringDays);
	int AdviceDelay(string stringUId, string stringMinutes);
	int AdviceComplete(string stringUId, string stringOptionType, string stringAdvice);
	int GetUserListOfNoAdvice(string stringLocalFilePath, string stringDays, unsigned int nPageTo, unsigned int nPageSize);
	int AdviceBegin(string stringUId);
	int GetUserListOfAdvicing(string stringLocalFilePath, unsigned int nPageTo, unsigned int nPageSize);
	int GetAskList(string stringLocalFilePath, string stringAskType, string stringAskEnd, string stringDoctorId, string stringIsAsk, string stringUserId, string stringStartTime, string stringEndTime, unsigned int nPageTo, unsigned int nPageSize);
	int GetAskDetail(string stringLocalFilePath, string stringAskId);
	int SubmitQuestion(string stringUserId, string stringAdvise, string stringReportType, string stringAskId, string stringAskedId, string stringDoctorPhotoPath);
	int GetCliniqueListOfHospital(string stringLocalFilePath, string stringHospitalId);
	int GetStatisticsInfo(string stringLocalFilePath, string stringTypeId, string stringMonth, string stringMonthDay, unsigned int nPageTo, unsigned int nPageSize);
	int GetUserHealthList(string stringLocalFilePath, string stringNextId, string stringTypeId, string stringSecondTypeId, string stringName, string stringSex, string stringThirdIdType, 
		string stringThirdId, string stringHospitalId, string stringUnitSenderId, string stringIsData, string stringIntervalTime, string stringPopulations, string stringContractDoctor, string stringIsContract, string stringPageSize);
	int GetAdviceImplemention(string& stringPercent, string stringThirdId, string stringHospitalId);
	int ChangePersonType(string stringThirdId, string stringPeopleType, string stringHospitalId);
	int SetNextVisitTime(string stringThirdId, string stringInterViewTime, string stringHospitalId);
	int GetPersonTypeList(string stringLocalFilePath, string stringLanguage);
	int PersonIdRegisterAndLogin(string stringRole, string stringPId, string stringName, string stringSex, string stringBirthday, string stringAddress, string stringDeviceId, string stringHospitalId, 
		string stringLang, string stringRegFrom);
	int GetTrendRangeValue(string stringLocalFilePath);
	int OnlyDownloadCase(string stringLocalFilePath, string stringCaseId, string stringReceiverId, string stringUri, unsigned long nFileSize, pProgressAndSpeedCallback pCallback);
	int HoldPlaceAndAnalyzeBegin(string stringCaseId, string stringHospitalId, string stringReceiverId, string stringCaseType);
	int ReviewReportWithFile(string stringCaseId, string stringReceiverId, string stringReportKeyId, string stringReviewState, string stringLoaclFilePath, string stringDiagnostic, 
		string stringDigitalSign, string stringTimeStampSign, string stringResultFlag, string stringFileType, pProgressAndSpeedCallback pCallback);
	int AddTag(string& stringTagId, string stringTagName, string stringTagDescription);
	int GetTagList(string stringLocalFilePath, string stringSenderId);
	int AddCaseToTag(string stringTagId, string stringTagName, string stringCaseId, string stringCaseType, string stringCaseTypeName, string stringPatientName);
	int GetCaseListOfTag(string stringLocalFilePath, string stringTagId, string stringSenderId, string stringCaseType);
	
	//公共接口
	int KeepSession();
	int Logout();
	int UploadIm(string stringSenderId, string stringSenderName, string stringReceiverId, string stringMsgDirection, string stringMsgType, 
		string stringMsgContent, string stringAudioFilePath, string stringCaseId, string stringCaseType);
	int GetImList(string stringLocalFilePath, string stringSenderId, string stringCaseId, string stringCaseType, string stringMsgDirection, string stringMsgType);
	int GetImList2(string stringLocalFilePath, string stringNextId);
	int DownloadIm(string stringLocalFilePath, string stringMsgId, string stringUri, unsigned long nFileSize);
	int DownloadImComplete(string stringMsgId);
	int ChangePwd(string stringOldPwd, string stringNewPwd);
	int Feedback(string stringTitle, string stringNotes, string stringPhone, string stringPicPath);
	int GetReportListOfCase(string stringLocalFilePath, string stringCaseId);
	int GetHospitalListOfGroup(string stringLocalFilePath, string stringGroupId, string stringType, string stringCaseType, unsigned int nPageTo, unsigned int nPageSize);
	int GetDoctorListOfHospital(string stringLocalFilePath, string stringHospitalId, string stringDoctorId, string stringCaseType, string stringDoctorType, unsigned int nPageTo, unsigned int nPageSize);
	int AbortSession();
	int GetEventStream(string stringLocalFilePath, string stringCaseId, string stringOriginalHospital);
	int GetCaseTypeList(string stringLocalFilePath, string stringLanguage);
	int DownloadPhoto(string stringLocalFilePath, string stringUri, unsigned long nFileSize, pProgressAndSpeedCallback pCallback);
	int GetAlreadyUploadFileSize(string stringUri, int& nFileSize);
	int SendPhoneMsg(string stringCardId, string stringPhone, string stringMsg);
	int GetFileWithHttp(string stringLocalFilePath, string stringUrl, int bContinue);
	int PostForm(string& stringResponse, string stringHost, string stringUrl, map<string, string> mapParams);
	int GetCardUserReportList(string stringLocalFilePath, string stringReportId, string stringReportStartDate, string stringReportEndDate, string stringCaseType, string stringState, string stringOrderBy, 
		string stringThirdIdType, string stringThirdId, string stringNextId, string stringPageSize);
	int SearchCaseAdvanced(string stringLocalFilePath, string stringHospitalId, string stringDiagnostic, 
		string stringName, string stringSex, QUERY_RANGE rangeAge, QUERY_RANGE rangeCreateTime, string stringMode, 
		string stringCaseType, QUERY_ECG ecg, string stringFrom, string stringSize);
	int SearchCaseAdvancedString(string stringLocalFilePath, string stringHospitalId, string stringDiagnostic, 
		string stringName, string stringSex, string stringJsonAge, string stringJsonCreateTime, string stringMode, 
		string stringCaseType, string stringJsonEcg, string stringFrom, string stringSize);
	int GetPayCode(string stringLocalFilePath, string stringType, string stringCaseId);
	int GetOrderInfo(string stringLocalFilePath, string stringType, string stringCaseId);

	//客户端接口
	int UserLogin(string stringLocalFilePath, string stringUsername, string stringPassword, string stringRole, string stringToken, string& stringIp);
	int GetUserInfo(string stringLocalFilePath);
	int GetHospitalInfo(string stringLocalFilePath, string stringHospitalId);
	int GetDoctorInfo(string stringLocalFilePath, string stringDoctorId);
	int UploadCase(string& stringCaseId, string stringName, string stringThirdId, string stringDataType, string stringDataTypeName, string stringNotes, string stringHospitalId, 
		string stringHospitalName, string stringDoctorId, string stringDoctorName, string stringCheckTime, string stringOtherParam, string stringDeviceId, string stringDeviceName, string stringCaseOrigin, string stringYzlb, string stringSex, 
		string stringApplicationNo, string stringAutoMatic, string stringLocalCasePath, pProgressAndSpeedCallback pCallback);
	int UserDownloadReport(string stringLocalFilePath, string stringCaseId, string stringReportId, string stringUri, unsigned long nFileSize, pProgressAndSpeedCallback pCallback);
	int GetCaseReportState(string& stringReportState, string stringCaseId);
	int UserGetReportList(string stringLocalFilePath, string stringReportId, string stringReportStartDate, string stringReportEndDate, 
		string stringCaseType, string stringReportState, string stringOrderBy, unsigned long nPageTo, unsigned long nPageSize);
	int UserGetCaseList(string stringLocalFilePath, string stringCaseId, string stringCaseType, string stringCaseState, string stringThirdId, string stringName, 
		string stringStartDate, string stringEndDate, string stringCheckStartDate, string stringCheckEndDate, string stringCaseOrigin, 
		string stringYzlb, string stringSex, string stringIsRead, string stringOrderBy, unsigned int nPageTo, unsigned int nPageSize);
	int GetAdvice(string stringLocalFilePath, string stringSgId, string stringUserId, string stringDoctorId, string stringDownloadState, string stringType, 
		string stringStartDate, string stringEndDate, unsigned int nPageTo, unsigned int nPageSize);
	int MarkReportRead(string stringReportId);
	int CardUserChangeInfo(string stringPId, string stringName, string stringSex, string stringTel, string stringBirthday, string stringAddress, string stringLmp, string stringToken, string stringHeight, string stringWeight, string stringPhoto, string stringVCodeBeFollowed, string stringHospitalId, string stringOtherInfo);
	int CliniqueChangeInfo(string stringPId, string stringName, string stringTel, string stringNotes);
	int UploadTrendData(string stringCardId, string stringDeviceId, string stringDeviceName, string stringCaseId, vector<RECORD_DATA> vRecordData);
	int UploadTrendDataString(string stringCardId, string stringDeviceId, string stringDeviceName, string stringCaseId, vector<RECORD_DATA_STRING> vRecordData);
	int UploadTrendDataFile(string stringFilePath);
	int VerifyThirdId(string& stringCardId, string stringUId, string stringType);
	int PersonIdRegister(string stringLocalFilePath, string stringType, string stringRegFrom, string stringPersonId, string stringName, string stringSex, string stringBirthday, string stringAddress, 
		string stringPhone, string stringHeigth, string stringUploadUnitId, string stringHospitalId, string stringHGroupId, string stringScode, string stringWxUId, string stringLmp, string stringWeight, string stringPassword, 
		string stringThirdCode, string stringIsRegister);
	int GetVerificationCode(string stringType, string stringPhone);
	int GetDataDictionary(string stringLocalFilePath);
	int LoggingHealthBaseInfo(string stringUserId, string stringWeblock, string stringNation, string stringProfression, string stringCrowd, string stringBloodType, 
		string stringJwsJiBing, string stringJwsShouShu, string stringJwsWaiShang, string stringJwsShuXue, string stringJwsBaoLuShi, string stringJwsBaoLuShiBz, 
		string stringJwsYiChuanJiBing, string stringGmsGuoMinShi, string stringGmsGuoMinShiBz, string stringJzsFuQin, string stringJzsFuQinBz, string stringJzsMuQin, 
		string stringJzsMuQinBz, string stringJzsXdjm, string stringJzsXdjmBz, string stringJzsZiNv, string stringJzsZiNvBz, string stringJzsQiTa);
	int LoggingHealthCurrentInfo(string stringUserId, string stringShfsYinShi, string stringShfsXlzk, string stringShfsXlzkBz, string stringShfsXltz, string stringShfsXiYan, 
		string stringShfsYinJiu, string stringShfsSyqk, string stringShfsYunDong, string stringShfsYunDongShiJian, string stringSysjcDanGuChun, string stringSysjcXueChangGui, 
		string stringSysjcXueChangGuiBz, string stringSysjcNiaoChangGui, string stringSysjcNiaoChangGuiBz, string stringSysjcQiTa, string stringYongYao, string stringZhuYuan, 
		string stringJkwtNaoXueGuan, string stringJkwtNaoXueGuanBz, string stringJkwtShenZang, string stringJkwtShenZangBz, string stringJkwtXinZang, string stringJkwtXinZangBz, 
		string stringJkwtXueGuan, string stringJkwtXueGuanBz, string stringJkwtYanBu, string stringJkwtYanBuBz, string stringJkwtShenJing, string stringJkwtQiTa, string stringDate);
	int UpdateCaseInfo(string& stringDoctorId, string stringHospitalId, string stringCaseId, string stringOtherParam, string stringFlag, string stringNotes, string stringReceiverId);
	int PersonIdRegisterAndLogin(string stringLocalFilePath, string stringRole, string stringPId, string stringName, string stringSex, string stringBirthday, string stringAddress, string stringDeviceId, string stringHospitalId, 
		string stringLang, string stringRegFrom);
	int UploadTrendData2(string stringTargetPath);
	int CardBindPhone(string stringPhone, string stringScode);
	int ResetPassword(string stringPhone, string stringScode, string stringNewPwd);
	int UploadDaily(string stringType, string stringCreateTime, string stringFiles);
	int GetDateListOfDaily(string stringLocalFilePath, string stringYear, string stringMonth);
	int GetDailyOfDate(string stringLocalFilePath, string stringDate, string stringType);
	int ShareDaily(string stringLocalFilePath, string stringType, string stringCreateTime);
	int DeleteDaily(string stringType, string stringDate);
	int PhoneScanQR(string& stringSence, string stringQRCode);
	int PhoneScanQRVerify(string stringQRCode);
	int ApplyQRCode(string stringLocalFilePath, string stringType, string stringScene);
	int ValidateUserAccount(string stringLocalFilePath, string stringLoginId, string stringPassword, string stringRole);
	int RegisterBluetoothDevice(string stringUniqueSN, string stringSerial, string stringBluetoothAddress, string stringBluetoothName, string stringDeviceType);
	int GetBluetoothInfoOfDevice(string stringLocalFilePath, string stringUniqueSN);
	int ChangeDeviceType(string stringUniqueSN, string stringDeviceType);
	int UnregisterBluetoothDevice(string stringUniqueSN);
	int GetHospitalListOfArea(string stringLocalFilePath, string stringAreaId, string stringRank, string stringBeta);
	int UploadCommonFile(string& stringUri, string stringType, string stringExtra, string stringLocalFilePath, string stringFileType, pProgressAndSpeedCallback pCallbcak);
	int SearchSomebody(string stringLocalFilePath, string stringUserId);
	int FollowSomebody(string stringUserId, string stringUsername, string stringNickname, string stringVCode);
	int MyFollowing(string stringLocalFilePath, string stringUserId, string stringNextId, string stringFType);
	int MyFans(string stringLocalFilePath, string stringUserId, string stringNextId);
	int ChangeNicknameOfFollowed(string stringUserId, string stringNickname);
	int CancelFollowing(string stringUserId);
	int PublishHealthyKnowledge(vector<HEALTHY_KNOWLEDGE> vHealthKnowledge);
	int GetListOfHealthyKnowledge(string stringLocalFilePath, string stringSenderId, string stringNextId, string stringIds, string stringPageSize, string stringOrderBy);
	int GenerateOrDeletePngZlibReport(string stringLocalFilePath, string stringServerPath, string stringFlag);
	int GetDailyList(string stringLocalFilePath, string stringType, string stringStartDate, string stringEndDate, string stringOrderBy, string stringNextId, string stringPageSize);
	int ChangeViewLikeCollectCount(string stringHealthTipId, string stringUri, string stringTitle, string stringLike, string stringRead, string stringCollect);
	int ChangeAdviceExecutiveState(string stringSgId, string stringState);
	int GetCliniqueReportList(string stringLocalFilePath, string stringCliniqueSenderId, string stringStartDate, 
		string stringEndDate, string stringState, string stringName, string stringPatientSourceCode, string stringYzlb, string stringPIdType, string stringPId, string stringHb, string stringASectionId, string stringNextId, string stringPageSize);
	int GetUserListBindingOrNot(string stringLocalFilePath, string stringNextId, string stringPageSize, string stringIsBind, string stringIsActive);
	int BindUser(string stringUId, string stringUName);
	int UnbindUser(string stringUId);
	int GetCardUserListOfCustomer(string stringLocalFilePath, string stringThirdId, string stringName, string stringCaseType, string stringStartDate, string stringEndDate, string stringNextId, unsigned int nPageSize);
	int SignDoctor(string stringUId);
	int RefundFee(string stringType, string stringCaseId, string stringRefundFee);
	int HoldPlaceReview(string stringReportId);
	int RollBackReview(string stringReportId);
	int ResetReviewHoldPlace(string stringReportId, string stringOccupantId, string stringOccupantName, string stringSuperKey);
	int ResetCaseHoldPlace(string stringCaseId, string stringReceiverId, string stringOccupantId, string stringOccupantName, string stringSuperKey);

	//冠新接口
//	int CreateSession_GuanXin(string stringUsername, string stringPassword);
	int GetAccessToken_GuanXin(string stringLocalFilePath, string stringUsername, string stringPassword);
	int GetPersonInfo_GuanXin(string stringLocalFilePath, string stringAccessToken, string stringPersonName, string stringPersonGender, string stringPersonIdentity, string stringPersonBirthDate);
	int UploadECGData_GuanXin(string stringLocalFilePath, string stringECGXmlPath, string stringECGDataPath);
	int UploadOtherData_GuanXin(string stringLocalFilePath, string stringOtherXmlPath);
	int UploadCheckData_GuanXin(string stringLocalFilePath, string stringCheckXmlPath);
	int UploadPersonInfo_GuanXin(string stringLocalFilePath, string stringPersonInfoXmlPath);

	

	//院内使用接口
	int SetCliniqueBusy(string stringCliniqueId);
	int SetCliniqueFree(string stringCliniqueId);
	int SetCliniqueChecking(string stringCliniqueId);
	int SetCliniqueShoutComplete(string stringCliniqueId);
	int GetPatientInfoByAppNum(string stringLocalFilePath, string stringHospitalId, string stringOrderType, string stringOrderId, string stringReqDepart, string stringOrderState, string stringCheckType, string stringStartDate, string stringEndDate, string stringClinicId);
	int GetPatientInfoByAppNumToHis(string stringLocalFilePath, string stringHospitalId, string stringOrderType, string stringOrderId);
	int GetPatientInfoByAppNumToPhmsAndHis(string stringLocalFilePath, string stringHospitalId, string stringOrderType, string stringOrderId);
	int ApplyCheck(string stringCliniqueId, string stringOrderId, string stringOrderType);
	int GetWaitingCheck(string stringLocalFilePath, string stringCliniqueId);
	int GetCliniqueState(string stringLocalFilePath, string stringCliniqueId);
	int NotifyCaseState(string stringHospitalId, string stringOrderType, string stringOrderId);
	int NotifyReportState(string stringReportId);
	int SetDoctorAgentId(string stringDoctorAgentId);
	int NotifyAppState(string stringUnitId, string stringOrderId, string stringOrderType, string stringState);
	int VerifyAppState(string stringLocalFilePath, string stringOrderId, string stringOrderType);
	int GetHisReportState(string stringLocalFilePath, string stringHospitalId, string stringOrderType, string stringOrderId);
	int SetHisCriticalValue(string stringHospitalId, string stringCaseId, string stringCriticalValues);
	//东亚医讯接口
	int GetDoctorList_999120(string stringLocalFilePath, string stringKtCliniqueId, string stringCaseTypeId);
	int ValidatePay_999120(string stringLocalFilePath, string stringConsumeId, string stringKtCliniqueId, string stringCardNumber, string stringKtDoctorId, string stringConsumeMoney, string stringCaseTypeId);
	int UpdateCaseForConsumerRecord_999120(string stringLocalFilePath, string stringConsumeId, string stringKtCaseId, string stringCaseTypeId);
	//北京心医接口
	int ValidatePay_XinJie(string stringLocalFilePath, string stringRrn, string stringLocalDate);
	int NotifyCheck_XinJie(string stringLocalFilePath, string stringRrn, string stringLocalDate);
	//电信健康小屋接口
	int UserAuthorize_Telecom(string stringLocalFilePath, string stringPersonId, string stringName, string stringNation, string stringAddress, string stringRoomId);
	int BloodPressureUpload_Telecom(string stringLocalFilePath, string stringPersonType, string stringPersonId, string stringSBP, string stringDBP, string stringAverageValue, string stringPulseRate, string stringDetectionTime, 
		string stringEquCode, string stringRoomId);
	int BloodOxygenUpload_Telecom(string stringLocalFilePath, string stringPersonType, string stringPersonId, string stringBloodOxygen, string stringPulseRate, string stringDetectionTime, 
		string stringEquCode, string stringRoomId);
	int BloodSugarUpload_Telecom(string stringLocalFilePath, string stringPersonType, string stringPersonId, string stringBloodSugar, string stringDetectionTime, 
		string stringEquCode, string stringRoomId);
	int HeightWeightUpload_Telecom(string stringLocalFilePath, string stringPersonType, string stringPersonId, string stringHeight, string stringWeight, string stringBMI, string stringDetectionTime, 
		string stringEquCode, string stringRoomId);
	int LungUpload_Telecom(string stringLocalFilePath, string stringPersonType, string stringPersonId, string stringM_B1, string stringBSA, string stringBSA1, string stringMMF, string stringMefSeventyFive, string stringMefFifty, 
		string stringMefTwentyFive, string stringMefFVST, string stringIC, string stringIRV, string stringERV, string stringFevOne, string stringFevTwo, string stringFevThree, 
		string stringFevOnePer, string stringFevTwoPer, string stringFevThreePer, string stringRR, string stringFVC, string stringPEF, string stringTV, string stringVC, string stringMVV, 
		string stringMV, string stringVSTHeight, string stringVSMvvBsa, string stringVSMvvOne, string stringSVC, string stringFevOneZeroFive, string stringFevOneZeroFivePer, string stringPefOne, 
		string stringPefTwo, string stringPefThree, string stringFive, string stringFiveOne, string stringFiveTwo, string stringFiveThree, string stringPIF, string stringMifFifty, 
		string stringDetectionTime, string stringEquCode, string stringRoomId);
	int ECGUpload_Telecom(string stringLocalFilePath, string stringPersonType, string stringPersonId, string stringDetectionTime, 
		string stringEquCode, string stringRoomId, TELCOM_ECG ecgData);
	//东软熙康接口
	int Login_XiKang(string stringLocalFilePath, string stringUsername, string stringPassword);
	int RegisterPatientId_XiKang(string stringLocalFilePath, string stringName, string stringSex, string stringPhoneNum, string stringProofNum);
	int RegisterPatientInfo_XiKang(string stringLocalFilePath, string stringPatientOpenId, string stringName, string stringSex, string stringPhoneNum, string stringProofNum, string stringPassword);
	int BindPatient_XiKang(string stringLocalFilePath, string stringDoctorOpenId, string stringPatientOpenId);
	int GetPatientIdList_XiKang(string stringLocalFilePath, string stringDoctorOpenId);
	int GetPatientInfo_XiKang(string stringLocalFilePath, string stringPatientOpenId);
	int SearchPatientId_XiKang(string stringLocalFilePath, string stringKey, string stringValue);
	int GetCaseList_XiKang(string stringLocalFilePath, vector<string> vPatientOpenId);
	int SubmitCase_XiKang(string stringLocalFilePath, string stringPatientOpenId, string stringAdvice, string stringCaseType, string stringEcgXmlContent);
	int GetReportFile_XiKang(string stringLocalFilePath, string stringCaseId);
	int UploadFile_XiKang(string stringLocalFilePath, string stringCaseId, string stringCaseFilePath);
	//公卫接口
	int ExecuteGWService(string stringLocalFilePath, string stringLastPath, string stringJson, string stringFilePath);
	//通用RESTful接口
	int ExecuteCommonRESTful(string stringLocalFilePath, string stringIp, string stringPort, string stringUrl, string stringContentType, string stringPayload);
};
