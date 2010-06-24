// SulParser.cpp

#include "stdafx.h"
#include "SulParser.h"
#include "SulTypes.h"
#include "SulFile.h"
#include "SulString.h"

using namespace Sigma;

/*
#ifdef TARGET_WINDOWS
#pragma warning ( disable:4127 )		// conditional expression is constant
#endif//TARGET_WINDOWS
*/

CSulParser::CSulParser()
{
	m_dwSize	= 0;
	m_pBuf		= 0;
	m_dwIndex	= 0;

	memset( &m_cDelimiters, 0, 256 );
	memset( &m_cTokens, 0, 256 );
	memset( &m_cSentenceStart, 0, 256 );
	memset( &m_cSentenceEnd, 0, 256 );
	
	// create default char delimiters
	AddCharDelimiter( ' ' );			// space
	AddCharDelimiter( 0x0D );			// return 
	AddCharDelimiter( 0x09 );			// tab
	AddCharDelimiter( 0x0A );			// new line

	// create default char tokens
	AddCharToken( ';' );
	AddCharToken( ',' );
	AddCharToken( '(' );
	AddCharToken( ')' );
	AddCharToken( '[' );
	AddCharToken( ']' );
	AddCharToken( '=' );
	AddCharToken( '+' );
	
	// create default comment starter
	AddCommentStart( "/*" );

	// create default comment end
	AddCommentEnd( "*/" );

	// create default comment eol
	AddCommentEOL( "//" );

	// create default sentence start
	AddSentenceCharStart( '"' );

	// create default sentence end
	AddSentenceCharEnd( '"' );

	m_vecTokens.reserve( 1024 );
}

CSulParser::~CSulParser()
{
	if ( m_pBuf )
	{
		delete[] m_pBuf;
	}
}

bool CSulParser::Init( const CSulString& sFileName )
{
	osg::ref_ptr<CSulFile> fileTmp = new CSulFile;
	
	if ( !fileTmp->Load( sFileName ) )
	{
		return false;
	}
	
	return ParseData( fileTmp->GetDataPtr(), fileTmp->GetSize() );
}

bool CSulParser::InitStringParse( const CSulString& s )
{
	if ( s.empty() )
	{
		return false;
	}

	// FIXME: who deletes pBuf???
	Sigma::uint32 dwSize = s.size();
	Sigma::uint8* pBuf = new Sigma::uint8[dwSize];
	memcpy( pBuf, s.c_str(), dwSize );
	return ParseData( pBuf, dwSize );
}

bool CSulParser::ParseData( const Sigma::uint8* pData, Sigma::uint32 iSize )
{
	assert( !m_pBuf );

	m_dwSize = iSize;
	
	m_pBuf = new Sigma::uint8[m_dwSize];

	memcpy( m_pBuf, pData, m_dwSize );
	
	PreProcessData();
	
	return true;
}

void CSulParser::AddCharDelimiter( char cChar )
{
	m_cDelimiters[(unsigned int) cChar] = 1;
}

void CSulParser::AddCharToken( char cChar )
{
	m_cTokens[(unsigned int) cChar] = 1;
}

void CSulParser::RemoveCharToken( Sigma::int8 cChar )
{
	m_cTokens[(unsigned int) cChar] = 0;
}

void CSulParser::AddCommentStart( char* lpszCommentStart )
{
	string strTmp;

	strTmp = lpszCommentStart;

	m_vecCommentStart.push_back( CMyString(strTmp) );
}

void CSulParser::AddCommentEnd( char* lpszCommentEnd )
{
	string strTmp;

	strTmp = lpszCommentEnd;

	m_vecCommentEnd.push_back( strTmp );
}

void CSulParser::AddCommentEOL( char* lpszCommentEOL )
{
	string strTmp;

	strTmp = lpszCommentEOL;

	m_vecCommentEOL.push_back( strTmp );
}

void CSulParser::AddSentenceStart( char* lpszText )
{
	string strTmp;

	strTmp = lpszText;

	m_vecSentenceStart.push_back( strTmp );
}

void CSulParser::AddSentenceEnd( char* lpszText )
{
	string strTmp;

	strTmp = lpszText;

	m_vecSentenceEnd.push_back( strTmp );
}

void CSulParser::AddSentenceCharStart( char cChar )
{
	m_cSentenceStart[(unsigned int) cChar] = 1;
}

void CSulParser::AddSentenceCharEnd( char cChar )
{
	m_cSentenceEnd[(unsigned int) cChar] = 1;
}

long CSulParser::IsSentenceStart( char* lpszText )
{
	if ( !m_vecSentenceStart.size() )
	{
		return 0;
	}

	VECTOR_STRING_REF::iterator iString;

	iString = m_vecSentenceStart.begin();
	while ( iString!=m_vecSentenceStart.end() )
	{
		if ( !strcmp( iString->m_str.c_str(), lpszText ) )
			return (long)iString->m_str.size();

		++iString;
	}

	return 0;
}

long CSulParser::IsSentenceEnd( char* lpszText )
{
	if ( !m_vecSentenceEnd.size() )
	{
		return 0;
	}

	VECTOR_STRING_REF::iterator iString;

	iString = m_vecSentenceEnd.begin();
	while ( iString!=m_vecSentenceEnd.end() )
	{
		if ( strstr( lpszText, iString->m_str.c_str() ) )
			return (long)iString->m_str.size();

		++iString;
	}

	return 0;
}

bool CSulParser::IsCommentStart( char* lpszText )
{
	VECTOR_STRING_REF::iterator iString;

	iString = m_vecCommentStart.begin();
	while ( iString!=m_vecCommentStart.end() )
	{
		if ( !strcmp( iString->m_str.c_str(), lpszText ) )
			return true;

		++iString;
	}

	return false;
}

bool CSulParser::IsCommentEnd(  char* lpszText )
{
	VECTOR_STRING_REF::iterator iString;

	iString = m_vecCommentEnd.begin();
	while ( iString!=m_vecCommentEnd.end() )
	{
		if ( strstr( lpszText, iString->m_str.c_str() ) )
			return true;

		++iString;
	}

	return false;
}

bool CSulParser::IsCommentEOL( char* lpszText )
{
	VECTOR_STRING_REF::iterator iString;

	iString = m_vecCommentEOL.begin();
	while ( iString!=m_vecCommentEOL.end() )
	{
		if ( strstr( lpszText, iString->m_str.c_str() ) )
			return true;

		++iString;
	}

	return false;
}

void CSulParser::PreProcessData()
{
	uint32			dwIndex;
	long			lSize;
	std::string		strTmp;

	dwIndex = 0;

	while ( dwIndex<m_dwSize )
	{
		strTmp.clear();

		// if character is a token then add it to tokens vector
		if ( m_cTokens[(unsigned int) m_pBuf[dwIndex]] )
		{
			strTmp = m_pBuf[dwIndex];
			m_vecTokens.push_back( strTmp );
			++dwIndex;
			continue;
		}

		while ( 1 )
		{
			// if character is a delimiter 
			if ( m_cDelimiters[(unsigned int) m_pBuf[dwIndex]] )
			{
				if ( strTmp.size() )
				{
					m_vecTokens.push_back( strTmp );
				}

				++dwIndex;
				break;
			}

			//if ( dwIndex==(m_dwSize-1) )
			if ( dwIndex==m_dwSize )
			{
				m_vecTokens.push_back( strTmp );
				break;
			}

			if ( m_cTokens[(unsigned int) m_pBuf[dwIndex]] )
			{
				// if any string has been made then add it
				if ( strTmp.size() )
				{
					m_vecTokens.push_back( strTmp );
				}

				// add the character as a token
				m_vecTokens.push_back( CSulString( m_pBuf[dwIndex] ) );
				++dwIndex;

				break;
			}

			if ( m_cSentenceStart[(unsigned int) m_pBuf[dwIndex]] )
			{
				if ( strTmp.size() )
				{
					m_vecTokens.push_back( strTmp );
				}

				strTmp.clear();
				++dwIndex;

				// fast forward to sentence char end
				while ( dwIndex<m_dwSize )
				{
					if ( m_cSentenceEnd[(unsigned int) m_pBuf[dwIndex]] )
					{
						++dwIndex;
						break;
					}

					strTmp += m_pBuf[dwIndex];

					++dwIndex;
				}

				m_vecTokens.push_back( strTmp );

				break;
			}

			strTmp += m_pBuf[dwIndex];
			++dwIndex;

			lSize = IsSentenceStart( (char*)strTmp.c_str() );
			if ( lSize )
			{
				strTmp.clear();

				// fast forward to sentence end
				while ( dwIndex<m_dwSize )
				{
					lSize = IsSentenceEnd( (char*)strTmp.c_str() );
					if ( lSize )
					{
						strTmp.resize( strTmp.size()-lSize );
						break;
					}

					++dwIndex;
				}

				break;
			}

			if ( IsCommentEOL( (char*)strTmp.c_str() ) )
			{
				// fast forward to EOL and break
				while ( dwIndex<m_dwSize )
				{
					if ( m_pBuf[dwIndex]==0x0d )
					{
						++dwIndex;
						break;
					}

					++dwIndex;
				}

				break;
			}

			if ( IsCommentStart( (char*)strTmp.c_str() ) )
			{
				// fast forward to comment end and break
				strTmp.clear();
				++dwIndex;

				while ( dwIndex<m_dwSize )
				{
					strTmp += m_pBuf[dwIndex];
					++dwIndex;

					if ( IsCommentEnd( (char*)strTmp.c_str()) )
						break;
				}

				break;
			}
		}

		
	}
}

void CSulParser::Reset()
{
	m_dwIndex = 0;
}

void CSulParser::IncIndex( uint32 i )
{
	if ( (m_dwIndex+i)>=m_vecTokens.size() )
	{
		m_dwIndex = (uint32)m_vecTokens.size();
		return;
	}

	m_dwIndex += i;
}

void CSulParser::DecIndex( uint32 i )
{
	if ( i<=m_dwIndex )
	{
		m_dwIndex -= i;
	}
}

void CSulParser::Goto( const CSulString& sText )
{
	const int8* p;

	p = GetToken();
	while ( p )
	{
		if ( CSulString(p)==sText )
		{
			return;
		}

		p = GetToken();
	}
}

int8* CSulParser::GetToken()
{
	if ( m_dwIndex==m_vecTokens.size() )
		return 0;

	m_dwIndex++;

	return (char*)m_vecTokens.begin()[m_dwIndex-1].m_str.c_str();
}

CSulString CSulParser::GetStringToken()
{
	return CSulString( GetToken() );
}

char* CSulParser::PeekToken()
{
	if ( m_dwIndex==m_vecTokens.size() )
		return 0;

	return (char*)m_vecTokens.begin()[m_dwIndex].m_str.c_str();
}

char* CSulParser::OldToken()
{
	if ( !m_dwIndex )
		return 0;

	return (char*)m_vecTokens.begin()[m_dwIndex-1].m_str.c_str();
}

