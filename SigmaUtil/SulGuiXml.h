// SulGuiXml.h

#ifndef __SULGUIXML_H__
#define __SULGUIXML_H__

#include "SulXmlParser.h"
#include "SulGuiEventHandler.h"

class CSulGuiXml : public CSulXmlParser
{
private:
	typedef std::map< CSulString, osg::ref_ptr<CSulXmlAttr> >	MAP_ATTR;

public:
					CSulGuiXml( osg::Group* pRootGroup, CSulGuiEventHandler* pEventHandler );

	void			elementStart( const class CSulString& sName, class CSulXmlAttr* pAttr );

	void			elementEnd( const CSulString& sName );

	CSulXmlAttr*	get( const CSulString& sTag );

private:
	MAP_ATTR		m_mapAttr;

	osg::Group*		m_rRootGroup;
	osg::Group*		m_curGroup;
	osg::ref_ptr<CSulGuiEventHandler> m_rEventHandler;
};

#endif // __SULGUIXML_H__