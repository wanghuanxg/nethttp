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
//��������UserLogin
//���ߣ�YangJiannan
//���ڣ�2012.12.3
//���ܣ��ͻ��˵�¼  
//���������handle					���������
//			stringLocalFilePath		�����˻���Ϣ�ļ�����·��
//			stringUsername			�û���
//			stringPassword			����
//			stirngRole				�û���ɫ
//			stringToken				App��¼���豸token
//���������stringIp				������ip
//����ֵ�� ��������󷵻���
//�����ͷ���ֵ����ϸ��Ϣ�μ�Net_Interface_V1.doc��4.3.1
//�޸ļ�¼��
//==================================================================
int NET_API UserLogin(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringUsername, const char* stringPassword, const char* stringRole, const char* stringToken, char* stringIp);

//==================================================================
//��������GetUserInfo
//���ߣ�YangJiannan
//���ڣ�2012.12.3
//���ܣ��ͻ��˻�õ�¼�û�����������Ϣ  
//���������handle					���������
//			stringLocalFilePath		�û�����������Ϣ�ļ�����·��
//����ֵ�� ��������󷵻���
//�����ͷ���ֵ����ϸ��Ϣ�μ�Net_Interface_V1.doc��4.3.2
//�޸ļ�¼��
//==================================================================
int NET_API GetUserInfo(NET_HANDLE handle, const char* stringLocalFilePath);

//==================================================================
//��������GetHospitalInfo
//���ߣ�YangJiannan
//���ڣ�2012.12.3
//���ܣ��ͻ��˻��ҽԺ��Ϣ  
//���������handle					���������
//			stringLocalFilePath		ҽԺ��Ϣ�ļ�����·��
//			stringHospitalId		ҽԺId
//����ֵ�� ��������󷵻���
//�����ͷ���ֵ����ϸ��Ϣ�μ�Net_Interface_V1.doc��4.3.3
//�޸ļ�¼��
//==================================================================
int NET_API GetHospitalInfo(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringHospitalId);

//==================================================================
//��������GetDoctorInfo
//���ߣ�YangJiannan
//���ڣ�2012.12.3
//���ܣ��ͻ��˻��ҽ����Ϣ  
//���������handle					���������
//			stringLocalFilePath		ҽ����Ϣ�ļ�����·��
//			stringDoctorId			ҽ��Id
//����ֵ�� ��������󷵻���
//�����ͷ���ֵ����ϸ��Ϣ�μ�Net_Interface_V1.doc��4.3.4
//�޸ļ�¼��
//==================================================================
int NET_API GetDoctorInfo(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringDoctorId);

//==================================================================
//��������UploadCase
//���ߣ�YangJiannan
//���ڣ�2012.12.3
//���ܣ��ͻ����ϴ����� 
//���������handle					���������
//			stringName				��������
//			stringThirdId			������Id
//			stringDataType			��������
//			stringNotes				��������
//			stringHospitalId		ҽԺId
//			stringHospitalName		ҽԺ����
//			stringDoctorId			ҽ��Id
//			stringDoctorName		ҽԺ����
//			stringOtherParam		�������������Ա����䡢����ţ�
//			stringCaseOrigin		����Ժ����Դ
//			stringYzlb				סԺ��/�����
//			stringSex				�Ա�
//			stringApplicationNo		���뵥��
//			stringAutoMatic			�Զ���ϣ�0���˹���ϡ�1���Զ���ϡ�Ĭ��Ϊ0��
//			stringLocalCasePath		���ز���·��
//���������stringCaseId			����Id�����ڻ��case id��
//����ֵ�� ��������󷵻���
//�����ͷ���ֵ����ϸ��Ϣ�μ�Net_Interface_V1.doc��4.3.5
//�޸ļ�¼��
//==================================================================
int NET_API UploadCase(NET_HANDLE handle, char* stringCaseId, const char* stringName, const char* stringThirdId, const char* stringDataType, const char* stringDataTypeName, const char* stringNotes, const char* stringHospitalId, 
			   const char* stringHospitalName, const char* stringDoctorId, const char* stringDoctorName, const char* stringCheckTime, const char* stringOtherParam, const char* stringDeviceId, const char* stringDeviceName, const char* stringCaseOrigin, const char* stringYzlb, const char* stringSex, 
			   const char* stringApplicationNo, const char* stringAutoMatic, const char* stringLocalCasePath, pProgressAndSpeedCallback pCallback);

//==================================================================
//��������UserDownloadReport
//���ߣ�YangJiannan
//���ڣ�2012.12.3
//���ܣ��ͻ������ر���  
//���������handle					���������
//			stringLocalFilePath		���ر����ļ�����·��
//			stringCaseId			����Id
//			stringReportId			����Id
//			stringUri				�����ļ�������·��
//			nFileSize				�����ļ�����
//����ֵ�� ��������󷵻���
//�����ͷ���ֵ����ϸ��Ϣ�μ�Net_Interface_V1.doc��4.3.6
//�޸ļ�¼��
//==================================================================
int NET_API UserDownloadReport(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringCaseId, const char* stringReportId, const char* stringUri, unsigned long nFileSize, 
					   pProgressAndSpeedCallback pCallback);

//==================================================================
//��������GetCaseReportState
//���ߣ�YangJiannan
//���ڣ�2012.12.3
//���ܣ��ͻ���ĳ�����ı���״̬ 
//���������handle					���������
//			stringReportState		����״̬��������������ڻ��ĳ�����ı���״̬��
//			stringCaseId			����Id
//����ֵ�� ��������󷵻���
//�����ͷ���ֵ����ϸ��Ϣ�μ�Net_Interface_V1.doc��4.3.7
//�޸ļ�¼��
//==================================================================
int NET_API GetCaseReportState(NET_HANDLE handle, char* stringReportState, const char* stringCaseId);

//==================================================================
//��������UserGetReportList
//���ߣ�YangJiannan
//���ڣ�2012.12.3
//���ܣ��ͻ��˻���û������б�
//���������handle					���������
//			stringLocalFilePath		�û������б��ļ����ر���·��
//			stringReportId			����Id
//			stringReportStartDate	�������ɿ�ʼʱ��
//			stringReportEndDate		�������ɽ���ʱ��
//			stringCaseType			��������
//			stringReportState		����״̬
//			stringOrderBy			ʱ������1������ 2������ Ĭ�ϵ���
//			nPageTo					�ڼ�ҳ
//			nPageSize				ÿҳ����
//����ֵ�� ��������󷵻���
//�����ͷ���ֵ����ϸ��Ϣ�μ�Net_Interface_V1.doc��4.3.8
//�޸ļ�¼��
//==================================================================
int NET_API UserGetReportList(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringReportId, const char* stringReportStartDate, const char* stringReportEndDate, 
					  const char* stringCaseType, const char* stringReportState, const char* stringOrderBy, unsigned long nPageTo, unsigned long nPageSize);

//==================================================================
//��������UserGetCaseList
//���ߣ�YangJiannan
//���ڣ�2012.12.3
//���ܣ��ͻ��˻���û������б� 
//���������handle					���������
//			stringLocalFilePath		�����б��ļ����ر���·��
//			stringCaseId			����Id
//			stringCaseType			��������
//			stringCaseState			����״̬
//			stringThirdId			������Id
//			stringName				��������
//			stringStartDate			�����ϴ���ʼʱ��
//			stringEndDate			�����ϴ�����ʱ��
//			stringCheckStartDate	������鿪ʼʱ��
//			stringCheckEndDate		����������ʱ��
//			stringCaseOrigin		����Ժ����Դ
//			stringYzlb				�����/סԺ��
//			stringSex				�Ա�
//			stringIsRead			�����Ƿ��Ѷ�
//			stringOrderBy			����ʽ��1������2������Ĭ�ϵ���
//			nPageTo					��ʾ�ڼ�ҳ
//			nPageSize				ÿҳ��ʾ����
//����ֵ�� ��������󷵻���
//�����ͷ���ֵ����ϸ��Ϣ�μ�Net_Interface_V1.doc��4.3.9
//�޸ļ�¼��
//==================================================================
int NET_API UserGetCaseList(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringCaseId, const char* stringCaseType, const char* stringCaseState, const char* stringThirdId, const char* stringName, 
							const char* stringStartDate, const char* stringEndDate, const char* stringCheckStartDate, const char* stringCheckEndDate, const char* stringCaseOrigin, 
							const char* stringYzlb, const char* stringSex, const char* stringIsRead, const char* stringOrderBy, unsigned int nPageTo, unsigned int nPageSize);



//==================================================================
//��������GetAdvice
//���ߣ�YangJiannan
//���ڣ�2015.11.26
//���ܣ���ý�������
//���������handle					���������
//			stringLocalFilePath		�����ļ�����·��
//			stringSgId				���������
//			stringUserId			���û�ID
//			stringDoctorId			���ɽ����ҽ��ID
//			stringDownloadState		����״̬
//			stringType				��������
//			stringStartDate			�������ɿ�ʼʱ��
//			stringEndDate			�������ɽ���ʱ��
//			nPageTo					�ڼ�ҳ
//			nPageSize				ÿҳ��������
//����ֵ�� ��������󷵻���
//�����ͷ���ֵ����ϸ��Ϣ�μ�Net_Interface_V1.doc��4.3.10
//�޸ļ�¼��
//==================================================================
int NET_API GetAdvice(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringSgId, const char* stringUserId, const char* stringDoctorId, const char* stringDownloadState, const char* stringType, 
			  const char* stringStartDate, const char* stringEndDate, unsigned int nPageTo, unsigned int nPageSize);


//==================================================================
//��������MarkReportRead
//���ߣ�YangJiannan
//���ڣ�2012.12.3
//���ܣ��ͻ��˱�Ǳ���Ϊ�Ѷ�
//���������handle					���������
//			stringReportId			����Id
//����ֵ�� ��������󷵻���
//�����ͷ���ֵ����ϸ��Ϣ�μ�Net_Interface_V1.doc��4.3.10
//�޸ļ�¼��
//==================================================================
int NET_API MarkReportRead(NET_HANDLE handle, const char* stringReportId);
//==================================================================
//��������CardUserChangeInfo
//���ߣ�YangJiannan
//���ڣ�2012.12.3
//���ܣ��ͻ��˿��û��޸ĸ�����Ϣ 
//���������handle					���������
//			stringPId				���֤��
//			stringName				����
//			stringSex				�Ա�
//			stringTel				�绰����
//			stringBirthday			��������
//			stringAddress			��ַ
//			stringLmp				(ĩ�����ٵ�һ�죬���������ֻ�Ӧ��ʹ��)
//			stringToken				Apple�豸��Token��Apple�ֻ��ͻ���ʹ��
//			stringHeight			���
//			stringWeight			����
//			stringPhoto				1105�ӿڷ��صķ�������Ƭ�洢·��
//			stringVCodeBeFollowed	��ע��֤�룬���˹�ע��ʱ��Ҫ�������֤��
//			stringHospitalId		ҽԺ��
//			stringOtherInfo			�û�������Ϣ
//����ֵ�� ��������󷵻���
//�����ͷ���ֵ����ϸ��Ϣ�μ�Net_Interface_V1.doc��4.3.11
//�޸ļ�¼��
//==================================================================
int NET_API CardUserChangeInfo(NET_HANDLE handle, const char* stringPId, const char* stringName, const char* stringSex, const char* stringTel, const char* stringBirthday, const char* stringAddress, const char* stringLmp, const char* stringToken, const char* stringHeight, 
							   const char* stringWeight, const char* stringPhoto, const char* stringVCodeBeFollowed, const char* stringHospitalId, const char* stringOtherInfo);

//==================================================================
//��������CliniqueChangeInfo
//���ߣ�YangJiannan
//���ڣ�2012.12.3
//���ܣ��ͻ��˻����û��޸Ļ�����Ϣ 
//���������handle					���������
//			stringPId				���֤��
//			stringName				����
//			stringTel				�绰����
//			stringNotes				��������
//����ֵ�� ��������󷵻���
//�����ͷ���ֵ����ϸ��Ϣ�μ�Net_Interface_V1.doc��4.3.12
//�޸ļ�¼��
//==================================================================
int NET_API CliniqueChangeInfo(NET_HANDLE handle, const char* stringPId, const char* stringName, const char* stringTel, const char* stringNotes);

//==================================================================
//��������UploadTrendData
//���ߣ�YangJiannan
//���ڣ�2013.5.16
//���ܣ��ϴ��������� 
//���������handle					���������
//			stringCardId			����
//			stringDeviceId			�豸id
//			stringDeviceName		�豸��
//			stringCaseId			������
//			pRecordData				���������׵�ַ
//			nArrayLen				�������鳤��
//����ֵ�� ��������󷵻���
//�����ͷ���ֵ����ϸ��Ϣ�μ�Net_Interface_V1.doc��4.3.12
//�޸ļ�¼��2013.9.2,����double����Ĭ�϶�����С�����1λ����������������ϵ�ҡ�
//==================================================================
int NET_API UploadTrendData(NET_HANDLE handle, const char* stringCardId, const char* stringDeviceId, const char* stringDeviceName, const char* stringCaseId, const RECORD_DATA* pRecordData, int nArrayLen);

//==================================================================
//��������UploadTrendDataString
//���ߣ�YangJiannan
//���ڣ�2015.5.27
//���ܣ��ϴ��������� 
//���������handle					���������
//			stringCardId			����
//			stringDeviceId			�豸id
//			stringDeviceName		�豸��
//			stringCaseId			������
//			pRecordData				���������׵�ַ
//			nArrayLen				�������鳤��
//����ֵ�� ��������󷵻���
//==================================================================
int NET_API UploadTrendDataString(NET_HANDLE handle, const char* stringCardId, const char* stringDeviceId, const char* stringDeviceName, const char* stringCaseId, const RECORD_DATA_STRING* pRecordData, int nArrayLen);


//==================================================================
//��������UploadTrendDataFile
//���ߣ�YangJiannan
//���ڣ�2015.5.27
//���ܣ��ϴ��������� 
//���������handle					���������
//			stringFilePath			���齨XML�ļ�·��
//����ֵ�� ��������󷵻���
//==================================================================
int NET_API UploadTrendDataFile(NET_HANDLE handle, const char* stringFilePath);



//==================================================================
//��������VerifyThirdId
//���ߣ�YangJiannan
//���ڣ�2013.9.9
//���ܣ���֤���������Ƿ����
//���������handle					���������
//			stringUId				�ֻ��ţ����֤�ţ����ţ�Ψһ���
//			stringType				id����
//���������stringCardId			����
//����ֵ�� ��������󷵻���
//�����ͷ���ֵ����ϸ��Ϣ�μ�Net_Interface_V1.doc��4.3.13
//==================================================================
int NET_API VerifyThirdId(NET_HANDLE handle, char* stringCardId, const char* stringUId, const char* stringType);

//==================================================================
//��������GetDoctorList_999120
//���ߣ�YangJiannan
//���ڣ�2013.9.10
//���ܣ�����ҽѶ�ӿڣ����ҽ���б�
//���������handle					���������
//			stringLocalFilePath		�����ļ�����·��
//			stringKtCliniqueId		������
//			stringCaseTypeId		�������ͺ�
//����ֵ�� ��������󷵻���
//�����ͷ���ֵ����ϸ��Ϣ�μ�Net_Interface_V1.doc��4.3.14
//==================================================================
int NET_API GetDoctorList_999120(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringKtCliniqueId, const char* stringCaseTypeId);

//==================================================================
//��������ValidatePay_999120
//���ߣ�YangJiannan
//���ڣ�2013.9.10
//���ܣ�����ҽѶ�ӿڣ���֤����
//���������handle					���������
//			stringLocalFilePath		�����ļ�����·��
//			stringConsumeId			���ѵ��ż�¼
//			stringKtCliniqueId		������
//			stringCardNumber		��Ա����
//			stringKtDoctorId		ҽ����
//			stringConsumeMoney		�������ѽ��
//			stringCaseTypeId		�������ͺ�
//����ֵ�� ��������󷵻���
//�����ͷ���ֵ����ϸ��Ϣ�μ�Net_Interface_V1.doc��4.3.15
//==================================================================
int NET_API ValidatePay_999120(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringConsumeId, const char* stringKtCliniqueId, const char* stringCardNumber, const char* stringKtDoctorId, const char* stringConsumeMoney, const char* stringCaseTypeId);

//==================================================================
//��������UpdateCaseForConsumerRecord_999120
//���ߣ�YangJiannan
//���ڣ�2013.9.10
//���ܣ�����ҽѶ�ӿڣ��������Ѽ�¼����Ӧ�Ĳ���
//���������handle					���������
//			stringLocalFilePath		�����ļ�����·��
//			stringConsumeId			���ѵ��ż�¼
//			stringKtCaseId			������
//			stringCaseTypeId		�������ͺ�
//����ֵ�� ��������󷵻���
//�����ͷ���ֵ����ϸ��Ϣ�μ�Net_Interface_V1.doc��4.3.16
//==================================================================
int NET_API UpdateCaseForConsumerRecord_999120(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringConsumeId, const char* stringKtCaseId, const char* stringCaseTypeId);

//==================================================================
//��������PersonIdRegister
//���ߣ�YangJiannan
//���ڣ�2013.10.11
//���ܣ����֤��ע��
//���������handle					���������
//			stringLocalFilePath		�����ļ�����·����ֻ��typeȡֵΪ6��ʱ�������Чֵ������type���մ���
//			stringType				ע������
//			stringRegFrom			ע��������Դ
//			stringPersonId			���֤��
//			stringName				����
//			stringSex				�Ա�
//			stringBirthday			����
//			stringAddress			סַ
//			stringPhone				�绰
//			stringHeigth			���
//			stringUploadUnitId		������
//			stringHospitalId		ҽԺ��
//			stringHGroupId			ҽԺ��֯��
//			stringScode				�ֻ���֤��
//			stringWxUId				΢��Ψһ��
//			stringLmp				ĩ�����ٵ�һ��
//			stringWeight			�û�����
//			stringPassword			��ʼ����
//			stringThirdCode			��������¼��
//			stringIsRegister		�Ƿ�ע��
//����ֵ�� ��������󷵻���
//�����ͷ���ֵ����ϸ��Ϣ�μ�Net_Interface_V1.doc��4.3.17
//==================================================================
int NET_API PersonIdRegister(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringType,const char* stringRegFrom, const char* stringPersonId, const char* stringName, const char* stringSex, const char* stringBirthday, const char* stringAddress, 
					 const char* stringPhone, const char* stringHeigth, const char* stringUploadUnitId, const char* stringHospitalId, const char* stringHGroupId, const char* stringScode, const char* stringWxUId, const char* stringLmp,const char*  stringWeight, const char* stringPassword, 
					 const char* stringThirdCode, const char* stringIsRegister);

//==================================================================
//��������GetDataDictionary
//���ߣ�YangJiannan
//���ڣ�2013.10.31
//���ܣ���������ֵ�
//���������handle					���������
//			stringLocalFilePath		�����ļ�����·��
//����ֵ�� ��������󷵻���
//�����ͷ���ֵ����ϸ��Ϣ�μ�Net_Interface_V1.doc��4.3.18
//==================================================================
int NET_API GetDataDictionary(NET_HANDLE handle, const char* stringLocalFilePath);

//==================================================================
//��������LoggingHealthBaseInfo
//���ߣ�YangJiannan
//���ڣ�2013.10.31
//���ܣ���������������Ϣ¼��
//���������handle					���������
//			������
//����ֵ�� ��������󷵻���
//�����ͷ���ֵ����ϸ��Ϣ�μ�Net_Interface_V1.doc��4.3.19
//==================================================================
int NET_API LoggingHealthBaseInfo(NET_HANDLE handle, const char* stringUserId, const char* stringWeblock, const char* stringNation, const char* stringProfression, const char* stringCrowd, const char* stringBloodType, 
						  const char* stringJwsJiBing, const char* stringJwsShouShu, const char* stringJwsWaiShang, const char* stringJwsShuXue, const char* stringJwsBaoLuShi, const char* stringJwsBaoLuShiBz, 
						  const char* stringJwsYiChuanJiBing, const char* stringGmsGuoMinShi, const char* stringGmsGuoMinShiBz, const char* stringJzsFuQin, const char* stringJzsFuQinBz, const char* stringJzsMuQin, 
						  const char* stringJzsMuQinBz, const char* stringJzsXdjm, const char* stringJzsXdjmBz, const char* stringJzsZiNv, const char* stringJzsZiNvBz, const char* stringJzsQiTa);

//==================================================================
//��������LoggingHealthCurrentInfo
//���ߣ�YangJiannan
//���ڣ�2013.10.31
//���ܣ��������������Ϣ¼��
//���������handle					���������
//			������
//����ֵ�� ��������󷵻���
//�����ͷ���ֵ����ϸ��Ϣ�μ�Net_Interface_V1.doc��4.3.20
//==================================================================
int NET_API LoggingHealthCurrentInfo(NET_HANDLE handle, const char* stringUserId, const char* stringShfsYinShi, const char* stringShfsXlzk, const char* stringShfsXlzkBz, const char* stringShfsXltz, const char* stringShfsXiYan, 
							 const char* stringShfsYinJiu, const char* stringShfsSyqk, const char* stringShfsYunDong, const char* stringShfsYunDongShiJian, const char* stringSysjcDanGuChun, const char* stringSysjcXueChangGui, 
							 const char* stringSysjcXueChangGuiBz, const char* stringSysjcNiaoChangGui, const char* stringSysjcNiaoChangGuiBz, const char* stringSysjcQiTa, const char* stringYongYao, const char* stringZhuYuan, 
							 const char* stringJkwtNaoXueGuan, const char* stringJkwtNaoXueGuanBz, const char* stringJkwtShenZang, const char* stringJkwtShenZangBz, const char* stringJkwtXinZang, const char* stringJkwtXinZangBz, 
							 const char* stringJkwtXueGuan, const char* stringJkwtXueGuanBz, const char* stringJkwtYanBu, const char* stringJkwtYanBuBz, const char* stringJkwtShenJing, const char* stringJkwtQiTa, const char* stringDate);

//==================================================================
//��������UpdateCaseInfo
//���ߣ�YangJiannan
//���ڣ�2014.2.13
//���ܣ��������������Ϣ¼��
//���������handle					���������
//			stringHospitalId		ҽԺid
//			stringCaseId			������
//			stringOtherParam		��������
//			stringFlag				������־
//			stringNotes				�޸ı�ע
//���������stringDoctorId			ҽ����
//����ֵ�� ��������󷵻���
//�����ͷ���ֵ����ϸ��Ϣ�μ�Net_Interface_V1.doc��4.3.21
//==================================================================
int NET_API UpdateCaseInfo(NET_HANDLE handle, char* stringDoctorId, const char* stringHospitalId, const char* stringCaseId, const char* stringOtherParam, const char* stringFlag, const char* stringNotes, const char* stringReceiverId);

//==================================================================
//��������PersonIdRegisterAndLogin
//���ߣ�YangJiannan
//���ڣ�2014.3.25
//���ܣ��������������Ϣ¼��
//���������handle					���������
//			stringLocalFilePath		�����ļ�·��
//			stringRole				�û����
//			stringPId				���֤��
//			stringName				����
//			stringSex				�Ա�
//			stringBirthday			����
//			stringAddress			��ַ
//			stringDeviceId			�豸Id
//			stringHospitalId		ҽԺId
//			stringLang				����
//			stringRegFrom			ע��������Դ
//����ֵ�� ��������󷵻���
//�����ͷ���ֵ����ϸ��Ϣ�μ�Net_Interface_V1.doc��4.3.22
//==================================================================
int NET_API PersonIdRegisterAndLogin(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringRole, const char* stringPId, const char* stringName, const char* stringSex, const char* stringBirthday, const char* stringAddress, const char* stringDeviceId, const char* stringHospitalId, 
							 const char* stringLang, const char* stringRegFrom);

//==================================================================
//��������UploadTrendData2
//���ߣ�YangJiannan
//���ڣ�2014.3.28
//���ܣ��������������Ϣ¼��
//���������handle					���������
//			stringTargetPath		�����ļ�·��
//����ֵ�� ��������󷵻���
//�����ͷ���ֵ����ϸ��Ϣ�μ�Net_Interface_V1.doc��4.3.23
//==================================================================
int NET_API UploadTrendData2(NET_HANDLE handle, const char* stringTargetPath);

//==================================================================
//��������CardBindPhone
//���ߣ�YangJiannan
//���ڣ�2014.8.20
//���ܣ����û����ֻ�
//���������handle					���������
//			stringPhone				�ֻ���
//			stringScode				��֤��
//����ֵ�� ��������󷵻���
//
//==================================================================
int NET_API CardBindPhone(NET_HANDLE handle, const char* stringPhone, const char* stringScode);

//==================================================================
//��������ResetPassword
//���ߣ�YangJiannan
//���ڣ�2014.8.20
//���ܣ����û���������
//���������handle					���������
//			stringPhone				�ֻ���
//			stringScode				��֤��
//			stringNewPwd			������
//����ֵ�� ��������󷵻���
//
//==================================================================
int NET_API ResetPassword(NET_HANDLE handle, const char* stringPhone, const char* stringScode, const char* stringNewPwd);

//==================================================================
//��������UploadDaily
//���ߣ�YangJiannan
//���ڣ�2015.9.2
//���ܣ����ĳ���������ռǵ������б�
//���������handle					���������
//			stringType				����
//			stringCreateTime		����ʱ��
//			stringFiles				�Զ����ʽ�ַ���
//����ֵ�� ��������󷵻���
//
//==================================================================
int NET_API UploadDaily(NET_HANDLE handle, const char* stringType, const char* stringCreateTime, const char* stringFiles);

//==================================================================
//��������GetDateListOfDaily
//���ߣ�YangJiannan
//���ڣ�2015.6.24
//���ܣ����ĳ���������ռǵ������б�
//���������handle					���������
//			stringLocalPath			�����ļ�����·��
//			stringYear				���
//			stringMonth				�·�
//����ֵ�� ��������󷵻���
//
//==================================================================
int NET_API GetDateListOfDaily(NET_HANDLE handle, const char* stringLocalPath, const char* stringYear, const char* stringMonth);

//==================================================================
//��������GetDailyOfDate
//���ߣ�YangJiannan
//���ڣ�2015.6.24
//���ܣ�����ռ�����
//���������handle					���������
//			stringLocalPath			�����ļ�����·��
//			stringDate				����
//			stringType				����
//����ֵ�� ��������󷵻���
//
//==================================================================
int NET_API GetDailyOfDate(NET_HANDLE handle, const char* stringLocalPath, const char* stringDate, const char* stringType);

//==================================================================
//��������ShareDaily
//���ߣ�YangJiannan
//���ڣ�2015.6.24
//���ܣ������ռ�
//���������handle					���������
//			stringLocalPath			�����ļ�����·��
//			stringType				����
//			stringCreateTime		����ʱ��
//����ֵ�� ��������󷵻���
//
//==================================================================
int NET_API ShareDaily(NET_HANDLE handle, const char* stringLocalPath, const char* stringType, const char* stringCreateTime);

//==================================================================
//��������DeleteDaily
//���ߣ�YangJiannan
//���ڣ�2015.6.24
//���ܣ�ɾ���ռ�
//���������handle					���������
//			stringType				����
//			stringDate				����
//����ֵ�� ��������󷵻���
//
//==================================================================
int NET_API DeleteDaily(NET_HANDLE handle, const char* stringType, const char* stringDate);

//==================================================================
//��������PhoneScanQR
//���ߣ�YangJiannan
//���ڣ�2014.12.15
//���ܣ����û���������
//���������handle					���������
//			stringQRCode			��ά��ͼƬ�����������
//����ֵ�� ��������󷵻���
//
//==================================================================
int NET_API PhoneScanQR(NET_HANDLE handle, char* stringSence, const char* stringQRCode);

//==================================================================
//��������PhoneScanQRVerify
//���ߣ�YangJiannan
//���ڣ�2014.12.15
//���ܣ����û���������
//���������handle					���������
//			stringQRCode			��ά��ͼƬ�����������
//����ֵ�� ��������󷵻���
//
//==================================================================
int NET_API PhoneScanQRVerify(NET_HANDLE handle, const char* stringQRCode);

//==================================================================
//��������RegisterBluetoothDevice
//���ߣ�YangJiannan
//���ڣ�2015.3.17
//���ܣ�ע����޸��豸������Ϣ
//���������handle					���������
//			stringUniqueSN			�豸Ψһ��
//			stringSerial			���к�
//			stringBluetoothAddress	������ַ
//			stringBluetoothName		������
//			stringDeviceType		�豸���ͱ���
//����ֵ�� ��������󷵻���
//
//==================================================================
int NET_API RegisterBluetoothDevice(NET_HANDLE handle, const char* stringUniqueSN, const char* stringSerial, const char* stringBluetoothAddress, const char* stringBluetoothName, const char* stringDeviceType);

//==================================================================
//��������GetBluetoothInfoOfDevice
//���ߣ�YangJiannan
//���ڣ�2015.3.17
//���ܣ�����豸��������Ϣ
//���������handle					���������
//			stringUniqueSN			�豸Ψһ��
//����ֵ�� ��������󷵻���
//
//==================================================================
int NET_API GetBluetoothInfoOfDevice(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringUniqueSN);

//==================================================================
//��������ChangeDeviceType
//���ߣ�YangJiannan
//���ڣ�2015.3.17
//���ܣ��ı��豸����
//���������handle					���������
//			stringUniqueSN			�豸Ψһ��
//			stringDeviceType		�豸���ͱ���
//����ֵ�� ��������󷵻���
//
//==================================================================
int NET_API ChangeDeviceType(NET_HANDLE handle, const char* stringUniqueSN, const char* stringDeviceType);

//==================================================================
//��������UnregisterBluetoothDevice
//���ߣ�YangJiannan
//���ڣ�2015.3.17
//���ܣ��豸���ϣ�ȡ��ע�ᣩ
//���������handle					���������
//			stringUniqueSN			�豸Ψһ��
//����ֵ�� ��������󷵻���
//
//==================================================================
int NET_API UnregisterBluetoothDevice(NET_HANDLE handle, const char* stringUniqueSN);


//==================================================================
//��������ValidatePay_XinJie
//���ߣ�YangJiannan
//���ڣ�2015.5.25
//���ܣ������Ľ���֤���ѽӿ�
//���������handle					���������
//			stringLocalFilePath		�����ļ�����·��
//			stringRrn				�ο���
//			localdate				��������
//����ֵ�� ��������󷵻���
//
//==================================================================
int NET_API ValidatePay_XinJie(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringRrn, const char* stringLocalDate);

//==================================================================
//��������NotifyCheck_XinJie
//���ߣ�YangJiannan
//���ڣ�2015.6.11
//���ܣ������Ľ�֪ͨ�ο�����ʹ�ýӿ�
//���������handle					���������
//			stringLocalFilePath		�����ļ�����·��
//			stringRrn				�ο���
//			localdate				��������
//����ֵ�� ��������󷵻���
//
//==================================================================
int NET_API NotifyCheck_XinJie(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringRrn, const char* stringLocalDate);

//==================================================================
//��������GetHospitalListOfArea
//���ߣ�YangJiannan
//���ڣ�2015.6.29
//���ܣ���õ���ҽԺ�б�
//���������handle					���������
//			stringLocalFilePath		�����ļ�����·��
//			stringAreaId			����ID
//����ֵ�� ��������󷵻���
//
//==================================================================
int NET_API GetHospitalListOfArea(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringAreaId, const char* stringRank, const char* stringBeta);



//==================================================================
//��������SearchSomebody
//���ߣ�YangJiannan
//���ڣ�2015.6.29
//���ܣ���õ���ҽԺ�б�
//���������handle					���������
//			stringLocalFilePath		�����ļ�����·��
//			stringUserId			�û�ID
//����ֵ�� ��������󷵻���
//
//==================================================================
int NET_API SearchSomebody(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringUserId);

//==================================================================
//��������FollowSomebody
//���ߣ�YangJiannan
//���ڣ�2015.6.29
//���ܣ���עĳ��
//���������handle					���������
//			stringLocalFilePath		�����ļ�����·��
//			stringUserId			�û�ID
//			stringUsername			�û���
//			stringNickname			�û��ǳ�
//			stringVCode				��֤��
//����ֵ�� ��������󷵻���
//
//==================================================================
int NET_API FollowSomebody(NET_HANDLE handle, const char* stringUserId, const char* stringUsername, const char* stringNickname, const char* stringVCode);

//==================================================================
//��������MyFollowing
//���ߣ�YangJiannan
//���ڣ�2015.6.29
//���ܣ��ҹ�ע����
//���������handle					���������
//			stringLocalFilePath		�����ļ�����·��
//			stringUserId			�û�ID
//			stringNextId			��ȡ�б��λ��
//����ֵ�� ��������󷵻���
//
//==================================================================
int NET_API MyFollowing(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringUserId, const char* stringNextId, const char* stringFType);



//==================================================================
//��������ChangeNicknameOfFollowed
//���ߣ�YangJiannan
//���ڣ�2015.6.29
//���ܣ��ı䱻��ע���ǳ�
//���������handle					���������
//			stringLocalFilePath		�����ļ�����·��
//			stringUserId			�û�ID
//			stringNickname			�ǳ�
//����ֵ�� ��������󷵻���
//
//==================================================================
int NET_API ChangeNicknameOfFollowed(NET_HANDLE handle, const char* stringUserId, const char* stringNickname);


//==================================================================
//��������CancelFollowing
//���ߣ�YangJiannan
//���ڣ�2015.6.29
//���ܣ�ȡ����ע
//���������handle					���������
//			stringLocalFilePath		�����ļ�����·��
//			stringUserId			�û�ID
//����ֵ�� ��������󷵻���
//
//==================================================================
int NET_API CancelFollowing(NET_HANDLE handle, const char* stringUserId);


//==================================================================
//��������GenerateOrDeletePngZlibReport
//���ߣ�YangJiannan
//���ڣ�2015.9.9
//���ܣ�������ɾ��pngzlib����
//���������handle					���������
//			stringLocalFilePath		�����ļ�����·��
//			stringServerPath		���������·��
//			stringFlag				������1����ɾ����2����־
//����ֵ�� ��������󷵻���
//
//==================================================================
int NET_API GenerateOrDeletePngZlibReport(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringServerPath, const char* stringFlag);



//==================================================================
//��������GetDailyList
//���ߣ�YangJiannan
//���ڣ�2015.9.30
//���ܣ���������ռ��б�
//���������handle					���������
//			stringLocalFilePath		�����ļ�����·��
//			stringType				����
//			stringStartDate			��ʼ����
//			stringEndDate			��������
//			stringOrderBy			����
//			stringNextId			��ȡλ��
//			stringPageSize			ÿ����ȡ��������
//����ֵ�� ��������󷵻���
//
//==================================================================
int NET_API GetDailyList(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringType, const char* stringStartDate, const char* stringEndDate, const char* stringOrderBy, const char* stringNextId, const char* stringPageSize);


//==================================================================
//��������ChangeViewLikeCollectCount
//���ߣ�YangJiannan
//���ڣ�2015.11.26
//���ܣ��޸Ľ���֪ʶ�Ķ������ղش���
//���������handle					���������
//			stringHealthTipId		����֪ʶID
//			stringUri				����֪ʶȫ��URL
//			stringTitle				����֪ʶ����
//			stringLike				���ޱ仯��
//			stringRead				�Ķ��仯��
//			stringCollect			�ղر仯��
//����ֵ�� ��������󷵻���
//
//==================================================================
int NET_API ChangeViewLikeCollectCount(NET_HANDLE handle, const char* stringHealthTipId, const char* stringUri, const char* stringTitle, const char* stringLike, const char* stringRead, const char* stringCollect);


//==================================================================
//��������ChangeAdviceExecutiveState
//���ߣ�YangJiannan
//���ڣ�2015.11.26
//���ܣ���ý�������ִ�ж�
//���������handle					���������
//			stringSgId				��������ID
//			stringState				��������ִ��״̬
//����ֵ�� ��������󷵻���
//
//==================================================================
int NET_API ChangeAdviceExecutiveState(NET_HANDLE handle, const char* stringSgId, const char* stringState);


//==================================================================
//��������GetAccessToken_GuanXin
//���ߣ�YangJiannan
//���ڣ�2015.11.10
//���ܣ��ӹ��¹���ƽ̨��÷��ʸ�����Ϣ��Ҫ��AccessToken
//���������handle					���������
//			stringLocalFilePath		�����ļ�����·��
//			stringUsername			����
//			stringPassword			��ʼ����
//����ֵ�� ��������󷵻���
//
//==================================================================
int NET_API GetAccessToken_GuanXin(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringUsername, const char* stringPassword);


//==================================================================
//��������CreateSession_GuanXin
//���ߣ�YangJiannan
//���ڣ�2015.11.12
//���ܣ�����ƽ̨������session
//���������handle					���������
//			stringUsername			�û���
//			stringPassword			����
//����ֵ�� ��������󷵻���
//
//==================================================================
//int NET_API CreateSession_GuanXin(NET_HANDLE handle, const char* stringUsername, const char* stringPassword);

//==================================================================
//��������GetPersonInfo_GuanXin
//���ߣ�YangJiannan
//���ڣ�2015.11.10
//���ܣ��ӹ��¹���ƽ̨��ø�����Ϣ
//���������handle					���������
//			stringLocalFilePath		�����ļ�����·��
//			stringAccessToken		AccessToken
//			stringPersonName		�û�����
//			stringPersonGender		�û��Ա�
//			stringPersonIdentity	�û����֤��
//			stringPersonBirthDate	�û���������
//����ֵ�� ��������󷵻���
//
//==================================================================
int NET_API GetPersonInfo_GuanXin(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringAccessToken, const char* stringPersonName, const char* stringPersonGender, const char* stringPersonIdentity, const char* stringPersonBirthDate);


//==================================================================
//��������UploadECGData_GuanXin
//���ߣ�YangJiannan
//���ڣ�2016.4.19
//���ܣ�ͬ���ĵ����ݵ�����ƽ̨
//���������handle					���������
//			stringLocalFilePath		�����ļ�����·��
//			stringECGXmlPath		�ĵ����XML�ļ�����·��
//			stringECGDataPath		�ĵ粨���ļ�����·��
//����ֵ�� ��������󷵻���
//
//==================================================================
int NET_API UploadECGData_GuanXin(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringECGXmlPath, const char* stringECGDataPath);

//==================================================================
//��������UploadOtherData_GuanXin
//���ߣ�YangJiannan
//���ڣ�2016.4.19
//���ܣ�ͬ�����ĵ����ݵ�����ƽ̨
//���������handle					���������
//			stringLocalFilePath		�����ļ�����·��
//			stringOtherXmlPath		���ĵ����XML�ļ�����·��
//����ֵ�� ��������󷵻���
//
//==================================================================
int NET_API UploadOtherData_GuanXin(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringOtherXmlPath);

//==================================================================
//��������UploadCheckData_GuanXin
//���ߣ�YangJiannan
//���ڣ�2016.4.19
//���ܣ�ͬ��������ݵ�����ƽ̨
//���������handle					���������
//			stringLocalFilePath		�����ļ�����·��
//			stringCheckXmlPath		���ĵ����XML�ļ�����·��
//����ֵ�� ��������󷵻���
//
//==================================================================
int NET_API UploadCheckData_GuanXin(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringCheckXmlPath);

//==================================================================
//��������UploadPersonInfo_GuanXin
//���ߣ�YangJiannan
//���ڣ�2016.11.2
//���ܣ�ͬ�������Ϣ������ƽ̨
//���������handle					���������
//			stringLocalFilePath		�����ļ�����·��
//			stringPersonInfoXmlPath		���ĵ����XML�ļ�����·��
//����ֵ�� ��������󷵻���
//
//==================================================================
int NET_API UploadPersonInfo_GuanXin(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringPersonInfoXmlPath);

//==================================================================
//��������Login_XiKang
//���ߣ�YangJiannan
//���ڣ�2016.6.29
//���ܣ����ʵ�¼
//���������handle					���������
//			stringLocalFilePath		�����ļ�����·��
//			stringUsername			�û���
//			stringPassword			����
//����ֵ�� ��������󷵻���
//
//==================================================================
int NET_API Login_XiKang(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringUsername, const char* stringPassword);

//==================================================================
//��������RegisterPatientId_XiKang
//���ߣ�YangJiannan
//���ڣ�2016.6.30
//���ܣ�ע���ԱID
//���������handle					���������
//			stringLocalFilePath		�����ļ�����·��
//			stringName				����
//			stringSex				�Ա�(�Ա���� 0 δ֪���Ա�;1 ����;2 Ů��;9 δ˵�����Ա�)
//			stringPhoneNum			�ֻ���
//			stringProofNum			���֤��
//����ֵ�� ��������󷵻���
//
//==================================================================
int NET_API RegisterPatientId_XiKang(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringName, const char* stringSex, const char* stringPhoneNum, const char* stringProofNum);

//==================================================================
//��������RegisterPatientInfo_XiKang
//���ߣ�YangJiannan
//���ڣ�2016.6.30
//���ܣ�ע���Ա�˻���Ϣ
//���������handle					���������
//			stringLocalFilePath		�����ļ�����·��
//			stringPatientOpenId		��Աopenid��RegisterPatientId_XiKang�ӿڻ�ã�
//			stringName				����
//			stringSex				�Ա�(�Ա���� 0 δ֪���Ա�;1 ����;2 Ů��;9 δ˵�����Ա�)
//			stringPhoneNum			�ֻ���
//			stringProofNum			���֤��
//			stringPassword			����
//����ֵ�� ��������󷵻���
//
//==================================================================
int NET_API RegisterPatientInfo_XiKang(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringPatientOpenId, const char* stringName, const char* stringSex, const char* stringPhoneNum, const char* stringProofNum, const char* stringPassword);

//==================================================================
//��������BindPatient_XiKang
//���ߣ�YangJiannan
//���ڣ�2016.6.30
//���ܣ��󶨻�Ա
//���������handle					���������
//			stringLocalFilePath		�����ļ�����·��
//			stringDoctorOpenId		���ʵ�openid����¼��ã�
//			stringPatientOpenId		��Ա��openid��RegisterPatientId_XiKang�ӿڻ�ã�
//����ֵ�� ��������󷵻���
//
//==================================================================
int NET_API BindPatient_XiKang(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringDoctorOpenId, const char* stringPatientOpenId);

//==================================================================
//��������GetPatientIdList_XiKang
//���ߣ�YangJiannan
//���ڣ�2016.6.30
//���ܣ���û�Աid�б�
//���������handle					���������
//			stringLocalFilePath		�����ļ�����·��
//			stringDoctorOpenId		���ʵ�openid����¼��ã�
//����ֵ�� ��������󷵻���
//
//==================================================================
int NET_API GetPatientIdList_XiKang(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringDoctorOpenId);

//==================================================================
//��������SearchPatientId_XiKang
//���ߣ�YangJiannan
//���ڣ�2016.11.29
//���ܣ����һ�ԱOpenID
//���������handle					���������
//			stringLocalFilePath		�����ļ�����·��
//			stringKey		�������ؼ��֣�Ψһ�Բ�ѯ������EMAILNUM(����),MOBILENUM(�ֻ���),PROOFNUM(֤����),PERSONNICKNAME(�ǳ�),MEMBERCARDNO(��Ա����)��
//			stringValue		����ֵ
//����ֵ�� ��������󷵻���
//
//==================================================================
int NET_API SearchPatientId_XiKang(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringKey, const char* stringValue);

//==================================================================
//��������GetPatientInfo_XiKang
//���ߣ�YangJiannan
//���ڣ�2016.6.30
//���ܣ����ݻ�Աid��û�Ա��Ϣ
//���������handle					���������
//			stringLocalFilePath		�����ļ�����·��
//			stringPatientOpenId		��Ա��openid��RegisterPatientId_XiKang�ӿڻ�ã�
//����ֵ�� ��������󷵻���
//
//==================================================================
int NET_API GetPatientInfo_XiKang(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringPatientOpenId);

//==================================================================
//��������GetCaseList_XiKang
//���ߣ�YangJiannan
//���ڣ�2016.6.30
//���ܣ���û�����Ϣ�б�
//���������handle					���������
//			stringLocalFilePath		�����ļ�����·��
//			arrPatientOpenId		��Ա��openid��RegisterPatientId_XiKang�ӿڻ�ã�������
//			nLength					���鳤��
//����ֵ�� ��������󷵻���
//
//==================================================================
int NET_API GetCaseList_XiKang(NET_HANDLE handle, const char* stringLocalFilePath, const char** arrPatientOpenId, int nLength);

//==================================================================
//��������SubmitCase_XiKang
//���ߣ�YangJiannan
//���ڣ�2016.6.30
//���ܣ��ύ������Ϣ����̬�ĵ粡��ͨ��stringEcgXmlContent���������Ͳ����Ժ�����UploadFile_XiKang�ӿڣ�
//���������handle					���������
//			stringLocalFilePath		�����ļ�����·��
//			stringPatientOpenId		��Ա��openid��RegisterPatientId_XiKang�ӿڻ�ã�
//			stringAdvice			ҽ������
//			stringCaseType			�������ͣ���̬ecg����staticECG_KT����̬ecg����dynamicECG_KT��Ѫѹ����bloodPressure_KT��
//����ֵ�� ��������󷵻���
//
//==================================================================
int NET_API SubmitCase_XiKang(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringPatientOpenId, const char* stringAdvice, const char* stringCaseType, const char* stringEcgXmlContent);

//==================================================================
//��������GetReportFile_XiKang
//���ߣ�YangJiannan
//���ڣ�2016.6.30
//���ܣ���û��ﱨ��url
//���������handle					���������
//			stringLocalFilePath		�����ļ�����·��
//			stringCaseId			����id��ͨ��SubmitCase_XiKang�ӿڻ�ã�
//����ֵ�� ��������󷵻���
//
//==================================================================
int NET_API GetReportFile_XiKang(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringCaseId);

//==================================================================
//��������UploadFile_XiKang
//���ߣ�YangJiannan
//���ڣ�2016.8.22
//���ܣ��ϴ������ļ�
//���������handle					���������
//			stringLocalFilePath		�����ļ�����·��
//			stringCaseId			����id��ͨ��SubmitCase_XiKang�ӿڻ�ã�
//			stringCaseFilePath		�����ļ�·��
//����ֵ�� ��������󷵻���
//
//==================================================================
int NET_API UploadFile_XiKang(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringCaseId, const char* stringCaseFilePath);

//==================================================================
//��������GetCardUserListOfCustomer
//���ߣ�YangJiannan
//���ڣ�2017.12.15
//���ܣ������ĳ�����ϴ��������Ŀ��û��б�
//���������handle					���������
//			stringLocalFilePath		�����ļ�����·��
//			stringThirdId			��������
//			stringName				���û�����
//			stringCaseType			�������ͣ�֧�ֶ�����Ÿ�����
//			stringStartDate			��ʼʱ��
//			stringEndDate			����ʱ��
//			stringNextId			nextid
//			nPageSize				һ��Ҫ������
//����ֵ�� ��������󷵻���
//
//==================================================================
int NET_API GetCardUserListOfCustomer(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringThirdId, const char* stringName, const char* stringCaseType, 
									  const char* stringStartDate, const char* stringEndDate, const char* stringNextId, unsigned int nPageSize);


//==================================================================
//��������RefundFee
//���ߣ�YangJiannan
//���ڣ�2017.12.15
//���ܣ��˿�
//���������handle					���������
//			stringType				֧��ƽ̨���ͣ�1��΢�ţ�
//			stringCaseId			������
//			stringRefundFee			�˻ؽ���λ�֣�
//����ֵ�� ��������󷵻���
//
//==================================================================
int NET_API RefundFee(NET_HANDLE handle, const char* stringType, const char* stringCaseId, const char* stringRefundFee);




#ifdef __cplusplus 
	} 
#endif 

#endif