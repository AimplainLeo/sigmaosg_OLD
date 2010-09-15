// SulXmlDataInfo.cpp

#include "stdafx.h"
#include "SulXmlDataInfo.h"

#ifdef _DEBUG
#define new DEBUG_CLIENTBLOCK
#endif

void CSulXmlDataInfo::setAttr( CSulXmlAttr* pAttr )
{
	m_rAttr = pAttr;
}

CSulXmlAttr* CSulXmlDataInfo::getAttr()
{
	return m_rAttr.get();
}

void CSulXmlDataInfo::setName( const char* pName )
{
	m_strName = pName;
}

const CSulString& CSulXmlDataInfo::getName() const
{
	return m_strName;
}