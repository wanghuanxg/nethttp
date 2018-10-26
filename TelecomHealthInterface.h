#ifndef PHMS_TELECOM_HEALTH_INTERFACE
#define PHMS_TELECOM_HEALTH_INTERFACE
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
//函数名：UserAuthorize_Telecom
//作者：YangJiannan
//日期：2015.6.3
//功能：验证身份证
//输入参数：handle					网络对象句柄		
//			stringLocalFilePath		本地文件路径保存
//			stringPersonId			身份证号
//			stringName				姓名
//			stringNation			民族
//			stringAddress			地址
//			stringRoomId			健康小屋号
//返回值： 正常或错误返回码
//使用范围：电信健康小屋设备集成软件
//修改记录：
//==================================================================
int NET_API UserAuthorize_Telecom(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringPersonId, const char* stringName, const char* stringNation, const char* stringAddress, const char* stringRoomId);


//==================================================================
//函数名：BloodPressureUpload_Telecom
//作者：YangJiannan
//日期：2015.6.3
//功能：上传血压数据
//输入参数：handle					网络对象句柄		
//			stringLocalFilePath		本地文件路径保存
//			stringPersonType		用户身份类型
//			stringPersonId			用户身份ID
//			stringSBP				收缩压
//			stringDBP				舒张压
//			stringAverageValue		平均压
//			stringPulseRate			脉率
//			stringDetectionTime		检查时间
//			stringEquCode			设备编号
//			stringRoomId			房间编号
//返回值： 正常或错误返回码
//使用范围：电信健康小屋设备集成软件
//修改记录：
//==================================================================
int NET_API BloodPressureUpload_Telecom(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringPersonType, const char* stringPersonId, const char* stringSBP, const char* stringDBP, const char* stringAverageValue, const char* stringPulseRate, const char* stringDetectionTime, 
								const char* stringEquCode, const char* stringRoomId);


//==================================================================
//函数名：BloodOxygenUpload_Telecom
//作者：YangJiannan
//日期：2015.6.3
//功能：上传血氧数据
//输入参数：handle					网络对象句柄		
//			stringLocalFilePath		本地文件路径保存
//			stringPersonType		用户身份类型
//			stringPersonId			身份证号
//			stringBloodOxygen		血氧值
//			stringPulseRate			脉率
//			stringDetectionTime		检查时间
//			stringEquCode			设备编号
//			stringRoomId			房间编号
//返回值： 正常或错误返回码
//使用范围：电信健康小屋设备集成软件
//修改记录：
//==================================================================
int NET_API BloodOxygenUpload_Telecom(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringPersonType, const char* stringPersonId, const char* stringBloodOxygen, const char* stringPulseRate, const char* stringDetectionTime, 
							  const char* stringEquCode, const char* stringRoomId);


//==================================================================
//函数名：BloodSugarUpload_Telecom
//作者：YangJiannan
//日期：2015.6.3
//功能：上传血糖数据
//输入参数：handle					网络对象句柄		
//			stringLocalFilePath		本地文件路径保存
//			stringPersonType		用户身份类型
//			stringPersonId			身份证号
//			stringBloodSugar		血糖值
//			stringDetectionTime		检查时间
//			stringEquCode			设备编号
//			stringRoomId			房间编号
//返回值： 正常或错误返回码
//使用范围：电信健康小屋设备集成软件
//修改记录：
//==================================================================
int NET_API BloodSugarUpload_Telecom(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringPersonType, const char* stringPersonId, const char* stringBloodSugar, const char* stringDetectionTime, 
							 const char* stringEquCode, const char* stringRoomId);


//==================================================================
//函数名：HeightWeightUpload_Telecom
//作者：YangJiannan
//日期：2015.6.3
//功能：上传身高体重数据
//输入参数：handle					网络对象句柄		
//			stringLocalFilePath		本地文件路径保存
//			stringPersonType		用户身份类型
//			stringPersonId			身份证号
//			stringHeight			身高
//			stringWeight			体重
//			stirngBMI				BMI
//			stringDetectionTime		检查时间
//			stringEquCode			设备编号
//			stringRoomId			房间编号
//返回值： 正常或错误返回码
//使用范围：电信健康小屋设备集成软件
//修改记录：
//==================================================================
int NET_API HeightWeightUpload_Telecom(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringPersonType, const char* stringPersonId, const char* stringHeight, const char* stringWeight, const char* stringBMI, const char* stringDetectionTime, 
							   const char* stringEquCode, const char* stringRoomId);




//==================================================================
//函数名：HeightWeightUpload_Telecom
//作者：YangJiannan
//日期：2015.6.3
//功能：上传肺功能数据
//输入参数：handle					网络对象句柄		
//			stringLocalFilePath		本地文件路径保存
//			stringPersonType		用户身份类型
//			stringPersonId			身份证号
//			stringM_B1				M/B1
//			stringBSA				体表面积
//			stringBSA1				BSA1
//			stringMMF				最大呼气中段流速 
//			stringMefSeventyFive	呼气至 75%时肺活量对应流速值 
//			stringMefFifty			呼气至 50%时肺活量对应流速值
//			stringMefTwentyFive		呼气至 25%时肺活量对应流速值
//			stringMefFVST			呼气至 50%25%肺活量时对应比
//			stringIC				深吸气量 
//			stringIRV				补吸气量 
//			stringERV				补呼气量 
//			stringFevOne			1 秒钟肺活量 
//			stringFevTwo			2 秒钟肺活量 
//			stringFevThree			3 秒钟肺活量 
//			stringFevOnePer			1 秒钟肺活量百分比 
//			stringFevTwoPer			2 秒钟肺活量百分比 
//			stringFevThreePer		3 秒钟肺活量百分比 
//			stringRR				呼吸频率 
//			stringFVC				用力肺活量 
//			stringPEF				峰值流量 
//			stringTV				潮气量 
//			stringVC				实测肺活量 
//			stringMVV				实测最大通气量 
//			stringMV				静息通气量 
//			stringVSTHeight			V25 与身高之比 
//			stringVSMvvBsa			实测最大通气量与体表面积之比 
//			stringVSMvvOne			最大通气量/1 秒量 
//			stringDetectionTime		检查时间
//			stringEquCode			设备编号
//			stringRoomId			房间编号
//返回值： 正常或错误返回码
//使用范围：电信健康小屋设备集成软件
//修改记录：
//==================================================================
int NET_API LungUpload_Telecom(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringPersonType, const char* stringPersonId, const char* stringM_B1, const char* stringBSA, const char* stringBSA1, const char* stringMMF, const char* stringMefSeventyFive, const char* stringMefFifty, 
					   const char* stringMefTwentyFive, const char* stringMefFVST, const char* stringIC, const char* stringIRV, const char* stringERV, const char* stringFevOne, const char* stringFevTwo, const char* stringFevThree, 
					   const char* stringFevOnePer, const char* stringFevTwoPer, const char* stringFevThreePer, const char* stringRR, const char* stringFVC, const char* stringPEF, const char* stringTV, const char* stringVC, const char* stringMVV, 
					   const char* stringMV, const char* stringVSTHeight, const char* stringVSMvvBsa, const char* stringVSMvvOne, const char* stringSVC, const char* stringFevOneZeroFive, const char* stringFevOneZeroFivePer, const char* stringPefOne, 
					   const char* stringPefTwo, const char* stringPefThree, const char* stringFive, const char* stringFiveOne, const char* stringFiveTwo, const char* stringFiveThree, const char* stringPIF, const char* stringMifFifty, 
					   const char* stringDetectionTime, const char* stringEquCode, const char* stringRoomId);


//==================================================================
//函数名：ECGUpload_Telecom
//作者：YangJiannan
//日期：2015.8.6
//功能：上传心电数据
//输入参数：handle					网络对象句柄		
//			stringLocalFilePath		本地文件路径保存
//			stringPersonType		用户身份类型
//			stringPersonId			身份证号
//			stringDetectionTime		检查时间
//			stringEquCode			设备编号
//			stringRoomId			房间编号
//			ECGData					心电数据
//返回值： 正常或错误返回码
//使用范围：电信健康小屋设备集成软件
//修改记录：
//==================================================================
int NET_API ECGUpload_Telecom(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringPersonType, const char* stringPersonId, const char* stringDetectionTime, 
									   const char* stringEquCode, const char* stringRoomId, TELCOM_ECG ecgData);


#ifdef __cplusplus 
} 
#endif 

#endif