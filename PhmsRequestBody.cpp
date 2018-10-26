#include "PhmsRequestBody.h"

CPhmsRequestBody::CPhmsRequestBody(void)
{
}


CPhmsRequestBody::CPhmsRequestBody(string xmlContent):m_xmlContent(xmlContent)
{
}

CPhmsRequestBody::CPhmsRequestBody(CPhmsRequestBody& phmsRequestBody)
{
	m_xmlContent = phmsRequestBody.GetXmlContent();
}

CPhmsRequestBody& CPhmsRequestBody::operator=(CPhmsRequestBody& phmsRequestBody)
{
	m_xmlContent = phmsRequestBody.GetXmlContent();
	return *this;
}


CPhmsRequestBody::~CPhmsRequestBody(void)
{
}



//Getter
string CPhmsRequestBody::GetXmlContent()
{
	return m_xmlContent;
}

//Setter
void CPhmsRequestBody::SetXmlContent(string xmlContent)
{
	m_xmlContent = xmlContent;
}


string CPhmsRequestBody::ToString()
{
	return m_xmlContent;
}



