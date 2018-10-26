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
//��������UserAuthorize_Telecom
//���ߣ�YangJiannan
//���ڣ�2015.6.3
//���ܣ���֤���֤
//���������handle					���������		
//			stringLocalFilePath		�����ļ�·������
//			stringPersonId			���֤��
//			stringName				����
//			stringNation			����
//			stringAddress			��ַ
//			stringRoomId			����С�ݺ�
//����ֵ�� ��������󷵻���
//ʹ�÷�Χ�����Ž���С���豸�������
//�޸ļ�¼��
//==================================================================
int NET_API UserAuthorize_Telecom(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringPersonId, const char* stringName, const char* stringNation, const char* stringAddress, const char* stringRoomId);


//==================================================================
//��������BloodPressureUpload_Telecom
//���ߣ�YangJiannan
//���ڣ�2015.6.3
//���ܣ��ϴ�Ѫѹ����
//���������handle					���������		
//			stringLocalFilePath		�����ļ�·������
//			stringPersonType		�û��������
//			stringPersonId			�û����ID
//			stringSBP				����ѹ
//			stringDBP				����ѹ
//			stringAverageValue		ƽ��ѹ
//			stringPulseRate			����
//			stringDetectionTime		���ʱ��
//			stringEquCode			�豸���
//			stringRoomId			������
//����ֵ�� ��������󷵻���
//ʹ�÷�Χ�����Ž���С���豸�������
//�޸ļ�¼��
//==================================================================
int NET_API BloodPressureUpload_Telecom(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringPersonType, const char* stringPersonId, const char* stringSBP, const char* stringDBP, const char* stringAverageValue, const char* stringPulseRate, const char* stringDetectionTime, 
								const char* stringEquCode, const char* stringRoomId);


//==================================================================
//��������BloodOxygenUpload_Telecom
//���ߣ�YangJiannan
//���ڣ�2015.6.3
//���ܣ��ϴ�Ѫ������
//���������handle					���������		
//			stringLocalFilePath		�����ļ�·������
//			stringPersonType		�û��������
//			stringPersonId			���֤��
//			stringBloodOxygen		Ѫ��ֵ
//			stringPulseRate			����
//			stringDetectionTime		���ʱ��
//			stringEquCode			�豸���
//			stringRoomId			������
//����ֵ�� ��������󷵻���
//ʹ�÷�Χ�����Ž���С���豸�������
//�޸ļ�¼��
//==================================================================
int NET_API BloodOxygenUpload_Telecom(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringPersonType, const char* stringPersonId, const char* stringBloodOxygen, const char* stringPulseRate, const char* stringDetectionTime, 
							  const char* stringEquCode, const char* stringRoomId);


//==================================================================
//��������BloodSugarUpload_Telecom
//���ߣ�YangJiannan
//���ڣ�2015.6.3
//���ܣ��ϴ�Ѫ������
//���������handle					���������		
//			stringLocalFilePath		�����ļ�·������
//			stringPersonType		�û��������
//			stringPersonId			���֤��
//			stringBloodSugar		Ѫ��ֵ
//			stringDetectionTime		���ʱ��
//			stringEquCode			�豸���
//			stringRoomId			������
//����ֵ�� ��������󷵻���
//ʹ�÷�Χ�����Ž���С���豸�������
//�޸ļ�¼��
//==================================================================
int NET_API BloodSugarUpload_Telecom(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringPersonType, const char* stringPersonId, const char* stringBloodSugar, const char* stringDetectionTime, 
							 const char* stringEquCode, const char* stringRoomId);


//==================================================================
//��������HeightWeightUpload_Telecom
//���ߣ�YangJiannan
//���ڣ�2015.6.3
//���ܣ��ϴ������������
//���������handle					���������		
//			stringLocalFilePath		�����ļ�·������
//			stringPersonType		�û��������
//			stringPersonId			���֤��
//			stringHeight			���
//			stringWeight			����
//			stirngBMI				BMI
//			stringDetectionTime		���ʱ��
//			stringEquCode			�豸���
//			stringRoomId			������
//����ֵ�� ��������󷵻���
//ʹ�÷�Χ�����Ž���С���豸�������
//�޸ļ�¼��
//==================================================================
int NET_API HeightWeightUpload_Telecom(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringPersonType, const char* stringPersonId, const char* stringHeight, const char* stringWeight, const char* stringBMI, const char* stringDetectionTime, 
							   const char* stringEquCode, const char* stringRoomId);




//==================================================================
//��������HeightWeightUpload_Telecom
//���ߣ�YangJiannan
//���ڣ�2015.6.3
//���ܣ��ϴ��ι�������
//���������handle					���������		
//			stringLocalFilePath		�����ļ�·������
//			stringPersonType		�û��������
//			stringPersonId			���֤��
//			stringM_B1				M/B1
//			stringBSA				������
//			stringBSA1				BSA1
//			stringMMF				�������ж����� 
//			stringMefSeventyFive	������ 75%ʱ�λ�����Ӧ����ֵ 
//			stringMefFifty			������ 50%ʱ�λ�����Ӧ����ֵ
//			stringMefTwentyFive		������ 25%ʱ�λ�����Ӧ����ֵ
//			stringMefFVST			������ 50%25%�λ���ʱ��Ӧ��
//			stringIC				�������� 
//			stringIRV				�������� 
//			stringERV				�������� 
//			stringFevOne			1 ���ӷλ��� 
//			stringFevTwo			2 ���ӷλ��� 
//			stringFevThree			3 ���ӷλ��� 
//			stringFevOnePer			1 ���ӷλ����ٷֱ� 
//			stringFevTwoPer			2 ���ӷλ����ٷֱ� 
//			stringFevThreePer		3 ���ӷλ����ٷֱ� 
//			stringRR				����Ƶ�� 
//			stringFVC				�����λ��� 
//			stringPEF				��ֵ���� 
//			stringTV				������ 
//			stringVC				ʵ��λ��� 
//			stringMVV				ʵ�����ͨ���� 
//			stringMV				��Ϣͨ���� 
//			stringVSTHeight			V25 �����֮�� 
//			stringVSMvvBsa			ʵ�����ͨ������������֮�� 
//			stringVSMvvOne			���ͨ����/1 ���� 
//			stringDetectionTime		���ʱ��
//			stringEquCode			�豸���
//			stringRoomId			������
//����ֵ�� ��������󷵻���
//ʹ�÷�Χ�����Ž���С���豸�������
//�޸ļ�¼��
//==================================================================
int NET_API LungUpload_Telecom(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringPersonType, const char* stringPersonId, const char* stringM_B1, const char* stringBSA, const char* stringBSA1, const char* stringMMF, const char* stringMefSeventyFive, const char* stringMefFifty, 
					   const char* stringMefTwentyFive, const char* stringMefFVST, const char* stringIC, const char* stringIRV, const char* stringERV, const char* stringFevOne, const char* stringFevTwo, const char* stringFevThree, 
					   const char* stringFevOnePer, const char* stringFevTwoPer, const char* stringFevThreePer, const char* stringRR, const char* stringFVC, const char* stringPEF, const char* stringTV, const char* stringVC, const char* stringMVV, 
					   const char* stringMV, const char* stringVSTHeight, const char* stringVSMvvBsa, const char* stringVSMvvOne, const char* stringSVC, const char* stringFevOneZeroFive, const char* stringFevOneZeroFivePer, const char* stringPefOne, 
					   const char* stringPefTwo, const char* stringPefThree, const char* stringFive, const char* stringFiveOne, const char* stringFiveTwo, const char* stringFiveThree, const char* stringPIF, const char* stringMifFifty, 
					   const char* stringDetectionTime, const char* stringEquCode, const char* stringRoomId);


//==================================================================
//��������ECGUpload_Telecom
//���ߣ�YangJiannan
//���ڣ�2015.8.6
//���ܣ��ϴ��ĵ�����
//���������handle					���������		
//			stringLocalFilePath		�����ļ�·������
//			stringPersonType		�û��������
//			stringPersonId			���֤��
//			stringDetectionTime		���ʱ��
//			stringEquCode			�豸���
//			stringRoomId			������
//			ECGData					�ĵ�����
//����ֵ�� ��������󷵻���
//ʹ�÷�Χ�����Ž���С���豸�������
//�޸ļ�¼��
//==================================================================
int NET_API ECGUpload_Telecom(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringPersonType, const char* stringPersonId, const char* stringDetectionTime, 
									   const char* stringEquCode, const char* stringRoomId, TELCOM_ECG ecgData);


#ifdef __cplusplus 
} 
#endif 

#endif