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
//��������DoctorLogin
//���ߣ�YangJiannan
//���ڣ�2012.12.3
//���ܣ�ר�Ҷ˵�¼������ǩ���ļ� 
//���������handle					���������
//			stringLocalFilePath		ר����Ϣ�ļ����ر���·��
//			stringUsername			�û���
//			stringPassword			����		
//			stringSignPath			ǩ���ļ����ر���·��
//			stringHospitalInfoPath	ҽԺ��Ϣ·��
//			stringHospitalPhotoPath	ҽԺlogo·��
//			stringCliniqueListPath	�ϴ�����Ϣ�б�·��
//			stringDoctorListPath	Ժ��ҽ���б�·��
//���������stringIp				������ip
//����ֵ�� ��������󷵻���
//�����ͷ���ֵ����ϸ��Ϣ�μ�Net_Interface_V1.doc��4.2.1
//�޸ļ�¼��
//==================================================================
int NET_API DoctorLogin(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringUsername, const char* stringPassword, const char* stringLanguage, const char* stringSignPath, 
						const char* stringHospitalInfoPath, const char* stringHospitalPhotoPath, const char* stringCliniqueListPath, const char* stringHospitalListPath, const char* stringDoctorListPath, char* stringIp);

//==================================================================
//��������UploadSign
//���ߣ�YangJiannan
//���ڣ�2012.12.3
//���ܣ�ר�Ҷ��ϴ�ǩ���ļ� 
//���������handle					���������
//			stringSignPath			����ǩ���ļ�·��
//����ֵ�� ��������󷵻���
//�����ͷ���ֵ����ϸ��Ϣ�μ�Net_Interface_V1.doc��4.2.2
//�޸ļ�¼��
//==================================================================
int NET_API UploadSign(NET_HANDLE handle, const char* stringSignPath);

//==================================================================
//��������GetUnanalyzedCommonCaseList
//���ߣ�YangJiannan
//���ڣ�2012.3.4
//���ܣ�ר�Ҷ˻�ñ�ҽԺ����δ���������б� 
//���������handle					���������
//			stringLocalFilePath		�б��ļ����ر���·��
//			stringOwnerId			ҽԺId����ҽ��senderid
//			stringOrderBy			����(1:����  2:����  Ĭ��:����)
//			nPageTo					�ڼ�ҳ
//			nPageSize				ÿҳ��������		
//����ֵ�� ��������󷵻���
//�����ͷ���ֵ����ϸ��Ϣ�μ�Net_Interface_V1.doc��4.2.3
//�޸ļ�¼��
//==================================================================
int NET_API GetUnanalyzedCaseList(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringOwnerId, const char* stringOrderBy, unsigned int nPageTo, unsigned int nPageSize);


//==================================================================
//��������GetUnanalyzedSelfCaseList
//���ߣ�YangJiannan
//���ڣ�2013.3.4
//���ܣ�ר�Ҷ˻�ñ�ҽԺ��ռλδ���������б� 
//���������handle					���������
//			stringLocalFilePath		�б��ļ����ر���·��
//			stringReceiverId		ҽ��senderId
//			nPageTo					�ڼ�ҳ
//			nPageSize				ÿҳ��������		
//����ֵ�� ��������󷵻���
//�����ͷ���ֵ����ϸ��Ϣ�μ�Net_Interface_V1.doc��4.2.3
//�޸ļ�¼����GetUnanalyzedCommonCaseList�ӿڲ�Ϊһ���ӿ�GetUnanalyzedCaseListp------2013.3.5
//==================================================================
//int NET_API GetUnanalyzedSelfCaseList(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringReceiverId, unsigned int nPageTo, unsigned int nPageSize);



//==================================================================
//��������SearchCaseFromServer
//���ߣ�YangJiannan
//���ڣ�2012.12.3
//���ܣ�ר�Ҷ��ֶ����ұ�ҽԺ�����б� 
//���������handle					���������
//			stringLocalFilePath		�����б��ر���·��
//			stringCaseId			����Id
//			stringCaseType			��������		
//			stringCaseSource		����Դ
//			stringCaseState			����״̬
//			stringPatientName		ҽ������
//			stringSenderId			������Id
//			stringStartDate			��ʼʱ��
//			stringEndDate			����ʱ��
//			stringCheckStartDate	��鿪ʼʱ��
//			stringCheckEndDate		������ʱ��
//			stringSenderName		��������
//			stringDiagnostic		��Ͻ���
//			stringCaseOrigin		Ժ�ڲ�����Դ
//			stringYzlb				�����/סԺ��
//			stringAId				���뵥��
//			stringSex				�Ա�
//			stringOrderBy			����ʱ������
//			stringThirdId			��������
//			stringIsFullName		�����Ƿ�ȷ��ѯ��0��1�ǣ�
//			nPageTo					�ڼ�ҳ
//			nPageSize				ÿҳ����
//����ֵ�� ��������󷵻���
//�����ͷ���ֵ����ϸ��Ϣ�μ�Net_Interface_V1.doc��4.2.4
//�޸ļ�¼��
//==================================================================
int NET_API SearchCaseFromServer(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringCaseId, const char* stringCaseType, const char* stringCaseSource, 
						 const char* stringCaseState, const char* stringPatientName, const char* stringSenderId, const char* stringReceiverId, const char* stringStartDate, const char* stringEndDate, 
						 const char* stringCheckStartDate, const char* stringCheckEndDate, const char* stringSenderName, const char* stringDiagnostic, const char* stringCaseOrigin, const char* stringYzlb, const char* stringAId, 
						 const char* stringSex, const char* stringOrderBy, const char* stringThirdId, const char* stringLang, const char* stringIsFullName, unsigned int nPageTo, unsigned int nPageSize);

//==================================================================
//��������GetAutoCaseInfo
//���ߣ�YangJiannan
//���ڣ�2012.12.3 chg 2014-08-20 by wh
//���ܣ�ר�Ҷ˻�ÿ��Զ����ز�����Ϣ 
//���������handle					���������
//			stringLocalFilePath		������Ϣ�ļ����ر���·��	
//			stringCaseType			��������	
//			stringSenderId			����������id	
//			stringCaseOrigin		������Դ		
//����ֵ�� ��������󷵻���
//�����ͷ���ֵ����ϸ��Ϣ�μ�Net_Interface_V1.doc��4.2.5
//�޸ļ�¼��
//==================================================================
int NET_API GetAutoCaseInfo(NET_HANDLE handle, const char* stringLocalFilePath,const char*  stringCaseType,const char*  stringSenderId,const char*  stringCaseOrigin);

//==================================================================
//��������AutoDownloadCase
//���ߣ�YangJiannan
//���ڣ�2012.12.3
//���ܣ�ר�Ҷ��Զ����ز��� 
//���������handle					���������
//			stringLocalFilePath		�����б��ر���·��
//			stringCaseId			����Id
//			stringReceiverId		������Id		
//			stringUri				�����ļ�������·��
//			nFileSize				�����ļ���С
//			pCallback				�������ٶ�ͳ�ƻص�����
//����ֵ�� ��������󷵻���
//�����ͷ���ֵ����ϸ��Ϣ�μ�Net_Interface_V1.doc��4.2.6
//�޸ļ�¼��
//==================================================================
// int NET_API AutoDownloadCase(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringCaseId, const char* stringReceiverId, const char* stringUri, unsigned long nFileSize, 
// 					 pProgressAndSpeedCallback pCallback);

//==================================================================
//��������ManualDownloadCase
//���ߣ�YangJiannan
//���ڣ�2012.12.3
//���ܣ�ר�Ҷ��ֶ����ز��� 
//���������handle					���������
//			stringLocalFilePath		�����б��ر���·��
//			stringCaseId			����Id
//			stringReceiverId		������Id		
//			stringUri				�����ļ�������·��
//			nFileSize				�����ļ���С
//			pCallback				�������ٶ�ͳ�ƻص�����
//			nBDownloadNotSelfOwn	�Ƿ����ط��Լ���������(1:�ǣ�0������:��)
//����ֵ�� ��������󷵻���
//�����ͷ���ֵ����ϸ��Ϣ�μ�Net_Interface_V1.doc��4.2.7
//�޸ļ�¼��
//==================================================================
int NET_API ManualDownloadCase(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringCaseId, const char* stringHospitalId, const char* stringReceiverId, const char* stringUri, unsigned long nFileSize, 
					   pProgressAndSpeedCallback pCallback, int nBDownloadNotSelfOwn);

//==================================================================
//��������RollbackCase
//���ߣ�YangJiannan
//���ڣ�2012.12.3
//���ܣ�ר�Ҷ˻��˲���
//���������handle					���������
//			stringCaseId			����Id
//			stringReceiverId		������Id		
//����ֵ�� ��������󷵻���
//�����ͷ���ֵ����ϸ��Ϣ�μ�Net_Interface_V1.doc��4.2.8
//�޸ļ�¼��
//==================================================================
int NET_API RollbackCase(NET_HANDLE handle, const char* stringCaseId, const char* stringReceiverId, const char* stringBackTo);

//==================================================================
//��������TransmitCase
//���ߣ�YangJiannan
//���ڣ�2012.12.3
//���ܣ�ר�Ҷ�ת������
//���������handle					���������
//			stringCaseId			����Id
//			stringReceiverId		������Id	
//			stringForwardToH		ת��ҽԺ��
//			stringForwardToHospitalName	ת��ҽԺ����
//			stringForwardToD		ת��ҽ����
//			stringForwardToDoctorName	ת��ҽ������
//����ֵ�� ��������󷵻���
//�����ͷ���ֵ����ϸ��Ϣ�μ�Net_Interface_V1.doc��4.2.9
//�޸ļ�¼��
//==================================================================
int NET_API TransmitCase(NET_HANDLE handle, const char* stringCaseId, const char* stringReceiverId, const char* stringForwardToH, const char* stringForwardToHospitalName,
				 const char* stringForwardToD, const char* stringForwardToDoctorName);

//==================================================================
//��������InvalidCase
//���ߣ�YangJiannan
//���ڣ�2012.12.3
//���ܣ�ר�Ҷ����ϲ���
//���������handle					���������
//			stringCaseId			����Id
//			stringReceiverId		������Id	
//			stringReason			����ԭ��
//����ֵ�� ��������󷵻���
//�����ͷ���ֵ����ϸ��Ϣ�μ�Net_Interface_V1.doc��4.2.10
//�޸ļ�¼��
//==================================================================
int NET_API InvalidCase(NET_HANDLE handle, const char* stringCaseId, const char* stringReceiverId, const char* stringReason);

//==================================================================
//��������AnalyzeBegin
//���ߣ�YangJiannan
//���ڣ�2012.12.3
//���ܣ�ר�Ҷ˿�ʼ��������
//���������handle					���������
//			stringCaseId			����Id
//			stringReceiverId		������Id
//			stringCaseType			��������
//����ֵ�� ��������󷵻���
//�����ͷ���ֵ����ϸ��Ϣ�μ�Net_Interface_V1.doc��4.2.11
//�޸ļ�¼��
//==================================================================
int NET_API AnalyzeBegin(NET_HANDLE handle, const char* stringCaseId, const char* stringReceiverId, const char* stringCaseType);

//==================================================================
//��������AnalyzeComplete
//���ߣ�YangJiannan
//���ڣ�2012.12.3
//���ܣ�ר�Ҷ˽�����������
//���������handle					���������
//			stringCaseId			����Id
//			stringReceiverId		������Id
//			stringCaseType			��������
//����ֵ�� ��������󷵻���
//�����ͷ���ֵ����ϸ��Ϣ�μ�Net_Interface_V1.doc��4.2.12
//�޸ļ�¼��
//==================================================================
int NET_API AnalyzeComplete(NET_HANDLE handle, const char* stringCaseId, const char* stringReceiverId, const char* stringCaseType);

//==================================================================
//��������UploadReport
//���ߣ�YangJiannan
//���ڣ�2012.12.3
//���ܣ�ר�Ҷ��ϴ�����
//���������handle					���������
//			stringCaseId			����Id
//			stringReceiverId		������Id	
//			stringDiagnosis			��Ͻ���
//			stringDigitalSign		����ǩ��
//			stringTimestampSign		ʱ���ǩ��
//			stringCaseType			��������
//			stringLocalReportPath	���汾��·��
//			nResultFlag				����������Ա�ʶ��0�����쳣��Σ��ֵ 1�����쳣��Σ��ֵ 2�����쳣��Σ��ֵ 3�����쳣��Σ��ֵ��
//			stringFileType			�����ļ����ͣ�0��pdf+bin  1���ĵ�ͼ������+bin��
//����ֵ�� ��������󷵻���
//�����ͷ���ֵ����ϸ��Ϣ�μ�Net_Interface_V1.doc��4.2.13
//�޸ļ�¼��
//==================================================================
int NET_API UploadReport(NET_HANDLE handle, char* stringReportId, const char* stringCaseId, const char* stringReceiverId, const char* stringDiagnosis, const char* stringDigitalSign, const char* stringTimestampSign, const char* stringCaseType, const char* stringLocalReportPath, int nResultFlag, 
				 const char* stringFileType, pProgressAndSpeedCallback pCallback);

//==================================================================
//��������DoctorDownloadReport
//���ߣ�YangJiannan
//���ڣ�2012.12.3
//���ܣ�ר�Ҷ����ر���
//���������handle					���������
//			stringLocalFilePath		���汾�ر���·��
//			stringUri				���������·��
//			nFileSize				�����С
//����ֵ�� ��������󷵻���
//�����ͷ���ֵ����ϸ��Ϣ�μ�Net_Interface_V1.doc��4.2.14
//�޸ļ�¼��
//==================================================================
int NET_API DoctorDownloadReport(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringUri, unsigned long nFileSize, pProgressAndSpeedCallback pCallback);

//==================================================================
//��������UploadAdvice
//���ߣ�YangJiannan
//���ڣ�2012.12.3
//���ܣ�ר�Ҷ��ϴ��������� 
//���������handle					���������
//			stringUserId			�����û�Id
//			stringUsername			��������
//			stringMsg				��������	
//			stringType				��������
//			stringIsSendMsg			�Ƿ��Ͷ���
//����ֵ�� ��������󷵻���
//�����ͷ���ֵ����ϸ��Ϣ�μ�Net_Interface_V1.doc��4.2.15
//�޸ļ�¼��
//==================================================================
int NET_API UploadAdvice(NET_HANDLE handle, const char* stringUserId, const char* stringUsername, const char* stringMsg, const char* stringType, const char* stringIsSendMsg);

//==================================================================
//��������DoctorChangeInfo
//���ߣ�YangJiannan
//���ڣ�2012.12.3
//���ܣ�ר�Ҷ��ϴ��������� 
//���������handle					���������
//			stringName				����
//			stringSex				�Ա�
//			stringTel				�绰����	
//			stringBirthyead			�������
//			stringPosition			ְ��
//			stringNotes				��������
//����ֵ�� ��������󷵻���
//�����ͷ���ֵ����ϸ��Ϣ�μ�Net_Interface_V1.doc��4.2.16
//�޸ļ�¼��
//==================================================================
int NET_API DoctorChangeInfo(NET_HANDLE handle, const char* stringName, const char* stringSex, const char* stringTel, const char* stringBirthyead, const char* stringPosition, const char* stringNotes);

//==================================================================
//��������GetDoctorInfo
//���ߣ�YangJiannan
//���ڣ�2013.1.4
//���ܣ�ר�һ�ø�����Ϣ 
//���������handle					���������
//			stringLocalFilePath		���ر����ļ�·��
//����ֵ�� ��������󷵻���
//�����ͷ���ֵ����ϸ��Ϣ�μ�Net_Interface_V1.doc��4.2.17
//�޸ļ�¼��2013.5.7,���Ӳ���stringSenderId����������ַ���ʾ������Ϣ�������ʾ������Ϣ��senderid��
//==================================================================
int NET_API DoctorGetSelfInfo(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringSenderId);

//==================================================================
//��������UploadSign
//���ߣ�YangJiannan
//���ڣ�2013.1.4
//���ܣ�ҽ���ϴ�������Ƭ 
//���������handle					���������
//			stringPhotoPath			��Ƭ�ļ����ر���·��
//����ֵ�� ��������󷵻���
//�����ͷ���ֵ����ϸ��Ϣ�μ�Net_Interface_V1.doc��4.2.18
//�޸ļ�¼��
//==================================================================
int NET_API UploadPhoto(NET_HANDLE handle, const char* stringPhotoPath);

//==================================================================
//��������GetReviewReportList
//���ߣ�YangJiannan
//���ڣ�2013.1.4
//���ܣ������˱����б� 
//���������handle					���������
//			stringLocalFilePath		���ر����ļ�·��
//			stringReportStartDate	���濪ʼʱ��
//			stringReportEndDate		�������ʱ��
//			stringReviewStartDate	��˿�ʼʱ��
//			stringReviewEndDate		��˽���ʱ��
//			stringReviewState		���״̬
//			stringCaseType			��������
//			stringPtName			��������
//			stringSenderId			�����ϴ���
//			stringYzlb				����/סԺ��
//			stringAId				���뵥��
//			stringReviewerId		�����senderid
//			stringReceiverId		������senderid
//			stringCaseId			������
//			nPageTo					�ڼ�ҳ
//			nPageSize				ÿҳ����
//����ֵ�� ��������󷵻���
//�����ͷ���ֵ����ϸ��Ϣ�μ�Net_Interface_V1.doc��4.2.19
//�޸ļ�¼��
//==================================================================
int NET_API GetReviewReportList(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringReportStartDate, const char* stringReportEndDate,const char* stringReviewStartDate,
						const char* stringReviewEndDate, const char* stringReviewState, const char* stringCaseType, const char* stringPtName, const char* stringSenderId, const char* stringYzlb, const char* stringAId, 
						const char* stringReviewerId, const char* stringReceiverId, const char* stringCaseId, unsigned int nPageTo, unsigned int nPageSize);

//==================================================================
//��������ReviewReport
//���ߣ�YangJiannan
//���ڣ�2013.1.4
//���ܣ���˱��� 
//���������handle					���������
//			stringReportKeyId		��������
//			stringReviewState		���״̬
//			stringPath				��˺�ı��������·��
//			stringDiagnostic		��Ͻ���
//			stringDigitalSign		����ǩ��
//			stringTimeStampSign		ʱ���ǩ��
//�����ͷ���ֵ����ϸ��Ϣ�μ�Net_Interface_V1.doc��4.2.20
//�޸ļ�¼��
//==================================================================
int NET_API ReviewReport(NET_HANDLE handle, const char* stringReportKeyId, const char* stringReviewState, const char* stringPath, const char* stringDiagnostic, 
						 const char* stringDigitalSign, const char* stringTimeStampSign);

//==================================================================
//��������GetCardUserList
//���ߣ�YangJiannan
//���ڣ�2013.1.4
//���ܣ���ø����û��б� 
//���������handle					���������
//			stringLocalFilePath		�����ļ�����·��
//			stringCardId			����id
//			stringSenderId			��¼ʱ���ص��û�ϵͳ�ڲ�Ψһ��
//			stringPersonId			�����û�����
//			stringTel				�绰
//			stringName				�����û�����
//			stringType				�����û�����
//			nPageTo					�ڼ�ҳ
//			nPageSize				ÿҳ����
//�����ͷ���ֵ����ϸ��Ϣ�μ�Net_Interface_V1.doc��4.2.21
//�޸ļ�¼��
//==================================================================
int NET_API GetCardUserList(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringCardId, const char* stringSenderId, const char* stringPersonId, const char* stringTel, 
					 const char* stringName, const char* stringType, unsigned int nPageTo, unsigned int nPageSize);

//==================================================================
//��������MarkCaseException
//���ߣ�YangJiannan
//���ڣ�2013.1.4
//���ܣ���ǲ���״̬Ϊ�쳣 
//���������handle					���������
//			stringCardId			����id
//			stringReceiverId		������id
//�����ͷ���ֵ����ϸ��Ϣ�μ�Net_Interface_V1.doc��4.2.22
//�޸ļ�¼��
//==================================================================
int NET_API MarkCaseException(NET_HANDLE handle, const char* stringCaseId, const char* stringReceiverId, int nResultFlag);

//==================================================================
//��������GetAllTeamList
//���ߣ�YangJiannan
//���ڣ�2013.1.4
//���ܣ��������ר���Ŷ��б� 
//���������handle					���������
//			stringLocalFilePath		�����ļ�����·��
//�����ͷ���ֵ����ϸ��Ϣ�μ�Net_Interface_V1.doc��4.2.23
//�޸ļ�¼��
//==================================================================
int NET_API GetAllTeamList(NET_HANDLE handle, const char* stringLocalFilePath);

//==================================================================
//��������GetDoctorListOfTeam
//���ߣ�YangJiannan
//���ڣ�2013.1.4
//���ܣ����ר���Ŷ���ҽ���б� 
//���������handle					���������
//			stringLocalFilePath		�����ļ�����·��
//			stringTeamId			ר���Ŷ�id
//�����ͷ���ֵ����ϸ��Ϣ�μ�Net_Interface_V1.doc��4.2.24
//�޸ļ�¼��
//==================================================================
int NET_API GetDoctorListOfTeam(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringTeamId);


//==================================================================
//��������DoctorHandledCaseStatistics
//���ߣ�YangJiannan
//���ڣ�2013.4.16
//���ܣ�ҽ������Ѵ�����ͳ����Ϣ
//���������handle					���������
//			stringLocalFilePath		�����ļ�����·��
//			stringCaseType			�������ͺ�
//			stringStartTime			���洦��ʼʱ��
//			stringEndTime			���洦�����ʱ��
//�����ͷ���ֵ����ϸ��Ϣ�μ�Net_Interface_V1.doc��4.2.25
//�޸ļ�¼��
//==================================================================
int NET_API DoctorHandledCaseStatistics(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringCaseType, const char* stringStartTime, const char* stringEndTime);

//==================================================================
//��������DoctorDownloadReport
//���ߣ�YangJiannan
//���ڣ�2013.9.30
//���ܣ�ר�Ҷ����ر���
//���������handle					���������
//			stringLocalCasePath		�������ر���·��
//			stringLocalReportPath	���汾�ر���·��
//			stringCaseId			������
//			stringReportUri			���������·��
//			nReportSize				�����С
//			pCallback				���ؽ����ٶȻص�
//����ֵ�� ��������󷵻���
//�����ͷ���ֵ����ϸ��Ϣ�μ�Net_Interface_V1.doc��4.2.26
//�޸ļ�¼��
//==================================================================
int NET_API DoctorDownloadCaseAndOneReportOfOldCm(NET_HANDLE handle, const char* stringLocalCasePath, const char* stringLocalReportPath, const char* stringCaseId, const char* stringReportUri, unsigned long nReportSize, pProgressAndSpeedCallback pCallback);

//==================================================================
//��������GetHealthDegree
//���ߣ�YangJiannan
//���ڣ�2013.11.11
//���ܣ�����û�������
//���������handle					���������
//			stringLocalFilePath		���ر���·��
//			stringUId				���û���¼��
//			stringInterUId			�ڲ�senderid
//			stringTrendType			������������
//			stringStartDate			��ʼʱ��
//			stringEndDate			����ʱ��
//			stringNextId			nextid
//����ֵ�� ��������󷵻���
//�����ͷ���ֵ����ϸ��Ϣ�μ�Net_Interface_V1.doc��4.2.27
//�޸ļ�¼��
//==================================================================
int NET_API GetTrendData(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringUId, const char* stringInterUId, const char* stringTrendType, const char* stringStartDate, const char* stringEndDate, const char* stringNextId);


//==================================================================
//��������GetHealthDegree
//���ߣ�YangJiannan
//���ڣ�2013.11.11
//���ܣ�����û�������
//���������handle					���������
//			stringLocalFilePath		���ر���·��
//			stringUId				���û���¼��
//			stringInterUId			�ڲ�senderid
//			stringDate				�·�yyyy-mm
//����ֵ�� ��������󷵻���
//�����ͷ���ֵ����ϸ��Ϣ�μ�Net_Interface_V1.doc��4.2.27
//�޸ļ�¼��
//==================================================================
int NET_API GetHealthDegree(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringUId, const char* stringInterUId, const char* stringDate);

//==================================================================
//��������GetUserOfNoAdviceAndMostData
//���ߣ�YangJiannan
//���ڣ�2013.11.11
//���ܣ���ȡ��ĿǰΪֹ����x����δ���߽��������Ҵ��ϴγ��߽������鵽�����ϴ�����������������
//���������handle					���������
//			stringLocalFilePath		���ر���·��
//			stringDays				����
//����ֵ�� ��������󷵻���
//�����ͷ���ֵ����ϸ��Ϣ�μ�Net_Interface_V1.doc��4.2.28
//�޸ļ�¼��
//==================================================================
int NET_API GetUserOfNoAdviceAndMostData(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringDays);

//==================================================================
//��������AdviceDelay
//���ߣ�YangJiannan
//���ڣ�2013.11.11
//���ܣ��������鴦����ʱ
//���������handle					���������
//			stringUId				���û���¼�Ż����ڲ�senderid
//			stringMinutes			������
//����ֵ�� ��������󷵻���
//�����ͷ���ֵ����ϸ��Ϣ�μ�Net_Interface_V1.doc��4.2.29
//�޸ļ�¼��
//==================================================================
int NET_API AdviceDelay(NET_HANDLE handle, const char* stringUId, const char* stringMinutes);

//==================================================================
//��������AdviceComplete
//���ߣ�YangJiannan
//���ڣ�2013.11.11
//���ܣ��������鴦����ʱ
//���������handle					���������
//			stringUId				���û���¼�Ż����ڲ�senderid
//			stringOptionType		��������
//			stringAdvice			��������
//����ֵ�� ��������󷵻���
//�����ͷ���ֵ����ϸ��Ϣ�μ�Net_Interface_V1.doc��4.2.30
//�޸ļ�¼��
//==================================================================
int NET_API AdviceComplete(NET_HANDLE handle, const char* stringUId, const char* stringOptionType, const char* stringAdvice);

//==================================================================
//��������GetUserListOfNoAdvice
//���ߣ�YangJiannan
//���ڣ�2013.11.11
//���ܣ���ȡ��ĿǰΪֹ����x����δ���߽��������û��б�
//���������handle					���������
//			stringLocalFilePath		���ر���·��
//			stringDays				����
//			nPageTo					�ڼ�ҳ
//			nPageSize				ÿҳ��¼��
//����ֵ�� ��������󷵻���
//�����ͷ���ֵ����ϸ��Ϣ�μ�Net_Interface_V1.doc��4.2.31
//�޸ļ�¼��
//==================================================================
int NET_API GetUserListOfNoAdvice(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringDays, unsigned int stringPageTo, unsigned int stringPageSize);

//==================================================================
//��������AdviceBegin
//���ߣ�YangJiannan
//���ڣ�2013.11.11
//���ܣ���ʼ����������
//���������handle					���������
//			stringUId				���û���¼�Ż����ڲ�senderid
//����ֵ�� ��������󷵻���
//�����ͷ���ֵ����ϸ��Ϣ�μ�Net_Interface_V1.doc��4.2.32
//�޸ļ�¼��
//==================================================================
int NET_API AdviceBegin(NET_HANDLE handle, const char* stringUId);

//==================================================================
//��������GetUserListOfAdvicing
//���ߣ�YangJiannan
//���ڣ�2013.11.11
//���ܣ���ȡ�����������ڴ����е��û��б�
//���������handle					���������
//			stringLocalFilePath		���ر���·��
//			nPageTo					�ڼ�ҳ
//			nPageSize				ÿҳ��¼��
//����ֵ�� ��������󷵻���
//�����ͷ���ֵ����ϸ��Ϣ�μ�Net_Interface_V1.doc��4.2.33
//�޸ļ�¼��
//==================================================================
int NET_API GetUserListOfAdvicing(NET_HANDLE handle, const char* stringLocalFilePath, unsigned int nPageTo, unsigned int nPageSize);

//==================================================================
//��������GetAskList
//���ߣ�YangJiannan
//���ڣ�2013.10.23
//���ܣ������ѯ�б�
//���������handle					���������
//			stringLocalFilePath		�ļ����ر���·��
//			stringAskType			��ѯ����
//			stringAskEnd			�Ƿ���ѯ�ѹر�
//			stringDoctorId			ҽ��id
//			stringIsAsk				�Ƿ�δ�ش�
//			stringUserId			���ӿڷ��ص�userid�ֶΣ����ڲ���ĳ�˵���ʷ��ѯ
//			stringStartTime			��ʼʱ��
//			stringEndTime			����ʱ��
//			nPageTo					�ڼ�ҳ
//			nPageSize				ÿҳ��¼��
//����ֵ�� ��������󷵻���
//�����ͷ���ֵ����ϸ��Ϣ�μ�Net_Interface_V1.doc��4.2.34
//�޸ļ�¼��
//==================================================================
int NET_API GetAskList(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringAskType, const char* stringAskEnd, const char* stringDoctorId, const char* stringIsAsk, const char* stringUserId, const char* stringStartTime, const char* stringEndTime, unsigned int nPageTo, unsigned int nPageSize);

//==================================================================
//��������GetAskDetail
//���ߣ�YangJiannan
//���ڣ�2013.10.23
//���ܣ����׷�ʺͻش��б�
//���������handle					���������
//			stringLocalFilePath		�����ļ�·��
//			stringAskId				��ѯid
//����ֵ�� ��������󷵻���
//�����ͷ���ֵ����ϸ��Ϣ�μ�Net_Interface_V1.doc��4.2.35
//�޸ļ�¼��
//==================================================================
int NET_API GetAskDetail(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringAskId);

//==================================================================
//��������SubmitQuestion
//���ߣ�YangJiannan
//���ڣ�2013.10.23
//���ܣ��ύ�ش�
//���������handle					���������
//			stringLocalFilePath		�ļ����ر���·��
//			stringUserId			�û�id
//			stringAdvise			�ش�
//			stringReportType		�ش�����
//			stringAskId				��ѯid
//			stringAskedId			׷��id
//			stringDoctorPhotoPath	ҽ����Ƭ������·��
//����ֵ�� ��������󷵻���
//�����ͷ���ֵ����ϸ��Ϣ�μ�Net_Interface_V1.doc��4.2.36
//�޸ļ�¼��
//==================================================================
int NET_API SubmitQuestion(NET_HANDLE handle, const char* stringUserId, const char* stringAdvise, const char* stringReportType, const char* stringAskId, const char* stringAskedId, const char* stringDoctorPhotoPath);

//==================================================================
//��������GetCliniqueListOfHospital
//���ߣ�YangJiannan
//���ڣ�2014.1.4
//���ܣ����Ժ�ڻ����б�
//���������handle					���������
//			stringLocalFilePath		�ļ����ر���·��
//			stringHospitalId		ҽԺid
//			nPageTo					�ڼ�ҳ
//			nPageSize				ÿҳ������
//����ֵ�� ��������󷵻���
//�����ͷ���ֵ����ϸ��Ϣ�μ�Net_Interface_V1.doc��4.2.37
//�޸ļ�¼��
//==================================================================
int NET_API GetCliniqueListOfHospital(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringHospitalId);

//==================================================================
//��������GetCliniqueListOfHospital
//���ߣ�YangJiannan
//���ڣ�2014.1.4
//���ܣ����ͳ����Ϣ
//���������handle					���������
//			stringLocalFilePath		�ļ����ر���·��
//			stringTypeId			ͳ������id
//			stringMonth				ͳ���·�
//			stringMonthDay			ͳ���·ݣ���ȷ���죬��Ϊ����ʼ�죬���ڷ���Ȼ�²�ѯ��
//����ֵ�� ��������󷵻���
//�����ͷ���ֵ����ϸ��Ϣ�μ�Net_Interface_V1.doc��4.2.39
//�޸ļ�¼��
//==================================================================
int NET_API GetStatisticsInfo(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringTypeId, const char* stringMonth, const char* stringMonthDay, unsigned int nPageTo, unsigned int nPageSize);

//==================================================================
//��������GetUserHealthList
//���ߣ�YangJiannan
//���ڣ�2014.3.14 chg 2014-08-20 by wh
//���ܣ�����û������б�
//���������handle					���������
//			stringLocalFilePath		�ļ����ر���·��
//			stringNextId			��ȡ�б��λ��
//			stringTypeId			һ��ɸѡ���
//			stringSecondTypeId		����ɸѡ���
//			stringName				����
//			stringSex				�Ա�
//			stringThirdIdType		ʶ������
//			stringThird				ʶ���
//			stringHospitalId		ҽԺId��Ĭ��Ϊҽ������ҽԺ��
//			stringUnitSenderId		�ϴ���sender������ö��Ÿ���
//			stringIsData			���������ݣ�1���� 2���ޣ�
//			stringIntervalTime		�������
//			stringPopulations		��Ⱥ����
//			stringContractDoctor	ǩԼҽ��
//			stringIsContract		�Ƿ�ǩԼ
//			stringPageSize			һ�η�����������
//����ֵ�� ��������󷵻���
//�޸ļ�¼��
//==================================================================
int NET_API GetUserHealthList(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringNextId, const char* stringTypeId, const char* stringSecondTypeId, const char* stringName, const char* stringSex, const char* stringThirdIdType, 
					  const char* stringThirdId, const char* stringHospitalId, const char* stringUnitSenderId, const char* stringIsData, const char* stringIntervalTime, const char* stringPopulations, const char* stringContractDoctor, const char* stringIsContract, const char* stringPageSize);
//==================================================================
//��������GetAdviceImplemention
//���ߣ�YangJiannan
//���ڣ�2014.3.14
//���ܣ���ý���ִ�ж�
//���������handle					���������
//			stringThirdId			����/�ֻ���/���֤��
//			stringHospitalId		ҽԺId��Ĭ��Ϊҽ������ҽԺ��
//���������stringPercent			����ִ�ж�
//����ֵ�� ��������󷵻���
//�޸ļ�¼��
//==================================================================
int NET_API GetAdviceImplemention(NET_HANDLE handle, char* stringPercent, const char* stringThirdId, const char* stringHospitalId);

//==================================================================
//��������ChangePersonType
//���ߣ�YangJiannan
//���ڣ�2014.3.14
//���ܣ������û�����
//���������handle					���������
//			stringThirdId			����/�ֻ���/���֤��
//			stringPeopleType		����
//			stringHospitalId		ҽԺId��Ĭ��Ϊҽ������ҽԺ��
//����ֵ�� ��������󷵻���
//�޸ļ�¼��
//==================================================================
int NET_API ChangePersonType(NET_HANDLE handle, const char* stringThirdId, const char* stringPeopleType, const char* stringHospitalId);

//==================================================================
//��������SetNextVisitTime
//���ߣ�YangJiannan
//���ڣ�2014.3.14
//���ܣ������û�����
//���������handle					���������
//			stringThirdId			����/�ֻ���/���֤��
//			stringInterViewTime		�´η�������
//			stringHospitalId		ҽԺId��Ĭ��Ϊҽ������ҽԺ��
//����ֵ�� ��������󷵻���
//�޸ļ�¼��
//==================================================================
int NET_API SetNextVisitTime(NET_HANDLE handle, const char* stringThirdId, const char* stringInterViewTime, const char* stringHospitalId);

//==================================================================
//��������GetPersonTypeList
//���ߣ�YangJiannan
//���ڣ�2014.3.14
//���ܣ������û�����
//���������handle					���������
//			stringLocalFilePath		���ر���·��
//			stringLanguage			����
//����ֵ�� ��������󷵻���
//�޸ļ�¼��
//==================================================================
int NET_API GetPersonTypeList(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringLanguage);



//==================================================================
//��������PublishHealthyKnowledge
//���ߣ�YangJiannan
//���ڣ�2015.7.24
//���ܣ���������֪ʶ
//���������handle					���������
//			pHealthKnowledge		����֪ʶ����
//			nArrayLen				�����С
//����ֵ�� ��������󷵻���
//�޸ļ�¼��
//==================================================================
int NET_API PublishHealthyKnowledge(NET_HANDLE handle, pHEALTHY_KNOWLEDGE pHealthKnowledge, unsigned int nArrayLen);


//==================================================================
//��������GetTrendRangeValue
//���ߣ�YangJiannan
//���ڣ�2015.12.9
//���ܣ������������ֵ
//���������handle					���������
//			stringLocalFilePath		�����ļ�����·��
//����ֵ�� ��������󷵻���
//�޸ļ�¼��
//==================================================================
int NET_API GetTrendRangeValue(NET_HANDLE handle, const char* stringLocalFilePath);

//==================================================================
//��������GetCliniqueReportList
//���ߣ�YangJiannan
//���ڣ�2016.8.10
//���ܣ����һ�����������ı����б�
//���������handle					���������
//			stringLocalFilePath		�����ļ�����·��
//			stringCliniqueSenderId	����SenderId��������ö��Ÿ���
//			stringStartDate			��ѯ��ʼ���ڣ������䣩
//			stringEndDate			��ѯ�������ڣ������䣩
//			stringState				����״̬��0��1��2��
//			stringName				��������
//			stringPatientSourceCode	������Դ���루��Ҫӳ�䵽HIS�ķ�����Ϣ��
//			stringYzlb				סԺ��/�����
//			stringPIdType			�������ͣ�1 Ժ�ڻ��߿��ţ�2 Ժ�ڻ���������/��ά�룻3 Ժ�ڻ������֤�ţ�4 Ժ�ڻ���ҽ���ţ�5 Ժ�ڻ����ֻ��ţ�
//			stringPId				���ߺ�
//			stringHb				����
//			stringASection			�������id
//			stringNextId			�´���ȡλ��
//			stringPageSize			ÿ����ȡ����
//����ֵ�� ��������󷵻���
//�޸ļ�¼��
//==================================================================
int NET_API GetCliniqueReportList(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringCliniqueSenderId, const char* stringStartDate, 
								  const char* stringEndDate, const char* stringState, const char* stringName, const char* stringPatientSourceCode, 
								  const char* stringYzlb, const char* stringPIdType, const char* stringPId, const char* stringHb, const char* stringASectionId, 
								  const char* stringNextId, const char* stringPageSize);

//==================================================================
//��������GetUserListBindingOrNot
//���ߣ�YangJiannan
//���ڣ�2017.4.13
//���ܣ������Ƿ��뵱ǰҽ�����������û�
//���������handle					���������
//			stringLocalFilePath		�����ļ�����·��
//			stringNextId			��ȡ�б��λ��
//			stringPageSize			ҳ��С
//			stringIsBind			�Ƿ�󶨣�0��δ�� 1���� �գ�����Ϊ��ѯ������
//			stringIsActive			��Ծ������1����Ծ�û�����  2������Ծ�û�����  �գ�����Ϊ��ѯ������
//����ֵ�� ��������󷵻���
//�޸ļ�¼��
//==================================================================
int NET_API GetUserListBindingOrNot(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringNextId, const char* stringPageSize, const char* stringIsBind, const char* stringIsActive);

//==================================================================
//��������BindUser
//���ߣ�YangJiannan
//���ڣ�2017.4.13
//���ܣ�ҽ����ĳ�û�
//���������handle					���������
//			stringUId				������id
//			stringUName				����������
//����ֵ�� ��������󷵻���
//�޸ļ�¼��
//==================================================================
int NET_API BindUser(NET_HANDLE handle, const char* stringUId, const char* stringUName);

//==================================================================
//��������UnbindUser
//���ߣ�YangJiannan
//���ڣ�2017.4.13
//���ܣ�ҽ�����ĳ�û�
//���������handle					���������
//			stringUId				������id
//����ֵ�� ��������󷵻���
//�޸ļ�¼��
//==================================================================
int NET_API UnbindUser(NET_HANDLE handle, const char* stringUId);

//==================================================================
//��������SearchCaseAdvanced
//���ߣ�YangJiannan
//���ڣ�2018.1.4
//���ܣ��߼���������
//���������handle					���������
//			stringLocalFilePath		�����ļ�����·��
//			stringHospitalId		ҽԺID
//			stringDiagnostic		��Ͻ���
//			stringName				����
//			stringSex				�Ա�
//			rangeAge				��������
//			rangeCreateTime			����ʱ������
//			stringMode				ģʽ and���ִ���ģʽ��or: �ִʻ�ģʽ
//			stringCaseType			��������
//			ecg						�ĵ�ͼ�������������䣬Ŀǰ�ýӿڽ�֧��9���Ͳ������ոò������ң��������;��㴫������Ҳ�ᱻ���ԡ�
//									�ӿ����ƽǶ�Ӧ�����ɲ��������������ȫ�����룬�����������Լ����Ժ�ͬ�������Ͳ�ͬ�����������
//									���Ǵ�ʵ�ýǶȣ����������QUERY_ECG�ṹ�塣��Ϊδ���������Ͳ�������������Ŀ����Բ���
//			stringFrom				��ʼλ�ã�Ĭ��0����ҳ��Ҫ����size�뷵�ص�total�ֶ�������
//			stringSize				ÿ����෵�صļ�¼��
//����ֵ�� ��������󷵻���
//�޸ļ�¼��
//==================================================================
int NET_API SearchCaseAdvanced(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringHospitalId, const char* stringDiagnostic, 
							   const char* stringName, const char* stringSex, QUERY_RANGE rangeAge, QUERY_RANGE rangeCreateTime, const char* stringMode, 
							   const char* stringCaseType, QUERY_ECG ecg, const char* stringFrom, const char* stringSize);

//rangeAge��rangeCreateTime��ecg��������ֱ�Ӵ�json��
//{age:{gte:���ֵ, lte:��Сֵ}}
//{createtime:{gte:���ֵ, lte:��Сֵ}}
//{HR:{gte:���ֵ, lte:��Сֵ}, PR_Int:{gte:���ֵ, lte:��Сֵ}, ......}
int NET_API SearchCaseAdvancedString(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringHospitalId, const char* stringDiagnostic, 
							   const char* stringName, const char* stringSex, const char* stringJsonAge, const char* stringJsonCreateTime, const char* stringMode, 
							   const char* stringCaseType, const char* stringJsonEcg, const char* stringFrom, const char* stringSize);

//==================================================================
//��������SignDoctor
//���ߣ�YangJiannan
//���ڣ�2018.3.9
//���ܣ�ҽ��ǩԼĳ�û�
//���������handle					���������
//			stringUId				������id
//����ֵ�� ��������󷵻���
//�޸ļ�¼��
//==================================================================
int NET_API SignDoctor(NET_HANDLE handle, const char* stringUId);


//==================================================================
//��������HoldPlaceReview
//���ߣ�YangJiannan
//���ڣ�2018.6.26
//���ܣ���˱���ռλ
//���������handle					���������
//			stringReportId			����id
//����ֵ�� ��������󷵻���
//�޸ļ�¼��
//==================================================================
int NET_API HoldPlaceReview(NET_HANDLE handle, const char* stringReportId);

//==================================================================
//��������RollBackReview
//���ߣ�YangJiannan
//���ڣ�2018.6.26
//���ܣ���˱���ռλ��ع�
//���������handle					���������
//			stringReportId			����id
//����ֵ�� ��������󷵻���
//�޸ļ�¼��
//==================================================================
int NET_API RollBackReview(NET_HANDLE handle, const char* stringReportId);

//==================================================================
//��������ResetReviewHoldPlace
//���ߣ�YangJiannan
//���ڣ�2018.6.26
//���ܣ����ñ���ռλ��
//���������handle					���������
//			stringReportId			����id
//			stringOccupantId		�±���ռλ��id
//			stringOccupantName		�±���ռλ������
//			stringSuperKey			��������
//����ֵ�� ��������󷵻���
//�޸ļ�¼��
//==================================================================
int NET_API ResetReviewHoldPlace(NET_HANDLE handle, const char* stringReportId, const char* stringOccupantId, const char* stringOccupantName, const char* stringSuperKey);

//==================================================================
//��������ResetCaseHoldPlace
//���ߣ�YangJiannan
//���ڣ�2018.6.26
//���ܣ����ñ���ռλ��
//���������handle					���������
//			stringCaseId			����id
//			stringReceiverId		������ǰռλ��id
//			stringOccupantId		�±���ռλ��id
//			stringOccupantName		�±���ռλ������
//			stringSuperKey			��������
//����ֵ�� ��������󷵻���
//�޸ļ�¼��
//==================================================================
int NET_API ResetCaseHoldPlace(NET_HANDLE handle, const char* stringCaseId, const char* stringReceiverId, const char* stringOccupantId, const char* stringOccupantName, const char* stringSuperKey);


//==================================================================
//��������OnlyDownloadCase
//���ߣ�YangJiannan
//���ڣ�2018.6.25
//���ܣ����ز�������ռλ��������ʼ���غ�������ɽӿڲ�Ӱ�����շ��ؽ�� 
//���������handle					���������
//			stringLocalFilePath		�����б��ر���·��
//			stringCaseId			����Id
//			stringReceiverId		������Id		
//			stringUri				�����ļ�������·��
//			nFileSize				�����ļ���С
//			pCallback				�������ٶ�ͳ�ƻص�����
//����ֵ�� ��������󷵻���
//�޸ļ�¼��
//==================================================================
int NET_API OnlyDownloadCase(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringCaseId, const char* stringReceiverId, const char* stringUri, unsigned long nFileSize, pProgressAndSpeedCallback pCallback);

//==================================================================
//��������HoldPlaceAndAnalyzeBegin
//���ߣ�YangJiannan
//���ڣ�2018.6.25
//���ܣ�ռλ����ʼ������ֻ��ռλ�ɹ�������½��� 
//���������handle					���������
//			stringCaseId			����Id
//			stringHospital			ҽԺId
//			stringReceiverId		ռλ��senderid
//			stringCaseType			�������ͺ�
//����ֵ�� ��������󷵻���
//�޸ļ�¼��
//==================================================================
int NET_API HoldPlaceAndAnalyzeBegin(NET_HANDLE handle, const char* stringCaseId, const char* stringHospitalId, const char* stringReceiverId, const char* stringCaseType);

//==================================================================
//��������ReviewReportWithFile
//���ߣ�YangJiannan
//���ڣ�2018.7.4
//���ܣ���˱���(�����ļ�) 
//���������handle					���������
//			stringCaseId			������
//			stringReceiverId		ռλ��senderid
//			stringReportKeyId		��������
//			stringReviewState		���״̬
//			stringFileLoalPath		�����ļ�����·��
//			stringDiagnostic		��Ͻ���
//			stringDigitalSign		����ǩ��
//			stringTimeStampSign		ʱ���ǩ��
//			stringFileType			�����ļ����ͣ�0����ͨ���棻1�����bin�ı��棩
//			pCallback				���Ȼص�
//�����ͷ���ֵ����ϸ��Ϣ�μ�Net_Interface_V1.doc��4.2.20
//�޸ļ�¼��
//==================================================================
int NET_API ReviewReportWithFile(NET_HANDLE handle, const char* stringCaseId, const char* stringReceiverId, const char* stringReportKeyId, const char* stringReviewState, const char* stringFileLoalPath, const char* stringDiagnostic, 
								const char* stringDigitalSign, const char* stringTimeStampSign, const char* stringResultFlag, const char* stringFileType, pProgressAndSpeedCallback pCallback);

//==================================================================
//��������AddTag
//���ߣ�YangJiannan
//���ڣ�2018.7.31
//���ܣ��¼ӱ�ǩ���ղؼУ�
//���������handle					���������
//			stringTagName			��ǩ��
//			stringTagDescription	��ǩ����
//���������stringTagId				��ǩID
//�޸ļ�¼��
//==================================================================
int NET_API AddTag(NET_HANDLE handle, char* stringTagId, const char* stringTagName, const char* stringTagDescription);

//==================================================================
//��������GetTagList
//���ߣ�YangJiannan
//���ڣ�2018.7.31
//���ܣ����ҽ����ǩ���ղؼУ��б�
//���������handle					���������
//			stringLocalFilePath		�����ļ�����·��
//			stringSenderId			ҽ��senderid
//�޸ļ�¼��
//==================================================================
int NET_API GetTagList(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringSenderId);

//==================================================================
//��������AddCaseToTag
//���ߣ�YangJiannan
//���ڣ�2018.7.31
//���ܣ�����������ǩ���Ѳ�������ĳ�ղؼУ�
//���������handle					���������
//			stringTagId				�����ļ�����·��
//			stringTagName			ҽ��senderid
//			stringCaseId			����id
//			stringCaseType			�������ͺ�
//			stringCaseTypeName		����������
//			stringPatientName		��������
//�޸ļ�¼��
//==================================================================
int NET_API AddCaseToTag(NET_HANDLE handle, const char* stringTagId, const char* stringTagName, const char* stringCaseId, const char* stringCaseType, const char* stringCaseTypeName, const char* stringPatientName);

//==================================================================
//��������GetCaseListOfTag
//���ߣ�YangJiannan
//���ڣ�2018.7.31
//���ܣ��������ĳ��ǩ�Ĳ����б�����ղؼ��ڲ����б�
//���������handle					���������
//			stringLocalFilePath		�����ļ�����·��
//			stringTagId				��ǩid
//			stringSenderId			ҽ��senderid
//			stringCaseType			�������ͺ�
//�޸ļ�¼��
//==================================================================
int NET_API GetCaseListOfTag(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringTagId, const char* stringSenderId, const char* stringCaseType);

#ifdef __cplusplus 
	} 
#endif 


#endif
