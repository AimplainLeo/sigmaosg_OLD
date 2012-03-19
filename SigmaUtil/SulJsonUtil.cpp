// SulJsonUtil.cpp

#include "stdafx.h"
#include "SulJsonUtil.h"
#include "SulJsonArray.h"
#include "SulJsonObject.h"
#include "SulParser.h"
#include "SulJsonString.h"

CSulJson* CSulJsonUtil::create( CSulParser* parser, CSulJson* curJson )
{
	CSulJson* root = 0;

	sigma::int8* p = parser->GetToken();
	while ( p )
	{
		CSulString s( p );

		if ( s=="," )
		{
			p = parser->GetToken();
			continue;
		}

		if ( s=="{" )
		{
			CSulJson* newJson = new CSulJsonObject( curJson );

			if ( curJson )
			{
				curJson->add( newJson );
			}

			curJson = newJson;
		}
		else if ( s=="[" )
		{
			CSulJson* newJson = new CSulJsonArray( curJson );

			if ( curJson )
			{
				curJson->add( newJson );
			}

			curJson = newJson;
		}
		else if ( s=="]" || s=="}" )
			curJson = curJson->getParent();
		else
		{
			if ( CSulJsonObject* pObj = dynamic_cast<CSulJsonObject*>(curJson) )
			{
				// key
				CSulString key( p );

				// expecting ":"
				p = parser->GetToken();
				assert( (*p)==':' );

				// we need to find out if it's a string or object
				sigma::int8* pp = parser->PeekToken();

				CSulJson* val = 0;
				if ( *pp=='{' || *pp=='[' )
				{
					// object
					val = create( parser, curJson );
				}
				else
				{
					p = parser->GetToken();
					CSulString s(p);

					// string
					val = new CSulJsonString( s );
				}

				// add key/val to the json object
				pObj->add( key, val );
			}
			
			if ( CSulJsonArray* pObj = dynamic_cast<CSulJsonArray*>(curJson) )
			{
				assert( 0 ); // not tested or completed
				// we only have values

				CSulJson* val = create( parser, curJson );

				pObj->add( val );
			}
		}

		if ( curJson==0 )
			break;

		// save root, so we can return it
		if ( root==0 )
			root = curJson;

		p = parser->GetToken();
	}

	return root;
}

CSulJson* CSulJsonUtil::create( const CSulString& json )
{
	osg::ref_ptr<CSulParser> parser = new CSulParser;
	parser->AddCharToken( '{' );
	parser->AddCharToken( '}' );
	parser->AddCharToken( ':' );
	parser->AddSentenceCharStart( 39 );  // '
	parser->AddSentenceCharEnd( 39 );  // '
	parser->InitStringParse( json );

	return create( parser );
}
