#ifndef PHMS_USER_INTERFACE
#define PHMS_USER_INTERFACE
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
//函数名：UserLogin
//作者：YangJiannan
//日期：2012.12.3
//功能：客户端登录  
//输入参数：handle					网络对象句柄
//			stringLocalFilePath		本地账户信息文件保存路径
//			stringUsername			用户名
//			stringPassword			密码
//			stirngRole				用户角色
//			stringToken				App登录用设备token
//输出参数：stringIp				服务器ip
//返回值： 正常或错误返回码
//参数和返回值的详细信息参见Net_Interface_V1.doc的4.3.1
//修改记录：
//==================================================================
int NET_API UserLogin(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringUsername, const char* stringPassword, const char* stringRole, const char* stringToken, char* stringIp);

//==================================================================
//函数名：GetUserInfo
//作者：YangJiannan
//日期：2012.12.3
//功能：客户端获得登录用户（机构）信息  
//输入参数：handle					网络对象句柄
//			stringLocalFilePath		用户（机构）信息文件保存路径
//返回值： 正常或错误返回码
//参数和返回值的详细信息参见Net_Interface_V1.doc的4.3.2
//修改记录：
//==================================================================
int NET_API GetUserInfo(NET_HANDLE handle, const char* stringLocalFilePath);

//==================================================================
//函数名：GetHospitalInfo
//作者：YangJiannan
//日期：2012.12.3
//功能：客户端获得医院信息  
//输入参数：handle					网络对象句柄
//			stringLocalFilePath		医院信息文件保存路径
//			stringHospitalId		医院Id
//返回值： 正常或错误返回码
//参数和返回值的详细信息参见Net_Interface_V1.doc的4.3.3
//修改记录：
//==================================================================
int NET_API GetHospitalInfo(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringHospitalId);

//==================================================================
//函数名：GetDoctorInfo
//作者：YangJiannan
//日期：2012.12.3
//功能：客户端获得医生信息  
//输入参数：handle					网络对象句柄
//			stringLocalFilePath		医生信息文件保存路径
//			stringDoctorId			医生Id
//返回值： 正常或错误返回码
//参数和返回值的详细信息参见Net_Interface_V1.doc的4.3.4
//修改记录：
//==================================================================
int NET_API GetDoctorInfo(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringDoctorId);

//==================================================================
//函数名：UploadCase
//作者：YangJiannan
//日期：2012.12.3
//功能：客户端上传病例 
//输入参数：handle					网络对象句柄
//			stringName				病人姓名
//			stringThirdId			第三方Id
//			stringDataType			数据类型
//			stringNotes				病例描述
//			stringHospitalId		医院Id
//			stringHospitalName		医院名称
//			stringDoctorId			医生Id
//			stringDoctorName		医院姓名
//			stringOtherParam		其他参数（如性别、年龄、门诊号）
//			stringCaseOrigin		病例院内来源
//			stringYzlb				住院号/门诊号
//			stringSex				性别
//			stringApplicationNo		申请单号
//			stringAutoMatic			自动诊断（0：人工诊断、1：自动诊断、默认为0）
//			stringLocalCasePath		本地病例路径
//输出参数：stringCaseId			病例Id（用于获得case id）
//返回值： 正常或错误返回码
//参数和返回值的详细信息参见Net_Interface_V1.doc的4.3.5
//修改记录：
//==================================================================
int NET_API UploadCase(NET_HANDLE handle, char* stringCaseId, const char* stringName, const char* stringThirdId, const char* stringDataType, const char* stringDataTypeName, const char* stringNotes, const char* stringHospitalId, 
			   const char* stringHospitalName, const char* stringDoctorId, const char* stringDoctorName, const char* stringCheckTime, const char* stringOtherParam, const char* stringDeviceId, const char* stringDeviceName, const char* stringCaseOrigin, const char* stringYzlb, const char* stringSex, 
			   const char* stringApplicationNo, const char* stringAutoMatic, const char* stringLocalCasePath, pProgressAndSpeedCallback pCallback);

//==================================================================
//函数名：UserDownloadReport
//作者：YangJiannan
//日期：2012.12.3
//功能：客户端下载报告  
//输入参数：handle					网络对象句柄
//			stringLocalFilePath		本地报告文件保存路径
//			stringCaseId			病例Id
//			stringReportId			报告Id
//			stringUri				报告文件服务器路径
//			nFileSize				报告文件大下
//返回值： 正常或错误返回码
//参数和返回值的详细信息参见Net_Interface_V1.doc的4.3.6
//修改记录：
//==================================================================
int NET_API UserDownloadReport(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringCaseId, const char* stringReportId, const char* stringUri, unsigned long nFileSize, 
					   pProgressAndSpeedCallback pCallback);

//==================================================================
//函数名：GetCaseReportState
//作者：YangJiannan
//日期：2012.12.3
//功能：客户端某病例的报告状态 
//输入参数：handle					网络对象句柄
//			stringReportState		报告状态（输出参数，用于获得某病例的报告状态）
//			stringCaseId			病例Id
//返回值： 正常或错误返回码
//参数和返回值的详细信息参见Net_Interface_V1.doc的4.3.7
//修改记录：
//==================================================================
int NET_API GetCaseReportState(NET_HANDLE handle, char* stringReportState, const char* stringCaseId);

//==================================================================
//函数名：UserGetReportList
//作者：YangJiannan
//日期：2012.12.3
//功能：客户端获得用户报告列表
//输入参数：handle					网络对象句柄
//			stringLocalFilePath		用户病例列表文件本地保存路径
//			stringReportId			报告Id
//			stringReportStartDate	报告生成开始时间
//			stringReportEndDate		报告生成结束时间
//			stringCaseType			病例类型
//			stringReportState		报告状态
//			stringOrderBy			时间排序（1：正序 2：倒序 默认倒序）
//			nPageTo					第几页
//			nPageSize				每页几条
//返回值： 正常或错误返回码
//参数和返回值的详细信息参见Net_Interface_V1.doc的4.3.8
//修改记录：
//==================================================================
int NET_API UserGetReportList(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringReportId, const char* stringReportStartDate, const char* stringReportEndDate, 
					  const char* stringCaseType, const char* stringReportState, const char* stringOrderBy, unsigned long nPageTo, unsigned long nPageSize);

//==================================================================
//函数名：UserGetCaseList
//作者：YangJiannan
//日期：2012.12.3
//功能：客户端获得用户病例列表 
//输入参数：handle					网络对象句柄
//			stringLocalFilePath		病例列表文件本地保存路径
//			stringCaseId			病例Id
//			stringCaseType			病例类型
//			stringCaseState			病例状态
//			stringThirdId			第三方Id
//			stringName				病人姓名
//			stringStartDate			病例上传开始时间
//			stringEndDate			病例上传结束时间
//			stringCheckStartDate	病例检查开始时间
//			stringCheckEndDate		病例检查结束时间
//			stringCaseOrigin		病例院内来源
//			stringYzlb				门诊号/住院号
//			stringSex				性别
//			stringIsRead			报告是否已读
//			stringOrderBy			排序方式（1：正序；2：倒序；默认倒序）
//			nPageTo					显示第几页
//			nPageSize				每页显示几条
//返回值： 正常或错误返回码
//参数和返回值的详细信息参见Net_Interface_V1.doc的4.3.9
//修改记录：
//==================================================================
int NET_API UserGetCaseList(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringCaseId, const char* stringCaseType, const char* stringCaseState, const char* stringThirdId, const char* stringName, 
							const char* stringStartDate, const char* stringEndDate, const char* stringCheckStartDate, const char* stringCheckEndDate, const char* stringCaseOrigin, 
							const char* stringYzlb, const char* stringSex, const char* stringIsRead, const char* stringOrderBy, unsigned int nPageTo, unsigned int nPageSize);



//==================================================================
//函数名：GetAdvice
//作者：YangJiannan
//日期：2015.11.26
//功能：获得健康建议
//输入参数：handle					网络对象句柄
//			stringLocalFilePath		本地文件保存路径
//			stringSgId				健康建议号
//			stringUserId			卡用户ID
//			stringDoctorId			生成建议的医生ID
//			stringDownloadState		下载状态
//			stringType				建议类型
//			stringStartDate			建议生成开始时间
//			stringEndDate			建议生成结束时间
//			nPageTo					第几页
//			nPageSize				每页数据条数
//返回值： 正常或错误返回码
//参数和返回值的详细信息参见Net_Interface_V1.doc的4.3.10
//修改记录：
//==================================================================
int NET_API GetAdvice(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringSgId, const char* stringUserId, const char* stringDoctorId, const char* stringDownloadState, const char* stringType, 
			  const char* stringStartDate, const char* stringEndDate, unsigned int nPageTo, unsigned int nPageSize);


//==================================================================
//函数名：MarkReportRead
//作者：YangJiannan
//日期：2012.12.3
//功能：客户端标记报告为已读
//输入参数：handle					网络对象句柄
//			stringReportId			报告Id
//返回值： 正常或错误返回码
//参数和返回值的详细信息参见Net_Interface_V1.doc的4.3.10
//修改记录：
//==================================================================
int NET_API MarkReportRead(NET_HANDLE handle, const char* stringReportId);
//==================================================================
//函数名：CardUserChangeInfo
//作者：YangJiannan
//日期：2012.12.3
//功能：客户端卡用户修改个人信息 
//输入参数：handle					网络对象句柄
//			stringPId				身份证号
//			stringName				姓名
//			stringSex				性别
//			stringTel				电话号码
//			stringBirthday			出生日期
//			stringAddress			地址
//			stringLmp				(末次例假第一天，宝宝心语手机应用使用)
//			stringToken				Apple设备的Token，Apple手机客户端使用
//			stringHeight			身高
//			stringWeight			体重
//			stringPhoto				1105接口返回的服务器照片存储路径
//			stringVCodeBeFollowed	关注验证码，别人关注我时需要输入该验证码
//			stringHospitalId		医院号
//			stringOtherInfo			用户其他信息
//返回值： 正常或错误返回码
//参数和返回值的详细信息参见Net_Interface_V1.doc的4.3.11
//修改记录：
//==================================================================
int NET_API CardUserChangeInfo(NET_HANDLE handle, const char* stringPId, const char* stringName, const char* stringSex, const char* stringTel, const char* stringBirthday, const char* stringAddress, const char* stringLmp, const char* stringToken, const char* stringHeight, 
							   const char* stringWeight, const char* stringPhoto, const char* stringVCodeBeFollowed, const char* stringHospitalId, const char* stringOtherInfo);

//==================================================================
//函数名：CliniqueChangeInfo
//作者：YangJiannan
//日期：2012.12.3
//功能：客户端机构用户修改机构信息 
//输入参数：handle					网络对象句柄
//			stringPId				身份证号
//			stringName				姓名
//			stringTel				电话号码
//			stringNotes				个人描述
//返回值： 正常或错误返回码
//参数和返回值的详细信息参见Net_Interface_V1.doc的4.3.12
//修改记录：
//==================================================================
int NET_API CliniqueChangeInfo(NET_HANDLE handle, const char* stringPId, const char* stringName, const char* stringTel, const char* stringNotes);

//==================================================================
//函数名：UploadTrendData
//作者：YangJiannan
//日期：2013.5.16
//功能：上传趋势数据 
//输入参数：handle					网络对象句柄
//			stringCardId			卡号
//			stringDeviceId			设备id
//			stringDeviceName		设备名
//			stringCaseId			病例号
//			pRecordData				数据数组首地址
//			nArrayLen				数据数组长度
//返回值： 正常或错误返回码
//参数和返回值的详细信息参见Net_Interface_V1.doc的4.3.12
//修改记录：2013.9.2,所有double参数默认都保留小数点后1位，如有特殊需求，联系我。
//==================================================================
int NET_API UploadTrendData(NET_HANDLE handle, const char* stringCardId, const char* stringDeviceId, const char* stringDeviceName, const char* stringCaseId, const RECORD_DATA* pRecordData, int nArrayLen);

//==================================================================
//函数名：UploadTrendDataString
//作者：YangJiannan
//日期：2015.5.27
//功能：上传趋势数据 
//输入参数：handle					网络对象句柄
//			stringCardId			卡号
//			stringDeviceId			设备id
//			stringDeviceName		设备名
//			stringCaseId			病例号
//			pRecordData				数据数组首地址
//			nArrayLen				数据数组长度
//返回值： 正常或错误返回码
//==================================================================
int NET_API UploadTrendDataString(NET_HANDLE handle, const char* stringCardId, const char* stringDeviceId, const char* stringDeviceName, const char* stringCaseId, const RECORD_DATA_STRING* pRecordData, int nArrayLen);


//==================================================================
//函数名：UploadTrendDataFile
//作者：YangJiannan
//日期：2015.5.27
//功能：上传趋势数据 
//输入参数：handle					网络对象句柄
//			stringFilePath			自组建XML文件路径
//返回值： 正常或错误返回码
//==================================================================
int NET_API UploadTrendDataFile(NET_HANDLE handle, const char* stringFilePath);



//==================================================================
//函数名：VerifyThirdId
//作者：YangJiannan
//日期：2013.9.9
//功能：验证第三方号是否存在
//输入参数：handle					网络对象句柄
//			stringUId				手机号，身份证号，卡号；唯一码等
//			stringType				id类型
//输出参数：stringCardId			卡号
//返回值： 正常或错误返回码
//参数和返回值的详细信息参见Net_Interface_V1.doc的4.3.13
//==================================================================
int NET_API VerifyThirdId(NET_HANDLE handle, char* stringCardId, const char* stringUId, const char* stringType);

//==================================================================
//函数名：GetDoctorList_999120
//作者：YangJiannan
//日期：2013.9.10
//功能：东亚医讯接口，获得医生列表
//输入参数：handle					网络对象句柄
//			stringLocalFilePath		本地文件保存路径
//			stringKtCliniqueId		机构号
//			stringCaseTypeId		病例类型号
//返回值： 正常或错误返回码
//参数和返回值的详细信息参见Net_Interface_V1.doc的4.3.14
//==================================================================
int NET_API GetDoctorList_999120(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringKtCliniqueId, const char* stringCaseTypeId);

//==================================================================
//函数名：ValidatePay_999120
//作者：YangJiannan
//日期：2013.9.10
//功能：东亚医讯接口，验证付费
//输入参数：handle					网络对象句柄
//			stringLocalFilePath		本地文件保存路径
//			stringConsumeId			消费单号记录
//			stringKtCliniqueId		机构号
//			stringCardNumber		会员卡号
//			stringKtDoctorId		医生号
//			stringConsumeMoney		本次消费金额
//			stringCaseTypeId		病例类型号
//返回值： 正常或错误返回码
//参数和返回值的详细信息参见Net_Interface_V1.doc的4.3.15
//==================================================================
int NET_API ValidatePay_999120(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringConsumeId, const char* stringKtCliniqueId, const char* stringCardNumber, const char* stringKtDoctorId, const char* stringConsumeMoney, const char* stringCaseTypeId);

//==================================================================
//函数名：UpdateCaseForConsumerRecord_999120
//作者：YangJiannan
//日期：2013.9.10
//功能：东亚医讯接口，更新消费记录所对应的病历
//输入参数：handle					网络对象句柄
//			stringLocalFilePath		本地文件保存路径
//			stringConsumeId			消费单号记录
//			stringKtCaseId			病例号
//			stringCaseTypeId		病例类型号
//返回值： 正常或错误返回码
//参数和返回值的详细信息参见Net_Interface_V1.doc的4.3.16
//==================================================================
int NET_API UpdateCaseForConsumerRecord_999120(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringConsumeId, const char* stringKtCaseId, const char* stringCaseTypeId);

//==================================================================
//函数名：PersonIdRegister
//作者：YangJiannan
//日期：2013.10.11
//功能：身份证号注册
//输入参数：handle					网络对象句柄
//			stringLocalFilePath		本地文件保存路径（只有type取值为6的时候可以有效值，其他type传空串）
//			stringType				注册类型
//			stringRegFrom			注册请求来源
//			stringPersonId			身份证号
//			stringName				姓名
//			stringSex				性别
//			stringBirthday			生日
//			stringAddress			住址
//			stringPhone				电话
//			stringHeigth			身高
//			stringUploadUnitId		机构号
//			stringHospitalId		医院号
//			stringHGroupId			医院组织号
//			stringScode				手机验证码
//			stringWxUId				微信唯一号
//			stringLmp				末次例假第一天
//			stringWeight			用户体重
//			stringPassword			初始密码
//			stringThirdCode			第三方登录号
//			stringIsRegister		是否注册
//返回值： 正常或错误返回码
//参数和返回值的详细信息参见Net_Interface_V1.doc的4.3.17
//==================================================================
int NET_API PersonIdRegister(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringType,const char* stringRegFrom, const char* stringPersonId, const char* stringName, const char* stringSex, const char* stringBirthday, const char* stringAddress, 
					 const char* stringPhone, const char* stringHeigth, const char* stringUploadUnitId, const char* stringHospitalId, const char* stringHGroupId, const char* stringScode, const char* stringWxUId, const char* stringLmp,const char*  stringWeight, const char* stringPassword, 
					 const char* stringThirdCode, const char* stringIsRegister);

//==================================================================
//函数名：GetDataDictionary
//作者：YangJiannan
//日期：2013.10.31
//功能：获得数据字典
//输入参数：handle					网络对象句柄
//			stringLocalFilePath		本地文件保存路径
//返回值： 正常或错误返回码
//参数和返回值的详细信息参见Net_Interface_V1.doc的4.3.18
//==================================================================
int NET_API GetDataDictionary(NET_HANDLE handle, const char* stringLocalFilePath);

//==================================================================
//函数名：LoggingHealthBaseInfo
//作者：YangJiannan
//日期：2013.10.31
//功能：健康档案基本信息录入
//输入参数：handle					网络对象句柄
//			其他略
//返回值： 正常或错误返回码
//参数和返回值的详细信息参见Net_Interface_V1.doc的4.3.19
//==================================================================
int NET_API LoggingHealthBaseInfo(NET_HANDLE handle, const char* stringUserId, const char* stringWeblock, const char* stringNation, const char* stringProfression, const char* stringCrowd, const char* stringBloodType, 
						  const char* stringJwsJiBing, const char* stringJwsShouShu, const char* stringJwsWaiShang, const char* stringJwsShuXue, const char* stringJwsBaoLuShi, const char* stringJwsBaoLuShiBz, 
						  const char* stringJwsYiChuanJiBing, const char* stringGmsGuoMinShi, const char* stringGmsGuoMinShiBz, const char* stringJzsFuQin, const char* stringJzsFuQinBz, const char* stringJzsMuQin, 
						  const char* stringJzsMuQinBz, const char* stringJzsXdjm, const char* stringJzsXdjmBz, const char* stringJzsZiNv, const char* stringJzsZiNvBz, const char* stringJzsQiTa);

//==================================================================
//函数名：LoggingHealthCurrentInfo
//作者：YangJiannan
//日期：2013.10.31
//功能：健康档案随访信息录入
//输入参数：handle					网络对象句柄
//			其他略
//返回值： 正常或错误返回码
//参数和返回值的详细信息参见Net_Interface_V1.doc的4.3.20
//==================================================================
int NET_API LoggingHealthCurrentInfo(NET_HANDLE handle, const char* stringUserId, const char* stringShfsYinShi, const char* stringShfsXlzk, const char* stringShfsXlzkBz, const char* stringShfsXltz, const char* stringShfsXiYan, 
							 const char* stringShfsYinJiu, const char* stringShfsSyqk, const char* stringShfsYunDong, const char* stringShfsYunDongShiJian, const char* stringSysjcDanGuChun, const char* stringSysjcXueChangGui, 
							 const char* stringSysjcXueChangGuiBz, const char* stringSysjcNiaoChangGui, const char* stringSysjcNiaoChangGuiBz, const char* stringSysjcQiTa, const char* stringYongYao, const char* stringZhuYuan, 
							 const char* stringJkwtNaoXueGuan, const char* stringJkwtNaoXueGuanBz, const char* stringJkwtShenZang, const char* stringJkwtShenZangBz, const char* stringJkwtXinZang, const char* stringJkwtXinZangBz, 
							 const char* stringJkwtXueGuan, const char* stringJkwtXueGuanBz, const char* stringJkwtYanBu, const char* stringJkwtYanBuBz, const char* stringJkwtShenJing, const char* stringJkwtQiTa, const char* stringDate);

//==================================================================
//函数名：UpdateCaseInfo
//作者：YangJiannan
//日期：2014.2.13
//功能：健康档案随访信息录入
//输入参数：handle					网络对象句柄
//			stringHospitalId		医院id
//			stringCaseId			病例号
//			stringOtherParam		病例参数
//			stringFlag				操作标志
//			stringNotes				修改备注
//输出参数：stringDoctorId			医生号
//返回值： 正常或错误返回码
//参数和返回值的详细信息参见Net_Interface_V1.doc的4.3.21
//==================================================================
int NET_API UpdateCaseInfo(NET_HANDLE handle, char* stringDoctorId, const char* stringHospitalId, const char* stringCaseId, const char* stringOtherParam, const char* stringFlag, const char* stringNotes, const char* stringReceiverId);

//==================================================================
//函数名：PersonIdRegisterAndLogin
//作者：YangJiannan
//日期：2014.3.25
//功能：健康档案随访信息录入
//输入参数：handle					网络对象句柄
//			stringLocalFilePath		本地文件路径
//			stringRole				用户类别
//			stringPId				身份证号
//			stringName				姓名
//			stringSex				性别
//			stringBirthday			生日
//			stringAddress			地址
//			stringDeviceId			设备Id
//			stringHospitalId		医院Id
//			stringLang				语言
//			stringRegFrom			注册请求来源
//返回值： 正常或错误返回码
//参数和返回值的详细信息参见Net_Interface_V1.doc的4.3.22
//==================================================================
int NET_API PersonIdRegisterAndLogin(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringRole, const char* stringPId, const char* stringName, const char* stringSex, const char* stringBirthday, const char* stringAddress, const char* stringDeviceId, const char* stringHospitalId, 
							 const char* stringLang, const char* stringRegFrom);

//==================================================================
//函数名：UploadTrendData2
//作者：YangJiannan
//日期：2014.3.28
//功能：健康档案随访信息录入
//输入参数：handle					网络对象句柄
//			stringTargetPath		本地文件路径
//返回值： 正常或错误返回码
//参数和返回值的详细信息参见Net_Interface_V1.doc的4.3.23
//==================================================================
int NET_API UploadTrendData2(NET_HANDLE handle, const char* stringTargetPath);

//==================================================================
//函数名：CardBindPhone
//作者：YangJiannan
//日期：2014.8.20
//功能：卡用户绑定手机
//输入参数：handle					网络对象句柄
//			stringPhone				手机号
//			stringScode				验证码
//返回值： 正常或错误返回码
//
//==================================================================
int NET_API CardBindPhone(NET_HANDLE handle, const char* stringPhone, const char* stringScode);

//==================================================================
//函数名：ResetPassword
//作者：YangJiannan
//日期：2014.8.20
//功能：卡用户重置密码
//输入参数：handle					网络对象句柄
//			stringPhone				手机号
//			stringScode				验证码
//			stringNewPwd			新密码
//返回值： 正常或错误返回码
//
//==================================================================
int NET_API ResetPassword(NET_HANDLE handle, const char* stringPhone, const char* stringScode, const char* stringNewPwd);

//==================================================================
//函数名：UploadDaily
//作者：YangJiannan
//日期：2015.9.2
//功能：获得某月有孕期日记的日期列表
//输入参数：handle					网络对象句柄
//			stringType				类型
//			stringCreateTime		创建时间
//			stringFiles				自定义格式字符串
//返回值： 正常或错误返回码
//
//==================================================================
int NET_API UploadDaily(NET_HANDLE handle, const char* stringType, const char* stringCreateTime, const char* stringFiles);

//==================================================================
//函数名：GetDateListOfDaily
//作者：YangJiannan
//日期：2015.6.24
//功能：获得某月有孕期日记的日期列表
//输入参数：handle					网络对象句柄
//			stringLocalPath			本地文件保存路径
//			stringYear				年份
//			stringMonth				月份
//返回值： 正常或错误返回码
//
//==================================================================
int NET_API GetDateListOfDaily(NET_HANDLE handle, const char* stringLocalPath, const char* stringYear, const char* stringMonth);

//==================================================================
//函数名：GetDailyOfDate
//作者：YangJiannan
//日期：2015.6.24
//功能：获得日记内容
//输入参数：handle					网络对象句柄
//			stringLocalPath			本地文件保存路径
//			stringDate				日期
//			stringType				类型
//返回值： 正常或错误返回码
//
//==================================================================
int NET_API GetDailyOfDate(NET_HANDLE handle, const char* stringLocalPath, const char* stringDate, const char* stringType);

//==================================================================
//函数名：ShareDaily
//作者：YangJiannan
//日期：2015.6.24
//功能：分享日记
//输入参数：handle					网络对象句柄
//			stringLocalPath			本地文件保存路径
//			stringType				类型
//			stringCreateTime		创建时间
//返回值： 正常或错误返回码
//
//==================================================================
int NET_API ShareDaily(NET_HANDLE handle, const char* stringLocalPath, const char* stringType, const char* stringCreateTime);

//==================================================================
//函数名：DeleteDaily
//作者：YangJiannan
//日期：2015.6.24
//功能：删除日记
//输入参数：handle					网络对象句柄
//			stringType				类型
//			stringDate				日期
//返回值： 正常或错误返回码
//
//==================================================================
int NET_API DeleteDaily(NET_HANDLE handle, const char* stringType, const char* stringDate);

//==================================================================
//函数名：PhoneScanQR
//作者：YangJiannan
//日期：2014.12.15
//功能：卡用户重置密码
//输入参数：handle					网络对象句柄
//			stringQRCode			二维码图片解析后的数据
//返回值： 正常或错误返回码
//
//==================================================================
int NET_API PhoneScanQR(NET_HANDLE handle, char* stringSence, const char* stringQRCode);

//==================================================================
//函数名：PhoneScanQRVerify
//作者：YangJiannan
//日期：2014.12.15
//功能：卡用户重置密码
//输入参数：handle					网络对象句柄
//			stringQRCode			二维码图片解析后的数据
//返回值： 正常或错误返回码
//
//==================================================================
int NET_API PhoneScanQRVerify(NET_HANDLE handle, const char* stringQRCode);

//==================================================================
//函数名：RegisterBluetoothDevice
//作者：YangJiannan
//日期：2015.3.17
//功能：注册或修改设备蓝牙信息
//输入参数：handle					网络对象句柄
//			stringUniqueSN			设备唯一码
//			stringSerial			序列号
//			stringBluetoothAddress	蓝牙地址
//			stringBluetoothName		蓝牙名
//			stringDeviceType		设备类型编码
//返回值： 正常或错误返回码
//
//==================================================================
int NET_API RegisterBluetoothDevice(NET_HANDLE handle, const char* stringUniqueSN, const char* stringSerial, const char* stringBluetoothAddress, const char* stringBluetoothName, const char* stringDeviceType);

//==================================================================
//函数名：GetBluetoothInfoOfDevice
//作者：YangJiannan
//日期：2015.3.17
//功能：获得设备的蓝牙信息
//输入参数：handle					网络对象句柄
//			stringUniqueSN			设备唯一码
//返回值： 正常或错误返回码
//
//==================================================================
int NET_API GetBluetoothInfoOfDevice(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringUniqueSN);

//==================================================================
//函数名：ChangeDeviceType
//作者：YangJiannan
//日期：2015.3.17
//功能：改变设备类型
//输入参数：handle					网络对象句柄
//			stringUniqueSN			设备唯一码
//			stringDeviceType		设备类型编码
//返回值： 正常或错误返回码
//
//==================================================================
int NET_API ChangeDeviceType(NET_HANDLE handle, const char* stringUniqueSN, const char* stringDeviceType);

//==================================================================
//函数名：UnregisterBluetoothDevice
//作者：YangJiannan
//日期：2015.3.17
//功能：设备报废（取消注册）
//输入参数：handle					网络对象句柄
//			stringUniqueSN			设备唯一码
//返回值： 正常或错误返回码
//
//==================================================================
int NET_API UnregisterBluetoothDevice(NET_HANDLE handle, const char* stringUniqueSN);


//==================================================================
//函数名：ValidatePay_XinJie
//作者：YangJiannan
//日期：2015.5.25
//功能：北京心界验证付费接口
//输入参数：handle					网络对象句柄
//			stringLocalFilePath		本地文件保存路径
//			stringRrn				参考号
//			localdate				所属日期
//返回值： 正常或错误返回码
//
//==================================================================
int NET_API ValidatePay_XinJie(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringRrn, const char* stringLocalDate);

//==================================================================
//函数名：NotifyCheck_XinJie
//作者：YangJiannan
//日期：2015.6.11
//功能：北京心界通知参考号已使用接口
//输入参数：handle					网络对象句柄
//			stringLocalFilePath		本地文件保存路径
//			stringRrn				参考号
//			localdate				所属日期
//返回值： 正常或错误返回码
//
//==================================================================
int NET_API NotifyCheck_XinJie(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringRrn, const char* stringLocalDate);

//==================================================================
//函数名：GetHospitalListOfArea
//作者：YangJiannan
//日期：2015.6.29
//功能：获得地区医院列表
//输入参数：handle					网络对象句柄
//			stringLocalFilePath		本地文件保存路径
//			stringAreaId			地区ID
//返回值： 正常或错误返回码
//
//==================================================================
int NET_API GetHospitalListOfArea(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringAreaId, const char* stringRank, const char* stringBeta);



//==================================================================
//函数名：SearchSomebody
//作者：YangJiannan
//日期：2015.6.29
//功能：获得地区医院列表
//输入参数：handle					网络对象句柄
//			stringLocalFilePath		本地文件保存路径
//			stringUserId			用户ID
//返回值： 正常或错误返回码
//
//==================================================================
int NET_API SearchSomebody(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringUserId);

//==================================================================
//函数名：FollowSomebody
//作者：YangJiannan
//日期：2015.6.29
//功能：关注某人
//输入参数：handle					网络对象句柄
//			stringLocalFilePath		本地文件保存路径
//			stringUserId			用户ID
//			stringUsername			用户名
//			stringNickname			用户昵称
//			stringVCode				验证码
//返回值： 正常或错误返回码
//
//==================================================================
int NET_API FollowSomebody(NET_HANDLE handle, const char* stringUserId, const char* stringUsername, const char* stringNickname, const char* stringVCode);

//==================================================================
//函数名：MyFollowing
//作者：YangJiannan
//日期：2015.6.29
//功能：我关注的人
//输入参数：handle					网络对象句柄
//			stringLocalFilePath		本地文件保存路径
//			stringUserId			用户ID
//			stringNextId			拉取列表的位置
//返回值： 正常或错误返回码
//
//==================================================================
int NET_API MyFollowing(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringUserId, const char* stringNextId, const char* stringFType);



//==================================================================
//函数名：ChangeNicknameOfFollowed
//作者：YangJiannan
//日期：2015.6.29
//功能：改变被关注的昵称
//输入参数：handle					网络对象句柄
//			stringLocalFilePath		本地文件保存路径
//			stringUserId			用户ID
//			stringNickname			昵称
//返回值： 正常或错误返回码
//
//==================================================================
int NET_API ChangeNicknameOfFollowed(NET_HANDLE handle, const char* stringUserId, const char* stringNickname);


//==================================================================
//函数名：CancelFollowing
//作者：YangJiannan
//日期：2015.6.29
//功能：取消关注
//输入参数：handle					网络对象句柄
//			stringLocalFilePath		本地文件保存路径
//			stringUserId			用户ID
//返回值： 正常或错误返回码
//
//==================================================================
int NET_API CancelFollowing(NET_HANDLE handle, const char* stringUserId);


//==================================================================
//函数名：GenerateOrDeletePngZlibReport
//作者：YangJiannan
//日期：2015.9.9
//功能：创建或删除pngzlib报告
//输入参数：handle					网络对象句柄
//			stringLocalFilePath		本地文件保存路径
//			stringServerPath		报告服务器路径
//			stringFlag				创建（1）或删除（2）标志
//返回值： 正常或错误返回码
//
//==================================================================
int NET_API GenerateOrDeletePngZlibReport(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringServerPath, const char* stringFlag);



//==================================================================
//函数名：GetDailyList
//作者：YangJiannan
//日期：2015.9.30
//功能：获得孕期日记列表
//输入参数：handle					网络对象句柄
//			stringLocalFilePath		本地文件保存路径
//			stringType				类型
//			stringStartDate			开始日期
//			stringEndDate			结束日期
//			stringOrderBy			排序
//			stringNextId			拉取位置
//			stringPageSize			每次拉取返回条数
//返回值： 正常或错误返回码
//
//==================================================================
int NET_API GetDailyList(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringType, const char* stringStartDate, const char* stringEndDate, const char* stringOrderBy, const char* stringNextId, const char* stringPageSize);


//==================================================================
//函数名：ChangeViewLikeCollectCount
//作者：YangJiannan
//日期：2015.11.26
//功能：修改健康知识阅读点赞收藏次数
//输入参数：handle					网络对象句柄
//			stringHealthTipId		健康知识ID
//			stringUri				健康知识全文URL
//			stringTitle				健康知识标题
//			stringLike				点赞变化数
//			stringRead				阅读变化数
//			stringCollect			收藏变化数
//返回值： 正常或错误返回码
//
//==================================================================
int NET_API ChangeViewLikeCollectCount(NET_HANDLE handle, const char* stringHealthTipId, const char* stringUri, const char* stringTitle, const char* stringLike, const char* stringRead, const char* stringCollect);


//==================================================================
//函数名：ChangeAdviceExecutiveState
//作者：YangJiannan
//日期：2015.11.26
//功能：获得健康建议执行度
//输入参数：handle					网络对象句柄
//			stringSgId				健康建议ID
//			stringState				健康建议执行状态
//返回值： 正常或错误返回码
//
//==================================================================
int NET_API ChangeAdviceExecutiveState(NET_HANDLE handle, const char* stringSgId, const char* stringState);


//==================================================================
//函数名：GetAccessToken_GuanXin
//作者：YangJiannan
//日期：2015.11.10
//功能：从冠新共卫平台获得访问个人信息需要的AccessToken
//输入参数：handle					网络对象句柄
//			stringLocalFilePath		本地文件保存路径
//			stringUsername			类型
//			stringPassword			开始日期
//返回值： 正常或错误返回码
//
//==================================================================
int NET_API GetAccessToken_GuanXin(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringUsername, const char* stringPassword);


//==================================================================
//函数名：CreateSession_GuanXin
//作者：YangJiannan
//日期：2015.11.12
//功能：冠新平台，创建session
//输入参数：handle					网络对象句柄
//			stringUsername			用户名
//			stringPassword			密码
//返回值： 正常或错误返回码
//
//==================================================================
//int NET_API CreateSession_GuanXin(NET_HANDLE handle, const char* stringUsername, const char* stringPassword);

//==================================================================
//函数名：GetPersonInfo_GuanXin
//作者：YangJiannan
//日期：2015.11.10
//功能：从冠新共卫平台获得个人信息
//输入参数：handle					网络对象句柄
//			stringLocalFilePath		本地文件保存路径
//			stringAccessToken		AccessToken
//			stringPersonName		用户姓名
//			stringPersonGender		用户性别
//			stringPersonIdentity	用户身份证号
//			stringPersonBirthDate	用户出生日期
//返回值： 正常或错误返回码
//
//==================================================================
int NET_API GetPersonInfo_GuanXin(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringAccessToken, const char* stringPersonName, const char* stringPersonGender, const char* stringPersonIdentity, const char* stringPersonBirthDate);


//==================================================================
//函数名：UploadECGData_GuanXin
//作者：YangJiannan
//日期：2016.4.19
//功能：同步心电数据到冠新平台
//输入参数：handle					网络对象句柄
//			stringLocalFilePath		本地文件保存路径
//			stringECGXmlPath		心电参数XML文件保存路径
//			stringECGDataPath		心电波形文件保存路径
//返回值： 正常或错误返回码
//
//==================================================================
int NET_API UploadECGData_GuanXin(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringECGXmlPath, const char* stringECGDataPath);

//==================================================================
//函数名：UploadOtherData_GuanXin
//作者：YangJiannan
//日期：2016.4.19
//功能：同步非心电数据到冠新平台
//输入参数：handle					网络对象句柄
//			stringLocalFilePath		本地文件保存路径
//			stringOtherXmlPath		非心电参数XML文件保存路径
//返回值： 正常或错误返回码
//
//==================================================================
int NET_API UploadOtherData_GuanXin(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringOtherXmlPath);

//==================================================================
//函数名：UploadCheckData_GuanXin
//作者：YangJiannan
//日期：2016.4.19
//功能：同步监测数据到冠新平台
//输入参数：handle					网络对象句柄
//			stringLocalFilePath		本地文件保存路径
//			stringCheckXmlPath		非心电参数XML文件保存路径
//返回值： 正常或错误返回码
//
//==================================================================
int NET_API UploadCheckData_GuanXin(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringCheckXmlPath);

//==================================================================
//函数名：UploadPersonInfo_GuanXin
//作者：YangJiannan
//日期：2016.11.2
//功能：同步身份信息到冠心平台
//输入参数：handle					网络对象句柄
//			stringLocalFilePath		本地文件保存路径
//			stringPersonInfoXmlPath		非心电参数XML文件保存路径
//返回值： 正常或错误返回码
//
//==================================================================
int NET_API UploadPersonInfo_GuanXin(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringPersonInfoXmlPath);

//==================================================================
//函数名：Login_XiKang
//作者：YangJiannan
//日期：2016.6.29
//功能：顾问登录
//输入参数：handle					网络对象句柄
//			stringLocalFilePath		本地文件保存路径
//			stringUsername			用户名
//			stringPassword			密码
//返回值： 正常或错误返回码
//
//==================================================================
int NET_API Login_XiKang(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringUsername, const char* stringPassword);

//==================================================================
//函数名：RegisterPatientId_XiKang
//作者：YangJiannan
//日期：2016.6.30
//功能：注册会员ID
//输入参数：handle					网络对象句柄
//			stringLocalFilePath		本地文件保存路径
//			stringName				姓名
//			stringSex				性别(性别代码 0 未知的性别;1 男性;2 女性;9 未说明的性别)
//			stringPhoneNum			手机号
//			stringProofNum			身份证号
//返回值： 正常或错误返回码
//
//==================================================================
int NET_API RegisterPatientId_XiKang(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringName, const char* stringSex, const char* stringPhoneNum, const char* stringProofNum);

//==================================================================
//函数名：RegisterPatientInfo_XiKang
//作者：YangJiannan
//日期：2016.6.30
//功能：注册会员账户信息
//输入参数：handle					网络对象句柄
//			stringLocalFilePath		本地文件保存路径
//			stringPatientOpenId		会员openid（RegisterPatientId_XiKang接口获得）
//			stringName				姓名
//			stringSex				性别(性别代码 0 未知的性别;1 男性;2 女性;9 未说明的性别)
//			stringPhoneNum			手机号
//			stringProofNum			身份证号
//			stringPassword			密码
//返回值： 正常或错误返回码
//
//==================================================================
int NET_API RegisterPatientInfo_XiKang(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringPatientOpenId, const char* stringName, const char* stringSex, const char* stringPhoneNum, const char* stringProofNum, const char* stringPassword);

//==================================================================
//函数名：BindPatient_XiKang
//作者：YangJiannan
//日期：2016.6.30
//功能：绑定会员
//输入参数：handle					网络对象句柄
//			stringLocalFilePath		本地文件保存路径
//			stringDoctorOpenId		顾问的openid（登录获得）
//			stringPatientOpenId		会员的openid（RegisterPatientId_XiKang接口获得）
//返回值： 正常或错误返回码
//
//==================================================================
int NET_API BindPatient_XiKang(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringDoctorOpenId, const char* stringPatientOpenId);

//==================================================================
//函数名：GetPatientIdList_XiKang
//作者：YangJiannan
//日期：2016.6.30
//功能：获得会员id列表
//输入参数：handle					网络对象句柄
//			stringLocalFilePath		本地文件保存路径
//			stringDoctorOpenId		顾问的openid（登录获得）
//返回值： 正常或错误返回码
//
//==================================================================
int NET_API GetPatientIdList_XiKang(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringDoctorOpenId);

//==================================================================
//函数名：SearchPatientId_XiKang
//作者：YangJiannan
//日期：2016.11.29
//功能：查找会员OpenID
//输入参数：handle					网络对象句柄
//			stringLocalFilePath		本地文件保存路径
//			stringKey		查找类别关键字（唯一性查询条件：EMAILNUM(邮箱),MOBILENUM(手机号),PROOFNUM(证件号),PERSONNICKNAME(昵称),MEMBERCARDNO(会员卡号)）
//			stringValue		查找值
//返回值： 正常或错误返回码
//
//==================================================================
int NET_API SearchPatientId_XiKang(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringKey, const char* stringValue);

//==================================================================
//函数名：GetPatientInfo_XiKang
//作者：YangJiannan
//日期：2016.6.30
//功能：根据会员id获得会员信息
//输入参数：handle					网络对象句柄
//			stringLocalFilePath		本地文件保存路径
//			stringPatientOpenId		会员的openid（RegisterPatientId_XiKang接口获得）
//返回值： 正常或错误返回码
//
//==================================================================
int NET_API GetPatientInfo_XiKang(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringPatientOpenId);

//==================================================================
//函数名：GetCaseList_XiKang
//作者：YangJiannan
//日期：2016.6.30
//功能：获得会诊信息列表
//输入参数：handle					网络对象句柄
//			stringLocalFilePath		本地文件保存路径
//			arrPatientOpenId		会员的openid（RegisterPatientId_XiKang接口获得）的数组
//			nLength					数组长度
//返回值： 正常或错误返回码
//
//==================================================================
int NET_API GetCaseList_XiKang(NET_HANDLE handle, const char* stringLocalFilePath, const char** arrPatientOpenId, int nLength);

//==================================================================
//函数名：SubmitCase_XiKang
//作者：YangJiannan
//日期：2016.6.30
//功能：提交会诊信息（静态心电病例通过stringEcgXmlContent，其他类型病例以后会添加UploadFile_XiKang接口）
//输入参数：handle					网络对象句柄
//			stringLocalFilePath		本地文件保存路径
//			stringPatientOpenId		会员的openid（RegisterPatientId_XiKang接口获得）
//			stringAdvice			医生建议
//			stringCaseType			会诊类型（静态ecg传入staticECG_KT；动态ecg传入dynamicECG_KT；血压传入bloodPressure_KT）
//返回值： 正常或错误返回码
//
//==================================================================
int NET_API SubmitCase_XiKang(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringPatientOpenId, const char* stringAdvice, const char* stringCaseType, const char* stringEcgXmlContent);

//==================================================================
//函数名：GetReportFile_XiKang
//作者：YangJiannan
//日期：2016.6.30
//功能：获得会诊报告url
//输入参数：handle					网络对象句柄
//			stringLocalFilePath		本地文件保存路径
//			stringCaseId			会诊id（通过SubmitCase_XiKang接口获得）
//返回值： 正常或错误返回码
//
//==================================================================
int NET_API GetReportFile_XiKang(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringCaseId);

//==================================================================
//函数名：UploadFile_XiKang
//作者：YangJiannan
//日期：2016.8.22
//功能：上传病例文件
//输入参数：handle					网络对象句柄
//			stringLocalFilePath		本地文件保存路径
//			stringCaseId			会诊id（通过SubmitCase_XiKang接口获得）
//			stringCaseFilePath		病例文件路径
//返回值： 正常或错误返回码
//
//==================================================================
int NET_API UploadFile_XiKang(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringCaseId, const char* stringCaseFilePath);

//==================================================================
//函数名：GetCardUserListOfCustomer
//作者：YangJiannan
//日期：2017.12.15
//功能：获得在某机构上传过病例的卡用户列表
//输入参数：handle					网络对象句柄
//			stringLocalFilePath		本地文件保存路径
//			stringThirdId			第三方号
//			stringName				卡用户姓名
//			stringCaseType			病理类型（支持多个逗号隔开）
//			stringStartDate			开始时间
//			stringEndDate			结束时间
//			stringNextId			nextid
//			nPageSize				一次要多少条
//返回值： 正常或错误返回码
//
//==================================================================
int NET_API GetCardUserListOfCustomer(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringThirdId, const char* stringName, const char* stringCaseType, 
									  const char* stringStartDate, const char* stringEndDate, const char* stringNextId, unsigned int nPageSize);


//==================================================================
//函数名：RefundFee
//作者：YangJiannan
//日期：2017.12.15
//功能：退款
//输入参数：handle					网络对象句柄
//			stringType				支付平台类型（1：微信）
//			stringCaseId			病例号
//			stringRefundFee			退回金额（单位分）
//返回值： 正常或错误返回码
//
//==================================================================
int NET_API RefundFee(NET_HANDLE handle, const char* stringType, const char* stringCaseId, const char* stringRefundFee);




#ifdef __cplusplus 
	} 
#endif 

#endif