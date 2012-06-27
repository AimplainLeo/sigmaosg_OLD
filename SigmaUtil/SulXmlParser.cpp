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

	// only TAGSTART elements can have data between start and end tags, so we check
	// FIXME: return and newlines are tokenized :(  ... we should include the whole data not just tokens
	// basically this means that all delimeters are not included in the data section.
	// The way the tokenizer is used in conjuction with the xml parser makes this impossible to solve with
	// the current design. I'm going to create a new parser that will handle the data section.. also the new
	// parser will be used to create a xml dom object.
	CSulString sData;
	if ( eElementType==TAGSTART )
	{
		while ( 1 )
		{
			sigma::int8* p = pSulParser->PeekToken();
			if ( !p || p[0]=='<' )
				break;

			sData+= pSulParser->GetToken();
		}
	}

	switch ( eElementType )
	{
		case TAGSTART:
			elementStart( strElementName, rAttr.get(), sData );
			break;

		case TAGEND:
			elementEnd( strElementName, rAttr.get() );
			elementEnd( strElementName );
			break;

		case TAGBOTH:
			elementStart( strElementName, rAttr.get(), sData );
			elementEnd( strElementName, rAttr.get() );
			elementEnd( strElementName );
			break;
	}

	return true;
}

bool CSulXmlParser::process( CSulParser* pParser )
{
	char* pszToken;

	pszToken = pParser->PeekToken();
	while ( pszToken )
	{
		// FIXME: if anything else besides "<" is peeked at the end of this scope, will cause
		// this routine to hang in an infinite loop

		// we are only interested in the start of an element
		if ( !STRICMP( pszToken, "<" ) )
		{
			if ( !parseElement( pParser ) )
			{
				return false;
			}
		}
		else
		{
			// if we get here we have data (because everything is tokenized we can not process this),
			// so currently we do not support data between elements (perhaps it's time to create a new xml parser)
		}

		pszToken = pParser->PeekToken();
	}

	loadFinished();

	return true;
}

bool CSulXmlParser::loadString( const CSulString& sXmlString )
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

	if ( !rSulParser->InitStringParse( sXmlString ) )
	{
		return false;
	}

	return process( rSulParser );
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
	
	if ( !rSulParser->Init( sXmlFile ) )
	{
		return false;
	}

	return process( rSulParser );
}


