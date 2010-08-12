// SulTokenizer.cpp

#include "stdafx.h"
#include "SulTokenizer.h"

CSulTokenizer::CSulTokenizer()
{
	memset( m_cToken, 0, 256 );
	memset( m_cDelimiter, 0, 256 );
	memset( m_cSentenceStart, 0, 256 );
	memset( m_cSentenceEnd, 0, 256 );

	// create default char delimiters
	RemoveCharAsDelimiter( ' ' );			// space
	RemoveCharAsDelimiter( 0x0D );			// return 
	RemoveCharAsDelimiter( 0x09 );			// tab
	RemoveCharAsDelimiter( 0x0A );			// new line

	// create default char tokens
	AddCharAsToken( ';' );
	AddCharAsToken( ',' );
	AddCharAsToken( '(' );
	AddCharAsToken( ')' );
	AddCharAsToken( '[' );
	AddCharAsToken( ']' );
	AddCharAsToken( '=' );
	AddCharAsToken( '+' );

	// create default sentence start
	AddCharAsSentenceStart( '"' );

	// create default sentence end
	AddCharAsSentenceEnd( '"' );
}

const CSulString& CSulTokenizer::GetToken( sigma::uint32 iIndex )
{
	static CSulString tmp( "" );

	if ( iIndex>=m_vecToken.size() )
	{
		return tmp;
	}

	return m_vecToken[iIndex];
}

void CSulTokenizer::AddCharAsToken( sigma::uint8 c )
{
	m_cToken[c] = 1;
}

void CSulTokenizer::RemoveCharAsToken( sigma::uint8 c )
{
	m_cToken[c] = 0;
}

void CSulTokenizer::AddCharAsDelimiter( sigma::uint8 c )
{
	m_cDelimiter[c] = 1;
}

void CSulTokenizer::RemoveCharAsDelimiter( sigma::uint8 c )
{
	m_cDelimiter[c] = 0;
}

void CSulTokenizer::AddCharAsSentenceStart( sigma::uint8 c )
{
	m_cSentenceStart[c] = 1;
}

void CSulTokenizer::RemoveCharAsSentenceStart( sigma::uint8 c )
{
	m_cSentenceStart[c] = 0;
}

void CSulTokenizer::AddCharAsSentenceEnd( sigma::uint8 c )
{
	m_cSentenceStart[c] = 1;
}

void CSulTokenizer::RemoveCharAsSentenceEnd( sigma::uint8 c )
{
	m_cSentenceStart[c] = 0;
}

// peter+marino				tokens: [peter] [+] [marino]
// peter"wraae marino"		tokens: [peter] [wraae marino]
bool CSulTokenizer::Parse( sigma::uint8* pData, sigma::uint32 iSize )
{
	sigma::uint32 i = 0;
	std::string sToken;

	while ( i<iSize )
	{
		// is char a token?
		if ( IsCharToken( pData[i] ) )
		{
			// token has already been built, so push it first
			if ( !sToken.empty() )
			{
				m_vecToken.push_back( sToken );
				sToken.clear();
			}

			// push character as token
			m_vecToken.push_back( pData[i] );
			++i;
			continue;
		}

		// is char a delimiter?
		if ( IsCharDelimiter( pData[i] ) )
		{
			// token has already been built, so push it
			if ( !sToken.empty() )
			{
				m_vecToken.push_back( sToken );
				sToken.clear();
			}

			++i;
			continue;
		}

		// is char a sentence start?
		if ( IsCharSentenceStart( pData[i] ) )
		{
			// token has already been built, so push it
			if ( !sToken.empty() )
			{
				m_vecToken.push_back( sToken );
				sToken.clear();
			}

			// skip char for sentence start
			++i;

			// we spool until we meet a sentence end (if no end then we stop and return false )
			while ( i<iSize && !IsCharSentenceEnd( pData[i] ) )
			{
				sToken += pData[i];
				++i;
			}

			// if no char for sentence end found... stop parsing and return false
			if ( i>=iSize )
			{
				return false;
			}

			// skip char for sentence end
			++i;

			// token has already been built, so push it
			if ( !sToken.empty() )
			{
				m_vecToken.push_back( sToken );
				sToken.clear();
			}
		}

		// build token
		sToken += pData[i];

		++i;
	}

	// token has already been built, so push it
	if ( !sToken.empty() )
	{
		m_vecToken.push_back( sToken );
	}

	return true;
}
