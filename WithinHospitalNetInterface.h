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
//��������SetCliniqueBusy
//���ߣ�YangJiannan
//���ڣ�2014.10.30
//���ܣ������ϴ���״̬Ϊæµ  
//���������handle					���������
//			stringCliniqueId		�ϴ���id
//����ֵ�� ��������󷵻���
//ʹ�÷�Χ���նˡ��к����
//�޸ļ�¼��
//==================================================================
int NET_API SetCliniqueBusy(NET_HANDLE handle, const char* stringCliniqueId);

//==================================================================
//��������SetCliniqueFree
//���ߣ�YangJiannan
//���ڣ�2014.10.30
//���ܣ������ϴ���״̬Ϊ����
//���������handle					���������
//			stringCliniqueId		�ϴ���id
//����ֵ�� ��������󷵻���
//ʹ�÷�Χ���ն�
//�޸ļ�¼��
//==================================================================
int NET_API SetCliniqueFree(NET_HANDLE handle, const char* stringCliniqueId);

//==================================================================
//��������SetCliniqueChecking
//���ߣ�YangJiannan
//���ڣ�2014.10.30
//���ܣ������ϴ���״̬Ϊ�����  
//���������handle					���������
//			stringCliniqueId		�ϴ���id
//����ֵ�� ��������󷵻���
//ʹ�÷�Χ���ն�
//�޸ļ�¼��
//==================================================================
int NET_API SetCliniqueChecking(NET_HANDLE handle, const char* stringCliniqueId);

//==================================================================
//��������SetCliniqueShoutComplete
//���ߣ�YangJiannan
//���ڣ�2014.10.30
//���ܣ������ϴ���״̬Ϊ�к���� 
//���������handle					���������
//			stringCliniqueId		�ϴ���id
//����ֵ�� ��������󷵻���
//ʹ�÷�Χ���к����
//�޸ļ�¼��
//==================================================================
int NET_API SetCliniqueShoutComplete(NET_HANDLE handle, const char* stringCliniqueId);


//==================================================================
//��������GetPatientInfoByAppNum
//���ߣ�YangJiannan
//���ڣ�2014.10.30
//���ܣ��������뵥�Ż�ò�����Ϣ
//���������handle					���������
//			stringLocalFilePath		�����ļ�����·��
//			stringOrderType			���뵥���ͣ�1������  2��סԺ  3������  4����죩
//			stringOrderId			���뵥��(S�����뵥�� M������� Z��סԺ�� T������ K������ W��δ֪)
//			stringReqDepart			������Ҵ���
//			stringOrderState		���뵥״̬��0��ȫ��   1�������   2���Ѽ�飩
//			stringCheckType			������
//			stringStartDate			�������ڿ�ʼʱ��
//			stringEndDate			�������ڽ���ʱ��
//			stringClinicId			����id
//���������
//����ֵ�� ��������󷵻���
//ʹ�÷�Χ���к���������Ͽͻ��ˡ��豸
//�޸ļ�¼��
//==================================================================
int NET_API GetPatientInfoByAppNum(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringHospitalId, const char* stringOrderType, const char* stringOrderId, 
								   const char* stringReqDepart, const char* stringOrderState, const char* stringCheckType, const char* stringStartDate, const char* stringEndDate, const char* stringClinicId);

//==================================================================
//��������GetPatientInfoByAppNumToHis
//���ߣ�YangJiannan
//���ڣ�2017.10.20
//���ܣ��������뵥�Ż�ò�����Ϣ
//���������handle					���������
//			stringLocalFilePath		�����ļ�����·��
//			stringOrderType			���뵥���ͣ�1������  2��סԺ  3������  4����죩
//			stringOrderId			���뵥��(S�����뵥�� M������� Z��סԺ�� T������ K������ W��δ֪)
//���������
//����ֵ�� ��������󷵻���
//ʹ�÷�Χ���к���������Ͽͻ��ˡ��豸
//�޸ļ�¼��
//==================================================================
int NET_API GetPatientInfoByAppNumToHis(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringHospitalId, const char* stringOrderType, const char* stringOrderId);

//==================================================================
//��������GetPatientInfoByAppNumToPhmsAndHis
//���ߣ�YangJiannan
//���ڣ�2017.10.20
//���ܣ��������뵥�Ż�ò�����Ϣ
//���������handle					���������
//			stringLocalFilePath		�����ļ�����·��
//			stringOrderType			���뵥���ͣ�1������  2��סԺ  3������  4����죩
//			stringOrderId			���뵥��(S�����뵥�� M������� Z��סԺ�� T������ K������ W��δ֪)
//���������
//����ֵ�� ��������󷵻���
//ʹ�÷�Χ���к���������Ͽͻ��ˡ��豸
//�޸ļ�¼��
//==================================================================
int NET_API GetPatientInfoByAppNumToPhmsAndHis(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringHospitalId, const char* stringOrderType, const char* stringOrderId);

//==================================================================
//��������ApplyCheck
//���ߣ�YangJiannan
//���ڣ�2014.10.30
//���ܣ�������  
//���������handle					���������
//			stringCliniqueId		�ϴ����
//			stringOrderId			���뵥��
//			stringOrderType			���뵥���ͣ�0������  1��סԺ  �գ�ȫ����
//����ֵ�� ��������󷵻���
//ʹ�÷�Χ���к����
//�޸ļ�¼��
//==================================================================
int NET_API ApplyCheck(NET_HANDLE handle, const char* stringCliniqueId, const char* stringOrderId, const char* stringOrderType);

//==================================================================
//��������GetWaitingCheck
//���ߣ�YangJiannan
//���ڣ�2014.10.30
//���ܣ���õȴ���黼����Ϣ  
//���������handle					���������
//			stringLocalFilePath		�����ļ�����·��
//			stringCliniqueId		�ϴ����
//����ֵ�� ��������󷵻���
//ʹ�÷�Χ���ն�
//�޸ļ�¼��
//==================================================================
int NET_API GetWaitingCheck(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringCliniqueId);

//==================================================================
//��������GetCliniqueState
//���ߣ�YangJiannan
//���ڣ�2014.10.30
//���ܣ�����ϴ���״̬  
//���������handle					���������
//			stringLocalFilePath		�����ļ�����·��
//			stringCliniqueId		�ϴ����
//����ֵ�� ��������󷵻���
//ʹ�÷�Χ���к����
//�޸ļ�¼��
//==================================================================
int NET_API GetCliniqueState(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringCliniqueId);

//==================================================================
//��������NotifyCaseState
//���ߣ�YangJiannan
//���ڣ�2014.11.17
//���ܣ�֪ͨHIS����״̬�ı�  
//���������handle					���������
//			stringHospitalId		ҽԺ��
//			stringOrderType			���뵥���ͣ�0������   1��סԺ��
//			stringOrderId			���뵥��
//����ֵ�� ��������󷵻���
//ʹ�÷�Χ���ͻ���
//�޸ļ�¼��
//==================================================================
int NET_API NotifyCaseState(NET_HANDLE handle, const char* stringHospitalId, const char* stringOrderType, const char* stringOrderId);

//==================================================================
//��������NotifyReportState
//���ߣ�YangJiannan
//���ڣ�2014.11.17
//���ܣ�֪ͨHIS����״̬�ı�  
//���������handle					���������
//			stringReportId			����id
//����ֵ�� ��������󷵻���
//ʹ�÷�Χ��ר�Ҷ�
//�޸ļ�¼��
//==================================================================
int NET_API NotifyReportState(NET_HANDLE handle, const char* stringReportId);

//==================================================================
//��������SetDoctorAgentId
//���ߣ�YangJiannan
//���ڣ�2015.2.28
//���ܣ�����PHMSϵͳԺ��ҽ�����ţ�ֻ����Ժ�ڶԽ�ʱ���õ�
//���������handle					���������
//			stringDoctorAgentId		Ժ��ҽ������
//����ֵ�� ��������󷵻���
//ʹ�÷�Χ��ר�Ҷ�
//�޸ļ�¼��
//==================================================================
int NET_API SetDoctorAgentId(NET_HANDLE handle, const char* stringDoctorAgentId);

//==================================================================
//��������NotifyAppState
//���ߣ�YangJiannan
//���ڣ�2017.10.27
//���ܣ�֪ͨ���������뵥״̬���Ѽ���������
//���������handle					���������
//			stringUnitId			�ϴ����¼��
//			stringOrderId			���뵥��
//			stringOrderType			��ӦGetPatientInfoByAppNum�ӿڷ��ص�patient_source_code
//			stringState				״̬��1���Ѽ�飬 2������
//����ֵ�� ��������󷵻���
//ʹ�÷�Χ���ͻ��˻��豸�ն�
//�޸ļ�¼��
//==================================================================
int NET_API NotifyAppState(NET_HANDLE handle, const char* stringUnitId, const char* stringOrderId, const char* stringOrderType, const char* stringState);

//==================================================================
//��������VerifyAppState
//���ߣ�YangJiannan
//���ڣ�2017.10.27
//���ܣ�ͨ����������֤���뵥�Ƿ��ѱ����������Ѽ��
//���������handle					���������
//			stringLocalFilePath		�����ļ�����·��
//			stringOrderId			���뵥��
//			stringOrderType			��ӦGetPatientInfoByAppNum�ӿڷ��ص�patient_source_code
//����ֵ�� ��������󷵻���
//ʹ�÷�Χ���ͻ��˻��豸�ն�
//�޸ļ�¼��
//==================================================================
int NET_API VerifyAppState(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringOrderId, const char* stringOrderType);


//==================================================================
//��������GetHisReportState
//���ߣ�YangJiannan
//���ڣ�2016.6.15
//���ܣ�������뵥��HIS�ж�Ӧ�����״̬
//���������handle					���������
//			stringHospitalId		ҽԺID
//			stringOrderType			���뵥����
//			stringOrderId			���뵥��
//����ֵ�� ��������󷵻���
//ʹ�÷�Χ��ר�Ҷ�
//�޸ļ�¼��
//==================================================================
int NET_API GetHisReportState(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringHospitalId, const char* stringOrderType, const char* stringOrderId);


//==================================================================
//��������SetHisCriticalValue
//���ߣ�YangJiannan
//���ڣ�2016.6.15
//���ܣ����ò���Σ��ֵ
//���������handle					���������
//			stringHospitalId		ҽԺID
//			stringCaseId			������
//			stringCriticalValues	Σ��ֵ
//����ֵ�� ��������󷵻���
//ʹ�÷�Χ��ר�Ҷ�
//�޸ļ�¼��
//==================================================================
int NET_API SetHisCriticalValue(NET_HANDLE handle, const char* stringHospitalId, const char* stringCaseId, const char* stringCriticalValues);

#ifdef __cplusplus 
} 
#endif 

#endif