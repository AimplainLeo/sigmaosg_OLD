// SulXmlParser.h

#ifndef __SULXMLPARSER_H__
#define __SULXMLPARSER_H__

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

	virtual bool	load( const class CSulString& sXmlFile );

	virtual void	elementStart( const CSulString& sName, CSulXmlAttr* pAttr ) {}
	virtual void	elementEnd( const CSulString& sName, CSulXmlAttr* pAttr  ) {}
	virtual void	elementEnd( const CSulString& sName ) {}

	virtual void	loadFinished() {}	

private:
	bool			parseElement( CSulParser* pSulParser );

private:
	// because ParseElement is called a lot these variables are only created one instance of for faster usage
	CSulString		strNameAttr;
	CSulString		strValueAttr;
	CSulString		strName;
	CSulString		strElementName;
};

#endif // __SULXMLPARSER_H__
