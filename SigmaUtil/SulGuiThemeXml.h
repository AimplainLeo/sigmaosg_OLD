// SulGuiThemeXml.h

#ifndef __SULGUITHEMEXML_H__
#define __SULGUITHEMEXML_H__

#include "SulXmlParser.h"
#include "SulXmlAttr.h"

class CSulGuiThemeXml : public CSulXmlParser
{
private:
	typedef std::map<CSulString, osg::ref_ptr<CSulXmlAttr> >	MAP_ELEMENT;

public:
					CSulGuiThemeXml();

	CSulString		getValue( const CSulString& element, const CSulString& attr );

	void			elementStart( const class CSulString& sName, class CSulXmlAttr* pAttr, CSulString sData );

private:
	MAP_ELEMENT		m_mapElement;
};

#endif // __SULGUITHEMEXML_H__