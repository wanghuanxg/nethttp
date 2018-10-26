#ifndef PHMS_WITHIN_HOSPITAL_INTERFACE
#define PHMS_WITHIN_HOSPITAL_INTERFACE
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
//函数名：SetCliniqueBusy
//作者：YangJiannan
//日期：2014.10.30
//功能：设置上传点状态为忙碌  
//输入参数：handle					网络对象句柄
//			stringCliniqueId		上传点id
//返回值： 正常或错误返回码
//使用范围：终端、叫号软件
//修改记录：
//==================================================================
int NET_API SetCliniqueBusy(NET_HANDLE handle, const char* stringCliniqueId);

//==================================================================
//函数名：SetCliniqueFree
//作者：YangJiannan
//日期：2014.10.30
//功能：设置上传点状态为空闲
//输入参数：handle					网络对象句柄
//			stringCliniqueId		上传点id
//返回值： 正常或错误返回码
//使用范围：终端
//修改记录：
//==================================================================
int NET_API SetCliniqueFree(NET_HANDLE handle, const char* stringCliniqueId);

//==================================================================
//函数名：SetCliniqueChecking
//作者：YangJiannan
//日期：2014.10.30
//功能：设置上传点状态为检查中  
//输入参数：handle					网络对象句柄
//			stringCliniqueId		上传点id
//返回值： 正常或错误返回码
//使用范围：终端
//修改记录：
//==================================================================
int NET_API SetCliniqueChecking(NET_HANDLE handle, const char* stringCliniqueId);

//==================================================================
//函数名：SetCliniqueShoutComplete
//作者：YangJiannan
//日期：2014.10.30
//功能：设置上传点状态为叫号完成 
//输入参数：handle					网络对象句柄
//			stringCliniqueId		上传点id
//返回值： 正常或错误返回码
//使用范围：叫号软件
//修改记录：
//==================================================================
int NET_API SetCliniqueShoutComplete(NET_HANDLE handle, const char* stringCliniqueId);


//==================================================================
//函数名：GetPatientInfoByAppNum
//作者：YangJiannan
//日期：2014.10.30
//功能：根据申请单号获得病人信息
//输入参数：handle					网络对象句柄
//			stringLocalFilePath		本地文件保存路径
//			stringOrderType			申请单类型（1：门诊  2：住院  3：急诊  4：体检）
//			stringOrderId			申请单号(S：申请单号 M：门诊号 Z：住院号 T：体检号 K：卡号 W：未知)
//			stringReqDepart			申请科室代码
//			stringOrderState		申请单状态（0：全部   1：待检查   2：已检查）
//			stringCheckType			检查类别
//			stringStartDate			开单日期开始时间
//			stringEndDate			开单日期结束时间
//			stringClinicId			诊室id
//输出参数：
//返回值： 正常或错误返回码
//使用范围：叫号软件、整合客户端、设备
//修改记录：
//==================================================================
int NET_API GetPatientInfoByAppNum(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringHospitalId, const char* stringOrderType, const char* stringOrderId, 
								   const char* stringReqDepart, const char* stringOrderState, const char* stringCheckType, const char* stringStartDate, const char* stringEndDate, const char* stringClinicId);

//==================================================================
//函数名：GetPatientInfoByAppNumToHis
//作者：YangJiannan
//日期：2017.10.20
//功能：根据申请单号获得病人信息
//输入参数：handle					网络对象句柄
//			stringLocalFilePath		本地文件保存路径
//			stringOrderType			申请单类型（1：门诊  2：住院  3：急诊  4：体检）
//			stringOrderId			申请单号(S：申请单号 M：门诊号 Z：住院号 T：体检号 K：卡号 W：未知)
//输出参数：
//返回值： 正常或错误返回码
//使用范围：叫号软件、整合客户端、设备
//修改记录：
//==================================================================
int NET_API GetPatientInfoByAppNumToHis(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringHospitalId, const char* stringOrderType, const char* stringOrderId);

//==================================================================
//函数名：GetPatientInfoByAppNumToPhmsAndHis
//作者：YangJiannan
//日期：2017.10.20
//功能：根据申请单号获得病人信息
//输入参数：handle					网络对象句柄
//			stringLocalFilePath		本地文件保存路径
//			stringOrderType			申请单类型（1：门诊  2：住院  3：急诊  4：体检）
//			stringOrderId			申请单号(S：申请单号 M：门诊号 Z：住院号 T：体检号 K：卡号 W：未知)
//输出参数：
//返回值： 正常或错误返回码
//使用范围：叫号软件、整合客户端、设备
//修改记录：
//==================================================================
int NET_API GetPatientInfoByAppNumToPhmsAndHis(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringHospitalId, const char* stringOrderType, const char* stringOrderId);

//==================================================================
//函数名：ApplyCheck
//作者：YangJiannan
//日期：2014.10.30
//功能：申请检查  
//输入参数：handle					网络对象句柄
//			stringCliniqueId		上传点号
//			stringOrderId			申请单号
//			stringOrderType			申请单类型（0：门诊  1：住院  空：全部）
//返回值： 正常或错误返回码
//使用范围：叫号软件
//修改记录：
//==================================================================
int NET_API ApplyCheck(NET_HANDLE handle, const char* stringCliniqueId, const char* stringOrderId, const char* stringOrderType);

//==================================================================
//函数名：GetWaitingCheck
//作者：YangJiannan
//日期：2014.10.30
//功能：获得等待检查患者信息  
//输入参数：handle					网络对象句柄
//			stringLocalFilePath		本地文件保存路径
//			stringCliniqueId		上传点号
//返回值： 正常或错误返回码
//使用范围：终端
//修改记录：
//==================================================================
int NET_API GetWaitingCheck(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringCliniqueId);

//==================================================================
//函数名：GetCliniqueState
//作者：YangJiannan
//日期：2014.10.30
//功能：获得上传点状态  
//输入参数：handle					网络对象句柄
//			stringLocalFilePath		本地文件保存路径
//			stringCliniqueId		上传点号
//返回值： 正常或错误返回码
//使用范围：叫号软件
//修改记录：
//==================================================================
int NET_API GetCliniqueState(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringCliniqueId);

//==================================================================
//函数名：NotifyCaseState
//作者：YangJiannan
//日期：2014.11.17
//功能：通知HIS病例状态改变  
//输入参数：handle					网络对象句柄
//			stringHospitalId		医院号
//			stringOrderType			申请单类型（0：门诊   1：住院）
//			stringOrderId			申请单号
//返回值： 正常或错误返回码
//使用范围：客户端
//修改记录：
//==================================================================
int NET_API NotifyCaseState(NET_HANDLE handle, const char* stringHospitalId, const char* stringOrderType, const char* stringOrderId);

//==================================================================
//函数名：NotifyReportState
//作者：YangJiannan
//日期：2014.11.17
//功能：通知HIS报告状态改变  
//输入参数：handle					网络对象句柄
//			stringReportId			报告id
//返回值： 正常或错误返回码
//使用范围：专家端
//修改记录：
//==================================================================
int NET_API NotifyReportState(NET_HANDLE handle, const char* stringReportId);

//==================================================================
//函数名：SetDoctorAgentId
//作者：YangJiannan
//日期：2015.2.28
//功能：设置PHMS系统院内医生工号，只有在院内对接时会用到
//输入参数：handle					网络对象句柄
//			stringDoctorAgentId		院内医生工号
//返回值： 正常或错误返回码
//使用范围：专家端
//修改记录：
//==================================================================
int NET_API SetDoctorAgentId(NET_HANDLE handle, const char* stringDoctorAgentId);

//==================================================================
//函数名：NotifyAppState
//作者：YangJiannan
//日期：2017.10.27
//功能：通知服务器申请单状态（已检查或跳过）
//输入参数：handle					网络对象句柄
//			stringUnitId			上传点登录号
//			stringOrderId			申请单号
//			stringOrderType			对应GetPatientInfoByAppNum接口返回的patient_source_code
//			stringState				状态（1：已检查， 2跳过）
//返回值： 正常或错误返回码
//使用范围：客户端或设备终端
//修改记录：
//==================================================================
int NET_API NotifyAppState(NET_HANDLE handle, const char* stringUnitId, const char* stringOrderId, const char* stringOrderType, const char* stringState);

//==================================================================
//函数名：VerifyAppState
//作者：YangJiannan
//日期：2017.10.27
//功能：通过服务器验证申请单是否已被跳过或者已检查
//输入参数：handle					网络对象句柄
//			stringLocalFilePath		本地文件保存路径
//			stringOrderId			申请单号
//			stringOrderType			对应GetPatientInfoByAppNum接口返回的patient_source_code
//返回值： 正常或错误返回码
//使用范围：客户端或设备终端
//修改记录：
//==================================================================
int NET_API VerifyAppState(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringOrderId, const char* stringOrderType);


//==================================================================
//函数名：GetHisReportState
//作者：YangJiannan
//日期：2016.6.15
//功能：获得申请单在HIS中对应报告的状态
//输入参数：handle					网络对象句柄
//			stringHospitalId		医院ID
//			stringOrderType			申请单类型
//			stringOrderId			申请单号
//返回值： 正常或错误返回码
//使用范围：专家端
//修改记录：
//==================================================================
int NET_API GetHisReportState(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringHospitalId, const char* stringOrderType, const char* stringOrderId);


//==================================================================
//函数名：SetHisCriticalValue
//作者：YangJiannan
//日期：2016.6.15
//功能：设置病例危急值
//输入参数：handle					网络对象句柄
//			stringHospitalId		医院ID
//			stringCaseId			病例号
//			stringCriticalValues	危急值
//返回值： 正常或错误返回码
//使用范围：专家端
//修改记录：
//==================================================================
int NET_API SetHisCriticalValue(NET_HANDLE handle, const char* stringHospitalId, const char* stringCaseId, const char* stringCriticalValues);

#ifdef __cplusplus 
} 
#endif 

#endif