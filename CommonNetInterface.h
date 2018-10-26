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
//����Ҫ��
//������Ŀ���ļ�����Ŀ¼������ڣ����򷵻��ļ���ʧ��
//��Ŀ¼����ʱ��֧�ֺ��֣�����Ҫ���Ըģ�(�Ѹ�֧�ֺ���2013-4-18)
//���ַ�ָ���������ΪNULL
//��Ҫ���ַ�����������ΪGBK�������п��ܸĳ�UTF8
//==================================================================


//==================================================================
//��������SetContecServerInfo
//���ߣ�YangJiannan
//���ڣ�2016.11.16
//���ܣ��û�����С��������Ŀ�������ip�Ͷ˿�
//���������stringContecServerAddr	������ip������
//			stringContecServerPort	�������˿�
//����ֵ�� ��
//��ע��������PhmsConfig.ini
//==================================================================
void NET_API SetContecServerInfo(const char* stringContecServerAddr, const char* stringContecServerPort);

//==================================================================
//��������SetAppDir
//���ߣ�YangJiannan
//���ڣ�2015.11.4
//���ܣ�����Android������ã����ù���Ŀ¼����Ϊ�޷��ӵײ������е�Android APP�Ĺ���Ŀ¼����Ҫ�ⲿ����
//���������stringWorkDir	����Ŀ¼
//����ֵ�� ��
//�޸ļ�¼��
//==================================================================
void NET_API SetAppDir(const char* stringWorkDir);


//==================================================================
//�������������ն����ͱ�ʶ
//���ߣ�YangJiannan
//���ڣ�2013.4.18
//���ܣ�����ÿһ���ն����ͱ�ʶ
//����������ն˱�ʶ�ţ��μ�P��û��DataType.h
//����ֵ�� ��ȷ�������
//�޸ļ�¼��
//==================================================================
int NET_API SetTerminalType(const char* stringTerminalType);


//==================================================================
//��������InitializeNetLibObject
//���ߣ�YangJiannan
//���ڣ�2012.12.3
//���ܣ���ʼ��������󣬻�������������ÿ���߳������Ҫʹ������ӿڣ������ȵ��øú��������������������Ժ�ÿ���ӿڶ�Ҫ����þ��
//�����������
//����ֵ�� ���������
//�޸ļ�¼��
//==================================================================
NET_HANDLE NET_API InitializeNetLibObject();

//==================================================================
//��������DeleteNetLibObject
//���ߣ�YangJiannan
//���ڣ�2012.12.3
//���ܣ�ɾ����������߳̽���֮ǰ������øú����������ڴ�й¶
//���������handle					���������
//����ֵ�� ��������󷵻���
//�޸ļ�¼��
//==================================================================
int NET_API DeleteNetLibObject(NET_HANDLE handle);

//==================================================================
//��������SetSessionId���˺���ֻ�ڿ����ʹ��ͬһsessionid��ʱ�����Ҫ�ã�ͬһ���̵�SessionIdʵ���û�͸������
//���ߣ�YangJiannan
//���ڣ�2013.1.18
//���ܣ����õ�ǰ���̵�Session Id��������֪SessionId�������ֱ�����ã�����Ҫ�ٴε�¼��������Ҳ������ͬһ�˺�ͬʱ��¼
//���������stringSessionId			SessionId
//����ֵ�� ��������󷵻���
//�޸ļ�¼��
//==================================================================
int NET_API SetSessionId(const char* stringSessionId);

//==================================================================
//��������GetSessionId���˺���ֻ�ڿ����ʹ��ͬһsessionid��ʱ�����Ҫ�ã�ͬһ���̵�SessionIdʵ���û�͸������
//���ߣ�YangJiannan
//���ڣ�2013.1.18
//���ܣ���õ�ǰ���̵�Session Id
//���������bufSessionId			buf��������������ڱ���SessionId
//			nBufMaxLen				buf����󳤶�(����С��128)
//����ֵ�� ��������󷵻���
//�޸ļ�¼��
//==================================================================
int NET_API GetSessionId(char* bufSessionId, unsigned int nBufMaxLen);

//==================================================================
//��������SetUsernameAndPassword���˺���ֻ�ڿ����ʹ��ͬһsessionid��ʱ�����Ҫ�ã�ͬһ���̵�SessionIdʵ���û�͸������
//���ߣ�YangJiannan
//���ڣ�2017.5.24
//���ܣ����õ�¼�˺����룬���ڼ���ǩ��
//���������stringUsername			�û���
//			stringPassword			����
//����ֵ�� ��������󷵻���
//�޸ļ�¼��
//==================================================================
int NET_API SetUsernameAndPassword(const char* stringUsername, const char* stringPassword);


//==================================================================
//��������HandleSessionSync
//���ߣ�YangJiannan
//���ڣ�2012.12.24
//���ܣ����ھ�session idʧЧ�����»����session id��Ȼ����ñ��ӿڣ�ʹ���������߳�session idͬ��
//���������stringSessionId			��Ч��session id
//����ֵ�� ��
//�޸ļ�¼��
//==================================================================
//void HandleSessionSync(const char* stringSessionId);



//==================================================================
//��������KeepSession
//���ߣ�YangJiannan
//���ڣ�2013.3.11
//���ܣ����ֻỰ
//���������handle					���������
//����ֵ�� ��������󷵻���
//�޸ļ�¼��
//==================================================================
int NET_API KeepSession(NET_HANDLE handle);


//==================================================================
//��������Logout
//���ߣ�YangJiannan
//���ڣ�2012.12.3
//���ܣ�ע����¼
//���������handle					���������
//����ֵ�� ��������󷵻���
//�޸ļ�¼��
//==================================================================
int NET_API Logout(NET_HANDLE handle);

//==================================================================
//��������UploadIm
//���ߣ�YangJiannan
//���ڣ�2012.12.3
//���ܣ��ϴ���ʱ��Ϣ
//���������handle					���������
//			stringSenderId			������Id
//			stringSenderName		����������
//			stringReceiverId		������Id
//			stringMsgDirection		��Ϣ����
//			stringMsgType			��Ϣ����
//			stringAudioFilePath		����Ƶ�ļ�����·��
//			stringCaseId			������
//			stringCaseType			��������
//����ֵ�� ��������󷵻���
//�޸ļ�¼��
//==================================================================
int NET_API UploadIm(NET_HANDLE handle, const char* stringSenderId, const char* stringSenderName, const char* stringReceiverId, const char* stringMsgDirection, const char* stringMsgType, 
			 const char* stringMsgContent, const char* stringAudioFilePath, const char* stringCaseId, const char* stringCaseType);

//==================================================================
//��������GetImList
//���ߣ�YangJiannan
//���ڣ�2012.12.3
//���ܣ���ü�ʱ��Ϣ�б�
//���������handle					���������
//			stringLocalFilePath		��ʱ��Ϣ�б��ر���·��
//			stringSenderId			������Id
//			stringCaseId			����Id
//			stringCaseType			��������
//			stringMsgDirection		��Ϣ����
//			stringMsgType			��Ϣ����
//����ֵ�� ��������󷵻���
//�޸ļ�¼��
//==================================================================
int NET_API GetImList(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringSenderId, const char* stringCaseId, const char* stringCaseType, const char* stringMsgDirection, const char* stringMsgType);

//==================================================================
//��������GetImList2
//���ߣ�YangJiannan
//���ڣ�2015.7.24
//���ܣ���ü�ʱ��Ϣ�б�2
//���������handle					���������
//			stringLocalFilePath		��ʱ��Ϣ�б��ر���·��
//			stringNextId			��ȥ��Ϣλ�ã��״δ�0
//����ֵ�� ��������󷵻���
//�޸ļ�¼��
//==================================================================
int NET_API GetImList2(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringNextId);

//==================================================================
//��������DownloadIm
//���ߣ�YangJiannan
//���ڣ�2012.12.3
//���ܣ���������Ƶ��Ϣ
//���������handle					���������
//			stringLocalFilePath		����Ƶ��Ϣ�ļ����ر���·��
//			stringMsgId				��ϢId
//			stringUri				����Ƶ��Ϣ�ļ�������·��
//			nFileSize				�ļ���С
//����ֵ�� ��������󷵻���
//�޸ļ�¼��
//==================================================================
int NET_API DownloadIm(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringMsgId, const char* stringUri, unsigned long nFileSize);

//==================================================================
//��������DownloadImComplete
//���ߣ�YangJiannan
//���ڣ�2014.3.21
//���ܣ���������Ƶ��Ϣ
//���������handle					���������
//			stringMsgId				��ϢId
//����ֵ�� ��������󷵻���
//�޸ļ�¼��
//==================================================================
int NET_API DownloadImComplete(NET_HANDLE handle, const char* stringMsgId);

//==================================================================
//��������ChangePwd
//���ߣ�YangJiannan
//���ڣ�2012.12.3
//���ܣ��޸�����
//���������handle					���������
//			stringOldPwd			������
//			stringNewPwd			������
//����ֵ�� ��������󷵻���
//�޸ļ�¼��
//==================================================================
int NET_API ChangePwd(NET_HANDLE handle, const char* stringOldPwd, const char* stringNewPwd);

//==================================================================
//��������Feedbcak
//���ߣ�YangJiannan
//���ڣ�2012.12.3
//���ܣ��ϴ��������
//���������handle					���������
//			stringTitle				����
//			stringNotes				��ϸ����
//			stringPhone				�绰����
//			stringPicPath			Ҫ�ϴ��ı����ļ�·��
//����ֵ�� ��������󷵻���
//�޸ļ�¼��
//==================================================================
int NET_API Feedback(NET_HANDLE handle, const char* stringTitle, const char* stringNotes, const char* stringPhone, const char* stringPicPath);

//==================================================================
//��������GetReportListOfCase
//���ߣ�YangJiannan
//���ڣ�2012.12.3
//���ܣ���ò����ı����б�
//���������handle					���������
//			stringLocalFilePath		�����б��ļ����ر���·��
//			stringCaseId			����Id
//����ֵ�� ��������󷵻���
//�޸ļ�¼��
//==================================================================
int NET_API GetReportListOfCase(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringCaseId);

//==================================================================
//��������GetHospitalListOfGroup
//���ߣ�YangJiannan
//���ڣ�2012.12.3
//���ܣ������֯��ҽԺ�б�
//���������handle					���������
//			stringLocalFilePath		ҽԺ�б��ļ����ر���·��
//			stringGroupId			��֯Id
//			stringType				ҽԺ���ͣ�0��ת��ҽԺ��1����ѡҽԺ��
//			nPageTo					�ڼ�ҳ
//			nPageSize				ÿҳ��������
//����ֵ�� ��������󷵻���
//�޸ļ�¼��
//==================================================================
int NET_API GetHospitalListOfGroup(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringGroupId, const char* stringType, const char* stringCaseType, unsigned int nPageTo, unsigned int nPageSize);

//==================================================================
//��������GetDoctorListOfHospital
//���ߣ�YangJiannan
//���ڣ�2012.12.3
//���ܣ����ҽԺ��ҽ���б�
//���������handle					���������
//			stringLocalFilePath		ҽ���б��ļ����ر���·��
//			stringHospitalId		ҽԺId
//			stringDoctorId			ҽ��Id
//			stringCaseType			��������
//			stringDoctorType		ҽ������
//			nPageTo					�ڼ�ҳ
//			nPageSize				ÿҳ��������
//����ֵ�� ��������󷵻���
//�޸ļ�¼��
//==================================================================
int NET_API GetDoctorListOfHospital(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringHospitalId, const char* stringDoctorId, const char* stringCaseType, const char* stringDoctorType, unsigned int nPageTo, unsigned int nPageSize);


//==================================================================
//��������AbortSession
//���ߣ�YangJiannan
//���ڣ�2012.12.13
//���ܣ���ʱ�˳�Http Session
//���������handle					���������
//����ֵ�� ��������󷵻���
//�޸ļ�¼��
//==================================================================
int NET_API AbortSession(NET_HANDLE handle);

//==================================================================
//��������GetEventStream
//���ߣ�YangJiannan
//���ڣ�2013.01.30
//���ܣ���ò����¼���
//���������handle					���������
//			stringLocalFilePath		�ļ����ر���·��
//			stringCaseId			������
//			stringOriginalHospital	����ԭʼҽԺ
//����ֵ�� ��������󷵻���
//�޸ļ�¼��
//==================================================================
int NET_API GetEventStream(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringCaseId, const char* stringOriginalHospital);

//==================================================================
//��������GetCaseTypeList
//���ߣ�YangJiannan
//���ڣ�2013.01.30
//���ܣ���ò�������Ȩ��
//���������handle					���������
//			stringLocalFilePath		�ļ����ر���·��
//			stringLanguage			ָ������������
//����ֵ�� ��������󷵻���
//�޸ļ�¼��
//==================================================================
int NET_API GetCaseTypeList(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringLanguage);

//==================================================================
//��������DownloadPhoto
//���ߣ�YangJiannan
//���ڣ�2013.4.8
//���ܣ�����ҽ����Ƭ(ͨ���ļ����ؽӿ�)
//���������handle					���������
//			stringLocalFilePath		�����ļ�����·��
//			stringUri				��Ƭ������·��
//			nFileSize				��Ƭ��С
//			pCallback				�ص�����
//�޸ļ�¼��
//==================================================================
int NET_API DownloadPhoto(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringUri, unsigned long nFileSize, pProgressAndSpeedCallback pCallback);

//==================================================================
//��������GetServerFileSize
//���ߣ�YangJiannan
//���ڣ�2018.7.24
//���ܣ���÷������ļ���С
//���������handle					���������
//			stringUri				�ļ�������·��
//���������nFileSize				�ļ���С
//�޸ļ�¼��
//==================================================================
int NET_API GetServerFileSize(NET_HANDLE handle, const char* stringUri, int& nFileSize);
//==================================================================
//��������SendPhoneMsg
//���ߣ�YangJiannan
//���ڣ�2013.4.23
//���ܣ����Ͷ��Žӿ�
//���������handle					���������
//			stringCardId			�û����ţ����û��б��е�senderidֵ
//			stringPhone				�ֻ�����
//			stringMsg				��������
//����˵����stringCardId��stringPhone��Ϊ��ѡ������������������һ����Ϊ���ַ�������stringCardIdΪ�գ�stringPhone��Ϊ��ʱ,
//			�κν�ɫ�����Ե��øýӿڷ��Ͷ��ţ���stringPhoneΪ�գ�stringCardId��Ϊ���ǣ�ҽ�����øýӿڸ����û����ͽ������飻
//			������Ϊ��ʱ����stringCardΪ׼��
//�����ͷ���ֵ����ϸ��Ϣ�μ�Net_Interface_V1.doc��4.5.18
//�޸ļ�¼��
//==================================================================
int NET_API SendPhoneMsg(NET_HANDLE handle, const char* stringCardId, const char* stringPhone, const char* stringMsg);

//==================================================================
//��������GetFileWithHttp
//���ߣ�YangJiannan
//���ڣ�2013.5.13
//���ܣ�ͨ��http�����ļ�����ͨ��php
//���������handle					���������
//			stringLocalFilePath		�ļ����ر���·��
//			stringUrl				�ļ�web url
//			bContinue				�Ƿ�ϵ�������0 ��1 �ǣ�
//����˵�������ӿڲ�ͬ�������κνӿڣ���Ϊ���ӿ�û�ж�Ӧ��PhmsЭ�飬ֻ��ͨ��http�����ļ�����ͨ��phpҳ�棻���ڲ��ӿ�������
//һ��NoPhms��֧�������ڲ���������NoPhms��ʶ���ɲ�ѯ�÷�֧���ýӿ�Ҳ��Ҫһ��handle����ֹ����̹߳���һ��handle������http�������Ӧ���ң�
//���Ǹ�handle���ж�Ӧ��CExternalNetInterface������Ҫsessionid��Ϊ��ͳһ��Ҳͬ����sessionid������ʵ����û���õ�sessionid��
//�޸ļ�¼��
//==================================================================
int NET_API GetFileWithHttp(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringUrl, int bContinue);


//==================================================================
//��������PostForm
//���ߣ�YangJiannan
//���ڣ�2013.4.22
//���ܣ��ύForm���������Ӧ����
//���������handle					���������
//			stringResponse			��Ӧ����buf
//			stringHost				��������ַ
//			stringUrl				ҳ��url
//			stringKey				Post����������
//			stringValue				Post����ֵ����
//			nSize					�����С
//����˵�������ӿڲ�ͬ�������κνӿڣ���Ϊ���ӿ�û�ж�Ӧ��PhmsЭ�飬ֻ��Postһ��Form�������ڲ��ӿ�������
//һ��Form��֧�������ڲ���������Form��ʶ���ɲ�ѯ�÷�֧���ýӿ�Ҳ��Ҫһ��handle����ֹ����̹߳���һ��handle������http�������Ӧ���ң�
//���Ǹ�handle���ж�Ӧ��CExternalNetInterface������Ҫsessionid��Ϊ��ͳһ��Ҳͬ����sessionid������ʵ����û���õ�sessionid��
//�����ͷ���ֵ����ϸ��Ϣ�μ�Net_Interface_V1.doc��4.5.18
//�޸ļ�¼��
//==================================================================
int NET_API PostForm(NET_HANDLE handle, char* stringResponse, unsigned int nResponseSize, const char* stringHost, const char* stringUrl, const char* stringKey[], const char* stringValue[], int nSize);

//==================================================================
//��������GetVerificationCode
//���ߣ�YangJiannan
//���ڣ�2013.10.17
//���ܣ����֤��ע��
//���������handle					���������
//			stringType				�����������
//			stringPhone				�绰����
//����ֵ�� ��������󷵻���
//���øýӿڣ�2013.12.11
//�ӿ���������:2014.1.4
//==================================================================
int NET_API GetVerificationCode(NET_HANDLE handle, const char* stringType, const char* stringPhone);

//==================================================================
//��������ApplyQRCode
//���ߣ�YangJiannan
//���ڣ�2014.12.10
//���ܣ������ά��
//���������handle					���������
//			stringLocalFilePath		�����ļ�����·��
//			stringType				��ά������
//			stringScene				��ά��ʹ�ó���
//����ֵ�� ��������󷵻���
//==================================================================
int NET_API ApplyQRCode(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringType, const char* stringScene);

//==================================================================
//��������ValidateUserAccount
//���ߣ�YangJiannan
//���ڣ�2014.12.10
//���ܣ������ά��
//���������handle					���������
//			stringLocalFilePath		�����ļ�����·��
//			stringLoginId			�û���
//			stringPassword			����
//			stringRole				�û���ɫ
//����ֵ�� ��������󷵻���
//��ע��ǿ�Ƽ��ܴ��䣻����֤ǩ����sessionid
//==================================================================
int NET_API ValidateUserAccount(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringLoginId, const char* stringPassword, const char* stringRole);

//==================================================================
//��������UploadCommonFile
//���ߣ�YangJiannan
//���ڣ�2015.6.29
//���ܣ�ͨ���ϴ��ļ�
//���������handle					���������
//			stringType				�ļ�����
//			stringExtra				������Ϣ
//			stringLocalFilePath		�����ļ�·��
//			stringFileType			�ļ����ͣ�0��pdf+bin   1���ĵ�ͼ������+bin    �գ�������
//			pCallbcak				�ص�����
//���������stringUri				�������ļ�����·��
//����ֵ�� ��������󷵻���
//�޸ļ�¼��
//==================================================================
int NET_API UploadCommonFile(NET_HANDLE handle, char* stringUri, const char* stringType, const char* stringExtra, const char* stringLocalFilePath, const char* stringFileType, pProgressAndSpeedCallback pCallbcak);


//==================================================================
//��������GetListOfHealthyKnowledge
//���ߣ�YangJiannan
//���ڣ�2015.7.23
//���ܣ���ý���֪ʶ�б�
//���������handle					���������
//			stringLocalFilePath		�����ļ�����·��
//			stringSenderId			ҽ��SenderId
//			stringNextId			��ȡλ��
//			stringIds				����֪ʶID
//			stringPageSize			ÿ����ȡ����
//			stringOrderBy			0������  1������
//����ֵ�� ��������󷵻���
//�޸ļ�¼��
//==================================================================
int NET_API GetListOfHealthyKnowledge(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringSenderId, const char* stringNextId, const char* stringIds, const char* stringPageSize, const char* stringOrderBy);


//==================================================================
//��������GetCardUserReportList
//���ߣ�YangJiannan
//���ڣ�2015.9.22
//���ܣ���ÿ��û������б�
//���������handle					���������
//			stringLocalFilePath		�����ļ�����·��
//			stringReportId			����Id
//			stringReportStartDate	���濪ʼʱ��
//			stringReportEndDate		�������ʱ��
//			stringCaseType			��������
//			stringState				����״̬
//			stringOrderBy			����ʽ
//			stringThirdIdType		������������
//			stringThirdId			��������
//			stringNextId			��ȡ�б���ȡ��λ��
//			stringPageSize			ÿ�η�����������
//����ֵ�� ��������󷵻���
//�޸ļ�¼��
//==================================================================
int NET_API GetCardUserReportList(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringReportId, const char* stringReportStartDate, const char* stringReportEndDate, const char* stringCaseType, const char* stringState, const char* stringOrderBy, 
		const char* stringThirdIdType, const char* stringThirdId, const char* stringNextId, const char* stringPageSize);

//==================================================================
//��������MyFans
//���ߣ�YangJiannan
//���ڣ�2015.6.29
//���ܣ��ҵķ�˿
//���������handle					���������
//			stringLocalFilePath		�����ļ�����·��
//			stringUserId			�û�ID
//			stringNextId			��ȡ�б��λ��
//����ֵ�� ��������󷵻���
//
//==================================================================
int NET_API MyFans(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringUserId, const char* stringNextId);

//==================================================================
//��������GetPayCode
//���ߣ�YangJiannan
//���ڣ�2018.1.17
//���ܣ���õ�����֧����
//���������handle					���������
//			stringLocalFilePath		�����ļ�����·��
//			stringUserId			�û�ID
//			stringNextId			��ȡ�б��λ��
//����ֵ�� ��������󷵻���
//
//==================================================================
int NET_API GetPayCode(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringType, const char* stringCaseId);

//==================================================================
//��������GetOrderInfo
//���ߣ�YangJiannan
//���ڣ�2018.1.17
//���ܣ���ö�����Ϣ
//���������handle					���������
//			stringLocalFilePath		�����ļ�����·��
//			stringUserId			�û�ID
//			stringNextId			��ȡ�б��λ��
//����ֵ�� ��������󷵻���
//
//==================================================================
int NET_API GetOrderInfo(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringType, const char* stringCaseId);

//==================================================================
//��������AbortSessionByThreadId
//���ߣ�YangJiannan
//���ڣ�2014.12.11
//���ܣ���ʱ�˳�Http Session
//���������threadId				�����߳�id
//����ֵ�� ��������󷵻���
//==================================================================
#if defined(_WIN32) || defined(_WIN64)
#define THREAD_ID unsigned long
#else
#define THREAD_ID unsigned long int
#endif
int NET_API AbortSessionByThreadId(THREAD_ID threadId);

//==================================================================
//��������BeginMsgListPush
//���ߣ�YangJiannan
//���ڣ�2013.6.19
//���ܣ���ʼ��ʱ��Ϣ�б�����
//���������stringSenderId			������Id
//			stringCaseId			����Id
//			stringCaseType			��������
//			stringMsgDirection		��Ϣ����
//			stringMsgType			��Ϣ����
//			stringVersion			���˲����⣬��������ȫ������
//			pCallback				���ͳɹ��ص�����
//			pFailCallback			����ʧ�ܻص�����
//����ֵ�� ���������
//�����ͷ���ֵ����ϸ��Ϣ�μ�Net_Interface_V1.doc��4.5.20
//��ע��pCallback����ʱ�����ٷ������ɹ�������Ϣ �ڳɹ������������ӣ����������ֱ�ΪNULL, 0��
//		pFailCallback����ʱ�����ٴ�����COMMON_SESSION_INVALID��COMMON_LOGIN_ANOTHER_PLACE��COMMON_SERVER_NO_SUPPORT��COMMON_INVALID_CHARSET��COMMON_NET_CONFIG_ERROR��
//								COMMON_URL_NOT_FOUND��COMMON_NO_NET_CONFIG_FILE���ò����������̢߳�����������ֻ���ú����ԣ������������߳�
//==================================================================
NET_HANDLE NET_API BeginMsgListPush(const char* stringSenderId, const char* stringCaseId, const char* stringCaseType, const char* stringMsgType, const char* stringMsgDirection, const char* stringVersion, 
									pHttpPushCallback pCallback, pHttpPushFailCallback pFailCallback);

//==================================================================
//��������EndPush
//���ߣ�YangJiannan
//���ڣ�2012.12.3
//���ܣ��������ͣ��������κ����ͣ�ֻҪ����ж�Ӧ
//���������handle					���������
//����ֵ�� ��������󷵻���
//�����ͷ���ֵ����ϸ��Ϣ�μ�Net_Interface_V1.doc��4.5.21
//�޸ļ�¼��
//��ע��������BeginMsgListPush�д������ȷ�ص��ʹ���ص��е��ô˺���������������
//==================================================================
int NET_API EndPush(NET_HANDLE handle);


//==================================================================
//��������ExecuteGWService
//���ߣ�YangJiannan
//���ڣ�2018.3.5
//���ܣ�����������
//���������handle					���������
//			stringLocalFilePath		�ļ����ر���·��
//			stringLastPath			����url��path���ֵ����һ���֣�����https://<server ip or domain>/healthApi/v1/login�е�login
//			stringJson				json
//			stringUploadFilePath	Ҫ�ϴ��ļ��ı���·��
//����ֵ�� ��������󷵻���
//==================================================================
int NET_API ExecuteGWService(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringLastPath, const char* stringJson, const char* stringUploadFilePath);


//==================================================================
//��������ExecuteCommonRESTful
//���ߣ�YangJiannan
//���ڣ�2018.3.5
//���ܣ�����������
//���������handle					���������
//			stringLocalFilePath		�ļ����ر���·��
//			stringIp				Ŀ���������ַ
//			stringPort				Ŀ��������˿�
//			stringUrl				Ŀ�������url���˿ں�Ĳ��֣�����"http://ip:port/Collect/InitDevice"�е�/Collect/InitDevice 
//			stringContentType		http����ͷ�е�Content-Type������text/json
//			stringPayload			��Ч����
//����ֵ�� ��������󷵻���
//==================================================================
int NET_API ExecuteCommonRESTful(NET_HANDLE handle, const char* stringLocalFilePath, const char* stringIp, const char* stringPort, const char* stringUrl, const char* stringContentType, const char* stringPayload);

#ifdef __cplusplus 
	} 
#endif 

#endif