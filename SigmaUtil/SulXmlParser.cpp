// SulXmlParser.cpp

#include "stdafx.h"
#include "SulXmlParser.h"
#include "SulParser.h"
#include "SulXmlAttr.h"

CSulXmlParser::CSulXmlParser()
{
}

CSulXmlParser::~CSulXmlParser()
{
}

bool CSulXmlParser::parseElement( CSulParser* pSulParser )
{
	osg::ref_ptr<CSulXmlAttr>	rAttr = new CSulXmlAttr;

	enum EELEMENTTYPE
	{
		TAGSTART,
		TAGEND,
		TAGBOTH
	};

	rAttr->reset();

	//////////////////////////////////
	// get all attributes
	//////////////////////////////////
	EELEMENTTYPE	eElementType;
	char*			pszToken;
	bool			bNextTokenIsElementName;

	eElementType			= TAGSTART;
	strElementName			= "";
	bNextTokenIsElementName = false;

	while ( 1 )
	{
		// get token
		pszToken = pSulParser->GetToken();
		if ( !pszToken )
		{
			return false;
		}

		// convert to a simple string form,.. for easier access
		strName = pszToken;

		// <!-- --> need to bypass comments
		if ( pszToken[0]=='<' && pSulParser->PeekToken() && !STRNICMP( pSulParser->PeekToken(), "!--", 3 ) )
		{
			while ( 1 )
			{
				pszToken = pSulParser->GetToken();
				if ( !pszToken )
				{
					return false;
				}

				CSulString sTmp = pszToken;

				if ( sTmp=="--" && pSulParser->PeekToken() )
				{
					CSulString sTmp2 = pSulParser->PeekToken();
					if ( sTmp2==">" )
					{
						pSulParser->GetToken();
						break;
					}
				}

				if ( sTmp.rfind( "--" )!=std::string::npos )
				{
					CSulString sTmp2 = pSulParser->PeekToken();
					if ( sTmp2==">" )
					{
						pSulParser->GetToken();
						break;
					}
				}
			}

			continue;
		}

		// "<?" don't know what this is,.. but I'll use it as a comment and jump the whole tag over
		if ( pszToken[0]=='<' && pSulParser->PeekToken() && !STRICMP( pSulParser->PeekToken(), "?" ) )
		{
			while ( 1 )
			{
				pszToken = pSulParser->GetToken();
				if ( !pszToken )
				{
					return false;
				}

				if ( pszToken[0]=='?'&& pSulParser->PeekToken() && !STRICMP( pSulParser->PeekToken(), ">" ) )
				{
					pSulParser->GetToken();
					break;
				}
			}

			continue;
		}

		// is this a start and end element? (next element must be '>', if not then there is an xml error )
		if ( pszToken[0]=='/' )
		{
			if ( pSulParser->PeekToken() && !STRICMP( pSulParser->PeekToken(), ">" ) )
			{
				eElementType = TAGBOTH;
				continue;
			}

		}

		if ( pszToken[0]=='<' && pSulParser->PeekToken() && !STRICMP( pSulParser->PeekToken(), "/" ) )
		{
			eElementType			= TAGEND;
			bNextTokenIsElementName = true;
			pSulParser->GetToken();
			continue;
		}

		if ( bNextTokenIsElementName )
		{
			strElementName			= pszToken;
			bNextTokenIsElementName = false;
			continue;
		}

		// are we at the start?
		if ( pszToken[0]=='<' )
		{
			bNextTokenIsElementName = true;
			continue;
		}

		// are we at the end?
		if ( pszToken[0]=='>' )
		{
			break;
		}

		////////////////////////////////////////////////////////////////////////////////////
		// if we get to here,. .then we should have a valid name and value
		////////////////////////////////////////////////////////////////////////////////////

		strNameAttr = strName;

		// check for '=' ( safty check )
		pszToken = pSulParser->GetToken();
		if ( !pszToken )
		{
			return false;
		}

		if ( pszToken[0]!='=' )
		{
			return false;
		}

		// value
		pszToken = pSulParser->GetToken();
		if ( !pszToken )
		{
			return false;
		}

		strValueAttr = pszToken;
		rAttr->add( strNameAttr.c_str(), strValueAttr.c_str() );
	}

	//////////////////////////////
	// process element
	//////////////////////////////

	switch ( eElementType )
	{
		case TAGSTART:
			elementStart( strElementName, rAttr.get() );
			break;

		case TAGEND:
			elementEnd( strElementName, rAttr.get() );
			elementEnd( strElementName );
			break;

		case TAGBOTH:
			elementStart( strElementName, rAttr.get() );
			elementEnd( strElementName, rAttr.get() );
			elementEnd( strElementName );
			break;
	}

	return true;
}

bool CSulXmlParser::load( const CSulString& sXmlFile )
{
	osg::ref_ptr<CSulParser> rSulParser = new CSulParser;

	////////////////////////////////////////////////////////
	// define delimitors for an xml file
	////////////////////////////////////////////////////////

	rSulParser->AddCharToken( '<' );
	rSulParser->AddCharToken( '>' );
	rSulParser->AddCharToken( '?' );
	rSulParser->AddCharToken( '/' );

	////////////////////////////////////////////////////////
	// load it and parse
	////////////////////////////////////////////////////////
	char* pszToken;

	if ( !rSulParser->Init( sXmlFile ) )
	{
		return false;
	}

	pszToken = rSulParser->PeekToken();
	while ( pszToken )
	{
		// FIXME: if anything else besides "<" is peeked at the end of this scope, will cause
		// this routine to hang in an infinite loop

		// we are only interested in the start of an element
		if ( !STRICMP( pszToken, "<" ) )
		{
			if ( !parseElement( rSulParser.get() ) )
			{
				return false;
			}
		}

		pszToken = rSulParser->PeekToken();
	}

	loadFinished();

	return true;
}


