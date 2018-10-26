#pragma once
#include <string>
#include <vector>
#include <map>
#include "PhmsDataType.h"
#include "PhmsSession.h"
#include "Poco/Event.h"

using namespace std;
using Poco::Event;
class CInternalNetIntetface
{
public:
	CInternalNetIntetface(void);
	~CInternalNetIntetface(void);
private:
	CPhmsSession* m_pPhmsSession;
	int m_nSaveType;
	string m_stringContent;
	string m_stringFilePath;
	Event m_eventSocketAbort;
	string m_stringIp;
public:
	//Setter
	void SetSaveType(int nSaveType);
	void SetFilePath(string stringFilePath);
	void SetSessionId(string stringSessionId);
	void SetCommandType(EnumPhmsCommandType enumCommandType);
	void SetGWSessionId(string stringSessionId);
	//Getter
	int GetSaveType();
	string GetContent();
	string GetSessionId();
	EnumPhmsCommandType GetCommandType();
	string GetIp();
	string GetGWSessionId();
private:
	typedef struct  tagSEGMENT_INFO
	{
		string m_stringStart;
		string m_stringEnd;
		string m_stringMd5;
	}SEGMENT_INFO;
	typedef vector< SEGMENT_INFO > vSegmentInfo;
	int LoadServerPathAndAlreadyUploadSize(string stringLocalIniPath, string& stringServerPath, string& stringAlreadyUploadSize);
	int StoreServerPathAndAlreadyUploadSize(string stringLocalIniPath, string stringServerPath, string stringAlreadyUploadSize);
	int GetUploadSegmentInfo(int nSegmentCount,unsigned long nAlreadyUploadSize, unsigned long nThisTimeUploadSize, string stringLocalPath, vSegmentInfo& vSI);
	int GenerateTempUploadFile(int nAlreadyUploadSize, int nThisTimeUploadSize, string stringLocalFilePath, string& stringTempUploadFilePath);
	int GetOrder(unsigned long nTotalSize, unsigned long nAlreadyUploadSize, unsigned long nThisTimeUploadSize, string& stringOrder);
	int GetDownloadTempFileSize(string stringTempDirectory, int nThreadCount, unsigned long* pTempFileSize);
	int DivideSegment(unsigned long nRemainFileSize, unsigned long nSegmentMaxSize, int& nThreadCount, unsigned long* pSegmentFileSize);
	int CombineSegmentFile(string stringLocalFile, string stringTempDirectory, int nThreadCount);
public:
	//内部接口--针对PHMS接口
	int Login(string stringRole, string stringUsername, string stringPassword, string stringLanguage, string stringToken, bool bMustSsl);

	int MechineRegister(string stringCountry, string stringAreaId, string stringArea, string stringDevice, string stringSignature, string stringSerialnum);

	int Logout();
	int SetDoctorSign(string stringFilePath);

	int SetDoctorPhoto(string stringFilePath);

	int ApplyNewCase(string stringName, string stringThirdId, string stringDataType, string stringDataTypeName, string stringNotes, string stringHospitalId, 
		string stringHospitalName, string stringDoctorId, string stringDoctorName, string stringCheckTime, string stringOtherParam, string stringDeviceId, string stringDeviceName, 
		string stringMd5, string stringCaseOrigin, string stringYzlb, string stringSex, string stringApplicationNo, string stringAutoMatic);

	int UploadCaseSegment(string stringCaseId, unsigned long nTotalSize,  unsigned long nAlreadyUploadSize, 
		unsigned long nThisTimeUploadSize, string stringServerPath, string stringLocalPath);

	int CaseHoldPlace(string stringCaseId, string stringReceiverId);

	int GetAutoDownloadCase(string stringCaseType,string stringSenderId,string stringCaseOrigin);

	int DownloadCaseBegin(string stringCaseId, string stringReceiverId);

	int DownloadFileSegment(string stringUri, unsigned long nBeginPos, unsigned long nEndPos);

	int DownloadCaseComplete(string stringCaseId, string stringReceiverId);

	int InvalidCase(string stringCaseId, string stringReceiverId, string stringReason);

	int TransmitCase(string stringCaseId, string stringReceiverId, string stringForwardToH, string stringForwardToHospitalName, 
		string stringForwardToD, string stringForwardToDoctorName);

	int RollbackCase(string stringCaseId, string stringReceiverId, string stringBackTo);

	int GetHospitalListOfGroup(string stringHGroupId, string stringType, string stringCaseType, unsigned int nPageTo, unsigned int nPageSize);

	int GetDoctorListOfHospital(string stringHospitalId, string stringDoctorId, string stringCaseType, string stringDoctorType, unsigned int nPageTo, unsigned int nPageSize);

	int AnalyzeBegin(string stringCaseId, string stringReceiverId, string stringCaseType);

	int AnalyzeComplete(string stringCaseId, string stringReceiverId, string stringCaseType);

	int ApplyNewReport(string stringCaseId, string stringReceiverId, string stringDiagnosis, string stringMd5, string stringDigitalSign, string stringTimestampSign, string stringCaseType);

	int UploadReportSegment(string stringCaseId, string stringReceiverId, string stringCaseType, string stringReportId, unsigned long nTotalSize,
		unsigned long nAlreadyUploadSize, unsigned long nThisTimeUploadSize, string stringServerPath, string stringLocalPath, string stringFileType);

	int DownloadReportBegin(string stringCaseId, string stringReportId);

	int DownloadReportComplete(string stringCaseId, string stringReportId);

	int GetAlreadyUploadFileSize(string stringUri);

	int GetReviewReportList(string stringReportStartDate, string stringReportEndDate,string stringReviewStartDate,
		string stringReviewEndDate, string stringReviewState, string stringCaseType, string stringPtName, string stringSenderId, string stringYzlb, string stringAId, 
		string stringReviewerId, string stringReceiverId, string stringCaseId, unsigned int nPageTo, unsigned int nPageSize);

	int ReviewReport(string stringReportKeyId, string stringReviewState, string stringPath, string stringDiagnostic, 
		string stringDigitalSign, string stringTimeStampSign);

	int GetReportStateOfCase(string stringCaseId);

	int GetCaseListOfDoctor(string stringCaseId, string stringCaseType, string stringCaseSource, string stringCaseState,
		string stringPatientName, string stringSenderId, string stringReceiverId, string stringStartDate, string stringEndDate, string stringCheckStartDate,
		string stringCheckEndDate, string stringSenderName, string stringDiagnostic, string stringCaseOrigin, string stringYzlb, string stringAId, string stringSex, string stringOrderBy, 
		string stringThirdId, string stringLang, string stringIsFullName, unsigned int nPageTo, unsigned int nPageSize);

	int GetHospitalInfo(string stringHospitalId);

	int UploadIm(string stringSenderId, string stringSenderName, string stringReceiverId, string stringMsgDirection,
		string stringMsgType, string stringMsgContent, string stringCaseId, string stringCaseType);

	int GetImList(string stringSenderId, string stringCaseId, string stringCaseType, string stringMsgDirection,
		string stringMsgType);

	int GetImList2(string stringNextId);

	int DownloadImComplete(string stringMsgId);

	int SendPhoneMsg (string stringCardId, string stringPhoneNum, string stringMsg);

	int UploadTrendData(string stringCardId, string stringDeviceId, string stringDeviceName, string stringCaseId, vector<RECORD_DATA> vRecordData);

	int UploadTrendDataString(string stringCardId, string stringDeviceId, string stringDeviceName, string stringCaseId, vector<RECORD_DATA_STRING> vRecordData);

	int UploadTrendDataFile(string stringFilePath);
	
	int UploadAudioSegment (string stringMsgId, unsigned long nTotalSize, unsigned long nAlreadyUploadSize, 
		unsigned long nThisTimeUploadSize, string stringServerPath, string stringLocalPath);

	int UploadAdvice (string stringUserId, string stringUsername, string stringMsg, string stringType, string stringIsSendMsg);

	int GetAdvice (string stringSgId, string stringUserId, string stringDoctorId, string stringDownloadState, string stringType, 
		string stringStartDate, string stringEndDate, unsigned int nPageTo, unsigned int nPageSize);

	int DownloadAdviceComplete (string stringAdviceId);

	int GetReportListOfCase (string stringCaseId);

	int GetSelfInfo (string stringSenderId);//接口有变动，加了一个参数senderid，表示要查找信息的人，如为空表示自己

	int ChangePwd (string stringOldPwd, string stringNewPwd);

	int DoctorChangeInfo (string stringName, string stringSex, string stringTel, string stringBirthdear,
		string stringPosition, string stringNotes);

	int CliniqueChangeInfo (string stringPid, string stringName, string stringTel, string stringNotes);

	int UserChangeInfo (string stringPid, string stringName, string stringSex, string stringTel, string stringBirthday,
		string stringAddress, string stringLmp, string stringToken, string stringHeight, string stringWeight, string stringPhoto, string stringVCodeBeFollowed, string stringHospitalId, string stringOtherInfo);

	int GetCaseListOfUser (string stringCaseId, string stringCaseType, string stringCaseState, string stringThirdId, string stringName, 
		string stringStartDate, string stringEndDate, string stringCheckStartDate, string stringCheckEndDate, string stringCaseOrigin, 
		string stringYzlb, string stringSex, string stringIsRead, string stringOrderBy, unsigned int nPageTo, unsigned int nPageSize);

	int GetReportListOfUser (string stringReportId, string stringReportStartDate, string stringReportEndDate,
		string stringCaseType, string stringReportState, string stringOrderBy, unsigned int nPageTo, unsigned int nPageSize);

	int GetCardUserList (string stringCardId, string stringSenderId, string stringPersonId, string stringTel, string stringName,
		string stringType, unsigned int nPageTo, unsigned int nPageSize);

	int KeepSession();

	int MarkCaseException(string stringCaseId, string stringReceiverId, int nResultFlag);

	int MarkReportRead(string stringReportId);

	int Feedback(string stringTitle, string stringNotes, string stringPhone, string stringFilePath);

	int UploadTrendData2(string stringTargetPath);

	int GetAllTeamList();

	int GetDoctorListOfTeam(string stringTeamId);

	int GetEventStream(string stringCaseId, string stringOriginalHospital);

	int GetCaseTypeList(string stringLanguage);

	int DoctorHandledCaseStatistics(string stringCaseType, string stringStartTime, string stringEndTime);

	int UserRegister(string stringPhone, string stringName, string stringHeigth);

	int GetTrendData(string stringUId, string stringInterUId, string stringTrendType, string stringStartDate, string stringEndDate, string stringNextId);//该接口目前只支持保存到文件

	int GetLastPhysiologicalData();

	int GetHealthDegree(string stringUId, string stringInterUId, string stringDate);

	int GetUserOfNoAdviceAndMostData(string stringDays);

	int AdviceDelay(string stringUId, string stringMinutes);

	int AdviceComplete(string stringUId, string stringOptionType, string stringAdvice);

	int GetUserListOfNoAdvice(string stringDays, unsigned int nPageTo, unsigned int nPageSize);

	int AdviceBegin(string stringUId);

	int GetUserListOfAdvicing(unsigned int nPageTo, unsigned int nPageSize);

	int VerifyThirdId(string stringUId, string stringType);

	int PersonIdRegister(string stringType, string stringRegFrom, string stringPersonId, string stringName, string stringSex, string stringBirthday, string stringAddress, 
		string stringPhone, string stringHeigth, string stringUploadUnitId, string stringHospitalId, string stringHGroupId, string stringScode, string stringWxUId, string stringLmp, string stringWeight, string stringPassword, 
		string stringThirdCode, string stringIsRegister);

	int GetVerificationCode(string stringType, string stringPhone);

	int GetAskList(string stringAskType, string stringAskEnd, string stringDoctorId, string stringIsAsk, string stringUserId, string stringStartTime, string stringEndTime, unsigned int nPageTo, unsigned int nPageSize);

	int GetAskDetail(string stringAskId);

	int GetQuestionList(string stringDoctorId, string stringReportType, string stringAskId);

	int SubmitQuestion(string stringUserId, string stringAdvise, string stringReportType, string stringAskId, string stringAskedId, string stringDoctorPhotoPath);

	int GetDataDictionary();

	int LoggingHealthBaseInfo(string stringUserId, string stringWeblock, string stringNation, string stringProfression, string stringCrowd, string stringBloodType, 
		string stringJwsJiBing, string stringJwsShouShu, string stringJwsWaiShang, string stringJwsShuXue, string stringJwsBaoLuShi, string stringJwsBaoLuShiBz, 
		string stringJwsYiChuanJiBing, string stringGmsGuoMinShi, string stringGmsGuoMinShiBz, string stringJzsFuQin, string stringJzsFuQinBz, string stringJzsMuQin, 
		string stringJzsMuQinBz, string stringJzsXdjm, string stringJzsXdjmBz, string stringJzsZiNv, string stringJzsZiNvBz, string stringJzsQiTa);

	int LoggingHealthCurrentInfo(string stringUserId, string stringShfsYinShi, string stringShfsXlzk, string stringShfsXlzkBz, string stringShfsXltz, string stringShfsXiYan, 
		string stringShfsYinJiu, string stringShfsSyqk, string stringShfsYunDong, string stringShfsYunDongShiJian, string stringSysjcDanGuChun, string stringSysjcXueChangGui, 
		string stringSysjcXueChangGuiBz, string stringSysjcNiaoChangGui, string stringSysjcNiaoChangGuiBz, string stringSysjcQiTa, string stringYongYao, string stringZhuYuan, 
		string stringJkwtNaoXueGuan, string stringJkwtNaoXueGuanBz, string stringJkwtShenZang, string stringJkwtShenZangBz, string stringJkwtXinZang, string stringJkwtXinZangBz, 
		string stringJkwtXueGuan, string stringJkwtXueGuanBz, string stringJkwtYanBu, string stringJkwtYanBuBz, string stringJkwtShenJing, string stringJkwtQiTa, string stringDate);

	int GetCliniqueListOfHospital(string stringHospitalId);

	int GetStatisticsInfo(string stringTypeId, string stringMonth, string stringMonthDay, unsigned int nPageTo, unsigned int nPageSize);

	int UpdateCaseInfo(string stringHospitalId, string stringCaseId, string stringOtherParam, string stringFlag, string stringNotes, string stringReceiverId);

	int GetUserHealthList(string stringNextId, string stringTypeId, string stringSecondTypeId, string stringName, string stringSex, string stringThirdIdType, 
		string stringThirdId, string stringHospitalId, string stringUnitSenderId, string stringIsData, string stringIntervalTime, string stringPopulations, string stringContractDoctor, string stringIsContract, string stringPageSize);

	int GetAdviceImplemention(string stringThirdId, string stringHospitalId);

	int ChangePersonType(string stringThirdId, string stringPeopleType, string stringHospitalId);

	int SetNextVisitTime(string stringThirdId, string stringInterViewTime, string stringHospitalId);

	int GetPersonTypeList(string stringLanguage);

	int PersonIdRegisterAndLogin(string stringRole, string stringPId, string stringName, string stringSex, string stringBirthday, string stringAddress, string stringDeviceId, string stringMd5, 
		string stringLang, string stringRegFrom);

	int CardBindPhone(string stringPhone, string stringScode);

	int ResetPassword(string stringPhone, string stringScode, string stringNewPwd);

	int UploadDaily(string stringType, string stringCreateTime, string stringFiles);

	int GetDateListOfDaily(string stringYear, string stringMonth);

	int GetDailyOfDate(string stringDate, string stringType);

	int ShareDaily(string stringType, string stringCreateTime);

	int DeleteDaily(string stringType, string stringDate);

	int PhoneScanQR(string stringQRCode);

	int PhoneScanQRVerify(string stringQRCode);

	int ApplyQRCode(string stringType, string stringScene);

	int ValidateUserAccount(string stringLoginId, string stringPassword, string stringRole);

	int RegisterBluetoothDevice(string stringUniqueSN, string stringSerial, string stringBluetoothAddress, string stringBluetoothName, string stringDeviceType);
	
	int GetBluetoothInfoOfDevice(string stringUniqueSN);
	
	int ChangeDeviceType(string stringUniqueSN, string stringDeviceType);
	
	int UnregisterBluetoothDevice(string stringUniqueSN);

	int GetHospitalListOfArea(string stringAreaId, string stringRank, string stringBeta);
	int UploadCommonFileSegment (string stringType, string stringExtra, unsigned long nTotalSize, unsigned long nAlreadyUploadSize, 
		unsigned long nThisTimeUploadSize, string stringServerPath, string stringLocalPath, string stringFileType);
	int SearchSomebody(string stringUserId);
	int FollowSomebody(string stringUserId, string stringUsername, string stringNickname, string stringVCode);
	int MyFollowing(string stringUserId, string stringNextId, string stringFType);
	int MyFans(string stringUserId, string stringNextId);
	int ChangeNicknameOfFollowed(string stringUserId, string stringNickname);
	int CancelFollowing(string stringUserId);
	int PublishHealthyKnowledge(vector<HEALTHY_KNOWLEDGE> vHealthKnowledge);
	int GetListOfHealthyKnowledge(string stringSenderId, string stringNextId, string stringIds, string stringPageSize, string stringOrderBy);
	int GenerateOrDeletePngZlibReport(string stringServerPath, string stringFlag);
	int GetDailyList(string stringType, string stringStartDate, string stringEndDate, string stringOrderBy, string stringNextId, string stringPageSize);

	int GetCardUserReportList(string stringReportId, string stringReportStartDate, string stringReportEndDate, string stringCaseType, string stringState, string stringOrderBy, 
		string stringThirdIdType, string stringThirdId, string stringNextId, string stringPageSize);
	int ChangeViewLikeCollectCount(string stringHealthTipId, string stringUri, string stringTitle, string stringLike, string stringRead, string stringCollect);
	int ChangeAdviceExecutiveState(string stringSgId, string stringState);
	int GetTrendRangeValue();
	int GetCliniqueReportList(string stringCliniqueSenderId, string stringStartDate, 
		string stringEndDate, string stringState, string stringName, string stringPatientSourceCode, string stringYzlb, string stringPIdType, string stringPId, string stringHb, string stringASectionId, string stringNextId, string stringPageSize);
	int GetUserListBindingOrNot(string stringNextId, string stringPageSize, string stringIsBind, string stringIsActive);
	int BindUser(string stringUId, string stringUName);
	int UnbindUser(string stringUId);
	int GetCardUserListOfCustomer(string stringThirdId, string stringName, string stringCaseType, string stringStartDate, string stringEndDate, string stringNextId, unsigned int nPageSize);
	int SearchCaseAdvanced(string stringHospitalId, string stringDiagnostic, 
		string stringName, string stringSex, QUERY_RANGE rangeAge, QUERY_RANGE rangeCreateTime, string stringMode, 
		string stringCaseType, QUERY_ECG ecg, string stringFrom, string stringSize);
	int SearchCaseAdvancedString(string stringHospitalId, string stringDiagnostic, 
		string stringName, string stringSex, string stringJsonAge, string stringJsonCreateTime, string stringMode, 
		string stringCaseType, string stringJsonEcg, string stringFrom, string stringSize);
	int GetPayCode(string stringType, string stringCaseId);
	int GetOrderInfo(string stringType, string stringCaseId);
	int SignDoctor(string stringUId);
	int RefundFee(string stringType, string stringCaseId, string stringRefundFee);
	int HoldPlaceReview(string stringReportId);
	int RollBackReview(string stringReportId);
	int ResetReviewHoldPlace(string stringReportId, string stringOccupantId, string stringOccupantName, string stringSuperKey);
	int ResetCaseHoldPlace(string stringCaseId, string stringReceiverId, string stringOccupantId, string stringOccupantName, string stringSuperKey);
	int AddTag(string stringTagName, string stringTagDescription);
	int GetTagList(string stringSenderId);
	int AddCaseToTag(string stringTagId, string stringTagName, string stringCaseId, string stringCaseType, string stringCaseTypeName, string stringPatientName);
	int GetCaseListOfTag(string stringTagId, string stringSenderId, string stringCaseType);

	//冠新接口
//	int CreateSession_GuanXin(string stringUsername, string stringPassword);
	int GetAccessToken_GuanXin(string stringUsername, string stringPassword);
	int GetPersonInfo_GuanXin(string stringAccessToken, string stringPersonName, string stringPersonGender, string stringPersonIdentity, string stringPersonBirthDate);
	int UploadECGData_GuanXin(string stringECGXmlPath, string stringECGDataPath);
	int UploadOtherData_GuanXin(string stringOtherXmlPath);
	int UploadCheckData_GuanXin(string stringCheckXmlPath);
	int UploadPersonInfo_GuanXin(string stringPersonInfoXmlPath);





	//自定义内部接口，间接针对一个或多个Phms接口
	//下载文件这个接口比较特殊，规定服务器返回的内容只能保存文件，不走对象的nSaveType指定，保存的服务器路径通过stringLocalFilePath传进来
	int DownloadFile (int nThreadCount, string stringUri, unsigned long nFileSize, string stringLocalFilePath, pProgressAndSpeedCallback pCallback);

	int UploadCase (string stringCaseId, string stringLocalCasePath, pProgressAndSpeedCallback pCallback);

	int UploadReport (string stringCaseId, string stringReceiverId, string stringCaseType, string stringReportId, string stringLocalReportPath, string stringFileType, pProgressAndSpeedCallback pCallback);

	int UploadAudio(string stringMsgId, string stringLocalAudioPath, pProgressAndSpeedCallback pCallback);

	int UploadCommonFile(string& stringUri, string stringType, string stringExtra, string stringLocalFilePath, string stringFileType, pProgressAndSpeedCallback pCallback);

	//用于即时退出Http Session
	int AbortSession();



	//内部接口--针对NoPhms接口，直接用http协议请求文件，不通过php
	int GetFileWithHttp(string stringUrl, int bContinue);

	//内部接口--针对Form表单
	int PostForm(string stringHost, string stringUrl, map<string, string> mapParam);

	//院内使用接口
	int SetCliniqueState(string stringCliniqueId, string stringState);
	int GetPatientInfoByAppNum(string stringHospitalId, string stringOrderType, string stringOrderId, string stringReqDepart, string stringOrderState, string stringCheckType, string stringStartDate, string stringEndDate,string stringClinicId);
	int GetPatientInfoByAppNumToHis(string stringHospitalId, string stringOrderType, string stringOrderId);
	int GetPatientInfoByAppNumToPhmsAndHis(string stringHospitalId, string stringOrderType, string stringOrderId);
	int ApplyCheck(string stringCliniqueId, string stringOrderId, string stringOrderType);
	int GetWaitingCheck(string stringCliniqueId);
	int GetCliniqueState(string stringCliniqueId);
	int NotifyCaseState(string stringHospitalId, string stringOrderType, string stringOrderId);
	int NotifyReportState(string stringReportId);
	int SetDoctorAgentId(string stringDoctorAgentId);
	int NotifyAppState(string stringUnitId, string stringOrderId, string stringOrderType, string stringState);
	int VerifyAppState(string stringOrderId, string stringOrderType);
	int GetHisReportState(string stringHospitalId, string stringOrderType, string stringOrderId);
	int SetHisCriticalValue(string stringHospitalId, string stringCaseId, string stringCriticalValues);

	//东亚医讯Xml接口
	int GetDoctorList_999120(string stringKtCliniqueId, string stringCaseTypeId);

	int ValidatePay_999120(string stringConsumeId, string stringKtCliniqueId, string stringCardNumber, string stringKtDoctorId, string stringConsumeMoney, string stringCaseTypeId);

	int UpdateCaseForConsumerRecord_999120(string stringConsumeId, string stringKtCaseId, string stringCaseTypeId);

	//北京心医接口
	int ValidatePay_XinJie(string stringRrn, string stringLocalDate);
	int NotifyCheck_XinJie(string stringRrn, string stringLocalDate);

	//电信健康小屋接口
	int UserAuthorize_Telecom(string stringPersonId, string stringName, string stringNation, string stringAddress, string stringRoomId);
	int BloodPressureUpload_Telecom(string stringPersonType, string stringPersonId, string stringSBP, string stringDBP, string stringAverageValue, string stringPulseRate, string stringDetectionTime, 
		string stringEquCode, string stringRoomId);
	int BloodOxygenUpload_Telecom(string stringPersonType, string stringPersonId, string stringBloodOxygen, string stringPulseRate, string stringDetectionTime, 
		string stringEquCode, string stringRoomId);
	int BloodSugarUpload_Telecom(string stringPersonType, string stringPersonId, string stringBloodSugar, string stringDetectionTime, 
		string stringEquCode, string stringRoomId);
	int HeightWeightUpload_Telecom(string stringPersonType, string stringPersonId, string stringHeight, string stringWeight, string stringBMI, string stringDetectionTime, 
		string stringEquCode, string stringRoomId);
	int LungUpload_Telecom(string stringPersonType, string stringPersonId, string stringM_B1, string stringBSA, string stringBSA1, string stringMMF, string stringMefSeventyFive, string stringMefFifty, 
		string stringMefTwentyFive, string stringMefFVST, string stringIC, string stringIRV, string stringERV, string stringFevOne, string stringFevTwo, string stringFevThree, 
		string stringFevOnePer, string stringFevTwoPer, string stringFevThreePer, string stringRR, string stringFVC, string stringPEF, string stringTV, string stringVC, string stringMVV, 
		string stringMV, string stringVSTHeight, string stringVSMvvBsa, string stringVSMvvOne, string stringSVC, string stringFevOneZeroFive, string stringFevOneZeroFivePer, string stringPefOne, 
		string stringPefTwo, string stringPefThree, string stringFive, string stringFiveOne, string stringFiveTwo, string stringFiveThree, string stringPIF, string stringMifFifty, 
		string stringDetectionTime, string stringEquCode, string stringRoomId);
	int ECGUpload_Telecom(string stringPersonType, string stringPersonId, string stringDetectionTime, string stringEquCode, string stringRoomId, TELCOM_ECG ecgData);

	//东软熙康定制
	int Login_XiKang(string stringUsername, string stringPassword);
	int RegisterPatientId_XiKang(string stringName, string stringSex, string stringPhoneNum, string stringProofNum);
	int RegisterPatientInfo_XiKang(string stringPatientOpenId, string stringName, string stringSex, string stringPhoneNum, string stringProofNum, string stringPassword);
	int BindPatient_XiKang(string stringDoctorOpenId, string stringPatientOpenId);
	int GetPatientIdList_XiKang(string stringDoctorOpenId);
	int SearchPatientId_XiKang(string stringKey, string stringValue);
	int GetPatientInfo_XiKang(string stringPatientOpenId);
	int GetCaseList_XiKang(vector<string> vPatientOpenId);
	int SubmitCase_XiKang(string stringPatientOpenId, string stringAdvice, string stringCaseType, string stringEcgXmlContent);
	int GetReportFile_XiKang(string stringCaseId);
	int UploadFile_XiKang(string stringCaseId, string stringCaseFilePath);

	//公卫接口
	int ExecuteGWService(string stringLastPath, string stringJson, string stringFilePath);
	int ExecuteCommonRESTful(string stringIp, string stringPort, string stringUrl, string stringContentType, string stringPayload);
};
