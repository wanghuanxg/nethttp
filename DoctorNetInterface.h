#ifndef PHMS_DOCTOR_INTERFACE
#define PHMS_DOCTOR_INTERFACE
#include "PhmsDataType.h"



#if defined(_WIN32) || defined(_WIN32_WCE)
#if defined(PHMS_DLL_EXPORTS)
#define NET_API __declspec(dllexport)
#else
#define NET_API __declspec(dllimport)	
#endif
#endif


#if !defined(NET_API)
#define NET_API
#endif



#ifdef __cplusplus 
extern "C" { 
#endif 

//==================================================================
//函数名：DoctorLogin
//作者：YangJiannan
//日期：2012.12.3
//功能：专家端登录并下载签名文件 
//输入参数：handle					网络对象句柄
//			stringLocalFilePath		专家信息文件本地保存路径
//			stringUsername			用户名
//			stringPassword			密码		
//			stringSignPath			签名文件本地保存路径
//			stringHospitalInfoPath	医院信息路径
//			stringHospitalPhotoPath	医院logo路径
//			stringCliniqueListPath	上传点信息列表路径
//			stringDoctorListPath	院内医生列表路径
//输出参数：stringIp				服务器ip
//返回值： 正常或错误返回码
//参数和返回值的详细信息参见Net_Interface_V1.doc的4.2.1
//修改记录：
//==================================================================
int NET_API DoctorLogin(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringUsername, const char* stringPassword, const char* stringLanguage, const char* stringSignPath, 
						const char* stringHospitalInfoPath, const char* stringHospitalPhotoPath, const char* stringCliniqueListPath, const char* stringHospitalListPath, const char* stringDoctorListPath, char* stringIp);

//==================================================================
//函数名：UploadSign
//作者：YangJiannan
//日期：2012.12.3
//功能：专家端上传签名文件 
//输入参数：handle					网络对象句柄
//			stringSignPath			本地签名文件路径
//返回值： 正常或错误返回码
//参数和返回值的详细信息参见Net_Interface_V1.doc的4.2.2
//修改记录：
//==================================================================
int NET_API UploadSign(NET_HANDLE handle, const char* stringSignPath);

//==================================================================
//函数名：GetUnanalyzedCommonCaseList
//作者：YangJiannan
//日期：2012.3.4
//功能：专家端获得本医院公共未分析病例列表 
//输入参数：handle					网络对象句柄
//			stringLocalFilePath		列表文件本地保存路径
//			stringOwnerId			医院Id或者医生senderid
//			stringOrderBy			排序(1:正序  2:倒序  默认:正序)
//			nPageTo					第几页
//			nPageSize				每页几条数据		
//返回值： 正常或错误返回码
//参数和返回值的详细信息参见Net_Interface_V1.doc的4.2.3
//修改记录：
//==================================================================
int NET_API GetUnanalyzedCaseList(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringOwnerId, const char* stringOrderBy, unsigned int nPageTo, unsigned int nPageSize);


//==================================================================
//函数名：GetUnanalyzedSelfCaseList
//作者：YangJiannan
//日期：2013.3.4
//功能：专家端获得本医院自占位未分析病例列表 
//输入参数：handle					网络对象句柄
//			stringLocalFilePath		列表文件本地保存路径
//			stringReceiverId		医生senderId
//			nPageTo					第几页
//			nPageSize				每页几条数据		
//返回值： 正常或错误返回码
//参数和返回值的详细信息参见Net_Interface_V1.doc的4.2.3
//修改记录：与GetUnanalyzedCommonCaseList接口并为一个接口GetUnanalyzedCaseListp------2013.3.5
//==================================================================
//int NET_API GetUnanalyzedSelfCaseList(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringReceiverId, unsigned int nPageTo, unsigned int nPageSize);



//==================================================================
//函数名：SearchCaseFromServer
//作者：YangJiannan
//日期：2012.12.3
//功能：专家端手动查找本医院病例列表 
//输入参数：handle					网络对象句柄
//			stringLocalFilePath		病例列表本地保存路径
//			stringCaseId			病例Id
//			stringCaseType			病例类型		
//			stringCaseSource		病例源
//			stringCaseState			病例状态
//			stringPatientName		医生名称
//			stringSenderId			发送者Id
//			stringStartDate			开始时间
//			stringEndDate			结束时间
//			stringCheckStartDate	检查开始时间
//			stringCheckEndDate		检查结束时间
//			stringSenderName		发送者名
//			stringDiagnostic		诊断结论
//			stringCaseOrigin		院内病例来源
//			stringYzlb				门诊号/住院号
//			stringAId				申请单号
//			stringSex				性别
//			stringOrderBy			病例时间排序
//			stringThirdId			第三方号
//			stringIsFullName		姓名是否精确查询（0否，1是）
//			nPageTo					第几页
//			nPageSize				每页几条
//返回值： 正常或错误返回码
//参数和返回值的详细信息参见Net_Interface_V1.doc的4.2.4
//修改记录：
//==================================================================
int NET_API SearchCaseFromServer(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringCaseId, const char* stringCaseType, const char* stringCaseSource, 
						 const char* stringCaseState, const char* stringPatientName, const char* stringSenderId, const char* stringReceiverId, const char* stringStartDate, const char* stringEndDate, 
						 const char* stringCheckStartDate, const char* stringCheckEndDate, const char* stringSenderName, const char* stringDiagnostic, const char* stringCaseOrigin, const char* stringYzlb, const char* stringAId, 
						 const char* stringSex, const char* stringOrderBy, const char* stringThirdId, const char* stringLang, const char* stringIsFullName, unsigned int nPageTo, unsigned int nPageSize);

//==================================================================
//函数名：GetAutoCaseInfo
//作者：YangJiannan
//日期：2012.12.3 chg 2014-08-20 by wh
//功能：专家端获得可自动下载病例信息 
//输入参数：handle					网络对象句柄
//			stringLocalFilePath		病例信息文件本地保存路径	
//			stringCaseType			病历类型	
//			stringSenderId			病历发送者id	
//			stringCaseOrigin		病历来源		
//返回值： 正常或错误返回码
//参数和返回值的详细信息参见Net_Interface_V1.doc的4.2.5
//修改记录：
//==================================================================
int NET_API GetAutoCaseInfo(NET_HANDLE handle, const char* stringLocalFilePath,const char*  stringCaseType,const char*  stringSenderId,const char*  stringCaseOrigin);

//==================================================================
//函数名：AutoDownloadCase
//作者：YangJiannan
//日期：2012.12.3
//功能：专家端自动下载病例 
//输入参数：handle					网络对象句柄
//			stringLocalFilePath		病例列表本地保存路径
//			stringCaseId			病例Id
//			stringReceiverId		接收者Id		
//			stringUri				病例文件服务器路径
//			nFileSize				病例文件大小
//			pCallback				进度与速度统计回调函数
//返回值： 正常或错误返回码
//参数和返回值的详细信息参见Net_Interface_V1.doc的4.2.6
//修改记录：
//==================================================================
// int NET_API AutoDownloadCase(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringCaseId, const char* stringReceiverId, const char* stringUri, unsigned long nFileSize, 
// 					 pProgressAndSpeedCallback pCallback);

//==================================================================
//函数名：ManualDownloadCase
//作者：YangJiannan
//日期：2012.12.3
//功能：专家端手动下载病例 
//输入参数：handle					网络对象句柄
//			stringLocalFilePath		病例列表本地保存路径
//			stringCaseId			病例Id
//			stringReceiverId		接收者Id		
//			stringUri				病例文件服务器路径
//			nFileSize				病例文件大小
//			pCallback				进度与速度统计回调函数
//			nBDownloadNotSelfOwn	是否下载非自己所属病例(1:是，0或其他:否)
//返回值： 正常或错误返回码
//参数和返回值的详细信息参见Net_Interface_V1.doc的4.2.7
//修改记录：
//==================================================================
int NET_API ManualDownloadCase(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringCaseId, const char* stringHospitalId, const char* stringReceiverId, const char* stringUri, unsigned long nFileSize, 
					   pProgressAndSpeedCallback pCallback, int nBDownloadNotSelfOwn);

//==================================================================
//函数名：RollbackCase
//作者：YangJiannan
//日期：2012.12.3
//功能：专家端回退病例
//输入参数：handle					网络对象句柄
//			stringCaseId			病例Id
//			stringReceiverId		接收者Id		
//返回值： 正常或错误返回码
//参数和返回值的详细信息参见Net_Interface_V1.doc的4.2.8
//修改记录：
//==================================================================
int NET_API RollbackCase(NET_HANDLE handle, const char* stringCaseId, const char* stringReceiverId, const char* stringBackTo);

//==================================================================
//函数名：TransmitCase
//作者：YangJiannan
//日期：2012.12.3
//功能：专家端转发病例
//输入参数：handle					网络对象句柄
//			stringCaseId			病例Id
//			stringReceiverId		接收者Id	
//			stringForwardToH		转发医院号
//			stringForwardToHospitalName	转发医院名称
//			stringForwardToD		转发医生号
//			stringForwardToDoctorName	转发医生姓名
//返回值： 正常或错误返回码
//参数和返回值的详细信息参见Net_Interface_V1.doc的4.2.9
//修改记录：
//==================================================================
int NET_API TransmitCase(NET_HANDLE handle, const char* stringCaseId, const char* stringReceiverId, const char* stringForwardToH, const char* stringForwardToHospitalName,
				 const char* stringForwardToD, const char* stringForwardToDoctorName);

//==================================================================
//函数名：InvalidCase
//作者：YangJiannan
//日期：2012.12.3
//功能：专家端作废病例
//输入参数：handle					网络对象句柄
//			stringCaseId			病例Id
//			stringReceiverId		接收者Id	
//			stringReason			作废原因
//返回值： 正常或错误返回码
//参数和返回值的详细信息参见Net_Interface_V1.doc的4.2.10
//修改记录：
//==================================================================
int NET_API InvalidCase(NET_HANDLE handle, const char* stringCaseId, const char* stringReceiverId, const char* stringReason);

//==================================================================
//函数名：AnalyzeBegin
//作者：YangJiannan
//日期：2012.12.3
//功能：专家端开始分析病例
//输入参数：handle					网络对象句柄
//			stringCaseId			病例Id
//			stringReceiverId		接收者Id
//			stringCaseType			病例类型
//返回值： 正常或错误返回码
//参数和返回值的详细信息参见Net_Interface_V1.doc的4.2.11
//修改记录：
//==================================================================
int NET_API AnalyzeBegin(NET_HANDLE handle, const char* stringCaseId, const char* stringReceiverId, const char* stringCaseType);

//==================================================================
//函数名：AnalyzeComplete
//作者：YangJiannan
//日期：2012.12.3
//功能：专家端结束分析病例
//输入参数：handle					网络对象句柄
//			stringCaseId			病例Id
//			stringReceiverId		接收者Id
//			stringCaseType			病例类型
//返回值： 正常或错误返回码
//参数和返回值的详细信息参见Net_Interface_V1.doc的4.2.12
//修改记录：
//==================================================================
int NET_API AnalyzeComplete(NET_HANDLE handle, const char* stringCaseId, const char* stringReceiverId, const char* stringCaseType);

//==================================================================
//函数名：UploadReport
//作者：YangJiannan
//日期：2012.12.3
//功能：专家端上传报告
//输入参数：handle					网络对象句柄
//			stringCaseId			病例Id
//			stringReceiverId		接收者Id	
//			stringDiagnosis			诊断结论
//			stringDigitalSign		数字签名
//			stringTimestampSign		时间戳签名
//			stringCaseType			病例类型
//			stringLocalReportPath	报告本地路径
//			nResultFlag				病例结果属性标识（0：无异常无危急值 1：有异常无危急值 2：无异常有危急值 3：有异常有危急值）
//			stringFileType			报告文件类型（0：pdf+bin  1：心电图机报告+bin）
//返回值： 正常或错误返回码
//参数和返回值的详细信息参见Net_Interface_V1.doc的4.2.13
//修改记录：
//==================================================================
int NET_API UploadReport(NET_HANDLE handle, char* stringReportId, const char* stringCaseId, const char* stringReceiverId, const char* stringDiagnosis, const char* stringDigitalSign, const char* stringTimestampSign, const char* stringCaseType, const char* stringLocalReportPath, int nResultFlag, 
				 const char* stringFileType, pProgressAndSpeedCallback pCallback);

//==================================================================
//函数名：DoctorDownloadReport
//作者：YangJiannan
//日期：2012.12.3
//功能：专家端下载报告
//输入参数：handle					网络对象句柄
//			stringLocalFilePath		报告本地保存路径
//			stringUri				报告服务器路径
//			nFileSize				报告大小
//返回值： 正常或错误返回码
//参数和返回值的详细信息参见Net_Interface_V1.doc的4.2.14
//修改记录：
//==================================================================
int NET_API DoctorDownloadReport(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringUri, unsigned long nFileSize, pProgressAndSpeedCallback pCallback);

//==================================================================
//函数名：UploadAdvice
//作者：YangJiannan
//日期：2012.12.3
//功能：专家端上传健康建议 
//输入参数：handle					网络对象句柄
//			stringUserId			个人用户Id
//			stringUsername			病人姓名
//			stringMsg				建议内容	
//			stringType				建议种类
//			stringIsSendMsg			是否发送短信
//返回值： 正常或错误返回码
//参数和返回值的详细信息参见Net_Interface_V1.doc的4.2.15
//修改记录：
//==================================================================
int NET_API UploadAdvice(NET_HANDLE handle, const char* stringUserId, const char* stringUsername, const char* stringMsg, const char* stringType, const char* stringIsSendMsg);

//==================================================================
//函数名：DoctorChangeInfo
//作者：YangJiannan
//日期：2012.12.3
//功能：专家端上传健康建议 
//输入参数：handle					网络对象句柄
//			stringName				姓名
//			stringSex				性别
//			stringTel				电话号码	
//			stringBirthyead			出生年份
//			stringPosition			职务
//			stringNotes				个人描述
//返回值： 正常或错误返回码
//参数和返回值的详细信息参见Net_Interface_V1.doc的4.2.16
//修改记录：
//==================================================================
int NET_API DoctorChangeInfo(NET_HANDLE handle, const char* stringName, const char* stringSex, const char* stringTel, const char* stringBirthyead, const char* stringPosition, const char* stringNotes);

//==================================================================
//函数名：GetDoctorInfo
//作者：YangJiannan
//日期：2013.1.4
//功能：专家获得个人信息 
//输入参数：handle					网络对象句柄
//			stringLocalFilePath		本地保存文件路径
//返回值： 正常或错误返回码
//参数和返回值的详细信息参见Net_Interface_V1.doc的4.2.17
//修改记录：2013.5.7,增加参数stringSenderId，如果传空字符表示本人信息，否则表示他人信息（senderid）
//==================================================================
int NET_API DoctorGetSelfInfo(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringSenderId);

//==================================================================
//函数名：UploadSign
//作者：YangJiannan
//日期：2013.1.4
//功能：医生上传个人照片 
//输入参数：handle					网络对象句柄
//			stringPhotoPath			照片文件本地保存路径
//返回值： 正常或错误返回码
//参数和返回值的详细信息参见Net_Interface_V1.doc的4.2.18
//修改记录：
//==================================================================
int NET_API UploadPhoto(NET_HANDLE handle, const char* stringPhotoPath);

//==================================================================
//函数名：GetReviewReportList
//作者：YangJiannan
//日期：2013.1.4
//功能：获得审核报告列表 
//输入参数：handle					网络对象句柄
//			stringLocalFilePath		本地保存文件路径
//			stringReportStartDate	报告开始时间
//			stringReportEndDate		报告结束时间
//			stringReviewStartDate	审核开始时间
//			stringReviewEndDate		审核结束时间
//			stringReviewState		审核状态
//			stringCaseType			病例类型
//			stringPtName			患者姓名
//			stringSenderId			病例上传方
//			stringYzlb				门诊/住院号
//			stringAId				申请单号
//			stringReviewerId		审核人senderid
//			stringReceiverId		报告人senderid
//			stringCaseId			病例号
//			nPageTo					第几页
//			nPageSize				每页几条
//返回值： 正常或错误返回码
//参数和返回值的详细信息参见Net_Interface_V1.doc的4.2.19
//修改记录：
//==================================================================
int NET_API GetReviewReportList(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringReportStartDate, const char* stringReportEndDate,const char* stringReviewStartDate,
						const char* stringReviewEndDate, const char* stringReviewState, const char* stringCaseType, const char* stringPtName, const char* stringSenderId, const char* stringYzlb, const char* stringAId, 
						const char* stringReviewerId, const char* stringReceiverId, const char* stringCaseId, unsigned int nPageTo, unsigned int nPageSize);

//==================================================================
//函数名：ReviewReport
//作者：YangJiannan
//日期：2013.1.4
//功能：审核报告 
//输入参数：handle					网络对象句柄
//			stringReportKeyId		报告主键
//			stringReviewState		审核状态
//			stringPath				审核后的报告服务器路径
//			stringDiagnostic		诊断结论
//			stringDigitalSign		数字签名
//			stringTimeStampSign		时间戳签名
//参数和返回值的详细信息参见Net_Interface_V1.doc的4.2.20
//修改记录：
//==================================================================
int NET_API ReviewReport(NET_HANDLE handle, const char* stringReportKeyId, const char* stringReviewState, const char* stringPath, const char* stringDiagnostic, 
						 const char* stringDigitalSign, const char* stringTimeStampSign);

//==================================================================
//函数名：GetCardUserList
//作者：YangJiannan
//日期：2013.1.4
//功能：获得个人用户列表 
//输入参数：handle					网络对象句柄
//			stringLocalFilePath		本地文件保存路径
//			stringCardId			病例id
//			stringSenderId			登录时返回的用户系统内部唯一号
//			stringPersonId			个人用户卡号
//			stringTel				电话
//			stringName				个人用户姓名
//			stringType				个人用户类型
//			nPageTo					第几页
//			nPageSize				每页几条
//参数和返回值的详细信息参见Net_Interface_V1.doc的4.2.21
//修改记录：
//==================================================================
int NET_API GetCardUserList(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringCardId, const char* stringSenderId, const char* stringPersonId, const char* stringTel, 
					 const char* stringName, const char* stringType, unsigned int nPageTo, unsigned int nPageSize);

//==================================================================
//函数名：MarkCaseException
//作者：YangJiannan
//日期：2013.1.4
//功能：标记病例状态为异常 
//输入参数：handle					网络对象句柄
//			stringCardId			病例id
//			stringReceiverId		接受者id
//参数和返回值的详细信息参见Net_Interface_V1.doc的4.2.22
//修改记录：
//==================================================================
int NET_API MarkCaseException(NET_HANDLE handle, const char* stringCaseId, const char* stringReceiverId, int nResultFlag);

//==================================================================
//函数名：GetAllTeamList
//作者：YangJiannan
//日期：2013.1.4
//功能：获得所有专家团队列表 
//输入参数：handle					网络对象句柄
//			stringLocalFilePath		本地文件保存路径
//参数和返回值的详细信息参见Net_Interface_V1.doc的4.2.23
//修改记录：
//==================================================================
int NET_API GetAllTeamList(NET_HANDLE handle, const char* stringLocalFilePath);

//==================================================================
//函数名：GetDoctorListOfTeam
//作者：YangJiannan
//日期：2013.1.4
//功能：获得专家团队内医生列表 
//输入参数：handle					网络对象句柄
//			stringLocalFilePath		本地文件保存路径
//			stringTeamId			专家团队id
//参数和返回值的详细信息参见Net_Interface_V1.doc的4.2.24
//修改记录：
//==================================================================
int NET_API GetDoctorListOfTeam(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringTeamId);


//==================================================================
//函数名：DoctorHandledCaseStatistics
//作者：YangJiannan
//日期：2013.4.16
//功能：医生获得已处理病例统计信息
//输入参数：handle					网络对象句柄
//			stringLocalFilePath		本地文件保存路径
//			stringCaseType			病例类型号
//			stringStartTime			报告处理开始时间
//			stringEndTime			报告处理结束时间
//参数和返回值的详细信息参见Net_Interface_V1.doc的4.2.25
//修改记录：
//==================================================================
int NET_API DoctorHandledCaseStatistics(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringCaseType, const char* stringStartTime, const char* stringEndTime);

//==================================================================
//函数名：DoctorDownloadReport
//作者：YangJiannan
//日期：2013.9.30
//功能：专家端下载报告
//输入参数：handle					网络对象句柄
//			stringLocalCasePath		病例本地保存路径
//			stringLocalReportPath	报告本地保存路径
//			stringCaseId			病例号
//			stringReportUri			报告服务器路径
//			nReportSize				报告大小
//			pCallback				下载进度速度回调
//返回值： 正常或错误返回码
//参数和返回值的详细信息参见Net_Interface_V1.doc的4.2.26
//修改记录：
//==================================================================
int NET_API DoctorDownloadCaseAndOneReportOfOldCm(NET_HANDLE handle, const char* stringLocalCasePath, const char* stringLocalReportPath, const char* stringCaseId, const char* stringReportUri, unsigned long nReportSize, pProgressAndSpeedCallback pCallback);

//==================================================================
//函数名：GetHealthDegree
//作者：YangJiannan
//日期：2013.11.11
//功能：获得用户健康度
//输入参数：handle					网络对象句柄
//			stringLocalFilePath		本地保存路径
//			stringUId				卡用户登录号
//			stringInterUId			内部senderid
//			stringTrendType			趋势数据类型
//			stringStartDate			开始时间
//			stringEndDate			结束时间
//			stringNextId			nextid
//返回值： 正常或错误返回码
//参数和返回值的详细信息参见Net_Interface_V1.doc的4.2.27
//修改记录：
//==================================================================
int NET_API GetTrendData(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringUId, const char* stringInterUId, const char* stringTrendType, const char* stringStartDate, const char* stringEndDate, const char* stringNextId);


//==================================================================
//函数名：GetHealthDegree
//作者：YangJiannan
//日期：2013.11.11
//功能：获得用户健康度
//输入参数：handle					网络对象句柄
//			stringLocalFilePath		本地保存路径
//			stringUId				卡用户登录号
//			stringInterUId			内部senderid
//			stringDate				月份yyyy-mm
//返回值： 正常或错误返回码
//参数和返回值的详细信息参见Net_Interface_V1.doc的4.2.27
//修改记录：
//==================================================================
int NET_API GetHealthDegree(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringUId, const char* stringInterUId, const char* stringDate);

//==================================================================
//函数名：GetUserOfNoAdviceAndMostData
//作者：YangJiannan
//日期：2013.11.11
//功能：获取到目前为止超过x天尚未出具健康建议且从上次出具健康建议到现在上传的趋势条数最多的人
//输入参数：handle					网络对象句柄
//			stringLocalFilePath		本地保存路径
//			stringDays				天数
//返回值： 正常或错误返回码
//参数和返回值的详细信息参见Net_Interface_V1.doc的4.2.28
//修改记录：
//==================================================================
int NET_API GetUserOfNoAdviceAndMostData(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringDays);

//==================================================================
//函数名：AdviceDelay
//作者：YangJiannan
//日期：2013.11.11
//功能：健康建议处理续时
//输入参数：handle					网络对象句柄
//			stringUId				卡用户登录号或者内部senderid
//			stringMinutes			分钟数
//返回值： 正常或错误返回码
//参数和返回值的详细信息参见Net_Interface_V1.doc的4.2.29
//修改记录：
//==================================================================
int NET_API AdviceDelay(NET_HANDLE handle, const char* stringUId, const char* stringMinutes);

//==================================================================
//函数名：AdviceComplete
//作者：YangJiannan
//日期：2013.11.11
//功能：健康建议处理续时
//输入参数：handle					网络对象句柄
//			stringUId				卡用户登录号或者内部senderid
//			stringOptionType		操作类型
//			stringAdvice			健康建议
//返回值： 正常或错误返回码
//参数和返回值的详细信息参见Net_Interface_V1.doc的4.2.30
//修改记录：
//==================================================================
int NET_API AdviceComplete(NET_HANDLE handle, const char* stringUId, const char* stringOptionType, const char* stringAdvice);

//==================================================================
//函数名：GetUserListOfNoAdvice
//作者：YangJiannan
//日期：2013.11.11
//功能：获取到目前为止超过x天尚未出具健康建议用户列表
//输入参数：handle					网络对象句柄
//			stringLocalFilePath		本地保存路径
//			stringDays				天数
//			nPageTo					第几页
//			nPageSize				每页记录数
//返回值： 正常或错误返回码
//参数和返回值的详细信息参见Net_Interface_V1.doc的4.2.31
//修改记录：
//==================================================================
int NET_API GetUserListOfNoAdvice(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringDays, unsigned int stringPageTo, unsigned int stringPageSize);

//==================================================================
//函数名：AdviceBegin
//作者：YangJiannan
//日期：2013.11.11
//功能：开始处理健康建议
//输入参数：handle					网络对象句柄
//			stringUId				卡用户登录号或者内部senderid
//返回值： 正常或错误返回码
//参数和返回值的详细信息参见Net_Interface_V1.doc的4.2.32
//修改记录：
//==================================================================
int NET_API AdviceBegin(NET_HANDLE handle, const char* stringUId);

//==================================================================
//函数名：GetUserListOfAdvicing
//作者：YangJiannan
//日期：2013.11.11
//功能：获取健康建议正在处理中的用户列表
//输入参数：handle					网络对象句柄
//			stringLocalFilePath		本地保存路径
//			nPageTo					第几页
//			nPageSize				每页记录数
//返回值： 正常或错误返回码
//参数和返回值的详细信息参见Net_Interface_V1.doc的4.2.33
//修改记录：
//==================================================================
int NET_API GetUserListOfAdvicing(NET_HANDLE handle, const char* stringLocalFilePath, unsigned int nPageTo, unsigned int nPageSize);

//==================================================================
//函数名：GetAskList
//作者：YangJiannan
//日期：2013.10.23
//功能：获得咨询列表
//输入参数：handle					网络对象句柄
//			stringLocalFilePath		文件本地保存路径
//			stringAskType			咨询类型
//			stringAskEnd			是否咨询已关闭
//			stringDoctorId			医生id
//			stringIsAsk				是否未回答
//			stringUserId			本接口返回的userid字段，用于查找某人的历史咨询
//			stringStartTime			开始时间
//			stringEndTime			结束时间
//			nPageTo					第几页
//			nPageSize				每页记录数
//返回值： 正常或错误返回码
//参数和返回值的详细信息参见Net_Interface_V1.doc的4.2.34
//修改记录：
//==================================================================
int NET_API GetAskList(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringAskType, const char* stringAskEnd, const char* stringDoctorId, const char* stringIsAsk, const char* stringUserId, const char* stringStartTime, const char* stringEndTime, unsigned int nPageTo, unsigned int nPageSize);

//==================================================================
//函数名：GetAskDetail
//作者：YangJiannan
//日期：2013.10.23
//功能：获得追问和回答列表
//输入参数：handle					网络对象句柄
//			stringLocalFilePath		本地文件路径
//			stringAskId				咨询id
//返回值： 正常或错误返回码
//参数和返回值的详细信息参见Net_Interface_V1.doc的4.2.35
//修改记录：
//==================================================================
int NET_API GetAskDetail(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringAskId);

//==================================================================
//函数名：SubmitQuestion
//作者：YangJiannan
//日期：2013.10.23
//功能：提交回答
//输入参数：handle					网络对象句柄
//			stringLocalFilePath		文件本地保存路径
//			stringUserId			用户id
//			stringAdvise			回答
//			stringReportType		回答类型
//			stringAskId				咨询id
//			stringAskedId			追问id
//			stringDoctorPhotoPath	医生照片服务器路径
//返回值： 正常或错误返回码
//参数和返回值的详细信息参见Net_Interface_V1.doc的4.2.36
//修改记录：
//==================================================================
int NET_API SubmitQuestion(NET_HANDLE handle, const char* stringUserId, const char* stringAdvise, const char* stringReportType, const char* stringAskId, const char* stringAskedId, const char* stringDoctorPhotoPath);

//==================================================================
//函数名：GetCliniqueListOfHospital
//作者：YangJiannan
//日期：2014.1.4
//功能：获得院内机构列表
//输入参数：handle					网络对象句柄
//			stringLocalFilePath		文件本地保存路径
//			stringHospitalId		医院id
//			nPageTo					第几页
//			nPageSize				每页多少条
//返回值： 正常或错误返回码
//参数和返回值的详细信息参见Net_Interface_V1.doc的4.2.37
//修改记录：
//==================================================================
int NET_API GetCliniqueListOfHospital(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringHospitalId);

//==================================================================
//函数名：GetCliniqueListOfHospital
//作者：YangJiannan
//日期：2014.1.4
//功能：获得统计信息
//输入参数：handle					网络对象句柄
//			stringLocalFilePath		文件本地保存路径
//			stringTypeId			统计类型id
//			stringMonth				统计月份
//			stringMonthDay			统计月份（精确到天，天为月起始天，用于非自然月查询）
//返回值： 正常或错误返回码
//参数和返回值的详细信息参见Net_Interface_V1.doc的4.2.39
//修改记录：
//==================================================================
int NET_API GetStatisticsInfo(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringTypeId, const char* stringMonth, const char* stringMonthDay, unsigned int nPageTo, unsigned int nPageSize);

//==================================================================
//函数名：GetUserHealthList
//作者：YangJiannan
//日期：2014.3.14 chg 2014-08-20 by wh
//功能：获得用户健康列表
//输入参数：handle					网络对象句柄
//			stringLocalFilePath		文件本地保存路径
//			stringNextId			拉取列表的位置
//			stringTypeId			一级筛选类别
//			stringSecondTypeId		二级筛选类别
//			stringName				姓名
//			stringSex				性别
//			stringThirdIdType		识别号类别
//			stringThird				识别号
//			stringHospitalId		医院Id（默认为医生所在医院）
//			stringUnitSenderId		上传点sender，多个用逗号隔开
//			stringIsData			有无新数据（1：有 2：无）
//			stringIntervalTime		间隔天数
//			stringPopulations		人群分类
//			stringContractDoctor	签约医生
//			stringIsContract		是否签约
//			stringPageSize			一次返回数据条数
//返回值： 正常或错误返回码
//修改记录：
//==================================================================
int NET_API GetUserHealthList(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringNextId, const char* stringTypeId, const char* stringSecondTypeId, const char* stringName, const char* stringSex, const char* stringThirdIdType, 
					  const char* stringThirdId, const char* stringHospitalId, const char* stringUnitSenderId, const char* stringIsData, const char* stringIntervalTime, const char* stringPopulations, const char* stringContractDoctor, const char* stringIsContract, const char* stringPageSize);
//==================================================================
//函数名：GetAdviceImplemention
//作者：YangJiannan
//日期：2014.3.14
//功能：获得建议执行度
//输入参数：handle					网络对象句柄
//			stringThirdId			卡号/手机号/身份证号
//			stringHospitalId		医院Id（默认为医生所在医院）
//输出参数：stringPercent			建议执行度
//返回值： 正常或错误返回码
//修改记录：
//==================================================================
int NET_API GetAdviceImplemention(NET_HANDLE handle, char* stringPercent, const char* stringThirdId, const char* stringHospitalId);

//==================================================================
//函数名：ChangePersonType
//作者：YangJiannan
//日期：2014.3.14
//功能：更改用户分类
//输入参数：handle					网络对象句柄
//			stringThirdId			卡号/手机号/身份证号
//			stringPeopleType		分类
//			stringHospitalId		医院Id（默认为医生所在医院）
//返回值： 正常或错误返回码
//修改记录：
//==================================================================
int NET_API ChangePersonType(NET_HANDLE handle, const char* stringThirdId, const char* stringPeopleType, const char* stringHospitalId);

//==================================================================
//函数名：SetNextVisitTime
//作者：YangJiannan
//日期：2014.3.14
//功能：更改用户分类
//输入参数：handle					网络对象句柄
//			stringThirdId			卡号/手机号/身份证号
//			stringInterViewTime		下次访视日期
//			stringHospitalId		医院Id（默认为医生所在医院）
//返回值： 正常或错误返回码
//修改记录：
//==================================================================
int NET_API SetNextVisitTime(NET_HANDLE handle, const char* stringThirdId, const char* stringInterViewTime, const char* stringHospitalId);

//==================================================================
//函数名：GetPersonTypeList
//作者：YangJiannan
//日期：2014.3.14
//功能：更改用户分类
//输入参数：handle					网络对象句柄
//			stringLocalFilePath		本地保存路径
//			stringLanguage			语言
//返回值： 正常或错误返回码
//修改记录：
//==================================================================
int NET_API GetPersonTypeList(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringLanguage);



//==================================================================
//函数名：PublishHealthyKnowledge
//作者：YangJiannan
//日期：2015.7.24
//功能：发布健康知识
//输入参数：handle					网络对象句柄
//			pHealthKnowledge		健康知识数组
//			nArrayLen				数组大小
//返回值： 正常或错误返回码
//修改记录：
//==================================================================
int NET_API PublishHealthyKnowledge(NET_HANDLE handle, pHEALTHY_KNOWLEDGE pHealthKnowledge, unsigned int nArrayLen);


//==================================================================
//函数名：GetTrendRangeValue
//作者：YangJiannan
//日期：2015.12.9
//功能：获得趋势区间值
//输入参数：handle					网络对象句柄
//			stringLocalFilePath		本地文件保存路径
//返回值： 正常或错误返回码
//修改记录：
//==================================================================
int NET_API GetTrendRangeValue(NET_HANDLE handle, const char* stringLocalFilePath);

//==================================================================
//函数名：GetCliniqueReportList
//作者：YangJiannan
//日期：2016.8.10
//功能：获得一个或多个机构的报告列表
//输入参数：handle					网络对象句柄
//			stringLocalFilePath		本地文件保存路径
//			stringCliniqueSenderId	机构SenderId，多个可用逗号隔开
//			stringStartDate			查询开始日期（闭区间）
//			stringEndDate			查询结束日期（闭区间）
//			stringState				报告状态（0，1，2）
//			stringName				患者姓名
//			stringPatientSourceCode	患者来源编码（需要映射到HIS的返回信息）
//			stringYzlb				住院号/门诊号
//			stringPIdType			患者类型（1 院内患者卡号；2 院内患者条形码/二维码；3 院内患者身份证号；4 院内患者医保号；5 院内患者手机号）
//			stringPId				患者号
//			stringHb				床号
//			stringASection			申请科室id
//			stringNextId			下次拉取位置
//			stringPageSize			每次拉取条数
//返回值： 正常或错误返回码
//修改记录：
//==================================================================
int NET_API GetCliniqueReportList(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringCliniqueSenderId, const char* stringStartDate, 
								  const char* stringEndDate, const char* stringState, const char* stringName, const char* stringPatientSourceCode, 
								  const char* stringYzlb, const char* stringPIdType, const char* stringPId, const char* stringHb, const char* stringASectionId, 
								  const char* stringNextId, const char* stringPageSize);

//==================================================================
//函数名：GetUserListBindingOrNot
//作者：YangJiannan
//日期：2017.4.13
//功能：根据是否与当前医生绑定来过滤用户
//输入参数：handle					网络对象句柄
//			stringLocalFilePath		本地文件保存路径
//			stringNextId			拉取列表的位置
//			stringPageSize			页大小
//			stringIsBind			是否绑定（0：未绑定 1：绑定 空：不作为查询条件）
//			stringIsActive			活跃度排序（1：活跃用户优先  2：不活跃用户优先  空：不作为查询条件）
//返回值： 正常或错误返回码
//修改记录：
//==================================================================
int NET_API GetUserListBindingOrNot(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringNextId, const char* stringPageSize, const char* stringIsBind, const char* stringIsActive);

//==================================================================
//函数名：BindUser
//作者：YangJiannan
//日期：2017.4.13
//功能：医生绑定某用户
//输入参数：handle					网络对象句柄
//			stringUId				被绑定者id
//			stringUName				被绑定者名字
//返回值： 正常或错误返回码
//修改记录：
//==================================================================
int NET_API BindUser(NET_HANDLE handle, const char* stringUId, const char* stringUName);

//==================================================================
//函数名：UnbindUser
//作者：YangJiannan
//日期：2017.4.13
//功能：医生解绑某用户
//输入参数：handle					网络对象句柄
//			stringUId				被绑定者id
//返回值： 正常或错误返回码
//修改记录：
//==================================================================
int NET_API UnbindUser(NET_HANDLE handle, const char* stringUId);

//==================================================================
//函数名：SearchCaseAdvanced
//作者：YangJiannan
//日期：2018.1.4
//功能：高级病例查找
//输入参数：handle					网络对象句柄
//			stringLocalFilePath		本地文件保存路径
//			stringHospitalId		医院ID
//			stringDiagnostic		诊断结论
//			stringName				姓名
//			stringSex				性别
//			rangeAge				年龄区间
//			rangeCreateTime			创建时间区间
//			stringMode				模式 and：分词与模式；or: 分词或模式
//			stringCaseType			病例类型
//			ecg						心电图机病例参数区间，目前该接口仅支持9类型病例按照该参数查找，其他类型就算传参数了也会被忽略。
//									从库的设计角度应该做成参数名与参数区间全部传入，这样更灵活，可以兼容以后不同病例类型不同参数的情况，
//									但是从实用角度，还是设计了QUERY_ECG结构体。因为未来其他类型病例有类似需求的可能性不大。
//			stringFrom				开始位置，默认0，翻页需要根据size与返回的total字段来计算
//			stringSize				每次最多返回的记录数
//返回值： 正常或错误返回码
//修改记录：
//==================================================================
int NET_API SearchCaseAdvanced(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringHospitalId, const char* stringDiagnostic, 
							   const char* stringName, const char* stringSex, QUERY_RANGE rangeAge, QUERY_RANGE rangeCreateTime, const char* stringMode, 
							   const char* stringCaseType, QUERY_ECG ecg, const char* stringFrom, const char* stringSize);

//rangeAge、rangeCreateTime、ecg三个参数直接传json串
//{age:{gte:最大值, lte:最小值}}
//{createtime:{gte:最大值, lte:最小值}}
//{HR:{gte:最大值, lte:最小值}, PR_Int:{gte:最大值, lte:最小值}, ......}
int NET_API SearchCaseAdvancedString(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringHospitalId, const char* stringDiagnostic, 
							   const char* stringName, const char* stringSex, const char* stringJsonAge, const char* stringJsonCreateTime, const char* stringMode, 
							   const char* stringCaseType, const char* stringJsonEcg, const char* stringFrom, const char* stringSize);

//==================================================================
//函数名：SignDoctor
//作者：YangJiannan
//日期：2018.3.9
//功能：医生签约某用户
//输入参数：handle					网络对象句柄
//			stringUId				被绑定者id
//返回值： 正常或错误返回码
//修改记录：
//==================================================================
int NET_API SignDoctor(NET_HANDLE handle, const char* stringUId);


//==================================================================
//函数名：HoldPlaceReview
//作者：YangJiannan
//日期：2018.6.26
//功能：审核报告占位
//输入参数：handle					网络对象句柄
//			stringReportId			报告id
//返回值： 正常或错误返回码
//修改记录：
//==================================================================
int NET_API HoldPlaceReview(NET_HANDLE handle, const char* stringReportId);

//==================================================================
//函数名：RollBackReview
//作者：YangJiannan
//日期：2018.6.26
//功能：审核报告占位后回滚
//输入参数：handle					网络对象句柄
//			stringReportId			报告id
//返回值： 正常或错误返回码
//修改记录：
//==================================================================
int NET_API RollBackReview(NET_HANDLE handle, const char* stringReportId);

//==================================================================
//函数名：ResetReviewHoldPlace
//作者：YangJiannan
//日期：2018.6.26
//功能：重置报告占位人
//输入参数：handle					网络对象句柄
//			stringReportId			报告id
//			stringOccupantId		新报告占位人id
//			stringOccupantName		新报告占位人姓名
//			stringSuperKey			超级密码
//返回值： 正常或错误返回码
//修改记录：
//==================================================================
int NET_API ResetReviewHoldPlace(NET_HANDLE handle, const char* stringReportId, const char* stringOccupantId, const char* stringOccupantName, const char* stringSuperKey);

//==================================================================
//函数名：ResetCaseHoldPlace
//作者：YangJiannan
//日期：2018.6.26
//功能：重置报告占位人
//输入参数：handle					网络对象句柄
//			stringCaseId			病例id
//			stringReceiverId		病例当前占位人id
//			stringOccupantId		新报告占位人id
//			stringOccupantName		新报告占位人姓名
//			stringSuperKey			超级密码
//返回值： 正常或错误返回码
//修改记录：
//==================================================================
int NET_API ResetCaseHoldPlace(NET_HANDLE handle, const char* stringCaseId, const char* stringReceiverId, const char* stringOccupantId, const char* stringOccupantName, const char* stringSuperKey);


//==================================================================
//函数名：OnlyDownloadCase
//作者：YangJiannan
//日期：2018.6.25
//功能：下载病例，无占位操作，开始下载和下载完成接口不影响最终返回结果 
//输入参数：handle					网络对象句柄
//			stringLocalFilePath		病例列表本地保存路径
//			stringCaseId			病例Id
//			stringReceiverId		接收者Id		
//			stringUri				病例文件服务器路径
//			nFileSize				病例文件大小
//			pCallback				进度与速度统计回调函数
//返回值： 正常或错误返回码
//修改记录：
//==================================================================
int NET_API OnlyDownloadCase(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringCaseId, const char* stringReceiverId, const char* stringUri, unsigned long nFileSize, pProgressAndSpeedCallback pCallback);

//==================================================================
//函数名：HoldPlaceAndAnalyzeBegin
//作者：YangJiannan
//日期：2018.6.25
//功能：占位并开始分析，只有占位成功后才往下进行 
//输入参数：handle					网络对象句柄
//			stringCaseId			病例Id
//			stringHospital			医院Id
//			stringReceiverId		占位人senderid
//			stringCaseType			病例类型号
//返回值： 正常或错误返回码
//修改记录：
//==================================================================
int NET_API HoldPlaceAndAnalyzeBegin(NET_HANDLE handle, const char* stringCaseId, const char* stringHospitalId, const char* stringReceiverId, const char* stringCaseType);

//==================================================================
//函数名：ReviewReportWithFile
//作者：YangJiannan
//日期：2018.7.4
//功能：审核报告(附带文件) 
//输入参数：handle					网络对象句柄
//			stringCaseId			病例号
//			stringReceiverId		占位人senderid
//			stringReportKeyId		报告主键
//			stringReviewState		审核状态
//			stringFileLoalPath		报告文件本地路径
//			stringDiagnostic		诊断结论
//			stringDigitalSign		数字签名
//			stringTimeStampSign		时间戳签名
//			stringFileType			报告文件类型（0：普通报告；1：后挂bin的报告）
//			pCallback				进度回调
//参数和返回值的详细信息参见Net_Interface_V1.doc的4.2.20
//修改记录：
//==================================================================
int NET_API ReviewReportWithFile(NET_HANDLE handle, const char* stringCaseId, const char* stringReceiverId, const char* stringReportKeyId, const char* stringReviewState, const char* stringFileLoalPath, const char* stringDiagnostic, 
								const char* stringDigitalSign, const char* stringTimeStampSign, const char* stringResultFlag, const char* stringFileType, pProgressAndSpeedCallback pCallback);

//==================================================================
//函数名：AddTag
//作者：YangJiannan
//日期：2018.7.31
//功能：新加标签（收藏夹）
//输入参数：handle					网络对象句柄
//			stringTagName			标签名
//			stringTagDescription	标签描述
//输出参数：stringTagId				标签ID
//修改记录：
//==================================================================
int NET_API AddTag(NET_HANDLE handle, char* stringTagId, const char* stringTagName, const char* stringTagDescription);

//==================================================================
//函数名：GetTagList
//作者：YangJiannan
//日期：2018.7.31
//功能：获得医生标签（收藏夹）列表
//输入参数：handle					网络对象句柄
//			stringLocalFilePath		本地文件保存路径
//			stringSenderId			医生senderid
//修改记录：
//==================================================================
int NET_API GetTagList(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringSenderId);

//==================================================================
//函数名：AddCaseToTag
//作者：YangJiannan
//日期：2018.7.31
//功能：给病例贴标签（把病例放入某收藏夹）
//输入参数：handle					网络对象句柄
//			stringTagId				本地文件保存路径
//			stringTagName			医生senderid
//			stringCaseId			病例id
//			stringCaseType			病例类型号
//			stringCaseTypeName		病例类型名
//			stringPatientName		患者姓名
//修改记录：
//==================================================================
int NET_API AddCaseToTag(NET_HANDLE handle, const char* stringTagId, const char* stringTagName, const char* stringCaseId, const char* stringCaseType, const char* stringCaseTypeName, const char* stringPatientName);

//==================================================================
//函数名：GetCaseListOfTag
//作者：YangJiannan
//日期：2018.7.31
//功能：获得贴有某标签的病例列表（获得收藏夹内病例列表）
//输入参数：handle					网络对象句柄
//			stringLocalFilePath		本地文件保存路径
//			stringTagId				标签id
//			stringSenderId			医生senderid
//			stringCaseType			病例类型号
//修改记录：
//==================================================================
int NET_API GetCaseListOfTag(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringTagId, const char* stringSenderId, const char* stringCaseType);

#ifdef __cplusplus 
	} 
#endif 


#endif
