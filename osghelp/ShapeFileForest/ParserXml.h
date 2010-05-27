// ParserXml.h

#ifndef __PARSERXML_H__
#define __PARSERXML_H__

#include <SigmaUtil/SulDataShapeMask.h>
#include <SigmaUtil/SulXmlParser.h>

class CParserXml : public CSulXmlParser
{
public:

	void	ElementStart( const CSulString& sName, CSulXmlAttr* pAttr );

	VEC_DATASHAPEMASK& getDataShapeMask();

private:
	VEC_DATASHAPEMASK	m_vecDataMask;
};

#endif // __PARSERXML_H__