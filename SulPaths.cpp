// SulPaths.cpp

#include "stdafx.h"
#include "SulPaths.h"
#include "SulXmlAttr.h"
#include "SulXmlWriter.h"

void CSulPaths::Add( const CSulString& sPath )
{
	m_vecPath.push_back( sPath );
}

sigma::uint32 CSulPaths::GetCount()
{
	return (sigma::uint32)m_vecPath.size();
}

const CSulPaths::VECTOR_PATH& CSulPaths::GetPaths() const
{
	return m_vecPath;
}

void CSulPaths::Load( CSulXmlAttr* pAttr )
{
	if ( pAttr->exist( "location" ) )
	{
		CSulString& s = pAttr->get( "location" );
		m_vecPath.push_back( s );
	}
}

void CSulPaths::Save( CSulXmlWriter* pWriterXML )
{
	VECTOR_PATH::iterator i;

	i = m_vecPath.begin();
	while ( i!=m_vecPath.end() )
	{
		CSulXmlAttr* pAttr = pWriterXML->elementStart( "PATH" );
		pAttr->add( "location", (*i).c_str() );
		pWriterXML->elementEnd();

		++i;
	}
}
