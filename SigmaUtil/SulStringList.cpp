// SulStringList.cpp

#include "stdafx.h"
#include "SulStringList.h"
#include "SulParser.h"

CSulStringList::CSulStringList( const CSulString& s )
{
	Tokenize( s );
}

void CSulStringList::Tokenize( const CSulString& s )
{
	osg::ref_ptr<CSulParser> rParser = new CSulParser;
	rParser->RemoveCharToken( ',' );
	rParser->AddCharDelimiter( ',' );

	rParser->InitStringParse( s );

	sigma::int8* pToken = 0;
	while ( pToken = rParser->GetToken() )
	{
		m_vecString.push_back( pToken );
	}
}

const CSulStringList::VECTOR_STRING& CSulStringList::getList() const
{
	return m_vecString;
}

CSulString CSulStringList::getString( sigma::uint32 index )
{
	if ( !m_vecString.size() )
	{
		return "";
	}

	if ( index>=m_vecString.size() )
	{
		return "";
	}

	return m_vecString[index];
}

sigma::uint32 CSulStringList::getIndex( const CSulString& s )
{
	VECTOR_STRING::iterator iFound = find( m_vecString.begin(), m_vecString.end(), s );
	if ( iFound!=m_vecString.end() )
	{
		return (sigma::uint32)std::distance( m_vecString.begin(), iFound );
	}

	return 0;
}
