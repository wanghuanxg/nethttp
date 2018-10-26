/*以后与其他公司做对接以GuanXin实现为标准*/
#pragma once
#include "NetConfig.h"
#include "PhmsRequest.h"
#include "PhmsResponse.h"
#include "HttpSession.h"
#include "PhmsDataType.h"
#include "Poco/DOM/Document.h"
#include "Poco/AutoPtr.h"
#include "Poco/Mutex.h"

using Poco::Mutex;

enum EnumPhmsCommandType{
	LOGIN_TYPE=0,
	MAIN_TYPE,
	CHANGE_INFO_TYPE,
	REGISTER_TYPE
};

enum EnumXmlCommandType
{
	SELECT_DOCTOR_TYPE=0,
	PAY_VERIFICATION_TYPE,
	PAY_VERIFICATION_CASE_TYPE
};

enum EnumXmlCommandType_Telecom
{
	USER_AUTH=0,
	SYNC_BP,
	SYNC_BO,
	SYNC_BS,
	SYNC_HW,
	SYNC_LUNG,
	SYNC_ECG
};

enum EnumXinJieCommandType
{
	VALIDATE_PAY=0,
	NOTIFY_PAY
};

enum EnumGuanXinCommandType
{
	ACCESSTOKEN=0,
	PERSONINFO,
	SESSION,
	UPLOADECG,
	UPLOADNONECG,
	UPLOADMONITOR,
	UPLOADPERSONINFO
};

enum EnumXiKangCommandType
{
	GET_TOKEN_XIKANG=0,
	LOGIN_XIKANG,
	REGISTER_PATIENT_XIKANG,
	REGISTER_PATIENT_INFO_XIKANG,
	BIND_PATIENT_XIKANG,
	GET_PATIENT_ID_XIKANG,
	GET_PATIENT_INFO_XIKANG,
	SEARCH_PATIENT_ID_XIKANG,
	GET_CASE_LIST_XIKANG,
	SUBMIT_CASE_XIKANG,
	UPLOAD_FILE_XIKANG,
	GET_REPORT_URL_XIKANG
};

enum EnumPhmsCaseState
{
	CASE_STAT_NEW=0,
	CASE_STAT_UPLOADING,
	CASE_STAT_UPLOADED,
	CASE_STAT_DOWNLOADING,
	CASE_STAT_DOWNLOADED,
	CASE_STAT_DIAGNOSIS,
	CASE_STAT_VOID,
	CASE_STAT_DIAGNOSISED,
	CASE_STAT_TRANSMITING,
	CASE_STAT_TRANSMITED
};

enum EnumNetConfigType
{
	ORIGINAL_PHMS=0,
	DAOYI_XML,
	TELECOM_XML,
	GUANXIN_FORM,
	XIKANG_FORM,
	XINJIE_XML,
	DIRECTLY_TO_HIS,
	GW_FORM
};

class CPhmsSession
{
public:
	CPhmsSession(void);
	~CPhmsSession(void);
private:
	CHttpSession* m_pHttpSession;
	bool m_bSsl;//记录当前ssl的使用情况,即上一次ssl使用情况
	string m_stringSessionId;//只有Login/Logout函数才能改变这个值（即要想改变SessionId，必须重新登录），每个PhmsSession对象都用一个SessionId，且对用户透明
	string m_stringGWSessionId;//保存公卫系统的sessionid
	//以下三个参数用户指定服务器返回数据保存方式
	int m_nSaveType;
	string m_stringContent;
	string m_stringFilePath;
	EnumPhmsCommandType m_enumPhmsCommandType;
	bool m_bUseIp;
	string m_stringIp;
	Mutex m_mutexAbortAndSslSwitch;
	static string m_stringKey;//用于东亚医讯
	static string m_stringToken_XiKang;//用于东软熙康
	static string m_stringClientId_XiKang;
	static string m_stringClientSecret_XiKang;
	static string m_stringGWSecretKey;
	EnumXmlCommandType m_enumXmlCommandType;//用于东亚医讯
	EnumXmlCommandType_Telecom m_enumXmlCommandType_Telecom;//用于电信健康小屋
	EnumXinJieCommandType m_enumXinJieCommandType;//用于北京心界
	EnumGuanXinCommandType m_enumGuanXinCommandType;//用于冠新公共卫生平台
	EnumXiKangCommandType m_enumXiKangCommandType;//用于东软熙康
	EnumNetConfigType m_enumNetConfigType;
private:
	int ExecutePhmsSession(CPhmsRequest& phmsReuest, CPhmsResponse& phmsResponse, bool bMustSsl=false);
	int ExecuteXmlSession(string stringXmlContent, bool bMustSsl=false);
	int ExecuteXinJieSession(string stringBase64Content, bool bMustSsl=false);
	int ExecuteXmlSession_Telecom(string stringXmlContent, bool bMustSsl=false);
	int ExecuteGuanXinSession(map<string, string> mapParam, string stringDataPath, bool bMustSsl=false);
	int ExecuteXiKangSession(map<string, string> mapParam, string stringDataPath, string stringUrlOpenId, bool bMustSsl=false);
	//从服务器的登录命令返回xml内容中获得sessionid
	int GetSessionId(int nSaveType, string stringContent, string stringFilePath);
	string GetXiKangToken(int nSaveType, string stringContent, string stringFilePath);
	int GetRequestXml(Poco::AutoPtr<Poco::XML::Document> pDoc, string& stringXmlContent);
	int GetRequestXml(string stringFilePath, string& stringXmlContent);
	int GetRequestXmlNoBase64(Poco::AutoPtr<Poco::XML::Document> pDoc, string& stringXmlContent, bool bUtf8=false);
	int GetRequestXmlForAESAndBase64(Poco::AutoPtr<Poco::XML::Document> pDoc, string& stringXmlContent);
	int GetNetConfig(string stringFilePath, CNetConfig& netConfig, bool bMustSsl, EnumNetConfigType enumNetConfigType);//20171025添加，用于在不同业务找不同网络配置的时候，根据enum调用不同的函数GetXXXNetConfig函数
	int GetNetConfig(string stringFilePath, CNetConfig& netConfig, bool bMustSsl);
	int GetNoPhmsNetConfig(string stringFilePath, CNetConfig& netConfig, string stringUrl, bool bMustSsl);
	int GetXmlNetConfig(string stringFilePath, CNetConfig& netConfig, bool bMustSsl);
	int GetXmlNetConfig_Telecom(string stringFilePath, CNetConfig& netConfig, bool bMustSsl);
	int GetXinJieNetConfig(string stringFilePath, CNetConfig& netConfig, bool bMustSsl);
	int GetGuanXinNetConfig(string stringFilePath, CNetConfig& netConfig, bool bMustSsl);
	int GetXiKangNetConfig(string stringFilePath, CNetConfig& netConfig, bool bMustSsl);
	int GetDirectlyToHisConfig(string stringFilePath, CNetConfig& netConfig, bool bMustSsl);
	int GetGWNetConfig(string stringFilePath, CNetConfig& netConfig, bool bMustSsl);
	int GetFormNetConfig(string stringFilePath, CNetConfig& netConfig, string stringHost, string stringUrl, bool bMustSsl);
	int NoPhmsGetResult(istream* pInputStream, int bContinue=0);
	int NoPhmsInputStreamToString(istream* pInputStream, string& stringContent);
	int NoPhmsInputStreamToFile(istream* pInputStream, string& stringFilePath, bool bFileClearBeforeWrite);
public:
	int GetHttpExceptionInfo(int& nCode, string& stringMsgName, string& stringMsgText);
	int AbortSession();
public:
	//Setter
	void SetSaveType(int nSaveType);
	void SetFilePath(string stringFilePath);
	void SetSessionId(string stringSessionId);
	void SetGWSessionId(string stringSessionId);
	void SetCommandType(EnumPhmsCommandType enumCommandType);
	void SetXmlCommandType(EnumXmlCommandType enumXmlCommandType);
	void SetXmlCommandType_Telecom(EnumXmlCommandType_Telecom enumXmlCommandType_Telecom);
	void SetXinJieCommandType(EnumXinJieCommandType enumXinJieCommandType);
	void SetGuanXinCommandType(EnumGuanXinCommandType enumGuanXinCommandType);
	void SetXiKangCommandType(EnumXiKangCommandType enumXiKangCommandType);
	void SetNetConfigType(EnumNetConfigType enumNetConfigType);
	void SetUseIp(bool bUseIp);
	void SetGWSecretKey(string stringGWSecretKey);
	//Getter
	int GetSaveType();
	string GetContent();
	string GetSessionId();
	string GetGWSessionId();
	EnumPhmsCommandType GetCommandType();
	EnumXmlCommandType GetXmlCommandType();
	EnumXmlCommandType_Telecom GetXmlCommandType_Telecom();
	EnumXinJieCommandType GetXinJieCommandType();
	EnumGuanXinCommandType GetGuanXinCommandType();
	EnumNetConfigType GetNetConfigType();
	string GetIp();
	string GetGWSecretKey();
public:
	//PHMS接口
	int Login(string stringRole, string stringUsername, string stringPassword, string stringLanguage, string stringToken, bool bMustSsl);
	int MechineRegister(string stringCountry, string stringAreaId, string stringArea, 
		string stringDevide, string stringSignature, string stringSerialnum);
	int Logout();
	int SetDoctorSign(string stringMd5, string stringFilePath);
	int SetDoctorPhoto(string stringMd5, string stringFilePath);
	int ApplyNewCase(string stringName, string stringThirdId, string stringDataType, string stringDataTypeName, string stringNotes, string stringHospitalId,
		string stringHospitalName, string stringDoctorId, string stringDoctorName, string stringCheckTime, string stringOtherParams, string stringDeviceId, 
		string stringDeviceName, string stringMd5, string stringCaseOrigin, string stringYzlb, string stringSex, string stringApplicationNo, string stringAutoMatic);
	int UploadCaseSegment(string stringCaseId, string stringOrder, string stringTotalSize, string stringServerUri, string stringStart1, 
		string stringEnd1, string stringMd51, string stringStart2, string stringEnd2, string stringMd52, string stringStart3, 
		string stringEnd3,string stringMd53, string stringFilePath);
	int CaseHoldPlace(string stringCaseId, string stringReceiverId);
	int GetAutoDownloadCase(string stringCaseType,string stringSenderId,string stringCaseOrigin);
	int DownloadCaseBegin(string stringCaseId, string stringReceiverId);
	int DownloadFileSegment(string stringUri, string stringStart, string stringEnd);
	int DownloadCaseComplete(string stringCaseId, string stringReceiverId);
	int InvalidCase(string stringCaseId, string stringReceiverId, string stringReason);
	int TransmitCase(string stringCaseId, string stringReceiverId, string stringForwardToH, string stringForwardToHospitalName, 
		string stringForwardToD, string stringForwardToDoctorName);
	int Rollback(string stringCaseId, string stringReceiverId, string stringBackTo);
	int GetHospitalListOfGroup(string stringGroupId, string stringType, string stringCaseType, string stringPageTo, string stringPageSize);
	int GetDoctorListOfHospital(string stringHospitalId, string stringDoctorId, string stringCaseType, string stringDoctorType, string stringPageTo, string stringPageSize);
	int AnalyzeBegin(string stringCaseId, string stringReceiverId, string stringCaseType);
	int AnalyzeComplete(string stringCaseId, string stringReceiverId, string stringCaseType);
	int ApplyNewReport(string stringCaseId, string stringReceiverId, string stringDiagnosis, string stringMd5, string stringDigitalSign, string stringTimestampSign, string stringCaseType);
	int UploadReportSegment(string stringCaseId, string stringReceiverId, string stringCaseType, string stringReportId, string stringOrder, string stringTotalSize, 
		string stringServerUri, string stringStart1, string stringEnd1, string stringMd51, string stringStart2, string stringEnd2, 
		string stringMd52, string stringStart3, string stringEnd3, string stringMd53, string stringFilePath, string stringFileType);
	int DownloadReportBegin(string stringCaseId, string stringReportId);
	int DownloadReportComplete(string stringCaseId, string stringReportId);
	int GetAlreadyUploadFileSize(string stringServerUri);
	int GetReviewReportList(string stringReportStartDate, string stringReportEndDate, string stringReviewStartDate, string stringReviewEndDate, 
		string stringReviewState, string stringCaseType, string stringPtName, string stringSenderId, string stringYzlb, string stringAId, 
		string stringReviewerId, string stringReceiverId, string stringCaseId, string stringPageTo, string stringPageSize);
	int ReviewReport(string stringReportId, string stringReviewState, string stringPath, string stringDiagnostic, 
		string stringDigitalSign, string stringTimeStampSign);
	int GetReportStateOfCase(string stringCaseId);
	int GetCaseListOfDoctor(string stringCaseId, string stringCaseType, string stringCaseSource, string stringCaseState, string stringPatientName, 
		string stringSenderId, string stringReceiverId, string stringStartDate, string stringEndDate, string stringCheckStartDate, string stringCheckEndDate, 
		string stringSenderName, string stringDiagnostic, string stringCaseOrigin, string stringYzlb, string stringAId, string stringSex, string stringOrderBy, string stringThirdId, 
		string stringLang, string stringIsFullName, string stringPageTo, string stringPageSize);
	int GetHospitalInfo(string stringHospitalId);
	int UploadIm(string stringSenderId, string stringSenderName, string stringReceiverId, string stringMsgDirection, string stringMsgType, 
		string stringMsgContent, string stringCaseId, string stringCaseType);
	int GetImList(string stringSenderId, string stringCaseId, string stringCaseType, string stringMsgDirection, string stringMsgType);
	int GetImList2(string stringNextId);
	int ImDownloadComplete(string stringId);
	int SendPhoneMsg(string stringCaseId, string stringPhone, string stringMsg);
	int UploadTrendData(string stringCardId, string stringDeviceId, string stringDeviceName, string stringCaseId, vector<RECORD_DATA> vRecordData);
	int UploadTrendDataString(string stringCardId, string stringDeviceId, string stringDeviceName, string stringCaseId, vector<RECORD_DATA_STRING> vRecordData);
	int UploadTrendDataFile(string stringFilePath);
	int UploadAudioSegment(string stringMsgId, string stringOrder, string stringTotalSize, string stringServerUri, string stringStart1, 
		string stringEnd1, string stringMd51, string stringStart2, string stringEnd2, string stringMd52, string stringStart3, 
		string stringEnd3, string stringMd53, string stringFilePath);
	int UploadAdvice(string stringUserId, string stringUsername, string stringMsg, string stringType, string stringIsSendMsg);
	int GetAdvice(string stringSgId, string stringUserId, string stringDoctorId, string stringDownloadState, string stringType, string stringStartDate, 
		string stringEndDate, string stringPageTo, string stringPageSize);
	int DownloadAdviceComplete(string stringAdviceId);
	int GetReportListOfCase(string stringCaseId);
	int GetSelfInfo(string stringSenderId);//接口有变动，加了一个参数senderid，表示要查找信息的人，如为空表示自己
	int ChangePwd(string stringOldPassword, string stringNewPassword);
	int DoctorChangeSelfInfo(string stringName, string stringSex, string stringTel, string stringBirthYear, string stringPosition, 
		string stringNotes, string stringAgentId);
	int CliniqueChangeSelfInfo(string stringPersonId, string stringName, string stringTel, string stringNotes);
	int CardUserChangeSelfInfo(string stringPersonId, string stringName, string stringSex, string stringTel, string stringBirthday, 
		string stringAddress, string stringLmp, string stringToken, string stringHeight, string stringWeight, string stringPhoto, 
		string stringVCodeBeFollowed, string stringHospitalId, string stringOtherInfo);
	int GetCaseListOfUser(string stringCaseId, string stringCaseType, string stringCaseState, string stringThirdId, string stringName, 
		string stringStartDate, string stringEndDate, string stringCheckStartDate, string stringCheckEndDate, string stringCaseOrigin, 
		string stringYzlb, string stringSex, string stringIsRead, string stringOrderBy, string stringPageTo, string stringPageSize);
	int GetReportListOfUser(string stringReportId, string stringReportStartDate, string stringReportEndDate, string stringCaseType, 
		string stringState, string stringOrderBy, string stringPageTo, string stringPageSize);
	int GetCardUserList(string stringCardId, string stringSenderId, string stringPersonId, string stringTel, string stringName, string stringType,
		string stringPageTo, string stringPageSize);
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
	int GetUserListOfNoAdvice(string stringDays, string stringPageTo, string stringPageSize);
	int AdviceBegin(string stringUId);
	int GetUserListOfAdvicing(string stringPageTo, string stringPageSize);
	int VerifyThirdId(string stringUId, string stringType);
	int PersonIdRegister(string stringType, string stringRegFrom, string stringPersonId, string stringName, string stringSex, string stringBirthday, string stringAddress, 
		string stringPhone, string stringHeigth, string stringUploadUnitId, string stringHospitalId, string stringHGroupId, string stringScode, string stringWxUId, string stringLmp, string stringWeight, string stringPassword, 
		string stringThirdCode, string stringIsRegister);
	int GetVerificationCode(string stringType, string stringPhone);
	int GetAskList(string stringAskType, string stringAskEnd, string stringDoctorId, string stringIsAsk, string stringUserId, string stringStartTime, string stringEndTime, string stringPageTo, string stringPageSize);
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
	int GetStatisticsInfo(string stringTypeId, string stringMonth, string stringMonthDay, string stringPageTo, string stringPageSize);
	int UpdateCaseInfo(string stringHospitalId, string stringCaseId, string stringOtherParam, string stringFlag, string stringNotes, string stringReceiverId);
	int GetUserHealthList(string stringNextId, string stringTypeId, string stringSecondTypeId, string stringName, string stringSex, string strinThirdIdType, 
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
	int UploadCommonFileSegment(string stringType, string stringExtra, string stringOrder, string stringTotalSize, 
		string stringServerUri, string stringStart1, string stringEnd1, string stringMd51, string stringStart2, string stringEnd2, string stringMd52, string stringStart3, string stringEnd3, string stringMd53, string stringFilePath, string stringFileType);
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
	int GetCardUserListOfCustomer(string stringThirdId, string stringName, string stringCaseType, string stringStartDate, string stringEndDate, string stringNextId, string stringPageSize);
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


	//院内使用接口
	int SetCliniqueState(string stringCliniqueId, string stringState);
	int GetPatientInfoByAppNum(string stringHospitalId, string stringOrderType, string stringOrderId, string stringReqDepart, string stringOrderState, string stringCheckType, string stringStartDate, string stringEndDate, string stringClinicId);
	int GetPatientInfoByAppNumToHis(string stringHospitalId, string stringOrderType, string stringOrderId);
	int GetPatientInfoByAppNumToPhmsAndHis(string stringHospitalId, string stringOrderType, string stringOrderId);
	int ApplyCheck(string stringCliniqueId, string stringOrderId, string stringOrderType);
	int GetWaitingCheck(string stringCliniqueId);
	int GetCliniqueState(string stringCliniqueId);
	int NotifyCaseState(string stringHospitalId, string stringOrderType, string stringOrderId);
	int NotifyReportState(string stringReportId);
	int NotifyAppState(string stringUnitId, string stringOrderId, string stringOrderType, string stringState);
	int VerifyAppState(string stringOrderId, string stringOrderType);
	int GetHisReportState(string stringHospitalId, string stringOrderType, string stringOrderId);
	int SetHisCriticalValue(string stringHospitalId, string stringCaseId, string stringCriticalValues);
	//第三方组织命令暂时不支持

	//非PHMS协议接口
	int ExecuteNoPhmsSession(string stringUrl, int bContinue, bool bMustSsl=false);
	int ExecuteFormHttpSession(string stringHost, string stringUrl, map<string, string> mapParam, bool bMustSsl=false);

	//东亚医讯Xml接口
	int GetDoctorList_999120(string stringKtCliniqueId, string stringCaseTypeId);
	int ValidatePay_999120(string stringConsumeId, string stringKtCliniqueId, string stringCardNumber, string stringKtDoctorId, string stringConsumeMoney, string stringCaseTypeId);
	int UpdateCaseForConsumerRecord_999120(string stringConsumeId, string stringKtCaseId, string stringCaseTypeId);

	//北京心界Xml接口
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

	//东软熙康定制软件接口
	int GetToken_XiKang();
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

	//公卫系统接口
	int ExecuteGWService(string stringLastPath, map<string, string> mapParam, string stringFilePath, bool bMustSsl=false);
	int ExecuteCommonRESTful(string stringIp, string stringPort, string stringUrl, string stringContentType, string stringPayload);
};
