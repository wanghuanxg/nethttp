#include "InternalNetIntetface.h"
#include "Poco/Random.h"
#include "Poco/Buffer.h"
#include "Poco/Exception.h"
#include "Poco/MD5Engine.h"
#include "Poco/Util/IniFileConfiguration.h"
#include "Poco/Timer.h"
#include "Poco/AutoPtr.h"
#include "Poco/Thread.h"
#include "Poco/ThreadPool.h"
#include "Poco/File.h"
#include "Poco/StreamCopier.h"
#include "Poco/UUIDGenerator.h"
#include "Poco/Buffer.h"
#include "Poco/HMACEngine.h"
#include "Poco/SHA1Engine.h"
#include "Poco/Crypto/Cipher.h"
#include "Poco/Crypto/CipherFactory.h"
#include "Poco/Crypto/CipherKey.h"
#include <fstream>
#include <sstream>
#include "ErrorCode.h"
#include "PhmsLogger.h"
#include "Util.h"
#include "UploadTimerCallback.h"
#include "DownloadTimerCallback.h"
#include "DownloadThreadCallback.h"
#include "SessionIdManager.h"
#include <vector>
#include <map>

using Poco::Random;
using Poco::MD5Engine;
using Poco::Exception;
using Poco::DigestEngine;
using Poco::Util::IniFileConfiguration;
using Poco::Timer;
using Poco::TimerCallback;
using Poco::AutoPtr;
using Poco::Thread;
using Poco::ThreadPool;
using Poco::File;
using Poco::StreamCopier;
using Poco::UUIDGenerator;
using Poco::Buffer;
using Poco::HMACEngine;
using Poco::DigestEngine;
using Poco::SHA1Engine;
using Poco::Crypto::Cipher;
using Poco::Crypto::CipherFactory;
using Poco::Crypto::CipherKey;
using namespace std;

CInternalNetIntetface::CInternalNetIntetface(void):m_nSaveType(0)
{
	m_pPhmsSession = new CPhmsSession;
	m_eventSocketAbort.reset();
}

CInternalNetIntetface::~CInternalNetIntetface(void)
{
	delete m_pPhmsSession;
}

//Setter
void CInternalNetIntetface::SetSaveType(int nSaveType)
{
	this->m_nSaveType = nSaveType;
}
void CInternalNetIntetface::SetFilePath(string stringFilePath)
{
	this->m_stringFilePath = stringFilePath;
}
void CInternalNetIntetface::SetSessionId(string stringSessionId)
{
	m_pPhmsSession->SetSessionId(stringSessionId);
}
void CInternalNetIntetface::SetCommandType(EnumPhmsCommandType enumCommandType)
{
	m_pPhmsSession->SetCommandType(enumCommandType);
}
void CInternalNetIntetface::SetGWSessionId(string stringSessionId)
{
	m_pPhmsSession->SetGWSessionId(stringSessionId);
}
//Getter
int CInternalNetIntetface::GetSaveType()
{
	return m_nSaveType;
}
string CInternalNetIntetface::GetContent()
{
	return m_stringContent;
}
string CInternalNetIntetface::GetSessionId()
{
	return m_pPhmsSession->GetSessionId();
}
EnumPhmsCommandType CInternalNetIntetface::GetCommandType()
{
	return m_pPhmsSession->GetCommandType();
}
string CInternalNetIntetface::GetIp()
{
	return m_stringIp;
}
string CInternalNetIntetface::GetGWSessionId()
{
	return m_pPhmsSession->GetGWSessionId();
}

int CInternalNetIntetface::LoadServerPathAndAlreadyUploadSize(string stringLocalIniPath, string& stringServerPath, string& stringAlreadyUploadSize)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	File fileUploadIniConfig(stringLocalIniPath, "gbk");
	if(!fileUploadIniConfig.exists())
	{
		try
		{
			fileUploadIniConfig.createFile();
		}
		catch(Exception& e)
		{
			CPhmsLogger::GetPhmsLogger()->WriteLog(e, __FUNCTION__, __FILE__, __LINE__);
			CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_FILE_OPEN_FAIL), __FUNCTION__, __FILE__, __LINE__);
			return COMMON_FILE_OPEN_FAIL;
		}
	}
	AutoPtr<IniFileConfiguration> iniFile = new IniFileConfiguration(stringLocalIniPath, "gbk");
	//unsigned nAlreadyUploadSize = 0;
	stringServerPath = iniFile->getString("URI", "");
	if(stringServerPath.size() != 0)
	{
		this->SetSaveType(1);
		nRet = this->GetAlreadyUploadFileSize(stringServerPath);
		if(nRet != PHMS_SUCCESSFUL_RESULT)
		{
			CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
			return nRet;
		}
		stringAlreadyUploadSize = this->GetContent();
		return StoreServerPathAndAlreadyUploadSize(stringLocalIniPath, stringServerPath, stringAlreadyUploadSize);
	}
	return PHMS_SUCCESSFUL_RESULT;
}
int CInternalNetIntetface::StoreServerPathAndAlreadyUploadSize(string stringLocalIniPath, string stringServerPath, string stringAlreadyUploadSize)
{
	//IniFileConfiguration类只实现了load，未实现store，所以setXXXX函数只修改内存，不能保存到具体流（文件）,所以不能用IniFileConfiguration.setString
	//iniFile->setString("URI", stringServerPath);
	//iniFile->setString("ALREADY_UPLOAD_SIZE", stringAlreadyUploadSize);
	ofstream oFstream;
	oFstream.exceptions(ios_base::failbit|ios_base::badbit);
	try
	{
		oFstream.open(stringLocalIniPath.c_str(), ios_base::binary|ios_base::trunc);
		oFstream.write("URI=", 4);
		oFstream.write(stringServerPath.c_str(), (std::streamsize)stringServerPath.size());
		oFstream.write("\n", 1);
		oFstream.write("ALREADY_UPLOAD_SIZE=", 20);
		oFstream.write(stringAlreadyUploadSize.c_str(), (std::streamsize)stringAlreadyUploadSize.size());
		oFstream.write("\n", 1);
		oFstream.close();
	}
	catch(ios::failure& e)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(e.what(), __FUNCTION__, __FILE__, __LINE__);
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_FILE_WRITE_FAIL), __FUNCTION__, __FILE__, __LINE__);
		return COMMON_FILE_WRITE_FAIL;
	}
	return PHMS_SUCCESSFUL_RESULT;
}


int CInternalNetIntetface::GetOrder(unsigned long nTotalSize, unsigned long nAlreadyUploadSize, unsigned long nThisTimeUploadSize, string& stringOrder)
{
	//获得stringOrder参数
	if(nAlreadyUploadSize==0 && nTotalSize==nThisTimeUploadSize)
	{
		stringOrder = "0";
	}
	if(nAlreadyUploadSize==0 && nTotalSize>nThisTimeUploadSize)
	{
		stringOrder = "1";
	}
	if(nAlreadyUploadSize>0 && nTotalSize>nAlreadyUploadSize+nThisTimeUploadSize)
	{
		stringOrder = "2";
	}
	if(nAlreadyUploadSize>0 && nTotalSize==nAlreadyUploadSize+nThisTimeUploadSize)
	{
		stringOrder = "3";
	}
	return PHMS_SUCCESSFUL_RESULT;
}
int CInternalNetIntetface::GetUploadSegmentInfo(int nSegmentCount, unsigned long nAlreadyUploadSize, unsigned long nThisTimeUploadSize, string stringLocalPath, vSegmentInfo& vSI)
{
	//获得stringStart、stringEnd、stringMd5系列参数
	//****
	//注意：是取nAlreadyUploadSize+nThisTimeUploadSize部分中的三段，不是nThisTimeUploadSize中的三段,取nThisTimeUploadSize中三段的代码没删，注释了
	//****
	unsigned long nPerPartSize = 0;
	unsigned long nModeSize;
// 	nPerPartSize = nThisTimeUploadSize/3;;//--------用于取nThisTimeUploadSize的三段
// 	nModeSize = nThisTimeUploadSize%3;;//--------用于取nThisTimeUploadSize的三段
	nPerPartSize = (nAlreadyUploadSize+nThisTimeUploadSize)/3;
	nModeSize = (nAlreadyUploadSize+nThisTimeUploadSize)%3;

	string stringStartX;
	string stringEndX;
	string stringMd5X;
	
	//unsigned long nPartXEnd = nAlreadyUploadSize-1;//用于取nThisTimeUploadSize的三段
	unsigned long nPartXEnd = 0-1;
	unsigned long nPartXStart, nPartXSize, nRandomX, nStartX, nEndX;
	for(int i=0; i<nSegmentCount; i++)
	{
		//三段长度的分法：nPerPartSize, nPerPartSize, nPerPartSize+nModeSize
		nPartXStart = nPartXEnd+1;
		if(i == nSegmentCount-1)
		{
			//最后一段加模数
			nPartXEnd = nPartXStart+nPerPartSize+nModeSize-1;
		}
		else
		{
			nPartXEnd = nPartXStart+nPerPartSize-1;
		}
		nPartXSize = nPartXEnd-nPartXStart+1;
		//如果段长度大于1024，则随机取nStartX和nEndX;否则取段的全部
		if(nPartXSize > 1024)
		{
			Random random;
			random.seed();
			nRandomX = random.next(nPartXSize-1024+1);
			nStartX = nPartXStart+nRandomX;
			nEndX = nStartX+1024-1;
		}
		else
		{
			nStartX = nPartXStart;
			nEndX = nPartXEnd;
		}
		//计算md5
		char* bufSrcX = NULL;
		unsigned long nLengthX = nEndX-nStartX+1;
		bufSrcX = new char[nLengthX+1];
		memset(bufSrcX, 0, nLengthX+1);
		ifstream iFStreamX;
		iFStreamX.exceptions(ios::failbit|ios::badbit);
		try
		{
			//open函数默认文本打开，如果文本打开的话读到0就相当于eof了，所以指定ios_base::binary
			iFStreamX.open(stringLocalPath.c_str(), ios_base::binary|ios_base::in);
			iFStreamX.seekg(nStartX, std::ios_base::beg);
			//int nTempLengthX = 0;
			iFStreamX.read(bufSrcX, nLengthX);
			iFStreamX.close();
		}
		catch (ios::failure& e)
		{
			//写日志
			//int nReadCount = iFStreamX.gcount();
			delete bufSrcX;
			CPhmsLogger::GetPhmsLogger()->WriteLog(e.what(), __FUNCTION__, __FILE__, __LINE__);
			CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_FILE_READ_FAIL), __FUNCTION__, __FILE__, __LINE__);
			return COMMON_FILE_READ_FAIL;
		}
		stringMd5X = CUtil::GetMd5(bufSrcX, nLengthX);
		delete bufSrcX;

		stringStartX = CUtil::ULongToString(nStartX);

		stringEndX = CUtil::ULongToString(nEndX);

		SEGMENT_INFO si;
		si.m_stringStart = stringStartX;
		si.m_stringEnd = stringEndX;
		si.m_stringMd5 = stringMd5X;
		vSI.insert(vSI.end(), si);
	}
	return PHMS_SUCCESSFUL_RESULT;
}


int CInternalNetIntetface::GenerateTempUploadFile(int nAlreadyUploadSize, int nThisTimeUploadSize, string stringLocalFilePath, string& stringTempUploadFilePath)
{
	string stringUuid = UUIDGenerator::defaultGenerator().createOne().toString();
	stringTempUploadFilePath = stringLocalFilePath+"."+stringUuid;
	File fileTempUploadFile(stringTempUploadFilePath, "gbk");
	try
	{
		fileTempUploadFile.createFile();
	}
	catch(Exception& e)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(e, __FUNCTION__, __FILE__, __LINE__);
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_FILE_OPEN_FAIL), __FUNCTION__, __FILE__, __LINE__);
		return COMMON_FILE_OPEN_FAIL;
	}
	ifstream iFstream;
	ofstream oFstream;
	iFstream.exceptions(ios_base::eofbit|ios_base::failbit|ios_base::badbit);
	oFstream.exceptions(ios_base::failbit|ios_base::badbit);
	try
	{
		iFstream.open(stringLocalFilePath.c_str(), ios_base::binary);
		oFstream.open(stringTempUploadFilePath.c_str(), ios_base::binary|ios_base::trunc);
		iFstream.seekg(nAlreadyUploadSize, ios_base::beg);
		Buffer<char> bufFileContent(nThisTimeUploadSize);
		iFstream.read(bufFileContent.begin(), nThisTimeUploadSize);
		oFstream.write(bufFileContent.begin(), nThisTimeUploadSize);
		iFstream.close();
		oFstream.close();
	}
	catch(ios_base::failure& e)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(e.what(), __FUNCTION__, __FILE__, __LINE__);
		if(iFstream.eof() || iFstream.fail() || iFstream.bad())
		{
			CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_FILE_READ_FAIL), __FUNCTION__, __FILE__, __LINE__);
			return COMMON_FILE_READ_FAIL;
		}
		else
		{
			CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_FILE_WRITE_FAIL), __FUNCTION__, __FILE__, __LINE__);
			return COMMON_FILE_WRITE_FAIL;
		}
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::GetDownloadTempFileSize(string stringTempDirectory, int nThreadCount, unsigned long* pTempFileSize)
{
	//创建目录，并扫描临时文件，获得每个临时文件的大小，以方便继续下载
	File fileDir(stringTempDirectory, "gbk");
	try
	{
		fileDir.createDirectory();
	}
	catch(const Exception& e)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(e.what(), __FUNCTION__, __FILE__, __LINE__);
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_CREATE_DIR_FAIL), __FUNCTION__, __FILE__, __LINE__);
		return COMMON_CREATE_DIR_FAIL;
	}
	vector<File> vTempFile;
	vector<File>::const_iterator iter;
	fileDir.list(vTempFile);
	unsigned int nThreadNum = 0;
	for(nThreadNum=0; nThreadNum<nThreadCount; nThreadNum++)
	{
		for(iter=vTempFile.begin(); iter!=vTempFile.end(); iter++)
		{
			if(iter->path().find("PHMS_TEMP_"+CUtil::UIntToString(nThreadNum)) != string::npos)
			{
				pTempFileSize[nThreadNum] = (unsigned long)iter->getSize();
				break;
			}
		}
		if(iter == vTempFile.end())
		{
			pTempFileSize[nThreadNum] = 0;
		}
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::DivideSegment(unsigned long nRemainFileSize, unsigned long nSegmentMaxSize, int& nThreadCount, unsigned long* pSegmentFileSize)
{
	if(nRemainFileSize/(nSegmentMaxSize*nThreadCount) > 0)
	{
		for(int i=0; i<nThreadCount; i++)
		{
			pSegmentFileSize[i] = nSegmentMaxSize;
		}
	}
	else
	{
		//缺点：没有智能的判断文件过小不需要nThreadCount个线程
		//比如剩余2 bytes，要求3 Threads，程序也会创建两个线程（两个请求），去分别要一个字节。这种情况还是非常少见的，以后有时间再优化
		if(nRemainFileSize < nThreadCount)
		{
			//特殊情况，剩余文件size小于线程数
			for(int i=0; i<nRemainFileSize; i++)
			{
				pSegmentFileSize[i] = 1;
			}
			nThreadCount = nRemainFileSize;
		}
		else
		{
			//剩余文件size大于线程数,但小于nSegmentMaxSize*nThreadCount
			for(int i=0; i<nThreadCount-1; i++)
			{
				pSegmentFileSize[i] = nRemainFileSize/nThreadCount;
			}
			pSegmentFileSize[nThreadCount-1] = nRemainFileSize/nThreadCount + nRemainFileSize%nThreadCount;
		}
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::CombineSegmentFile(string stringLocalFile, string stringTempDirectory, int nThreadCount)
{
	ifstream iFstream;
	ofstream oFstream;
	iFstream.exceptions(ios_base::goodbit);
	oFstream.exceptions(ios_base::eofbit|ios_base::failbit);
	File fileDir(stringTempDirectory, "gbk");
	try
	{
		oFstream.open(stringLocalFile.c_str(), ios_base::out|ios_base::app|ios_base::binary);
		for(int i=0; i<nThreadCount; i++)
		{
			
			string stringTempFile;
			stringTempFile = stringTempDirectory+"/"+"PHMS_TEMP_"+CUtil::UIntToString(i);
			iFstream.open(stringTempFile.c_str(), ios_base::in|ios_base::binary);
			StreamCopier::copyStream(iFstream, oFstream);
			iFstream.clear();//必须清状态，否则下次打开失败
			iFstream.close();
			File fileTempFile(stringTempFile, "gbk");
			fileTempFile.remove();
		}
		
	}
	catch(ios_base::failure& e)
	{
		//写日志
		//iFstream.close();
		//oFstream.close();
		CPhmsLogger::GetPhmsLogger()->WriteLog(e.what(), __FUNCTION__, __FILE__, __LINE__);
		CPhmsLogger::GetPhmsLogger()->WriteLog(stringLocalFile, __FUNCTION__, __FILE__, __LINE__);
		fileDir.remove(true);
		if(oFstream.bad())
		{
			CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_COMBILE_FILE_FAIL), __FUNCTION__, __FILE__, __LINE__);
			return COMMON_COMBILE_FILE_FAIL;
		}
		else
		{
			CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_COMBILE_FILE_FAIL_BUT_CAN_RETRY), __FUNCTION__, __FILE__, __LINE__);
			return COMMON_COMBILE_FILE_FAIL_BUT_CAN_RETRY;
		}
	}
	oFstream.close();
	try
	{
		fileDir.remove(true);
	}
	catch(const Exception&)
	{
		//防止发生和下载定时器回调遍历文件夹冲突的情况下抛出异常，发生冲突即使删不掉也无所谓。
		//几率非常小，产生的后果影响也非常小（多一个空文件夹），所以就暂时不用互斥量控制了
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::Login(string stringRole, string stringUsername, string stringPassword, string stringLanguage, string stringToken, bool bMustSsl)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->Login(stringRole, stringUsername, stringPassword, stringLanguage, stringToken, bMustSsl);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		if(!m_eventSocketAbort.tryWait(500))
		{
			if(nRet==COMMON_SEND_FAIL || nRet==COMMON_SEND_TIMEOUT || nRet==COMMON_RECV_FAIL || nRet==COMMON_RECV_TIMEOUT)
			{
				m_pPhmsSession->SetUseIp(true);
				nRet = m_pPhmsSession->Login(stringRole, stringUsername, stringPassword, stringLanguage, stringToken, bMustSsl);
				m_pPhmsSession->SetUseIp(false);
				if(nRet != PHMS_SUCCESSFUL_RESULT)
				{
					CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
					return nRet;
				}
				else
				{
					if(m_nSaveType == 1)
					{
						m_stringContent = m_pPhmsSession->GetContent();
					}
					m_stringIp = m_pPhmsSession->GetIp();
					return PHMS_SUCCESSFUL_RESULT;
				}
			}
		}
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	m_stringIp = m_pPhmsSession->GetIp();
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::MechineRegister(string stringCountry, string stringAreaId, string stringArea, string stringDevice, string stringSignature, string stringSerialnum)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->MechineRegister(stringCountry, stringAreaId, stringArea, stringDevice, stringSignature, stringSerialnum);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::Logout()
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->Logout();
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}
int CInternalNetIntetface::SetDoctorSign(string stringFilePath)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	string stringMd5;
	nRet = CUtil::GetMd5(stringFilePath, stringMd5);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = m_pPhmsSession->SetDoctorSign(stringMd5, stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::SetDoctorPhoto(string stringFilePath)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	string stringMd5;
	nRet = CUtil::GetMd5(stringFilePath, stringMd5);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = m_pPhmsSession->SetDoctorPhoto(stringMd5, stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::ApplyNewCase(string stringName, string stringThirdId, string stringDataType, string stringDataTypeName, string stringNotes, string stringHospitalId, 
				 string stringHospitalName, string stringDoctorId, string stringDoctorName, string stringCheckTime, string stringOtherParam, string stringDeviceId, string stringDeviceName, 
				 string stringMd5, string stringCaseOrigin, string stringYzlb, string stringSex, string stringApplicationNo, string stringAutoMatic)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->ApplyNewCase(stringName, stringThirdId, stringDataType, stringDataTypeName, stringNotes, stringHospitalId, stringHospitalName, 
		stringDoctorId, stringDoctorName, stringCheckTime, stringOtherParam, stringDeviceId, stringDeviceName, stringMd5, stringCaseOrigin, stringYzlb, stringSex, stringApplicationNo, stringAutoMatic);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::UploadCaseSegment(string stringCaseId, unsigned long nTotalSize,  unsigned long nAlreadyUploadSize, 
					  unsigned long nThisTimeUploadSize, string stringServerPath, string stringLocalPath)
{
	//验证参数
	if(nTotalSize<=0 || nAlreadyUploadSize<0 || nThisTimeUploadSize<=0 ||nTotalSize<nAlreadyUploadSize+nThisTimeUploadSize)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_PARAM_VALIDATE_FAIL), __FUNCTION__, __FILE__, __LINE__);
		return COMMON_PARAM_VALIDATE_FAIL;
	}
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	
	//获得stringTotalSize参数
	string stringTotalSize;
	stringTotalSize = CUtil::ULongToString(nTotalSize);
	//获得stringOrder
	string stringOrder;
	this->GetOrder(nTotalSize, nAlreadyUploadSize, nThisTimeUploadSize, stringOrder);
	//获得Start\End\Md5信息
	vSegmentInfo vSi;
	nRet = this->GetUploadSegmentInfo(3, nAlreadyUploadSize, nThisTimeUploadSize, stringLocalPath, vSi);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	string stringTempUploadFilePath;
	nRet = this->GenerateTempUploadFile(nAlreadyUploadSize, nThisTimeUploadSize, stringLocalPath, stringTempUploadFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = m_pPhmsSession->UploadCaseSegment(stringCaseId, stringOrder, stringTotalSize, stringServerPath, vSi[0].m_stringStart, vSi[0].m_stringEnd, 
		vSi[0].m_stringMd5, vSi[1].m_stringStart, vSi[1].m_stringEnd, vSi[1].m_stringMd5, vSi[2].m_stringStart, vSi[2].m_stringEnd, 
		vSi[2].m_stringMd5, stringTempUploadFilePath);
	try
	{
		File fileTempUploadFile(stringTempUploadFilePath, "gbk");
		fileTempUploadFile.remove(false);
	}
	catch(const Exception&)
	{

	}
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::CaseHoldPlace(string stringCaseId, string stringReceiverId)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->CaseHoldPlace(stringCaseId, stringReceiverId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::GetAutoDownloadCase(string stringCaseType,string stringSenderId,string stringCaseOrigin)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->GetAutoDownloadCase( stringCaseType, stringSenderId, stringCaseOrigin);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::DownloadCaseBegin(string stringCaseId, string stringReceiverId)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->DownloadCaseBegin(stringCaseId, stringReceiverId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::DownloadFileSegment(string stringUri, unsigned long nBeginPos, unsigned long nEndPos)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	string stringStart = CUtil::ULongToString(nBeginPos);
	string stringEnd = CUtil::ULongToString(nEndPos);
	nRet = m_pPhmsSession->DownloadFileSegment(stringUri, stringStart, stringEnd);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::DownloadCaseComplete(string stringCaseId, string stringReceiverId)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->DownloadCaseComplete(stringCaseId, stringReceiverId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::InvalidCase(string stringCaseId, string stringReceiverId, string stringReason)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->InvalidCase(stringCaseId, stringReceiverId, stringReason);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::TransmitCase(string stringCaseId, string stringReceiverId, string stringForwardToH, string stringForwardToHospitalName, 
										string stringForwardToD, string stringForwardToDoctorName)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->TransmitCase(stringCaseId, stringReceiverId, stringForwardToH, stringForwardToHospitalName, stringForwardToD, stringForwardToDoctorName);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::RollbackCase(string stringCaseId, string stringReceiverId, string stringBackTo)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->Rollback(stringCaseId, stringReceiverId, stringBackTo);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::GetHospitalListOfGroup(string stringHGroupId, string stringType, string stringCaseType, unsigned int nPageTo, unsigned int nPageSize)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	string stringPageTo = CUtil::UIntToString(nPageTo);
	string stringPageSize = CUtil::UIntToString(nPageSize);
	nRet = m_pPhmsSession->GetHospitalListOfGroup(stringHGroupId, stringType, stringCaseType, stringPageTo, stringPageSize);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::GetDoctorListOfHospital(string stringHospitalId, string stringDoctorId, string stringCaseType, string stringDoctorType, unsigned int nPageTo, unsigned int nPageSize)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	string stringPageTo = CUtil::UIntToString(nPageTo);
	string stringPageSize = CUtil::UIntToString(nPageSize);
	nRet = m_pPhmsSession->GetDoctorListOfHospital(stringHospitalId, stringDoctorId, stringCaseType, stringDoctorType, stringPageTo, stringPageSize);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::AnalyzeBegin(string stringCaseId, string stringReceiverId, string stringCaseType)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->AnalyzeBegin(stringCaseId, stringReceiverId, stringCaseType);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::AnalyzeComplete(string stringCaseId, string stringReceiverId, string stringCaseType)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->AnalyzeComplete(stringCaseId, stringReceiverId, stringCaseType);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::ApplyNewReport(string stringCaseId, string stringReceiverId, string stringDiagnosis, string stringMd5, string stringDigitalSign, string stringTimestampSign, string stringCaseType)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->ApplyNewReport(stringCaseId, stringReceiverId, stringDiagnosis, stringMd5, stringDigitalSign, stringTimestampSign, stringCaseType);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::UploadReportSegment(string stringCaseId, string stringReceiverId, string stringCaseType, string stringReportId, unsigned long nTotalSize,
						unsigned long nAlreadyUploadSize, unsigned long nThisTimeUploadSize, string stringServerPath, string stringLocalPath, string stringFileType)
{
	//验证参数
	if(nTotalSize<=0 || nAlreadyUploadSize<0 || nThisTimeUploadSize<=0 ||nTotalSize<nAlreadyUploadSize+nThisTimeUploadSize)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_PARAM_VALIDATE_FAIL), __FUNCTION__, __FILE__, __LINE__);
		return COMMON_PARAM_VALIDATE_FAIL;
	}
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);

	//获得stringTotalSize参数
	string stringTotalSize;
	stringTotalSize = CUtil::ULongToString(nTotalSize);
	//获得stringOrder
	string stringOrder;
	this->GetOrder(nTotalSize, nAlreadyUploadSize, nThisTimeUploadSize, stringOrder);
	//获得Start\End\Md5信息
	vSegmentInfo vSi;
	nRet = this->GetUploadSegmentInfo(3, nAlreadyUploadSize, nThisTimeUploadSize, stringLocalPath, vSi);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	string stringTempUploadFilePath;
	nRet = this->GenerateTempUploadFile(nAlreadyUploadSize, nThisTimeUploadSize, stringLocalPath, stringTempUploadFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = m_pPhmsSession->UploadReportSegment(stringCaseId, stringReceiverId, stringCaseType, stringReportId, stringOrder, stringTotalSize, stringServerPath, vSi[0].m_stringStart, vSi[0].m_stringEnd, 
		vSi[0].m_stringMd5, vSi[1].m_stringStart, vSi[1].m_stringEnd, vSi[1].m_stringMd5, vSi[2].m_stringStart, vSi[2].m_stringEnd, 
		vSi[2].m_stringMd5, stringTempUploadFilePath, stringFileType);
	try
	{
		File fileTempUploadFile(stringTempUploadFilePath, "gbk");
		fileTempUploadFile.remove(false);
	}
	catch(const Exception&)
	{

	}
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::DownloadReportBegin(string stringCaseId, string stringReportId)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->DownloadReportBegin(stringCaseId, stringReportId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::DownloadReportComplete(string stringCaseId, string stringReportId)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->DownloadReportComplete(stringCaseId, stringReportId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::GetAlreadyUploadFileSize(string stringUri)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->GetAlreadyUploadFileSize(stringUri);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::GetReviewReportList(string stringReportStartDate, string stringReportEndDate,string stringReviewStartDate,
											   string stringReviewEndDate, string stringReviewState, string stringCaseType, string stringPtName, string stringSenderId, string stringYzlb, string stringAId, 
											   string stringReviewerId, string stringReceiverId, string stringCaseId, unsigned int nPageTo, unsigned int nPageSize)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	string stringPageTo = CUtil::UIntToString(nPageTo);
	string stringPageSize = CUtil::UIntToString(nPageSize);
	nRet = m_pPhmsSession->GetReviewReportList(stringReportStartDate, stringReportEndDate, stringReviewStartDate, stringReviewEndDate, 
		stringReviewState, stringCaseType, stringPtName, stringSenderId, stringYzlb, stringAId, stringReviewerId, stringReceiverId, stringCaseId, stringPageTo, stringPageSize);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::ReviewReport(string stringReportKeyId, string stringReviewState, string stringPath, string stringDiagnostic, 
										string stringDigitalSign, string stringTimeStampSign)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->ReviewReport(stringReportKeyId, stringReviewState, stringPath, stringDiagnostic, stringDigitalSign, stringTimeStampSign);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::GetReportStateOfCase(string stringCaseId)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->GetReportStateOfCase(stringCaseId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::GetCaseListOfDoctor(string stringCaseId, string stringCaseType, string stringCaseSource, string stringCaseState,
					  string stringPatientName, string stringSenderId, string stringReceiverId, string stringStartDate, string stringEndDate, string stringCheckStartDate,
					  string stringCheckEndDate, string stringSenderName, string stringDiagnostic, string stringCaseOrigin, string stringYzlb, string stringAId, string stringSex, string stringOrderBy, 
					  string stringThirdId, string stringLang, string stringIsFullName, unsigned int nPageTo, unsigned int nPageSize)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	string stringPageTo = CUtil::UIntToString(nPageTo);
	string stringPageSize = CUtil::UIntToString(nPageSize);
	nRet = m_pPhmsSession->GetCaseListOfDoctor(stringCaseId, stringCaseType, stringCaseSource, stringCaseState, stringPatientName, 
		stringSenderId, stringReceiverId, stringStartDate, stringEndDate, stringCheckStartDate, stringCheckEndDate, stringSenderName, 
		stringDiagnostic, stringCaseOrigin, stringYzlb, stringAId, stringSex, stringOrderBy, stringThirdId, stringLang, stringIsFullName, stringPageTo, stringPageSize);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::GetHospitalInfo(string stringHospitalId)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->GetHospitalInfo(stringHospitalId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::UploadIm(string stringSenderId, string stringSenderName, string stringReceiverId, string stringMsgDirection,
			 string stringMsgType, string stringMsgContent, string stringCaseId, string stringCaseType)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->UploadIm(stringSenderId, stringSenderName, stringReceiverId, stringMsgDirection, stringMsgType, stringMsgContent, 
		stringCaseId, stringCaseType);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::GetImList(string stringSenderId, string stringCaseId, string stringCaseType, string stringMsgDirection,
			  string stringMsgType)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->GetImList(stringSenderId, stringCaseId, stringCaseType, stringMsgDirection, stringMsgType);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::GetImList2(string stringNextId)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->GetImList2(stringNextId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::DownloadImComplete(string stringMsgId)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->ImDownloadComplete(stringMsgId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::SendPhoneMsg (string stringCardId, string stringPhoneNum, string stringMsg)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->SendPhoneMsg(stringCardId, stringPhoneNum, stringMsg);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::UploadTrendData(string stringCardId, string stringDeviceId, string stringDeviceName, string stringCaseId, vector<RECORD_DATA> vRecordData)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->UploadTrendData(stringCardId, stringDeviceId, stringDeviceName, stringCaseId, vRecordData);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::UploadTrendDataString(string stringCardId, string stringDeviceId, string stringDeviceName, string stringCaseId, vector<RECORD_DATA_STRING> vRecordData)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->UploadTrendDataString(stringCardId, stringDeviceId, stringDeviceName, stringCaseId, vRecordData);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::UploadTrendDataFile(string stringFilePath)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->UploadTrendDataFile(stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::UploadAudioSegment (string stringMsgId, unsigned long nTotalSize, unsigned long nAlreadyUploadSize, 
						unsigned long nThisTimeUploadSize, string stringServerPath, string stringLocalPath)
{
	//验证参数
	if(nTotalSize<=0 || nAlreadyUploadSize<0 || nThisTimeUploadSize<=0 ||nTotalSize<nAlreadyUploadSize+nThisTimeUploadSize)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_PARAM_VALIDATE_FAIL), __FUNCTION__, __FILE__, __LINE__);
		return COMMON_PARAM_VALIDATE_FAIL;
	}
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);

	//获得stringTotalSize参数
	string stringTotalSize;
	stringTotalSize = CUtil::ULongToString(nTotalSize);
	//获得stringOrder
	string stringOrder;
	this->GetOrder(nTotalSize, nAlreadyUploadSize, nThisTimeUploadSize, stringOrder);
	//获得Start\End\Md5信息
	vSegmentInfo vSi;
	nRet = this->GetUploadSegmentInfo(3, nAlreadyUploadSize, nThisTimeUploadSize, stringLocalPath, vSi);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	//生成临时文件
	string stringTempUploadFilePath;
	nRet = this->GenerateTempUploadFile(nAlreadyUploadSize, nThisTimeUploadSize, stringLocalPath, stringTempUploadFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = m_pPhmsSession->UploadAudioSegment(stringMsgId, stringOrder, stringTotalSize, stringServerPath, vSi[0].m_stringStart, vSi[0].m_stringEnd, 
		vSi[0].m_stringMd5, vSi[1].m_stringStart, vSi[1].m_stringEnd, vSi[1].m_stringMd5, vSi[2].m_stringStart, vSi[2].m_stringEnd, 
		vSi[2].m_stringMd5, stringTempUploadFilePath);
	try
	{
		File fileTempUploadFile(stringTempUploadFilePath, "gbk");
		fileTempUploadFile.remove(false);
	}
	catch(const Exception&)
	{

	}
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::UploadAdvice (string stringUserId, string stringUsername, string stringMsg, string stringType, string stringIsSendMsg)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->UploadAdvice(stringUserId, stringUsername, stringMsg, stringType, stringIsSendMsg);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::GetAdvice (string stringSgId, string stringUserId, string stringDoctorId, string stringDownloadState, string stringType, 
									  string stringStartDate, string stringEndDate, unsigned int nPageTo, unsigned int nPageSize)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	string stringPageTo = CUtil::UIntToString(nPageTo);
	string stringPageSize = CUtil::UIntToString(nPageSize);
	nRet = m_pPhmsSession->GetAdvice(stringSgId, stringUserId, stringDoctorId, stringDownloadState, stringType, stringStartDate, stringEndDate, stringPageTo, stringPageSize);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::DownloadAdviceComplete (string stringAdviceId)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->DownloadAdviceComplete(stringAdviceId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::GetReportListOfCase (string stringCaseId)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->GetReportListOfCase(stringCaseId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

//接口有变动，加了一个参数senderid，表示要查找信息的人，如为空表示自己
int CInternalNetIntetface::GetSelfInfo (string stringSenderId)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->GetSelfInfo(stringSenderId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::ChangePwd (string stringOldPwd, string stringNewPwd)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->ChangePwd(stringOldPwd, stringNewPwd);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::DoctorChangeInfo (string stringName, string stringSex, string stringTel, string stringBirthdear,
					  string stringPosition, string stringNotes)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->DoctorChangeSelfInfo(stringName, stringSex, stringTel, stringBirthdear, stringPosition, stringNotes, "");
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::CliniqueChangeInfo (string stringPid, string stringName, string stringTel, string stringNotes)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->CliniqueChangeSelfInfo(stringPid, stringName, stringTel, stringNotes);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::UserChangeInfo (string stringPid, string stringName, string stringSex, string stringTel, string stringBirthday,
					string stringAddress, string stringLmp, string stringToken, string stringHeight, string stringWeight, string stringPhoto, string stringVCodeBeFollowed, string stringHospitalId, string stringOtherInfo)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->CardUserChangeSelfInfo(stringPid, stringName, stringSex, stringTel, stringBirthday, stringAddress, stringLmp, stringToken, stringHeight, stringWeight, stringPhoto, stringVCodeBeFollowed, stringHospitalId, stringOtherInfo);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::GetCaseListOfUser (string stringCaseId, string stringCaseType, string stringCaseState, string stringThirdId, string stringName, 
											  string stringStartDate, string stringEndDate, string stringCheckStartDate, string stringCheckEndDate, string stringCaseOrigin, 
											  string stringYzlb, string stringSex, string stringIsRead, string stringOrderBy, unsigned int nPageTo, unsigned int nPageSize)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	string stringPageTo = CUtil::UIntToString(nPageTo);
	string stringPageSize = CUtil::UIntToString(nPageSize);
	nRet = m_pPhmsSession->GetCaseListOfUser(stringCaseId, stringCaseType, stringCaseState, stringThirdId, stringName, stringStartDate, stringEndDate, 
		stringCheckStartDate, stringCheckEndDate, stringCaseOrigin, stringYzlb, stringSex, stringIsRead, stringOrderBy, stringPageTo, stringPageSize);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::GetReportListOfUser (string stringReportId, string stringReportStartDate, string stringReportEndDate,
					   string stringCaseType, string stringReportState, string stringOrderBy, unsigned int nPageTo, unsigned int nPageSize)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	string stringPageTo = CUtil::UIntToString(nPageTo);
	string stringPageSize = CUtil::UIntToString(nPageSize);
	nRet = m_pPhmsSession->GetReportListOfUser(stringReportId, stringReportStartDate, stringReportEndDate, stringCaseType, stringReportState, stringOrderBy, stringPageTo, stringPageSize);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::GetCardUserList (string stringCardId, string stringSenderId, string stringPersonId, string stringTel, string stringName,
						   string stringType, unsigned int nPageTo, unsigned int nPageSize)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	string stringPageTo = CUtil::UIntToString(nPageTo);
	string stringPageSize = CUtil::UIntToString(nPageSize);
	nRet = m_pPhmsSession->GetCardUserList(stringCardId, stringSenderId, stringPersonId, stringTel, stringName, stringType, stringPageTo, stringPageSize);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::KeepSession()
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->KeepSession();
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::MarkCaseException(string stringCaseId, string stringReceiverId , int nResultFlag)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->MarkCaseException(stringCaseId, stringReceiverId, nResultFlag);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::MarkReportRead(string stringReportId)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->MarkReportRead(stringReportId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::Feedback(string stringTitle, string stringNotes, string stringPhone, string stringFilePath)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->Feedback(stringTitle, stringNotes, stringPhone, stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::UploadTrendData2(string stringTargetPath)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->UploadTrendData2(stringTargetPath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::GetAllTeamList()
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->GetAllTeamList();
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::GetDoctorListOfTeam(string stringTeamId)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->GetDoctorListOfTeam(stringTeamId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::GetEventStream(string stringCaseId, string stringOriginalHospital)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->GetEventStream(stringCaseId, stringOriginalHospital);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::GetCaseTypeList(string stringLanguage)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->GetCaseTypeList(stringLanguage);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::DoctorHandledCaseStatistics(string stringCaseType, string stringStartTime, string stringEndTime)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->DoctorHandledCaseStatistics(stringCaseType, stringStartTime, stringEndTime);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::UserRegister(string stringPhone, string stringName, string stringHeigth)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->UserRegister(stringPhone, stringName, stringHeigth);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::GetTrendData(string stringUId, string stringInterUId, string stringTrendType, string stringStartDate, string stringEndDate, string stringNextId)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->GetTrendData(stringUId, stringInterUId, stringTrendType, stringStartDate, stringEndDate, stringNextId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	//目前该接口只支持保存到文件
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::GetLastPhysiologicalData()
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->GetLastPhysiologicalData();
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::GetHealthDegree(string stringUId, string stringInterUId, string stringDate)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->GetHealthDegree(stringUId, stringInterUId, stringDate);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::GetUserOfNoAdviceAndMostData(string stringDays)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->GetUserOfNoAdviceAndMostData(stringDays);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::AdviceDelay(string stringUId, string stringMinutes)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->AdviceDelay(stringUId, stringMinutes);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::AdviceComplete(string stringUId, string stringOptionType, string stringAdvice)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->AdviceComplete(stringUId, stringOptionType, stringAdvice);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::GetUserListOfNoAdvice(string stringDays, unsigned int nPageTo, unsigned int nPageSize)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	string stringPageTo = CUtil::UIntToString(nPageTo);
	string stringPageSize = CUtil::UIntToString(nPageSize);
	nRet = m_pPhmsSession->GetUserListOfNoAdvice(stringDays, stringPageTo, stringPageSize);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::AdviceBegin(string stringUId)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->AdviceBegin(stringUId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::GetUserListOfAdvicing(unsigned int nPageTo, unsigned int nPageSize)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	string stringPageTo = CUtil::UIntToString(nPageTo);
	string stringPageSize = CUtil::UIntToString(nPageSize);
	nRet = m_pPhmsSession->GetUserListOfAdvicing(stringPageTo, stringPageSize);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::VerifyThirdId(string stringUId, string stringType)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->VerifyThirdId(stringUId, stringType);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::PersonIdRegister(string stringType, string stringRegFrom, string stringPersonId, string stringName, string stringSex, string stringBirthday, string stringAddress, 
					 string stringPhone, string stringHeigth, string stringUploadUnitId, string stringHospitalId, string stringHGroupId, string stringScode, string stringWxUId, string stringLmp, string stringWeight, string stringPassword, 
					 string stringThirdCode, string stringIsRegister)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->PersonIdRegister(stringType, stringRegFrom, stringPersonId, stringName, stringSex, stringBirthday, stringAddress, stringPhone, stringHeigth, stringUploadUnitId,
		stringHospitalId, stringHGroupId, stringScode, stringWxUId, stringLmp, stringWeight, stringPassword, stringThirdCode, stringIsRegister);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::GetVerificationCode(string stringType, string stringPhone)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->GetVerificationCode(stringType, stringPhone);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::GetAskList(string stringAskType, string stringAskEnd, string stringDoctorId, string stringIsAsk, string stringUserId, string stringStartTime, string stringEndTime, unsigned int nPageTo, unsigned int nPageSize)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	string stringPageTo = CUtil::UIntToString(nPageTo);
	string stringPageSize = CUtil::UIntToString(nPageSize);
	nRet = m_pPhmsSession->GetAskList(stringAskType, stringAskEnd, stringDoctorId, stringIsAsk, stringUserId, stringStartTime, stringEndTime, stringPageTo, stringPageSize);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::GetAskDetail(string stringAskId)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->GetAskDetail(stringAskId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::GetQuestionList(string stringDoctorId, string stringReportType, string stringAskId)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->GetQuestionList(stringDoctorId, stringReportType, stringAskId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::SubmitQuestion(string stringUserId, string stringAdvise, string stringReportType, string stringAskId, string stringAskedId, string stringDoctorPhotoPath)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->SubmitQuestion(stringUserId, stringAdvise, stringReportType, stringAskId, stringAskedId, stringDoctorPhotoPath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::GetDataDictionary()
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->GetDataDictionary();
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::LoggingHealthBaseInfo(string stringUserId, string stringWeblock, string stringNation, string stringProfression, string stringCrowd, string stringBloodType, 
						  string stringJwsJiBing, string stringJwsShouShu, string stringJwsWaiShang, string stringJwsShuXue, string stringJwsBaoLuShi, string stringJwsBaoLuShiBz, 
						  string stringJwsYiChuanJiBing, string stringGmsGuoMinShi, string stringGmsGuoMinShiBz, string stringJzsFuQin, string stringJzsFuQinBz, string stringJzsMuQin, 
						  string stringJzsMuQinBz, string stringJzsXdjm, string stringJzsXdjmBz, string stringJzsZiNv, string stringJzsZiNvBz, string stringJzsQiTa)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->LoggingHealthBaseInfo(stringUserId, stringWeblock, stringNation, stringProfression, stringCrowd, stringBloodType, stringJwsJiBing, stringJwsShouShu, stringJwsWaiShang, 
		stringJwsShuXue, stringJwsBaoLuShi, stringJwsBaoLuShiBz, stringJwsYiChuanJiBing, stringGmsGuoMinShi, stringGmsGuoMinShiBz, stringJzsFuQin, stringJzsFuQinBz, stringJzsMuQin, stringJzsMuQinBz, 
		stringJzsXdjm, stringJzsXdjmBz, stringJzsZiNv, stringJzsZiNvBz, stringJzsQiTa);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::LoggingHealthCurrentInfo(string stringUserId, string stringShfsYinShi, string stringShfsXlzk, string stringShfsXlzkBz, string stringShfsXltz, string stringShfsXiYan, 
							 string stringShfsYinJiu, string stringShfsSyqk, string stringShfsYunDong, string stringShfsYunDongShiJian, string stringSysjcDanGuChun, string stringSysjcXueChangGui, 
							 string stringSysjcXueChangGuiBz, string stringSysjcNiaoChangGui, string stringSysjcNiaoChangGuiBz, string stringSysjcQiTa, string stringYongYao, string stringZhuYuan, 
							 string stringJkwtNaoXueGuan, string stringJkwtNaoXueGuanBz, string stringJkwtShenZang, string stringJkwtShenZangBz, string stringJkwtXinZang, string stringJkwtXinZangBz, 
							 string stringJkwtXueGuan, string stringJkwtXueGuanBz, string stringJkwtYanBu, string stringJkwtYanBuBz, string stringJkwtShenJing, string stringJkwtQiTa, string stringDate)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->LoggingHealthCurrentInfo(stringUserId, stringShfsYinShi, stringShfsXlzk, stringShfsXlzkBz, stringShfsXltz, stringShfsXiYan, stringShfsYinJiu, stringShfsSyqk, stringShfsYunDong,
		stringShfsYunDongShiJian, stringSysjcDanGuChun, stringSysjcXueChangGui, stringSysjcXueChangGuiBz, stringSysjcNiaoChangGui, stringSysjcNiaoChangGuiBz, stringSysjcQiTa, stringYongYao, stringZhuYuan,
		stringJkwtNaoXueGuan, stringJkwtNaoXueGuanBz, stringJkwtShenZang, stringJkwtShenZangBz, stringJkwtXinZang, stringJkwtXinZangBz, stringJkwtXueGuan, stringJkwtXueGuanBz, stringJkwtYanBu, stringJkwtYanBuBz,
		stringJkwtShenJing, stringJkwtQiTa, stringDate);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::GetCliniqueListOfHospital(string stringHospitalId)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->GetCliniqueListOfHospital(stringHospitalId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::GetStatisticsInfo(string stringTypeId, string stringMonth, string stringMonthDay, unsigned int nPageTo, unsigned int nPageSize)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	string stringPageTo = CUtil::UIntToString(nPageTo);
	string stringPageSize = CUtil::UIntToString(nPageSize);
	nRet = m_pPhmsSession->GetStatisticsInfo(stringTypeId, stringMonth, stringMonthDay, stringPageTo, stringPageSize);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::UpdateCaseInfo(string stringHospitalId, string stringCaseId, string stringOtherParam, string stringFlag, string stringNotes, string stringReceiverId)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->UpdateCaseInfo(stringHospitalId, stringCaseId, stringOtherParam, stringFlag, stringNotes, stringReceiverId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::GetUserHealthList(string stringNextId, string stringTypeId, string stringSecondTypeId, string stringName, string stringSex, string stringThirdIdType, 
					  string stringThirdId, string stringHospitalId, string stringUnitSenderId, string stringIsData, string stringIntervalTime, string stringPopulations, string stringContractDoctor, string stringIsContract, string stringPageSize)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->GetUserHealthList(stringNextId, stringTypeId, stringSecondTypeId, stringName, stringSex, stringThirdIdType, stringThirdId, stringHospitalId, 
		stringUnitSenderId, stringIsData, stringIntervalTime, stringPopulations, stringContractDoctor, stringIsContract, stringPageSize);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::GetAdviceImplemention(string stringThirdId, string stringHospitalId)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->GetAdviceImplemention(stringThirdId, stringHospitalId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::ChangePersonType(string stringThirdId, string stringPeopleType, string stringHospitalId)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->ChangePersonType(stringThirdId, stringPeopleType, stringHospitalId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::SetNextVisitTime(string stringThirdId, string stringInterViewTime, string stringHospitalId)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->SetNextVisitTime(stringThirdId, stringInterViewTime, stringHospitalId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::GetPersonTypeList(string stringLanguage)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->GetPersonTypeList(stringLanguage);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::PersonIdRegisterAndLogin(string stringRole, string stringPId, string stringName, string stringSex, string stringBirthday, string stringAddress, string stringDeviceId, string stringMd5, 
							 string stringLang, string stringRegFrom)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->PersonIdRegisterAndLogin(stringRole, stringPId, stringName, stringSex, stringBirthday, stringAddress, stringDeviceId, stringMd5, stringLang, stringRegFrom);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::CardBindPhone(string stringPhone, string stringScode)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->CardBindPhone(stringPhone, stringScode);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::ResetPassword(string stringPhone, string stringScode, string stringNewPwd)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->ResetPassword(stringPhone, stringScode, stringNewPwd);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::UploadDaily(string stringType, string stringCreateTime, string stringFiles)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->UploadDaily(stringType, stringCreateTime, stringFiles);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::GetDateListOfDaily(string stringYear, string stringMonth)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->GetDateListOfDaily(stringYear, stringMonth);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::GetDailyOfDate(string stringDate, string stringType)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->GetDailyOfDate(stringDate, stringType);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::ShareDaily(string stringType, string stringCreateTime)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->ShareDaily(stringType, stringCreateTime);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::DeleteDaily(string stringType, string stringDate)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->DeleteDaily(stringType, stringDate);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::PhoneScanQR(string stringQRCode)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->PhoneScanQR(stringQRCode);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::PhoneScanQRVerify(string stringQRCode)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->PhoneScanQRVerify(stringQRCode);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::ApplyQRCode(string stringType, string stringScene)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->ApplyQRCode(stringType, stringScene);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::ValidateUserAccount(string stringLoginId, string stringPassword, string stringRole)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->ValidateUserAccount(stringLoginId, stringPassword, stringRole);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::RegisterBluetoothDevice(string stringUniqueSN, string stringSerial, string stringBluetoothAddress, string stringBluetoothName, string stringDeviceType)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->RegisterBluetoothDevice(stringUniqueSN, stringSerial, stringBluetoothAddress, stringBluetoothName, stringDeviceType);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::GetBluetoothInfoOfDevice(string stringUniqueSN)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->GetBluetoothInfoOfDevice(stringUniqueSN);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::ChangeDeviceType(string stringUniqueSN, string stringDeviceType)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->ChangeDeviceType(stringUniqueSN, stringDeviceType);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::UnregisterBluetoothDevice(string stringUniqueSN)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->UnregisterBluetoothDevice(stringUniqueSN);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::GetHospitalListOfArea(string stringAreaId, string stringRank, string stringBeta)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->GetHospitalListOfArea(stringAreaId, stringRank, stringBeta);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::UploadCommonFileSegment (string stringType, string stringExtra, unsigned long nTotalSize, unsigned long nAlreadyUploadSize, 
							 unsigned long nThisTimeUploadSize, string stringServerPath, string stringLocalPath, string stringFileType)
{
	//验证参数
	if(nTotalSize<=0 || nAlreadyUploadSize<0 || nThisTimeUploadSize<=0 ||nTotalSize<nAlreadyUploadSize+nThisTimeUploadSize)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_PARAM_VALIDATE_FAIL), __FUNCTION__, __FILE__, __LINE__);
		return COMMON_PARAM_VALIDATE_FAIL;
	}
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);

	//获得stringTotalSize参数
	string stringTotalSize;
	stringTotalSize = CUtil::ULongToString(nTotalSize);
	//获得stringOrder
	string stringOrder;
	this->GetOrder(nTotalSize, nAlreadyUploadSize, nThisTimeUploadSize, stringOrder);
	//获得Start\End\Md5信息
	vSegmentInfo vSi;
	nRet = this->GetUploadSegmentInfo(3, nAlreadyUploadSize, nThisTimeUploadSize, stringLocalPath, vSi);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	//生成临时文件
	string stringTempUploadFilePath;
	nRet = this->GenerateTempUploadFile(nAlreadyUploadSize, nThisTimeUploadSize, stringLocalPath, stringTempUploadFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	nRet = m_pPhmsSession->UploadCommonFileSegment(stringType, stringExtra, stringOrder, stringTotalSize, stringServerPath, vSi[0].m_stringStart, vSi[0].m_stringEnd, 
		vSi[0].m_stringMd5, vSi[1].m_stringStart, vSi[1].m_stringEnd, vSi[1].m_stringMd5, vSi[2].m_stringStart, vSi[2].m_stringEnd, 
		vSi[2].m_stringMd5, stringTempUploadFilePath, stringFileType);
	try
	{
		File fileTempUploadFile(stringTempUploadFilePath, "gbk");
		fileTempUploadFile.remove(false);
	}
	catch(const Exception&)
	{

	}
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::SearchSomebody(string stringUserId)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->SearchSomebody(stringUserId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}
int CInternalNetIntetface::FollowSomebody(string stringUserId, string stringUsername, string stringNickname, string stringVCode)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->FollowSomebody(stringUserId, stringUsername, stringNickname, stringVCode);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}
int CInternalNetIntetface::MyFollowing(string stringUserId, string stringNextId, string stringFType)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->MyFollowing(stringUserId, stringNextId, stringFType);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}
int CInternalNetIntetface::MyFans(string stringUserId, string stringNextId)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->MyFans(stringUserId, stringNextId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}
int CInternalNetIntetface::ChangeNicknameOfFollowed(string stringUserId, string stringNickname)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->ChangeNicknameOfFollowed(stringUserId, stringNickname);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}
int CInternalNetIntetface::CancelFollowing(string stringUserId)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->CancelFollowing(stringUserId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}
int CInternalNetIntetface::PublishHealthyKnowledge(vector<HEALTHY_KNOWLEDGE> vHealthKnowledge)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->PublishHealthyKnowledge(vHealthKnowledge);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}
int CInternalNetIntetface::GetListOfHealthyKnowledge(string stringSenderId, string stringNextId, string stringIds, string stringPageSize, string stringOrderBy)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->GetListOfHealthyKnowledge(stringSenderId, stringNextId, stringIds, stringPageSize, stringOrderBy);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}
int CInternalNetIntetface::GetCardUserReportList(string stringReportId, string stringReportStartDate, string stringReportEndDate, string stringCaseType, string stringState, string stringOrderBy, 
		string stringThirdIdType, string stringThirdId, string stringNextId, string stringPageSize)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->GetCardUserReportList(stringReportId, stringReportStartDate, stringReportEndDate, stringCaseType, stringState, stringOrderBy, stringThirdIdType, stringThirdId, stringNextId, stringPageSize);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::ChangeViewLikeCollectCount(string stringHealthTipId, string stringUri, string stringTitle, string stringLike, string stringRead, string stringCollect)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->ChangeViewLikeCollectCount(stringHealthTipId, stringUri, stringTitle, stringLike, stringRead, stringCollect);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::ChangeAdviceExecutiveState(string stringSgId, string stringState)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->ChangeAdviceExecutiveState(stringSgId, stringState);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::GetTrendRangeValue()
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->GetTrendRangeValue();
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::GetCliniqueReportList(string stringCliniqueSenderId, string stringStartDate, 
												 string stringEndDate, string stringState, string stringName, string stringPatientSourceCode, string stringYzlb, string stringPIdType, string stringPId, string stringHb, string stringASectionId, string stringNextId, string stringPageSize)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->GetCliniqueReportList(stringCliniqueSenderId, stringStartDate, stringEndDate, stringState, stringName, stringPatientSourceCode, stringYzlb, stringPIdType, stringPId, stringHb, stringASectionId, stringNextId, stringPageSize);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::GetUserListBindingOrNot(string stringNextId, string stringPageSize, string stringIsBind, string stringIsActive)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->GetUserListBindingOrNot(stringNextId, stringPageSize, stringIsBind, stringIsActive);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}
int CInternalNetIntetface::BindUser(string stringUId, string stringUName)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->BindUser(stringUId, stringUName);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}
int CInternalNetIntetface::UnbindUser(string stringUId)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->UnbindUser(stringUId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::GetCardUserListOfCustomer(string stringThirdId, string stringName, string stringCaseType, string stringStartDate, string stringEndDate, string stringNextId, unsigned int nPageSize)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	string stringPageSize = CUtil::UIntToString(nPageSize);
	nRet = m_pPhmsSession->GetCardUserListOfCustomer(stringThirdId, stringName, stringCaseType, stringStartDate, stringEndDate, stringNextId, stringPageSize);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::SearchCaseAdvanced(string stringHospitalId, string stringDiagnostic, 
					   string stringName, string stringSex, QUERY_RANGE rangeAge, QUERY_RANGE rangeCreateTime, string stringMode, 
					   string stringCaseType, QUERY_ECG ecg, string stringFrom, string stringSize)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->SearchCaseAdvanced(stringHospitalId, stringDiagnostic, stringName, stringSex, rangeAge, 
		rangeCreateTime, stringMode, stringCaseType, ecg, stringFrom, stringSize);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::SearchCaseAdvancedString(string stringHospitalId, string stringDiagnostic, 
							 string stringName, string stringSex, string stringJsonAge, string stringJsonCreateTime, string stringMode, 
							 string stringCaseType, string stringJsonEcg, string stringFrom, string stringSize)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->SearchCaseAdvancedString(stringHospitalId, stringDiagnostic, stringName, stringSex, stringJsonAge, 
		stringJsonCreateTime, stringMode, stringCaseType, stringJsonEcg, stringFrom, stringSize);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::GetPayCode(string stringType, string stringCaseId)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet =  m_pPhmsSession->GetPayCode(stringType, stringCaseId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::GetOrderInfo(string stringType, string stringCaseId)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet =  m_pPhmsSession->GetOrderInfo(stringType, stringCaseId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::SignDoctor(string stringUId)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet =  m_pPhmsSession->SignDoctor(stringUId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::RefundFee(string stringType, string stringCaseId, string stringRefundFee)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet =  m_pPhmsSession->RefundFee(stringType, stringCaseId, stringRefundFee);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}
int CInternalNetIntetface::HoldPlaceReview(string stringReportId)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet =  m_pPhmsSession->HoldPlaceReview(stringReportId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}
int CInternalNetIntetface::RollBackReview(string stringReportId)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet =  m_pPhmsSession->RollBackReview(stringReportId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}
int CInternalNetIntetface::ResetReviewHoldPlace(string stringReportId, string stringOccupantId, string stringOccupantName, string stringSuperKey)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet =  m_pPhmsSession->ResetReviewHoldPlace(stringReportId, stringOccupantId, stringOccupantName, stringSuperKey);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}
int CInternalNetIntetface::ResetCaseHoldPlace(string stringCaseId, string stringReceiverId, string stringOccupantId, string stringOccupantName, string stringSuperKey)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet =  m_pPhmsSession->ResetCaseHoldPlace(stringCaseId, stringReceiverId, stringOccupantId, stringOccupantName, stringSuperKey);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}


int CInternalNetIntetface::GenerateOrDeletePngZlibReport(string stringServerPath, string stringFlag)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->GenerateOrDeletePngZlibReport(stringServerPath, stringFlag);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::GetDailyList(string stringType, string stringStartDate, string stringEndDate, string stringOrderBy, string stringNextId, string stringPageSize)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->GetDailyList(stringType, stringStartDate, stringEndDate, stringOrderBy, stringNextId, stringPageSize);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::AddTag(string stringTagName, string stringTagDescription)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->AddTag(stringTagName, stringTagDescription);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::GetTagList(string stringSenderId)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->GetTagList(stringSenderId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::AddCaseToTag(string stringTagId, string stringTagName, string stringCaseId, string stringCaseType, string stringCaseTypeName, string stringPatientName)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->AddCaseToTag(stringTagId, stringTagName, stringCaseId, stringCaseType, stringCaseTypeName, stringPatientName);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::GetCaseListOfTag(string stringTagId, string stringSenderId, string stringCaseType)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->GetCaseListOfTag(stringTagId, stringSenderId, stringCaseType);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

// int CInternalNetIntetface::CreateSession_GuanXin(string stringUsername, string stringPassword)
// {
// 	int nRet = PHMS_SUCCESSFUL_RESULT;
// 	m_pPhmsSession->SetSaveType(m_nSaveType);
// 	m_pPhmsSession->SetFilePath(m_stringFilePath);
// 	nRet = m_pPhmsSession->CreateSession_GuanXin(stringUsername, stringPassword);
// 	if(nRet != PHMS_SUCCESSFUL_RESULT)
// 	{
// 		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
// 		return nRet;
// 	}
// 	if(m_nSaveType == 1)
// 	{
// 		m_stringContent = m_pPhmsSession->GetContent();
// 	}
// 	return PHMS_SUCCESSFUL_RESULT;
// }

int CInternalNetIntetface::GetAccessToken_GuanXin(string stringUsername, string stringPassword)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->GetAccessToken_GuanXin(stringUsername, stringPassword);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::GetPersonInfo_GuanXin(string stringAccessToken, string stringPersonName, string stringPersonGender, string stringPersonIdentity, string stringPersonBirthDate)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->GetPersonInfo_GuanXin(stringAccessToken, stringPersonName, stringPersonGender, stringPersonIdentity, stringPersonBirthDate);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::UploadECGData_GuanXin(string stringECGXmlPath, string stringECGDataPath)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->UploadECGData_GuanXin(stringECGXmlPath, stringECGDataPath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::UploadOtherData_GuanXin(string stringOtherXmlPath)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->UploadOtherData_GuanXin(stringOtherXmlPath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::UploadCheckData_GuanXin(string stringCheckXmlPath)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->UploadCheckData_GuanXin(stringCheckXmlPath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}

	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::UploadPersonInfo_GuanXin(string stringPersonInfoXmlPath)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->UploadPersonInfo_GuanXin(stringPersonInfoXmlPath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}

	return PHMS_SUCCESSFUL_RESULT;
}



//自定义内部接口
int CInternalNetIntetface::DownloadFile(int nThreadCount, string stringUri, unsigned long nFileSize, string stringLocalFilePath, pProgressAndSpeedCallback pCallback)
{
	if(nThreadCount>3 || nThreadCount<1)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_DOWNLOAD_THREAD_COUNT_TOO_LARGE), __FUNCTION__, __FILE__, __LINE__);
		return COMMON_DOWNLOAD_THREAD_COUNT_TOO_LARGE;
	}
	int nRet = PHMS_SUCCESSFUL_RESULT;
	if(nFileSize == 0)
	{
		//应用程序未传入文件大小，需要从服务器获得文件大小
		string stringServerFileSize;
		this->SetSaveType(1);
		nRet = this->GetAlreadyUploadFileSize(stringUri);
		if(nRet != PHMS_SUCCESSFUL_RESULT)
		{
			CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
			return nRet;
		}
		stringServerFileSize = this->GetContent();
		if(stringServerFileSize.size()==0 || stringServerFileSize.compare("0")==0)
		{
			CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_DOWNLOAD_FILE_SIZE_0), __FUNCTION__, __FILE__, __LINE__);
			return COMMON_DOWNLOAD_FILE_SIZE_0;
		}
		nFileSize = CUtil::StringToULong(stringServerFileSize);
	}
	const unsigned long nSegmentMaxSize = (1024*1024);
	unsigned long nAlreadyCombineFileSize = 0;
	unsigned long nRemainFileSize = 0;
	unsigned long nAlreadyDownloadFileSize = 0;
	//获得已经合并的文件大小
	File fileLocalFile(stringLocalFilePath, "gbk");
	if(fileLocalFile.exists())
	{
		nAlreadyCombineFileSize = (unsigned long)fileLocalFile.getSize();
	}
	else
	{
		nAlreadyCombineFileSize = 0;
	}
	nAlreadyDownloadFileSize = nAlreadyCombineFileSize;
	if(nAlreadyCombineFileSize == nFileSize)
	{
		if(pCallback != NULL)
		{
			(*pCallback)(100, 0, stringUri.c_str(), stringLocalFilePath.c_str());
		}
		return PHMS_SUCCESSFUL_RESULT;
	}
	if(nAlreadyCombineFileSize > nFileSize)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_LOCAL_FILE_MORE_THAN_SERVER_FILE), __FUNCTION__, __FILE__, __LINE__);
		return COMMON_LOCAL_FILE_MORE_THAN_SERVER_FILE;
	}
	else
	{
		nRemainFileSize = nFileSize - nAlreadyCombineFileSize;
	}
	//创建目录，并扫描临时文件，获得每个临时文件的大小，以方便继续下载
	string stringTempDirectory = stringLocalFilePath+"_PHMS_TEMP";
	unsigned long *pTempFileSize = new unsigned long[nThreadCount];
	memset(pTempFileSize, 0, nThreadCount*sizeof(unsigned long));
	this->GetDownloadTempFileSize(stringTempDirectory, nThreadCount, pTempFileSize);
	for(int i=0; i<nThreadCount; i++)
	{
		nAlreadyDownloadFileSize += pTempFileSize[i];
	}

	//创建并启动定时器
 	Timer timerDownload(1000, 1000);
 	CDownloadTimerCallback downloadTimerCallback(pCallback, nFileSize, stringLocalFilePath, stringTempDirectory, nAlreadyDownloadFileSize, nThreadCount, stringUri);
 	TimerCallback<CDownloadTimerCallback> adapterDowoloadTimer(downloadTimerCallback, &CDownloadTimerCallback::onTimer);
 	timerDownload.start(adapterDowoloadTimer);
	//下载
	ThreadPool threadPool(1, nThreadCount, 60, 0);
	unsigned long* pSegmentFileSize = new unsigned long[nThreadCount];
	memset(pSegmentFileSize, 0, nThreadCount*sizeof(unsigned long));
	while(nRemainFileSize > 0)
	{
		File fileDir(stringTempDirectory, "gbk");
		try
		{
			fileDir.createDirectory();
		}
		catch(const Exception& e)
		{
			CPhmsLogger::GetPhmsLogger()->WriteLog(e.what(), __FUNCTION__, __FILE__, __LINE__);
			CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_CREATE_DIR_FAIL), __FUNCTION__, __FILE__, __LINE__);
			return COMMON_CREATE_DIR_FAIL;
		}
		//获得分段
		memset(pSegmentFileSize, 0, nThreadCount*sizeof(unsigned long));
		this->DivideSegment(nRemainFileSize, nSegmentMaxSize, nThreadCount, pSegmentFileSize);
		//创建线程，分段下载
		vector<CInternalNetIntetface*> vNet;
		vector<CDownloadThreadCallback*> vDownloadThreadCallback;
		for(unsigned int i=0; i<nThreadCount; i++)
		{
			CDownloadThreadCallback* pDownloadThreadCallback = new CDownloadThreadCallback;
			vDownloadThreadCallback.push_back(pDownloadThreadCallback);
			pDownloadThreadCallback->SetUri(stringUri);
			pDownloadThreadCallback->SetLocalFile(stringTempDirectory+"/"+"PHMS_TEMP_"+CUtil::UIntToString(i));
			//获得之前Segment的总和
			unsigned long nPreSegmentTotal = 0;
			for(unsigned int j=0; j<i; j++)
			{
				nPreSegmentTotal += pSegmentFileSize[j];
			}
			pDownloadThreadCallback->SetStart(nAlreadyCombineFileSize+nPreSegmentTotal+pTempFileSize[i]);
			pDownloadThreadCallback->SetEnd(nAlreadyCombineFileSize+nPreSegmentTotal+pSegmentFileSize[i]-1);
			//cout<<downloadThreadCallback.GetStart()<<"		"<<downloadThreadCallback.GetEnd()<<endl;
			CInternalNetIntetface* pNetTemp = new CInternalNetIntetface;
			vNet.push_back(pNetTemp);
			pNetTemp->SetSessionId(this->GetSessionId());
			pNetTemp->SetCommandType(this->GetCommandType());
			pDownloadThreadCallback->SetNet(pNetTemp);
			if(pDownloadThreadCallback->GetStart() <= pDownloadThreadCallback->GetEnd())
			{
				threadPool.start(*pDownloadThreadCallback, "PHMS_TEMP_"+CUtil::UIntToString(i));
			}
		}
		//等待所有线程结束，两分钟，期间只要有一个线程两分钟内未结束，该函数都返回false
		//极致的情况，比如三个线程等待时间有可能无限接近6分钟

		//因为可能有网络太慢不能下载完成的情况，所以改成无限等待
		//threadPool.tryJoinAll(60*1000*3);
		while(1)
		{
			if(m_eventSocketAbort.tryWait(1000))
			{
				for(int i=0; i<vNet.size(); i++)
				{
					vNet[i]->AbortSession();
				}
				threadPool.joinAll();
				break;
			}
			if(threadPool.tryJoinAll(1000))
			{
				break;
			}
		}
		//清理线程对应网络对象
		vector<CInternalNetIntetface*>::const_iterator iter1;
		for(iter1=vNet.begin(); iter1!=vNet.end(); iter1++)
		{
			delete *iter1;
		}
		vNet.clear();
		vector<CDownloadThreadCallback*>::const_iterator iter2;
		for(iter2=vDownloadThreadCallback.begin(); iter2!=vDownloadThreadCallback.end(); iter2++)
		{
			delete *iter2;
		}
		vDownloadThreadCallback.clear();
		//判断是否所有线程都下载成功，并合并文件
		bool bSuccess = true;
		memset(pTempFileSize, 0, nThreadCount*sizeof(unsigned long));
		this->GetDownloadTempFileSize(stringTempDirectory, nThreadCount, pTempFileSize);
		for(int i=0; i<nThreadCount; i++)
		{
			if(pTempFileSize[i] > pSegmentFileSize[i])
			{
				File fileDir(stringTempDirectory, "gbk");
				fileDir.remove(true);

				CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_LOCAL_TEMP_TOO_LARGE), __FUNCTION__, __FILE__, __LINE__);
				delete pTempFileSize;
				delete pSegmentFileSize;
				timerDownload.stop();
				return COMMON_LOCAL_TEMP_TOO_LARGE;
			}
			if(pTempFileSize[i] < pSegmentFileSize[i])
			{
				bSuccess = false;
				break;
			}
		}
		if(bSuccess)
		{
			//合并文件
			nRet = this->CombineSegmentFile(stringLocalFilePath, stringTempDirectory, nThreadCount);
			if(nRet != PHMS_SUCCESSFUL_RESULT)
			{
				CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
				delete pTempFileSize;
				delete pSegmentFileSize;
				timerDownload.stop();
				return nRet;
			}
			for(int i=0; i<nThreadCount; i++)
			{
				nAlreadyCombineFileSize += pSegmentFileSize[i];
			}
			nRemainFileSize = nFileSize - nAlreadyCombineFileSize;
			memset(pTempFileSize, 0, nThreadCount*sizeof(unsigned long));
		}
		else
		{
			//写日志

			CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_DOWNLOAD_SEGMENT_FAIL_FOR_FILESIEZE_CHANGE), __FUNCTION__, __FILE__, __LINE__);
			delete pTempFileSize;
			delete pSegmentFileSize;
			timerDownload.stop();
			return COMMON_DOWNLOAD_SEGMENT_FAIL_FOR_FILESIEZE_CHANGE;
		}
	}
	delete pTempFileSize;
	delete pSegmentFileSize;
	timerDownload.stop(true);
	return PHMS_SUCCESSFUL_RESULT;
}

//UploadCase、UploadReport、UploadAudio三个函数目前只能支持大于3字节的文件上传，若文件小于3字节，上传失败
//因为UploadCaseSegment、UploadReportSegment、UploadAudioSegment三个函数目前只能支持大于3字节的文件上传
//因为PHMS协议未定义，当文件无法分出三段的情况下，应如何上传
int CInternalNetIntetface::UploadCase (string stringCaseId, string stringLocalCasePath, pProgressAndSpeedCallback pCallback)
{
	const unsigned long nMaxPerTimeSize = 512*1024;
	unsigned long nTotalSize = 0;
	int nRet = PHMS_SUCCESSFUL_RESULT;
	nRet = CUtil::GetFileSize(stringLocalCasePath, nTotalSize);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(nTotalSize < 3)
	{
		//该条件判断是为了不出现少于3字节上传的情况，因为服务器不支持少于3字节的上传
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_UPLOAD_LESS_3_BYTES), __FUNCTION__, __FILE__, __LINE__);
		return COMMON_UPLOAD_LESS_3_BYTES;
	}
	string stringLocalIniPath = stringLocalCasePath + ".ini";
	string stringServerPath;
	string stringAlreadyUploadSize;
	nRet = LoadServerPathAndAlreadyUploadSize(stringLocalIniPath, stringServerPath, stringAlreadyUploadSize);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	unsigned long nAlreadyUploadSize = 0;
	if(stringAlreadyUploadSize.size() > 0)
	{
		nAlreadyUploadSize = CUtil::StringToULong(stringAlreadyUploadSize);
	}
	if(nAlreadyUploadSize == nTotalSize)
	{
		if(pCallback != NULL)
		{
			(*pCallback)(100, 0, stringServerPath.c_str(), stringLocalCasePath.c_str());
		}
		return PHMS_SUCCESSFUL_RESULT;
	}
	if(nAlreadyUploadSize > nTotalSize)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return COMMON_UPLOAD_INI_SIZE_ERROR;
	}
	
	//创建并启动定时器
	Timer timerUpload(1000, 1000);
	CUploadTimerCallback uploadCallback(pCallback, stringLocalIniPath, nAlreadyUploadSize, nTotalSize, stringLocalCasePath);
	TimerCallback<CUploadTimerCallback> adapterUpload(uploadCallback, &CUploadTimerCallback::onTimer);
	timerUpload.start(adapterUpload);
	
	
	unsigned long nRemainSize = nTotalSize-nAlreadyUploadSize;
	while(nRemainSize > 0)
	{
		unsigned long nThisTimeUploadSize = 0;
		if(nRemainSize > nMaxPerTimeSize)
		{
			if(nRemainSize - nMaxPerTimeSize <3)
			{
				//该条件判断是为了不出现少于3字节上传的情况，因为服务器不支持少于3字节的上传
				nThisTimeUploadSize = nRemainSize;
			}
			else
			{
				nThisTimeUploadSize = nMaxPerTimeSize;
			}
		}
		else
		{
			nThisTimeUploadSize = nRemainSize;
		}
		this->SetSaveType(1);
		nRet = this->UploadCaseSegment(stringCaseId, nTotalSize, nAlreadyUploadSize, nThisTimeUploadSize, stringServerPath, stringLocalCasePath);
		if(nRet != PHMS_SUCCESSFUL_RESULT)
		{
			timerUpload.stop();
			CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
			return nRet;
		}
		stringServerPath = this->GetContent();
		stringAlreadyUploadSize = CUtil::ULongToString(nAlreadyUploadSize+nThisTimeUploadSize);
		nRet = StoreServerPathAndAlreadyUploadSize(stringLocalIniPath, stringServerPath, stringAlreadyUploadSize);
		if(nRet != PHMS_SUCCESSFUL_RESULT)
		{
			timerUpload.stop();
			CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
			return nRet;
		}
		nAlreadyUploadSize += nThisTimeUploadSize;
		nRemainSize = nTotalSize - nAlreadyUploadSize;
	}
	timerUpload.stop(true);
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::UploadReport(string stringCaseId, string stringReceiverId, string stringCaseType, string stringReportId, string stringLocalReportPath, string stringFileType, pProgressAndSpeedCallback pCallback)
{
	const unsigned long nMaxPerTimeSize = 512*1024;
	unsigned long nTotalSize = 0;
	int nRet = PHMS_SUCCESSFUL_RESULT;
	nRet = CUtil::GetFileSize(stringLocalReportPath, nTotalSize);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(nTotalSize < 3)
	{
		//该条件判断是为了不出现少于3字节上传的情况，因为服务器不支持少于3字节的上传
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_UPLOAD_LESS_3_BYTES), __FUNCTION__, __FILE__, __LINE__);
		return COMMON_UPLOAD_LESS_3_BYTES;
	}
	string stringLocalIniPath = stringLocalReportPath + ".ini";
	string stringServerPath;
	string stringAlreadyUploadSize;
	nRet = LoadServerPathAndAlreadyUploadSize(stringLocalIniPath, stringServerPath, stringAlreadyUploadSize);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	unsigned long nAlreadyUploadSize = 0;
	if(stringAlreadyUploadSize.size() > 0)
	{
		nAlreadyUploadSize = CUtil::StringToULong(stringAlreadyUploadSize);
	}
	if(nAlreadyUploadSize == nTotalSize)
	{
		if(pCallback != NULL)
		{
			(*pCallback)(100, 0, stringServerPath.c_str(), stringLocalReportPath.c_str());
		}
		return PHMS_SUCCESSFUL_RESULT;
	}
	if(nAlreadyUploadSize > nTotalSize)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return COMMON_UPLOAD_INI_SIZE_ERROR;
	}

	//创建并启动定时器
	Timer timerUpload(1000, 1000);
	CUploadTimerCallback uploadCallback(pCallback, stringLocalIniPath, nAlreadyUploadSize, nTotalSize, stringLocalReportPath);
	TimerCallback<CUploadTimerCallback> adapterUpload(uploadCallback, &CUploadTimerCallback::onTimer);
	timerUpload.start(adapterUpload);


	unsigned long nRemainSize = nTotalSize-nAlreadyUploadSize;
	while(nRemainSize > 0)
	{
		unsigned long nThisTimeUploadSize = 0;
		if(nRemainSize > nMaxPerTimeSize)
		{
			if(nRemainSize - nMaxPerTimeSize <3)
			{
				//该条件判断是为了不出现少于3字节上传的情况，因为服务器不支持少于3字节的上传
				nThisTimeUploadSize = nRemainSize;
			}
			else
			{
				nThisTimeUploadSize = nMaxPerTimeSize;
			}
		}
		else
		{
			nThisTimeUploadSize = nRemainSize;
		}
		this->SetSaveType(1);
		nRet = this->UploadReportSegment(stringCaseId, stringReceiverId, stringCaseType, stringReportId, nTotalSize, 
			nAlreadyUploadSize, nThisTimeUploadSize, stringServerPath, stringLocalReportPath, stringFileType);
		if(nRet != PHMS_SUCCESSFUL_RESULT)
		{
			timerUpload.stop();
			CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
			return nRet;
		}
		stringServerPath = this->GetContent();
		stringAlreadyUploadSize = CUtil::ULongToString(nAlreadyUploadSize+nThisTimeUploadSize);
		nRet = StoreServerPathAndAlreadyUploadSize(stringLocalIniPath, stringServerPath, stringAlreadyUploadSize);
		if(nRet != PHMS_SUCCESSFUL_RESULT)
		{
			timerUpload.stop();
			CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
			return nRet;
		}
		nAlreadyUploadSize += nThisTimeUploadSize;
		nRemainSize = nTotalSize - nAlreadyUploadSize;
	}
	timerUpload.stop(true);
	return PHMS_SUCCESSFUL_RESULT;
}


int CInternalNetIntetface::UploadAudio(string stringMsgId, string stringLocalAudioPath, pProgressAndSpeedCallback pCallback)
{
	const unsigned long nMaxPerTimeSize = 512*1024;
	unsigned long nTotalSize = 0;
	int nRet = PHMS_SUCCESSFUL_RESULT;
	nRet = CUtil::GetFileSize(stringLocalAudioPath, nTotalSize);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(nTotalSize < 3)
	{
		//该条件判断是为了不出现少于3字节上传的情况，因为服务器不支持少于3字节的上传
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_UPLOAD_LESS_3_BYTES), __FUNCTION__, __FILE__, __LINE__);
		return COMMON_UPLOAD_LESS_3_BYTES;
	}
	string stringLocalIniPath = stringLocalAudioPath + ".ini";
	string stringServerPath;
	string stringAlreadyUploadSize;
	nRet = LoadServerPathAndAlreadyUploadSize(stringLocalIniPath, stringServerPath, stringAlreadyUploadSize);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	unsigned long nAlreadyUploadSize = 0;
	if(stringAlreadyUploadSize.size() > 0)
	{
		nAlreadyUploadSize = CUtil::StringToULong(stringAlreadyUploadSize);
	}
	if(nAlreadyUploadSize == nTotalSize)
	{
		if(pCallback != NULL)
		{
			(*pCallback)(100, 0, stringServerPath.c_str(), stringLocalAudioPath.c_str());
		}
		return PHMS_SUCCESSFUL_RESULT;
	}
	if(nAlreadyUploadSize > nTotalSize)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return COMMON_UPLOAD_INI_SIZE_ERROR;
	}

	//创建并启动定时器
	Timer timerUpload(1000, 1000);
	CUploadTimerCallback uploadCallback(pCallback, stringLocalIniPath, nAlreadyUploadSize, nTotalSize, stringLocalAudioPath);
	TimerCallback<CUploadTimerCallback> adapterUpload(uploadCallback, &CUploadTimerCallback::onTimer);
	timerUpload.start(adapterUpload);


	unsigned long nRemainSize = nTotalSize-nAlreadyUploadSize;
	while(nRemainSize > 0)
	{
		unsigned long nThisTimeUploadSize = 0;
		if(nRemainSize > nMaxPerTimeSize)
		{
			if(nRemainSize - nMaxPerTimeSize <3)
			{
				//该条件判断是为了不出现少于3字节上传的情况，因为服务器不支持少于3字节的上传
				nThisTimeUploadSize = nRemainSize;
			}
			else
			{
				nThisTimeUploadSize = nMaxPerTimeSize;
			}
		}
		else
		{
			nThisTimeUploadSize = nRemainSize;
		}
		this->SetSaveType(1);
		nRet = this->UploadAudioSegment(stringMsgId, nTotalSize, nAlreadyUploadSize, nThisTimeUploadSize, stringServerPath, stringLocalAudioPath);
		if(nRet != PHMS_SUCCESSFUL_RESULT)
		{
			timerUpload.stop();
			CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
			return nRet;
		}
		stringServerPath = this->GetContent();
		stringAlreadyUploadSize = CUtil::ULongToString(nAlreadyUploadSize+nThisTimeUploadSize);
		nRet = StoreServerPathAndAlreadyUploadSize(stringLocalIniPath, stringServerPath, stringAlreadyUploadSize);
		if(nRet != PHMS_SUCCESSFUL_RESULT)
		{
			timerUpload.stop();
			CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
			return nRet;
		}
		nAlreadyUploadSize += nThisTimeUploadSize;
		nRemainSize = nTotalSize - nAlreadyUploadSize;
	}
	timerUpload.stop(true);
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::UploadCommonFile(string& stringUri, string stringType, string stringExtra, string stringLocalFilePath, string stringFileType, pProgressAndSpeedCallback pCallback)
{
	const unsigned long nMaxPerTimeSize = 512*1024;
	unsigned long nTotalSize = 0;
	int nRet = PHMS_SUCCESSFUL_RESULT;
	nRet = CUtil::GetFileSize(stringLocalFilePath, nTotalSize);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(nTotalSize < 3)
	{
		//该条件判断是为了不出现少于3字节上传的情况，因为服务器不支持少于3字节的上传
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_UPLOAD_LESS_3_BYTES), __FUNCTION__, __FILE__, __LINE__);
		return COMMON_UPLOAD_LESS_3_BYTES;
	}
	string stringLocalIniPath = stringLocalFilePath + ".ini";
	string stringServerPath;
	string stringAlreadyUploadSize;
	nRet = LoadServerPathAndAlreadyUploadSize(stringLocalIniPath, stringServerPath, stringAlreadyUploadSize);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	unsigned long nAlreadyUploadSize = 0;
	if(stringAlreadyUploadSize.size() > 0)
	{
		nAlreadyUploadSize = CUtil::StringToULong(stringAlreadyUploadSize);
	}
	if(nAlreadyUploadSize == nTotalSize)
	{
		if(pCallback != NULL)
		{
			(*pCallback)(100, 0, stringServerPath.c_str(), stringLocalFilePath.c_str());
		}
		stringUri = stringServerPath;
		return PHMS_SUCCESSFUL_RESULT;
	}
	if(nAlreadyUploadSize > nTotalSize)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return COMMON_UPLOAD_INI_SIZE_ERROR;
	}

	//创建并启动定时器
	Timer timerUpload(1000, 1000);
	CUploadTimerCallback uploadCallback(pCallback, stringLocalIniPath, nAlreadyUploadSize, nTotalSize, stringLocalFilePath);
	TimerCallback<CUploadTimerCallback> adapterUpload(uploadCallback, &CUploadTimerCallback::onTimer);
	timerUpload.start(adapterUpload);


	unsigned long nRemainSize = nTotalSize-nAlreadyUploadSize;
	while(nRemainSize > 0)
	{
		unsigned long nThisTimeUploadSize = 0;
		if(nRemainSize > nMaxPerTimeSize)
		{
			if(nRemainSize - nMaxPerTimeSize <3)
			{
				//该条件判断是为了不出现少于3字节上传的情况，因为服务器不支持少于3字节的上传
				nThisTimeUploadSize = nRemainSize;
			}
			else
			{
				nThisTimeUploadSize = nMaxPerTimeSize;
			}
		}
		else
		{
			nThisTimeUploadSize = nRemainSize;
		}
		this->SetSaveType(1);
		nRet = this->UploadCommonFileSegment(stringType, stringExtra, nTotalSize, nAlreadyUploadSize, nThisTimeUploadSize, stringServerPath, stringLocalFilePath, stringFileType);
		if(nRet != PHMS_SUCCESSFUL_RESULT)
		{
			timerUpload.stop();
			CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
			return nRet;
		}
		stringServerPath = this->GetContent();
		stringUri = stringServerPath;
		stringAlreadyUploadSize = CUtil::ULongToString(nAlreadyUploadSize+nThisTimeUploadSize);
		nRet = StoreServerPathAndAlreadyUploadSize(stringLocalIniPath, stringServerPath, stringAlreadyUploadSize);
		if(nRet != PHMS_SUCCESSFUL_RESULT)
		{
			timerUpload.stop();
			CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
			return nRet;
		}
		nAlreadyUploadSize += nThisTimeUploadSize;
		nRemainSize = nTotalSize - nAlreadyUploadSize;
	}
	timerUpload.stop(true);
	return PHMS_SUCCESSFUL_RESULT;
}


int CInternalNetIntetface::AbortSession()
{
	m_eventSocketAbort.set();
	return m_pPhmsSession->AbortSession();
}


//内部接口--针对NoPhms接口，直接用http协议请求文件，不通过php
int CInternalNetIntetface::GetFileWithHttp(string stringUrl, int bContinue)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->ExecuteNoPhmsSession(stringUrl, bContinue, false);
	if(nRet==COMMON_SEND_FAIL || nRet==COMMON_SEND_TIMEOUT || nRet==COMMON_RECV_FAIL || nRet==COMMON_RECV_TIMEOUT )
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		nRet = m_pPhmsSession->ExecuteNoPhmsSession(stringUrl, bContinue, true);
		if(nRet != PHMS_SUCCESSFUL_RESULT)
		{
			CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
			return nRet;
		}
	}
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

//内部接口--针对Form表单
int CInternalNetIntetface::PostForm(string stringHost, string stringUrl, map<string, string> mapParam)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->ExecuteFormHttpSession(stringHost, stringUrl, mapParam);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::SetCliniqueState(string stringCliniqueId, string stringState)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->SetCliniqueState(stringCliniqueId, stringState);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}
int CInternalNetIntetface::GetPatientInfoByAppNum(string stringHospitalId, string stringOrderType, string stringOrderId, string stringReqDepart, string stringOrderState, string stringCheckType, string stringStartDate, string stringEndDate, string stringClinicId)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->GetPatientInfoByAppNum(stringHospitalId, stringOrderType, stringOrderId, stringReqDepart, stringOrderState, stringCheckType, stringStartDate, stringEndDate, stringClinicId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}
int CInternalNetIntetface::GetPatientInfoByAppNumToHis(string stringHospitalId, string stringOrderType, string stringOrderId)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->GetPatientInfoByAppNumToHis(stringHospitalId, stringOrderType, stringOrderId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}
int CInternalNetIntetface::GetPatientInfoByAppNumToPhmsAndHis(string stringHospitalId, string stringOrderType, string stringOrderId)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->GetPatientInfoByAppNumToPhmsAndHis(stringHospitalId, stringOrderType, stringOrderId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}
int CInternalNetIntetface::ApplyCheck(string stringCliniqueId, string stringOrderId, string stringOrderType)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->ApplyCheck(stringCliniqueId, stringOrderId, stringOrderType);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		if(nRet != 220303)
		{
			CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		}
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}
int CInternalNetIntetface::GetWaitingCheck(string stringCliniqueId)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->GetWaitingCheck(stringCliniqueId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		if(nRet!=220403 && nRet!=220405)
		{
			CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		}
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}
int CInternalNetIntetface::GetCliniqueState(string stringCliniqueId)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->GetCliniqueState(stringCliniqueId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}
int CInternalNetIntetface::NotifyCaseState(string stringHospitalId, string stringOrderType, string stringOrderId)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->NotifyCaseState(stringHospitalId, stringOrderType, stringOrderId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}
int CInternalNetIntetface::NotifyReportState(string stringReportId)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->NotifyReportState(stringReportId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}
int CInternalNetIntetface::SetDoctorAgentId(string stringDoctorAgentId)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->DoctorChangeSelfInfo("", "", "", "", "", "", stringDoctorAgentId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::NotifyAppState(string stringUnitId, string stringOrderId, string stringOrderType, string stringState)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->NotifyAppState(stringUnitId, stringOrderId, stringOrderType, stringState);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::VerifyAppState(string stringOrderId, string stringOrderType)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->VerifyAppState(stringOrderId, stringOrderType);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::GetHisReportState(string stringHospitalId, string stringOrderType, string stringOrderId)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->GetHisReportState(stringHospitalId, stringOrderType, stringOrderId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::SetHisCriticalValue(string stringHospitalId, string stringCaseId, string stringCriticalValues)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->SetHisCriticalValue(stringHospitalId, stringCaseId, stringCriticalValues);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::GetDoctorList_999120(string stringKtCliniqueId, string stringCaseTypeId)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->GetDoctorList_999120(stringKtCliniqueId, stringCaseTypeId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::ValidatePay_999120(string stringConsumeId, string stringKtCliniqueId, string stringCardNumber, string stringKtDoctorId, string stringConsumeMoney, string stringCaseTypeId)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->ValidatePay_999120(stringConsumeId, stringKtCliniqueId, stringCardNumber, stringKtDoctorId, stringConsumeMoney, stringCaseTypeId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::UpdateCaseForConsumerRecord_999120(string stringConsumeId, string stringKtCaseId, string stringCaseTypeId)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->UpdateCaseForConsumerRecord_999120(stringConsumeId, stringKtCaseId, stringCaseTypeId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::ValidatePay_XinJie(string stringRrn, string stringLocalDate)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	//m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->ValidatePay_XinJie(stringRrn, stringLocalDate);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	//使用0102030405060708作为密钥进行AES解密
	CipherKey::ByteVec secret;
	CipherKey::ByteVec vi;
	string::iterator iter;
	string stringSecret = "0102030405060708";
	for(iter=stringSecret.begin(); iter!=stringSecret.end(); iter++)
	{
		secret.push_back(*iter);
		vi.push_back(*iter);
	}
	Cipher::Ptr pCipher = CipherFactory::defaultFactory().createCipher(CipherKey("aes128", secret, vi));
	string stringIn = m_stringContent;
	for(iter=stringIn.begin(); iter!=stringIn.end(); iter++)
	{
		if(*iter == '+')
		{
			*iter = '*';
		}
	}
	string stringOut;
	try
	{
		stringOut = pCipher->decryptString(stringIn, Cipher::ENC_BASE64);
	}
	catch(Poco::Exception &e)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(e, __FUNCTION__, __FILE__, __LINE__);
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_RECV_CONTENT_FAIL), __FUNCTION__, __FILE__, __LINE__);
		return COMMON_RECV_CONTENT_FAIL;
	}
	catch(const ios::failure& error)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(error.what(), __FUNCTION__, __FILE__, __LINE__);
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_OUTPUT_STREAM_FAIL), __FUNCTION__, __FILE__, __LINE__);
		return COMMON_OUTPUT_STREAM_FAIL;
	}

	
	try
	{
		File fileTarget(m_stringFilePath, "gbk");
		fileTarget.createFile();
	}
	catch(Exception& e)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(e, __FUNCTION__, __FILE__, __LINE__);
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_FILE_OPEN_FAIL), __FUNCTION__, __FILE__, __LINE__);
		return COMMON_FILE_OPEN_FAIL;
	}
	ofstream ofLocalTargetFile;
	ofLocalTargetFile.exceptions(ios::failbit|ios::badbit);
	try
	{
		ofLocalTargetFile.open(m_stringFilePath.c_str(), ios_base::trunc|ios_base::binary);
	}
	catch(const ios::failure& error)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(error.what(), __FUNCTION__, __FILE__, __LINE__);
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_FILE_OPEN_FAIL), __FUNCTION__, __FILE__, __LINE__);
		return COMMON_FILE_OPEN_FAIL;
	}
	try
	{
		ofLocalTargetFile<<stringOut;
		ofLocalTargetFile.flush();
		ofLocalTargetFile.close();
	}
	catch(const ios::failure& error)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(error.what(), __FUNCTION__, __FILE__, __LINE__);
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_FILE_WRITE_FAIL), __FUNCTION__, __FILE__, __LINE__);
		return COMMON_FILE_WRITE_FAIL;
	}
	return PHMS_SUCCESSFUL_RESULT;
}


int CInternalNetIntetface::NotifyCheck_XinJie(string stringRrn, string stringLocalDate)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	//m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->NotifyCheck_XinJie(stringRrn, stringLocalDate);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	//使用0102030405060708作为密钥进行AES解密
	CipherKey::ByteVec secret;
	CipherKey::ByteVec vi;
	string::iterator iter;
	string stringSecret = "0102030405060708";
	for(iter=stringSecret.begin(); iter!=stringSecret.end(); iter++)
	{
		secret.push_back(*iter);
		vi.push_back(*iter);
	}
	Cipher::Ptr pCipher = CipherFactory::defaultFactory().createCipher(CipherKey("aes128", secret, vi));
	string stringIn = m_stringContent;
	for(iter=stringIn.begin(); iter!=stringIn.end(); iter++)
	{
		if(*iter == '+')
		{
			*iter = '*';
		}
	}
	string stringOut;
	try
	{
		stringOut = pCipher->decryptString(stringIn, Cipher::ENC_BASE64);
	}
	catch(Poco::Exception &e)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(e, __FUNCTION__, __FILE__, __LINE__);
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_RECV_CONTENT_FAIL), __FUNCTION__, __FILE__, __LINE__);
		return COMMON_RECV_CONTENT_FAIL;
	}
	catch(const ios::failure& error)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(error.what(), __FUNCTION__, __FILE__, __LINE__);
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_OUTPUT_STREAM_FAIL), __FUNCTION__, __FILE__, __LINE__);
		return COMMON_OUTPUT_STREAM_FAIL;
	}


	try
	{
		File fileTarget(m_stringFilePath, "gbk");
		fileTarget.createFile();
	}
	catch(Exception& e)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(e, __FUNCTION__, __FILE__, __LINE__);
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_FILE_OPEN_FAIL), __FUNCTION__, __FILE__, __LINE__);
		return COMMON_FILE_OPEN_FAIL;
	}
	ofstream ofLocalTargetFile;
	ofLocalTargetFile.exceptions(ios::failbit|ios::badbit);
	try
	{
		ofLocalTargetFile.open(m_stringFilePath.c_str(), ios_base::trunc|ios_base::binary);
	}
	catch(const ios::failure& error)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(error.what(), __FUNCTION__, __FILE__, __LINE__);
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_FILE_OPEN_FAIL), __FUNCTION__, __FILE__, __LINE__);
		return COMMON_FILE_OPEN_FAIL;
	}
	try
	{
		ofLocalTargetFile<<stringOut;
		ofLocalTargetFile.flush();
		ofLocalTargetFile.close();
	}
	catch(const ios::failure& error)
	{
		//写日志
		CPhmsLogger::GetPhmsLogger()->WriteLog(error.what(), __FUNCTION__, __FILE__, __LINE__);
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_FILE_WRITE_FAIL), __FUNCTION__, __FILE__, __LINE__);
		return COMMON_FILE_WRITE_FAIL;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::UserAuthorize_Telecom(string stringPersonId, string stringName, string stringNation, string stringAddress, string stringRoomId)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->UserAuthorize_Telecom(stringPersonId, stringName, stringNation, stringAddress, stringRoomId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::BloodPressureUpload_Telecom(string stringPersonType, string stringPersonId, string stringSBP, string stringDBP, string stringAverageValue, string stringPulseRate, string stringDetectionTime, 
											  string stringEquCode, string stringRoomId)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->BloodPressureUpload_Telecom(stringPersonType, stringPersonId, stringSBP, stringDBP, stringAverageValue, stringPulseRate, stringDetectionTime, stringEquCode, stringRoomId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::BloodOxygenUpload_Telecom(string stringPersonType, string stringPersonId, string stringBloodOxygen, string stringPulseRate, string stringDetectionTime, 
							  string stringEquCode, string stringRoomId)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->BloodOxygenUpload_Telecom(stringPersonType, stringPersonId, stringBloodOxygen, stringPulseRate, stringDetectionTime, stringEquCode, stringRoomId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::BloodSugarUpload_Telecom(string stringPersonType, string stringPersonId, string stringBloodSugar, string stringDetectionTime, 
							 string stringEquCode, string stringRoomId)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->BloodSugarUpload_Telecom(stringPersonType, stringPersonId, stringBloodSugar, stringDetectionTime, stringEquCode, stringRoomId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::HeightWeightUpload_Telecom(string stringPersonType, string stringPersonId, string stringHeight, string stringWeight, string stringBMI, string stringDetectionTime, 
											 string stringEquCode, string stringRoomId)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->HeightWeightUpload_Telecom(stringPersonType, stringPersonId, stringHeight, stringWeight, stringBMI, stringDetectionTime, stringEquCode, stringRoomId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::LungUpload_Telecom(string stringPersonType, string stringPersonId, string stringM_B1, string stringBSA, string stringBSA1, string stringMMF, string stringMefSeventyFive, string stringMefFifty, 
									 string stringMefTwentyFive, string stringMefFVST, string stringIC, string stringIRV, string stringERV, string stringFevOne, string stringFevTwo, string stringFevThree, 
									 string stringFevOnePer, string stringFevTwoPer, string stringFevThreePer, string stringRR, string stringFVC, string stringPEF, string stringTV, string stringVC, string stringMVV, 
									 string stringMV, string stringVSTHeight, string stringVSMvvBsa, string stringVSMvvOne, string stringSVC, string stringFevOneZeroFive, string stringFevOneZeroFivePer, string stringPefOne, 
									 string stringPefTwo, string stringPefThree, string stringFive, string stringFiveOne, string stringFiveTwo, string stringFiveThree, string stringPIF, string stringMifFifty, 
									 string stringDetectionTime, string stringEquCode, string stringRoomId)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->LungUpload_Telecom(stringPersonType, stringPersonId, stringM_B1, stringBSA, stringBSA1, stringMMF, stringMefSeventyFive, stringMefFifty, stringMefTwentyFive, stringMefFVST, stringIC, 
		stringIRV, stringERV, stringFevOne, stringFevTwo, stringFevThree, stringFevOnePer, stringFevTwoPer, stringFevThreePer, stringRR, stringFVC, stringPEF, stringTV, stringVC, stringMVV, 
		stringMV, stringVSTHeight, stringVSMvvBsa, stringVSMvvOne, stringSVC, stringFevOneZeroFive, stringFevOneZeroFivePer, stringPefOne, stringPefTwo, stringPefThree, stringFive, stringFiveOne, 
		stringFiveTwo, stringFiveThree, stringPIF, stringMifFifty, stringDetectionTime, stringEquCode, stringRoomId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::ECGUpload_Telecom(string stringPersonType, string stringPersonId, string stringDetectionTime, string stringEquCode, string stringRoomId, TELCOM_ECG ecgData)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->ECGUpload_Telecom(stringPersonType, stringPersonId, stringDetectionTime, stringEquCode, stringRoomId, ecgData);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::Login_XiKang(string stringUsername, string stringPassword)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->Login_XiKang(stringUsername, stringPassword);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::RegisterPatientId_XiKang(string stringName, string stringSex, string stringPhoneNum, string stringProofNum)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->RegisterPatientId_XiKang(stringName, stringSex, stringPhoneNum, stringProofNum);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::RegisterPatientInfo_XiKang(string stringPatientOpenId, string stringName, string stringSex, string stringPhoneNum, string stringProofNum, string stringPassword)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->RegisterPatientInfo_XiKang(stringPatientOpenId, stringName, stringSex, stringPhoneNum, stringProofNum, stringPassword);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::BindPatient_XiKang(string stringDoctorOpenId, string stringPatientOpenId)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->BindPatient_XiKang(stringDoctorOpenId, stringPatientOpenId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::GetPatientIdList_XiKang(string stringDoctorOpenId)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->GetPatientIdList_XiKang(stringDoctorOpenId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::SearchPatientId_XiKang(string stringKey, string stringValue)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->SearchPatientId_XiKang(stringKey, stringValue);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::GetPatientInfo_XiKang(string stringPatientOpenId)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->GetPatientInfo_XiKang(stringPatientOpenId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::GetCaseList_XiKang(vector<string> vPatientOpenId)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->GetCaseList_XiKang(vPatientOpenId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::SubmitCase_XiKang(string stringPatientOpenId, string stringAdvice, string stringCaseType, string stringEcgXmlContent)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->SubmitCase_XiKang(stringPatientOpenId, stringAdvice, stringCaseType, stringEcgXmlContent);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::GetReportFile_XiKang(string stringCaseId)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->GetReportFile_XiKang(stringCaseId);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	if(m_stringContent.find(".pdf") == string::npos)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(COMMON_INVALID_URL), __FUNCTION__, __FILE__, __LINE__);
		return COMMON_INVALID_URL;
	}
	this->SetSaveType(2);
	this->SetFilePath(m_stringFilePath);
	string stringWorkingDir = CUtil::GetCurrentAppDir();
	string stringIniFilePath = stringWorkingDir+"PhmsConfig.ini";
	AutoPtr<IniFileConfiguration> iniFile = new IniFileConfiguration(stringIniFilePath, "gbk");
	string stringXiKangPdfAddress = iniFile->getString("XIKANG.PdfAddress", "dlres.xikang.com");
	string stringPdfUrl = "http://";
	stringPdfUrl += stringXiKangPdfAddress;
	//stringPdfUrl += "/phrhealthfile/d53353f72df6472c9d739e82a95b0326.pdf";
	stringPdfUrl += m_stringContent;
	nRet = GetFileWithHttp(stringPdfUrl, false);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::UploadFile_XiKang(string stringCaseId, string stringCaseFilePath)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->UploadFile_XiKang(stringCaseId, stringCaseFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::ExecuteGWService(string stringLastPath, string stringJson, string stringFilePath)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	map<string, string> mapParam;
	mapParam["format"] = "json";
	mapParam["content"] = stringJson;
	if(stringLastPath == "login")
	{
		mapParam["sid"] = "";
		mapParam["sign"] = "";
		mapParam["nonce"] = "";
	}
	else
	{
		mapParam["sid"] = this->GetGWSessionId();
		mapParam["nonce"] = UUIDGenerator::defaultGenerator().createOne().toString().substr(0, 32);
		HMACEngine<SHA1Engine> hmacSign(m_pPhmsSession->GetGWSecretKey());
		hmacSign.update(m_pPhmsSession->GetGWSecretKey() + stringJson + mapParam["sid"] + mapParam["nonce"]);
		mapParam["sign"] = DigestEngine::digestToHex(hmacSign.digest());
	}
	nRet = m_pPhmsSession->ExecuteGWService(stringLastPath, mapParam, stringFilePath);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(stringLastPath == "login")
	{
		map<string, string> mapResult;
		CUtil::ParseJsonObjectWithoutArray(m_nSaveType, m_pPhmsSession->GetContent(), m_stringFilePath, mapResult);
		if(mapResult["sid"] != "")
		{
			this->SetGWSessionId(mapResult["sid"]);
			CSessionIdManager::HandleGWSessionSync(mapResult["sid"]);
		}
		if(mapResult["secret_key"] != "")
		{
			m_pPhmsSession->SetGWSecretKey(mapResult["secret_key"]);
		}
	}
// 	if(mapResult["data"] != "")
// 	{
// 		CUtil::ParseJsonObjectWithoutArray(1, mapResult["data"], "", mapResult);
// 		if(mapResult["sid"] != "")
// 		{
// 			this->SetGWSessionId(mapResult["sid"]);
// 			CSessionIdManager::HandleGWSessionSync(mapResult["sid"]);
// 		}
// 		if(mapResult["secret_key"] != "")
// 		{
// 			m_pPhmsSession->SetGWSecretKey(mapResult["secret_key"]);
// 		}
// 	}
	
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}

int CInternalNetIntetface::ExecuteCommonRESTful(string stringIp, string stringPort, string stringUrl, string stringContentType, string stringPayload)
{
	int nRet = PHMS_SUCCESSFUL_RESULT;
	m_pPhmsSession->SetSaveType(m_nSaveType);
	m_pPhmsSession->SetFilePath(m_stringFilePath);
	nRet = m_pPhmsSession->ExecuteCommonRESTful(stringIp, stringPort, stringUrl, stringContentType, stringPayload);
	if(nRet != PHMS_SUCCESSFUL_RESULT)
	{
		CPhmsLogger::GetPhmsLogger()->WriteLog(CUtil::GetErrorMsg(nRet), __FUNCTION__, __FILE__, __LINE__);
		return nRet;
	}
	if(m_nSaveType == 1)
	{
		m_stringContent = m_pPhmsSession->GetContent();
	}
	return PHMS_SUCCESSFUL_RESULT;
}