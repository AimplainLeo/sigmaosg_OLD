// SulXmlParser.h

#ifndef __SULXMLPARSER_H__
#define __SULXMLPARSER_H__

/*
	NOTE: 

	This is a simple parser which only handles tags and attributes. Currently it does not handle data sections between tags. 

	The xml parser is currently based on the CSulParser which is a tokenizer, this is a bad solution for parsing xml files
	because of the data section between tags. This part needs to be redesigned.

*/

#include "SulExport.h"
#include "SulString.h"
#include <osg/Referenced>

class CSulParser;
class CSulXmlAttr;

class SUL_EXPORT CSulXmlParser : public osg::Referenced
{
public:
					CSulXmlParser();
	virtual			~CSulXmlParser();

	virtual bool	loadString( const CSulString& sXmlString );
	virtual bool	load( const class CSulString& sXmlFile );

	virtual void	elementStart( const CSulString& sName, CSulXmlAttr* pAttr, CSulString sData ) {}
	virtual void	elementEnd( const CSulString& sName, CSulXmlAttr* pAttr  ) {}
	virtual void	elementEnd( const CSulString& sName ) {}

	virtual void	loadFinished() {}	

private:
	bool			parseElement( CSulParser* pSulParser );

	bool			process( CSulParser* pParser );

private:
	// because ParseElement is called a lot these variables are only created one instance of for faster usage
	CSulString		strNameAttr;
	CSulString		strValueAttr;
	CSulString		strName;
	CSulString		strElementName;
};

#endif // __SULXMLPARSER_H__
