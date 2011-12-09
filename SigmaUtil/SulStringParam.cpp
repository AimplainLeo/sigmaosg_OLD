// SulStringParam.cpp

#include "stdafx.h"
#include "SulStringParam.h"
#include "SulParser.h"

CSulStringParam::CSulStringParam( const CSulString& s )
{
	addParamString( s );
}

// param strings are defined as param:value i.e. "width:100"
void CSulStringParam::addParamString( const CSulString& s )
{
	osg::ref_ptr<CSulParser> rParser = new CSulParser;
	rParser->AddSentenceCharStart( 39 );
	rParser->AddSentenceCharEnd( 39 );
	rParser->RemoveCharToken( ',' );
	rParser->AddCharDelimiter( ',' );
	rParser->AddCharDelimiter( ':' );
	rParser->ParseData( (sigma::uint8 *)s.c_str(), s.size() );

	const char* pTokenParam = rParser->GetToken();
	const char* pTokenValue = rParser->GetToken();
	while ( pTokenParam && pTokenValue )
	{
		add( pTokenParam, pTokenValue );
		pTokenParam = rParser->GetToken();
		pTokenValue = rParser->GetToken();
	}
}

void CSulStringParam::add( const CSulString& sParam, const CSulString& sValue )
{
	m_multimapParamValue.insert( MULTIMAP_PARAMVALUE::value_type(sParam, sValue) );
}

bool CSulStringParam::get( const CSulString& sParam, float& f, sigma::int32 index )
{
	if ( m_multimapParamValue.count( sParam )==0 )
		return false;

	MULTIMAP_PARAMVALUE::iterator i;
	i = m_multimapParamValue.lower_bound(sParam);
	if ( i==m_multimapParamValue.end() )
	{
		return false;
	}

	for ( sigma::int32 ii=0; ii<index; ii++ ) i++;
	f = i->second.asFloat();
	return true;
}

bool CSulStringParam::get( const CSulString& sParam, CSulString& sOut, sigma::int32 index )
{
	if ( m_multimapParamValue.count( sParam )==0 )
		return false;

	sigma::int32 isize = m_multimapParamValue.size();
	MULTIMAP_PARAMVALUE::iterator i;
	i = m_multimapParamValue.find( sParam );
	if ( i==m_multimapParamValue.end() )
	{
		return false;
	}

	for ( sigma::int32 ii=0; ii<index; ii++ ) i++;
	sOut = i->second;
	return true;
}

bool CSulStringParam::get( const CSulString& sParam, bool& b, sigma::int32 index )
{
	if ( m_multimapParamValue.count( sParam )==0 )
		return false;

	sigma::int32 isize = m_multimapParamValue.size();
	MULTIMAP_PARAMVALUE::iterator i;
	i = m_multimapParamValue.find( sParam );
	if ( i==m_multimapParamValue.end() )
	{
		return false;
	}

	for ( sigma::int32 ii=0; ii<index; ii++ ) i++;
	b = i->second.asBool();

	return true;
}

bool CSulStringParam::get( const CSulString& sParam, sigma::uint32& i, sigma::int32 index )
{
	if ( m_multimapParamValue.count( sParam )==0 )
		return false;

	MULTIMAP_PARAMVALUE::iterator it;
	it = m_multimapParamValue.lower_bound(sParam);
	if ( it==m_multimapParamValue.end() )
	{
		return false;
	}

	for ( sigma::int32 ii=0; ii<index; ii++ ) it++;
	i = it->second.asUint32();
	return true;
}