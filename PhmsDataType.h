#pragma once
typedef struct tagBLOODSUGER_DATA
{
	double bloodsuger;
	const char* flag; 
}BLOODSUGER_DATA;

typedef struct tagSPO2_DATA
{
	double spo2;
	double pr;
}SPO2_DATA;

typedef struct tagBP_DATA
{
	double sys;
	double mean;
	double dia;
}BP_DATA;

typedef struct tagFETAL_DATA
{
	double fetalHeartRate;
	double palacePressure;
	double quickening;
}FETAL_DATA;

typedef struct tagFVC_DATA
{
	double fvc;
	double fev1;
	double pef;
	double fev1Rate;
	double fef25;
	double fef2575;
	double fef75;
}FVC_DATA;

typedef struct tagPEDOMETER_DATA
{
	double setps;
	double distance;
	double calories;
}PEDOMETER_DATA;

typedef struct tagURINE
{
	const char* urineValue;
	const char* urineValueAdd;
	const char* urineValueNew;
}URINE_DATA;

typedef struct tagHR_CONCLUSION
{
	double vaule;
	const char* conclusion;
}HR_CONCLUSION;

typedef struct tagBLOOD_TEST
{
	double CHOL;
	double HDL;
	double TG;
	double LDL;
	double HB;
	double HCT;
}BLOOD_TEST, *pBLOOD_TEST;

typedef struct tagRECORD_DATA
{
	double hr;
	double weight;
	double height;
	double temp;
	double resp;
	double chol;
	double ua;
	BLOODSUGER_DATA bloodsugerData;
	SPO2_DATA spo2Data;
	BP_DATA bpData;
	FETAL_DATA fetalData;
	FVC_DATA fvcData;
	PEDOMETER_DATA pedometerData;
	const char* checkTime;
	URINE_DATA urineData;
	HR_CONCLUSION hrConclusion;
	BLOOD_TEST	bloodTest;
}RECORD_DATA;


typedef struct tagBLOODSUGER_DATA_STRING
{
	const char* bloodsuger;
	const char* flag; 
}BLOODSUGER_DATA_STRING;

typedef struct tagSPO2_DATA_STRING
{
	const char* spo2;
	const char* pr;
}SPO2_DATA_STRING;

typedef struct tagBP_DATA_STRING
{
	const char* sys;
	const char* mean;
	const char* dia;
}BP_DATA_STRING;

typedef struct tagFETAL_DATA_STRING
{
	const char* fetalHeartRate;
	const char* palacePressure;
	const char* quickening;
}FETAL_DATA_STRING;

typedef struct tagFVC_DATA_STRING
{
	const char* fvc;
	const char* fev1;
	const char* pef;
	const char* fev1Rate;
	const char* fef25;
	const char* fef2575;
	const char* fef75;
}FVC_DATA_STRING;

typedef struct tagPEDOMETER_DATA_STRING
{
	const char* setps;
	const char* distance;
	const char* calories;
}PEDOMETER_DATA_STRING;

typedef struct tagURINE_STRING
{
	const char* urineValue;
	const char* urineValueAdd;
	const char* urineValueNew;
}URINE_DATA_STRING;

typedef struct tagHR_CONCLUSION_STRING
{
	const char* vaule;
	const char* conclusion;
}HR_CONCLUSION_STRING;

typedef struct tagBLOOD_TEST_STRING
{
	const char*  CHOL;
	const char*  HDL;
	const char*  TG;
	const char*  LDL;
	const char*  HB;
	const char*  HCT;
}BLOOD_TEST_STRING, *pBLOOD_TEST_STRING;


typedef struct tagRECORD_DATA_STRING
{
	const char* hr;
	const char* weight;
	const char* height;
	const char* temp;
	const char* resp;
	const char* chol;
	const char* ua;
	const char* ag;
	BLOODSUGER_DATA_STRING bloodsugerData;
	SPO2_DATA_STRING spo2Data;
	BP_DATA_STRING bpData;
	FETAL_DATA_STRING fetalData;
	FVC_DATA_STRING fvcData;
	PEDOMETER_DATA_STRING pedometerData;
	const char* checkTime;
	URINE_DATA_STRING urineData;
	HR_CONCLUSION_STRING hrConclusion;
	BLOOD_TEST_STRING bloodTest;
}RECORD_DATA_STRING;


typedef struct tagHEALTHY_KNOWLEDGE
{
	const char* title;			//健康知识的标题
	const char* description;	//健康知识的描述
	const char* url;			//健康知识全文URI
	const char* picurl;			//健康知识的图片URI
}HEALTHY_KNOWLEDGE, *pHEALTHY_KNOWLEDGE;


typedef struct tagTELCOM_ECG
{
	const char** pArrayData;
	unsigned int nArrayLength;
	const char* adRate;
	const char* adAccuracy;
	const char* rangeMin;
	const char* rangeMax;
	const char* heartValue;
	const char* stLevel;
	const char* prInterval;
	const char* qtInterval;
	const char* rrInterval;
	const char* pWave;
	const char* rWave;
	const char* tWave;
	const char* result1;
	const char* result2;
	const char* result3;
	const char* result4;
	const char* result5;
}TELCOM_ECG, *pTELCOM_ECG;

typedef struct tagGUANXIN_DATA
{
	const char* id;
	const char* ArchiveId;
	const char* OperatorNo;
	const char* OperatorName;
	const char* MachineNo;
	const char* IdCard;
	const char* Height;
	const char* Weight;
	const char* Temperature;
	const char* Glucose;
	const char* Systolic;
	const char* Diastolic;
	const char* Oxygen;
	const char* PulseRate;
	const char* HR;
	const char* Paxis;
	const char* Taxis;
	const char* QRSaxis;
	const char* PR;
	const char* QRS;
	const char* QT;
	const char* QTc;
	const char* RV5;
	const char* SV1;
	const char* URO;
	const char* BLD;
	const char* BIL;
	const char* KET;
	const char* GLU;
	const char* PRO;
	const char* PH;
	const char* NIT;
	const char* WBC;
	const char* SG;
	const char* VC;
	const char* Diagnosis01;
	const char* ExamTime;
	const char* RTime;
	const char* Diagnosis02;
	const char* Diagnosis03;
	const char* Diagnosis04;
	const char* Diagnosis05;
	const char* Diagnosis06;
	const char* Diagnosis07;
}GUANXIN_DATA, *pGUANXIN_DATA;

typedef struct tagQUERY_RANGE
{
	const char* upperLimit;
	const char* lowerLimit;
}QUERY_RANGE, *pQUERY_RANGE;

typedef struct tagQUERY_ECG
{
	QUERY_RANGE hr;
	QUERY_RANGE pr_int;
	QUERY_RANGE qt_int;
	QUERY_RANGE qtc_int;
	QUERY_RANGE p_dur;
	QUERY_RANGE qrs_dur;
	QUERY_RANGE p_axis;
	QUERY_RANGE qrs_axis;
	QUERY_RANGE t_axis;
	QUERY_RANGE rv5;
	QUERY_RANGE sv1;

}QUERY_ECG, *pQUERY_ECG;

typedef char* NET_HANDLE;
typedef void (*pProgressAndSpeedCallback)(double dProgress, double dSpeed, const char* stringServerFilePath, const char* stringLocalFilePath);

//以下为推送成功与失败回调，※※不可以在回调函数中执行关闭推送操作，否则库报错※※
typedef void (*pHttpPushCallback)(const char* stringContent, unsigned long nContentSize);
//推送错误回调，nErrorCode取值COMMON_SESSION_INVALID:session失效，目前只有服务器重启才有可能；COMMON_LOGIN_ANOTHER_PLACE:相同账号异地登录;COMMON_SERVER_NO_SUPPORT:该用户类型不支持该种推送，咱无其他可能;COMMON_NET_CONFIG_ERROR:配置文件中文推送地址
typedef void (*pHttpPushFailCallback)(int nErrorCode/*, NET_HANDLE handle*/);

#define PC_DOCTOR_TERMINAL_TYPE		("contec-ctype1/1.0")
#define PC_USER_TERMINAL_TYPE		("contec-ctype2/1.0")
#define HM_TERMINAL_TYPE			("contec-ctype3/1.0")
#define CM_TERMINAL_TYPE			("contec-ctype4/1.0")
#define FOUR_SERIES_TERMINAL_TYPE	("contec-ctype5/1.0")
#define SEVEN_SERIES_TERMINAL_TYPE	("contec-ctype6/1.0")
#define ANDROID_TERMINAL_TYPE		("contec-ctype7/1.0")

