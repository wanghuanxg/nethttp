#pragma once
#include <string>
#include <vector>
#include <map>
using std::string;
using std::vector;
using std::map;
#include "Poco/Types.h"


class CUtil
{
public:
	static string& Ltrim(string &str);
	static string& Rtrim(string &str);
	static string& Trim(string &str);
	static string DoubleToString(long double dValue, int decplaces=6);
	static string ULongToString(unsigned long nValue);
	static unsigned long StringToULong(string stringValue);
	static string UIntToString(unsigned int nValue);
	static string Int64ToString(Poco::Int64 nValue);
	static string GetMd5(const char* bufSrc, int nLength);
	static int GetMd5(string stringFilePath, string& stringMd5);
	static int GetFileSize(string stringFilePath, unsigned long& nFileSize);
	static unsigned long GetFileSize(string stringFilePath);
	static int GetXmlFieldVaule(int nSaveType, string stringXmlContent, string stringXmlPath, string stringField, string& stringVaule);
	static int GetOrReplaceImagePathVector(int nSaveType, string stringXmlContent, string stringXmlPath, vector<string>& vImagePath);//���ٴ���Replace
	static int GetMapFromRequestXml(string stringXmlContent, string stringKey, map<string, string>& mapParam);
	static string GetErrorMsg(int nError);
	static int GBKToUTF8(string stringGbk, string& stringUtf8);
	static int UTF8ToGBK(string stringUtf8, string& stringGBK);
	static int EnvToUtf8(string stringEnv, string& stringUtf8);
	static string EnvToUtf8(string stringEnv);
	static int UTF8ToEnv(string stringUtf8, string& stringEnv);
	static string GetCurrentAppDir();
	static bool IsLegalGBK(string stringSrc);
	static bool IsLegalKOI8R(string stringSrc);
	static bool IsLegalUTF8(string stringSrc);
	static void SetAppDir(string stringWorkDir);//����Android�ⲿ���ù���Ŀ¼
	static void ReplaceStringInFile(string stringPath, string stringSrc, string stringDst);
	static void ReplaceString(string& stringContent, string stringSrc, string stringDst);//�滻Դ�ַ����е�����stringSrc�����ǵ�һ��
	static string GetFileNameFromPath(string stringPath);
	static void ParseJsonObjectWithoutArray(int nSaveType, string stringJsonContent, string stringJsonPath, map<string, string>& mapResult);
	static string MapToJson(map<string, string> mapParam);
	static int ParseStandardQueryString(string stringQuery, string& stringId, string& stringIdType);
private:
	static string m_stringAppDir;
};
