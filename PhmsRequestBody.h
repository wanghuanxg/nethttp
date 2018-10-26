#pragma once
#include <string>
#include <iostream>
using namespace std;
class CPhmsRequestBody
{
public:
	CPhmsRequestBody(void);
	CPhmsRequestBody(string xmlContent);
	CPhmsRequestBody(CPhmsRequestBody& phmsRequestBody);
	CPhmsRequestBody& operator=(CPhmsRequestBody& phmsRequestBody);
	~CPhmsRequestBody(void);
	//Getter
	string GetXmlContent();
	//Setter
	void SetXmlContent(string xmlContent);
	string ToString();
private:
	string m_xmlContent;
};
