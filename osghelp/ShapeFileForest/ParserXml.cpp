// ParserXml.cpp

#include "stdafx.h"
#include "ParserXml.h"
#include <SigmaUtil/SulXmlAttr.h>
#include <SigmaUtil/SulDataShapeMask.h>

void CParserXml::ElementStart( const CSulString& sName, CSulXmlAttr* pAttr )
{
	if ( sName=="MASK" )
	{
		m_vecDataMask.push_back( 
			*new CSulDataShapeMask(
				pAttr->get( "file" ),
				pAttr->get( "lineDist" ).asFloat()
			)
		);
	}
}

VEC_DATASHAPEMASK& CParserXml::getDataShapeMask()
{
	return m_vecDataMask;
}