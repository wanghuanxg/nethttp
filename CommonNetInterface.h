#ifndef PHMS_COMMOM_INTERFACE
#define PHMS_COMMON_INTERFACE
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
//参数要求：
//①所有目标文件所在目录必须存在，否则返回文件打开失败
//②目录中暂时不支持汉字（有需要可以改）(已改支持汉字2013-4-18)
//③字符指针参数不能为NULL
//④要求字符串参数编码为GBK，将来有可能改成UTF8
//==================================================================


//==================================================================
//函数名：SetContecServerInfo
//作者：YangJiannan
//日期：2016.11.16
//功能：用户会诊小程序设置目标服务器ip和端口
//输入参数：stringContecServerAddr	服务器ip或域名
//			stringContecServerPort	服务器端口
//返回值： 无
//备注：优先于PhmsConfig.ini
//==================================================================
void NET_API SetContecServerInfo(const char* stringContecServerAddr, const char* stringContecServerPort);

//==================================================================
//函数名：SetAppDir
//作者：YangJiannan
//日期：2015.11.4
//功能：用于Android程序调用，设置工作目录，因为无法从底层获得运行的Android APP的工作目录，需要外部设置
//输入参数：stringWorkDir	工作目录
//返回值： 无
//修改记录：
//==================================================================
void NET_API SetAppDir(const char* stringWorkDir);


//==================================================================
//函数名：设置终端类型标识
//作者：YangJiannan
//日期：2013.4.18
//功能：设置每一个终端类型标识
//输入参数：终端标识号，参见P还没收DataType.h
//返回值： 正确或错误码
//修改记录：
//==================================================================
int NET_API SetTerminalType(const char* stringTerminalType);


//==================================================================
//函数名：InitializeNetLibObject
//作者：YangJiannan
//日期：2012.12.3
//功能：初始化网络对象，获得网络对象句柄，每个线程中如果要使用网络接口，必须先调用该函数，获得网络对象句柄，以后每个接口都要传入该句柄
//输入参数：无
//返回值： 网络对象句柄
//修改记录：
//==================================================================
NET_HANDLE NET_API InitializeNetLibObject();

//==================================================================
//函数名：DeleteNetLibObject
//作者：YangJiannan
//日期：2012.12.3
//功能：删除网络对象，线程结束之前必须调用该函数，否则内存泄露
//输入参数：handle					网络对象句柄
//返回值： 正常或错误返回码
//修改记录：
//==================================================================
int NET_API DeleteNetLibObject(NET_HANDLE handle);

//==================================================================
//函数名：SetSessionId（此函数只在跨进程使用同一sessionid的时候才需要用，同一进程的SessionId实现用户透明管理）
//作者：YangJiannan
//日期：2013.1.18
//功能：设置当前进程的Session Id，这是已知SessionId的情况，直接设置，不需要再次登录，服务器也不允许同一账号同时登录
//输入参数：stringSessionId			SessionId
//返回值： 正常或错误返回码
//修改记录：
//==================================================================
int NET_API SetSessionId(const char* stringSessionId);

//==================================================================
//函数名：GetSessionId（此函数只在跨进程使用同一sessionid的时候才需要用，同一进程的SessionId实现用户透明管理）
//作者：YangJiannan
//日期：2013.1.18
//功能：获得当前进程的Session Id
//输入参数：bufSessionId			buf，输出函数，用于保存SessionId
//			nBufMaxLen				buf的最大长度(不得小于128)
//返回值： 正常或错误返回码
//修改记录：
//==================================================================
int NET_API GetSessionId(char* bufSessionId, unsigned int nBufMaxLen);

//==================================================================
//函数名：SetUsernameAndPassword（此函数只在跨进程使用同一sessionid的时候才需要用，同一进程的SessionId实现用户透明管理）
//作者：YangJiannan
//日期：2017.5.24
//功能：设置登录账号密码，用于计算签名
//输入参数：stringUsername			用户名
//			stringPassword			密码
//返回值： 正常或错误返回码
//修改记录：
//==================================================================
int NET_API SetUsernameAndPassword(const char* stringUsername, const char* stringPassword);


//==================================================================
//函数名：HandleSessionSync
//作者：YangJiannan
//日期：2012.12.24
//功能：用于旧session id失效后重新获得新session id，然后调用本接口，使其他网络线程session id同步
//输入参数：stringSessionId			有效的session id
//返回值： 无
//修改记录：
//==================================================================
//void HandleSessionSync(const char* stringSessionId);



//==================================================================
//函数名：KeepSession
//作者：YangJiannan
//日期：2013.3.11
//功能：保持会话
//输入参数：handle					网络对象句柄
//返回值： 正常或错误返回码
//修改记录：
//==================================================================
int NET_API KeepSession(NET_HANDLE handle);


//==================================================================
//函数名：Logout
//作者：YangJiannan
//日期：2012.12.3
//功能：注销登录
//输入参数：handle					网络对象句柄
//返回值： 正常或错误返回码
//修改记录：
//==================================================================
int NET_API Logout(NET_HANDLE handle);

//==================================================================
//函数名：UploadIm
//作者：YangJiannan
//日期：2012.12.3
//功能：上传即时消息
//输入参数：handle					网络对象句柄
//			stringSenderId			发送者Id
//			stringSenderName		发送者姓名
//			stringReceiverId		接收者Id
//			stringMsgDirection		消息方向
//			stringMsgType			消息类型
//			stringAudioFilePath		音视频文件本地路径
//			stringCaseId			病例号
//			stringCaseType			病例类型
//返回值： 正常或错误返回码
//修改记录：
//==================================================================
int NET_API UploadIm(NET_HANDLE handle, const char* stringSenderId, const char* stringSenderName, const char* stringReceiverId, const char* stringMsgDirection, const char* stringMsgType, 
			 const char* stringMsgContent, const char* stringAudioFilePath, const char* stringCaseId, const char* stringCaseType);

//==================================================================
//函数名：GetImList
//作者：YangJiannan
//日期：2012.12.3
//功能：获得即时消息列表
//输入参数：handle					网络对象句柄
//			stringLocalFilePath		即时消息列表本地保存路径
//			stringSenderId			发送者Id
//			stringCaseId			病例Id
//			stringCaseType			病例类型
//			stringMsgDirection		消息方向
//			stringMsgType			消息类型
//返回值： 正常或错误返回码
//修改记录：
//==================================================================
int NET_API GetImList(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringSenderId, const char* stringCaseId, const char* stringCaseType, const char* stringMsgDirection, const char* stringMsgType);

//==================================================================
//函数名：GetImList2
//作者：YangJiannan
//日期：2015.7.24
//功能：获得即时消息列表2
//输入参数：handle					网络对象句柄
//			stringLocalFilePath		即时消息列表本地保存路径
//			stringNextId			拉去消息位置，首次传0
//返回值： 正常或错误返回码
//修改记录：
//==================================================================
int NET_API GetImList2(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringNextId);

//==================================================================
//函数名：DownloadIm
//作者：YangJiannan
//日期：2012.12.3
//功能：下载音视频消息
//输入参数：handle					网络对象句柄
//			stringLocalFilePath		音视频消息文件本地保存路径
//			stringMsgId				消息Id
//			stringUri				音视频消息文件服务器路径
//			nFileSize				文件大小
//返回值： 正常或错误返回码
//修改记录：
//==================================================================
int NET_API DownloadIm(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringMsgId, const char* stringUri, unsigned long nFileSize);

//==================================================================
//函数名：DownloadImComplete
//作者：YangJiannan
//日期：2014.3.21
//功能：下载音视频消息
//输入参数：handle					网络对象句柄
//			stringMsgId				消息Id
//返回值： 正常或错误返回码
//修改记录：
//==================================================================
int NET_API DownloadImComplete(NET_HANDLE handle, const char* stringMsgId);

//==================================================================
//函数名：ChangePwd
//作者：YangJiannan
//日期：2012.12.3
//功能：修改密码
//输入参数：handle					网络对象句柄
//			stringOldPwd			旧密码
//			stringNewPwd			新密码
//返回值： 正常或错误返回码
//修改记录：
//==================================================================
int NET_API ChangePwd(NET_HANDLE handle, const char* stringOldPwd, const char* stringNewPwd);

//==================================================================
//函数名：Feedbcak
//作者：YangJiannan
//日期：2012.12.3
//功能：上传意见反馈
//输入参数：handle					网络对象句柄
//			stringTitle				标题
//			stringNotes				详细描述
//			stringPhone				电话号码
//			stringPicPath			要上传的本地文件路径
//返回值： 正常或错误返回码
//修改记录：
//==================================================================
int NET_API Feedback(NET_HANDLE handle, const char* stringTitle, const char* stringNotes, const char* stringPhone, const char* stringPicPath);

//==================================================================
//函数名：GetReportListOfCase
//作者：YangJiannan
//日期：2012.12.3
//功能：获得病例的报告列表
//输入参数：handle					网络对象句柄
//			stringLocalFilePath		报告列表文件本地保存路径
//			stringCaseId			病例Id
//返回值： 正常或错误返回码
//修改记录：
//==================================================================
int NET_API GetReportListOfCase(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringCaseId);

//==================================================================
//函数名：GetHospitalListOfGroup
//作者：YangJiannan
//日期：2012.12.3
//功能：获得组织内医院列表
//输入参数：handle					网络对象句柄
//			stringLocalFilePath		医院列表文件本地保存路径
//			stringGroupId			组织Id
//			stringType				医院类型（0：转发医院，1：可选医院）
//			nPageTo					第几页
//			nPageSize				每页几条数据
//返回值： 正常或错误返回码
//修改记录：
//==================================================================
int NET_API GetHospitalListOfGroup(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringGroupId, const char* stringType, const char* stringCaseType, unsigned int nPageTo, unsigned int nPageSize);

//==================================================================
//函数名：GetDoctorListOfHospital
//作者：YangJiannan
//日期：2012.12.3
//功能：获得医院内医生列表
//输入参数：handle					网络对象句柄
//			stringLocalFilePath		医生列表文件本地保存路径
//			stringHospitalId		医院Id
//			stringDoctorId			医生Id
//			stringCaseType			病例类型
//			stringDoctorType		医生类型
//			nPageTo					第几页
//			nPageSize				每页几条数据
//返回值： 正常或错误返回码
//修改记录：
//==================================================================
int NET_API GetDoctorListOfHospital(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringHospitalId, const char* stringDoctorId, const char* stringCaseType, const char* stringDoctorType, unsigned int nPageTo, unsigned int nPageSize);


//==================================================================
//函数名：AbortSession
//作者：YangJiannan
//日期：2012.12.13
//功能：即时退出Http Session
//输入参数：handle					网络对象句柄
//返回值： 正常或错误返回码
//修改记录：
//==================================================================
int NET_API AbortSession(NET_HANDLE handle);

//==================================================================
//函数名：GetEventStream
//作者：YangJiannan
//日期：2013.01.30
//功能：获得病例事件流
//输入参数：handle					网络对象句柄
//			stringLocalFilePath		文件本地保存路径
//			stringCaseId			病例号
//			stringOriginalHospital	病例原始医院
//返回值： 正常或错误返回码
//修改记录：
//==================================================================
int NET_API GetEventStream(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringCaseId, const char* stringOriginalHospital);

//==================================================================
//函数名：GetCaseTypeList
//作者：YangJiannan
//日期：2013.01.30
//功能：获得病例类型权限
//输入参数：handle					网络对象句柄
//			stringLocalFilePath		文件本地保存路径
//			stringLanguage			指定类型名语言
//返回值： 正常或错误返回码
//修改记录：
//==================================================================
int NET_API GetCaseTypeList(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringLanguage);

//==================================================================
//函数名：DownloadPhoto
//作者：YangJiannan
//日期：2013.4.8
//功能：下载医生照片(通用文件下载接口)
//输入参数：handle					网络对象句柄
//			stringLocalFilePath		本地文件保存路径
//			stringUri				照片服务器路径
//			nFileSize				照片大小
//			pCallback				回调函数
//修改记录：
//==================================================================
int NET_API DownloadPhoto(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringUri, unsigned long nFileSize, pProgressAndSpeedCallback pCallback);

//==================================================================
//函数名：GetServerFileSize
//作者：YangJiannan
//日期：2018.7.24
//功能：获得服务器文件大小
//输入参数：handle					网络对象句柄
//			stringUri				文件服务器路径
//输出参数：nFileSize				文件大小
//修改记录：
//==================================================================
int NET_API GetServerFileSize(NET_HANDLE handle, const char* stringUri, int& nFileSize);
//==================================================================
//函数名：SendPhoneMsg
//作者：YangJiannan
//日期：2013.4.23
//功能：发送短信接口
//输入参数：handle					网络对象句柄
//			stringCardId			用户卡号，卡用户列表中的senderid值
//			stringPhone				手机号码
//			stringMsg				短信内容
//附加说明：stringCardId和stringPhone都为可选参数，但必须至少有一个不为空字符串。当stringCardId为空，stringPhone不为空时,
//			任何角色都可以调用该接口发送短信；当stringPhone为空，stringCardId不为空是，医生调用该接口给卡用户发送健康建议；
//			当都不为空时，已stringCard为准。
//参数和返回值的详细信息参见Net_Interface_V1.doc的4.5.18
//修改记录：
//==================================================================
int NET_API SendPhoneMsg(NET_HANDLE handle, const char* stringCardId, const char* stringPhone, const char* stringMsg);

//==================================================================
//函数名：GetFileWithHttp
//作者：YangJiannan
//日期：2013.5.13
//功能：通过http请求文件，不通过php
//输入参数：handle					网络对象句柄
//			stringLocalFilePath		文件本地保存路径
//			stringUrl				文件web url
//			bContinue				是否断点续传（0 否，1 是）
//附加说明：本接口不同于其他任何接口，因为本接口没有对应的Phms协议，只是通过http请求文件，不通过php页面；在内部接口中走了
//一条NoPhms分支，所有内部函数都有NoPhms标识，可查询该分支；该接口也需要一个handle，防止多个线程公用一个handle，导致http请求和响应混乱；
//但是该handle所有对应的CExternalNetInterface对象不需要sessionid，为了统一，也同步了sessionid，不过实际上没有用到sessionid。
//修改记录：
//==================================================================
int NET_API GetFileWithHttp(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringUrl, int bContinue);


//==================================================================
//函数名：PostForm
//作者：YangJiannan
//日期：2013.4.22
//功能：提交Form表单并获得响应内容
//输入参数：handle					网络对象句柄
//			stringResponse			响应保存buf
//			stringHost				服务器地址
//			stringUrl				页面url
//			stringKey				Post参数名数组
//			stringValue				Post参数值数组
//			nSize					数组大小
//附加说明：本接口不同于其他任何接口，因为本接口没有对应的Phms协议，只是Post一个Form表单；在内部接口中走了
//一条Form分支，所有内部函数都有Form标识，可查询该分支；该接口也需要一个handle，防止多个线程公用一个handle，导致http请求和响应混乱；
//但是该handle所有对应的CExternalNetInterface对象不需要sessionid，为了统一，也同步了sessionid，不过实际上没有用到sessionid。
//参数和返回值的详细信息参见Net_Interface_V1.doc的4.5.18
//修改记录：
//==================================================================
int NET_API PostForm(NET_HANDLE handle, char* stringResponse, unsigned int nResponseSize, const char* stringHost, const char* stringUrl, const char* stringKey[], const char* stringValue[], int nSize);

//==================================================================
//函数名：GetVerificationCode
//作者：YangJiannan
//日期：2013.10.17
//功能：身份证号注册
//输入参数：handle					网络对象句柄
//			stringType				请求短信类型
//			stringPhone				电话号码
//返回值： 正常或错误返回码
//弃用该接口：2013.12.11
//接口另作他用:2014.1.4
//==================================================================
int NET_API GetVerificationCode(NET_HANDLE handle, const char* stringType, const char* stringPhone);

//==================================================================
//函数名：ApplyQRCode
//作者：YangJiannan
//日期：2014.12.10
//功能：申请二维码
//输入参数：handle					网络对象句柄
//			stringLocalFilePath		本地文件保存路径
//			stringType				二维码类型
//			stringScene				二维码使用场景
//返回值： 正常或错误返回码
//==================================================================
int NET_API ApplyQRCode(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringType, const char* stringScene);

//==================================================================
//函数名：ValidateUserAccount
//作者：YangJiannan
//日期：2014.12.10
//功能：申请二维码
//输入参数：handle					网络对象句柄
//			stringLocalFilePath		本地文件保存路径
//			stringLoginId			用户名
//			stringPassword			密码
//			stringRole				用户角色
//返回值： 正常或错误返回码
//备注：强制加密传输；不验证签名与sessionid
//==================================================================
int NET_API ValidateUserAccount(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringLoginId, const char* stringPassword, const char* stringRole);

//==================================================================
//函数名：UploadCommonFile
//作者：YangJiannan
//日期：2015.6.29
//功能：通用上传文件
//输入参数：handle					网络对象句柄
//			stringType				文件类型
//			stringExtra				额外信息
//			stringLocalFilePath		本地文件路径
//			stringFileType			文件类型（0：pdf+bin   1：心电图机报告+bin    空：不处理）
//			pCallbcak				回调函数
//输出参数：stringUri				服务器文件保存路径
//返回值： 正常或错误返回码
//修改记录：
//==================================================================
int NET_API UploadCommonFile(NET_HANDLE handle, char* stringUri, const char* stringType, const char* stringExtra, const char* stringLocalFilePath, const char* stringFileType, pProgressAndSpeedCallback pCallbcak);


//==================================================================
//函数名：GetListOfHealthyKnowledge
//作者：YangJiannan
//日期：2015.7.23
//功能：获得健康知识列表
//输入参数：handle					网络对象句柄
//			stringLocalFilePath		本地文件保存路径
//			stringSenderId			医生SenderId
//			stringNextId			拉取位置
//			stringIds				健康知识ID
//			stringPageSize			每次拉取条数
//			stringOrderBy			0：倒序  1：正序
//返回值： 正常或错误返回码
//修改记录：
//==================================================================
int NET_API GetListOfHealthyKnowledge(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringSenderId, const char* stringNextId, const char* stringIds, const char* stringPageSize, const char* stringOrderBy);


//==================================================================
//函数名：GetCardUserReportList
//作者：YangJiannan
//日期：2015.9.22
//功能：获得卡用户报告列表
//输入参数：handle					网络对象句柄
//			stringLocalFilePath		本地文件保存路径
//			stringReportId			报告Id
//			stringReportStartDate	报告开始时间
//			stringReportEndDate		报告结束时间
//			stringCaseType			病例类型
//			stringState				报告状态
//			stringOrderBy			排序方式
//			stringThirdIdType		第三方号类型
//			stringThirdId			第三方号
//			stringNextId			获取列表拉取的位置
//			stringPageSize			每次返回数据条数
//返回值： 正常或错误返回码
//修改记录：
//==================================================================
int NET_API GetCardUserReportList(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringReportId, const char* stringReportStartDate, const char* stringReportEndDate, const char* stringCaseType, const char* stringState, const char* stringOrderBy, 
		const char* stringThirdIdType, const char* stringThirdId, const char* stringNextId, const char* stringPageSize);

//==================================================================
//函数名：MyFans
//作者：YangJiannan
//日期：2015.6.29
//功能：我的粉丝
//输入参数：handle					网络对象句柄
//			stringLocalFilePath		本地文件保存路径
//			stringUserId			用户ID
//			stringNextId			拉取列表的位置
//返回值： 正常或错误返回码
//
//==================================================================
int NET_API MyFans(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringUserId, const char* stringNextId);

//==================================================================
//函数名：GetPayCode
//作者：YangJiannan
//日期：2018.1.17
//功能：获得第三方支付码
//输入参数：handle					网络对象句柄
//			stringLocalFilePath		本地文件保存路径
//			stringUserId			用户ID
//			stringNextId			拉取列表的位置
//返回值： 正常或错误返回码
//
//==================================================================
int NET_API GetPayCode(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringType, const char* stringCaseId);

//==================================================================
//函数名：GetOrderInfo
//作者：YangJiannan
//日期：2018.1.17
//功能：获得订单信息
//输入参数：handle					网络对象句柄
//			stringLocalFilePath		本地文件保存路径
//			stringUserId			用户ID
//			stringNextId			拉取列表的位置
//返回值： 正常或错误返回码
//
//==================================================================
int NET_API GetOrderInfo(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringType, const char* stringCaseId);

//==================================================================
//函数名：AbortSessionByThreadId
//作者：YangJiannan
//日期：2014.12.11
//功能：即时退出Http Session
//输入参数：threadId				网络线程id
//返回值： 正常或错误返回码
//==================================================================
#if defined(_WIN32) || defined(_WIN64)
#define THREAD_ID unsigned long
#else
#define THREAD_ID unsigned long int
#endif
int NET_API AbortSessionByThreadId(THREAD_ID threadId);

//==================================================================
//函数名：BeginMsgListPush
//作者：YangJiannan
//日期：2013.6.19
//功能：开始即时消息列表推送
//输入参数：stringSenderId			发送者Id
//			stringCaseId			病例Id
//			stringCaseType			病例类型
//			stringMsgDirection		消息方向
//			stringMsgType			消息类型
//			stringVersion			除此参数外，其他参数全部忽略
//			pCallback				推送成功回调函数
//			pFailCallback			推送失败回调函数
//返回值： 网络对象句柄
//参数和返回值的详细信息参见Net_Interface_V1.doc的4.5.20
//备注：pCallback调用时机：①服务器成功推送消息 ②成功建立推送连接（两个参数分别为NULL, 0）
//		pFailCallback调用时机：①错误码COMMON_SESSION_INVALID、COMMON_LOGIN_ANOTHER_PLACE、COMMON_SERVER_NO_SUPPORT、COMMON_INVALID_CHARSET、COMMON_NET_CONFIG_ERROR、
//								COMMON_URL_NOT_FOUND或COMMON_NO_NET_CONFIG_FILE调用并结束推送线程②其他错误码只调用后重试，不结束推送线程
//==================================================================
NET_HANDLE NET_API BeginMsgListPush(const char* stringSenderId, const char* stringCaseId, const char* stringCaseType, const char* stringMsgType, const char* stringMsgDirection, const char* stringVersion, 
									pHttpPushCallback pCallback, pHttpPushFailCallback pFailCallback);

//==================================================================
//函数名：EndPush
//作者：YangJiannan
//日期：2012.12.3
//功能：结束推送，可以是任何推送，只要句柄有对应
//输入参数：handle					网络对象句柄
//返回值： 正常或错误返回码
//参数和返回值的详细信息参见Net_Interface_V1.doc的4.5.21
//修改记录：
//备注：不能在BeginMsgListPush中传入的正确回调和错误回调中调用此函数，会引发死锁
//==================================================================
int NET_API EndPush(NET_HANDLE handle);


//==================================================================
//函数名：ExecuteGWService
//作者：YangJiannan
//日期：2018.3.5
//功能：请求公卫服务
//输入参数：handle					网络对象句柄
//			stringLocalFilePath		文件本地保存路径
//			stringLastPath			网络url的path部分的最后一部分，比如https://<server ip or domain>/healthApi/v1/login中的login
//			stringJson				json
//			stringUploadFilePath	要上传文件的本地路径
//返回值： 正常或错误返回码
//==================================================================
int NET_API ExecuteGWService(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringLastPath, const char* stringJson, const char* stringUploadFilePath);


//==================================================================
//函数名：ExecuteCommonRESTful
//作者：YangJiannan
//日期：2018.3.5
//功能：请求公卫服务
//输入参数：handle					网络对象句柄
//			stringLocalFilePath		文件本地保存路径
//			stringIp				目标服务器地址
//			stringPort				目标服务器端口
//			stringUrl				目标服务器url，端口后的部分，比如"http://ip:port/Collect/InitDevice"中的/Collect/InitDevice 
//			stringContentType		http请求头中的Content-Type，比如text/json
//			stringPayload			有效负载
//返回值： 正常或错误返回码
//==================================================================
int NET_API ExecuteCommonRESTful(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringIp, const char* stringPort, const char* stringUrl, const char* stringContentType, const char* stringPayload);

#ifdef __cplusplus 
	} 
#endif 

#endif