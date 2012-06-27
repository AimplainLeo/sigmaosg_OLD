// SulGuiThemeXml.cpp

#include "stdafx.h"
#include "SulGuiThemeXml.h"

CSulGuiThemeXml::CSulGuiThemeXml() :
CSulXmlParser()
{
}

CSulString CSulGuiThemeXml::getValue( const CSulString& element, const CSulString& attr )
{
	MAP_ELEMENT::iterator iFound = m_mapElement.find( element );
	if ( iFound!=m_mapElement.end() )
	{
		return iFound->second->get( attr );
	}

	return "";
}

void CSulGuiThemeXml::elementStart( const class CSulString& sName, class CSulXmlAttr* pAttr, CSulString sData )
{
	if ( sName!="THEME" )
	{
		m_mapElement[sName] = pAttr;
	}
}

