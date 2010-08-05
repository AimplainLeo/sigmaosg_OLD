// SulGuiXml.h

#ifndef __SULGUIXML_H__
#define __SULGUIXML_H__

#include "SulXmlParser.h"
#include "SulGuiEventHandler.h"
#include "SulGuiRadioButtonGroup.h"
#include "SulGuiListBox.h"

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

	Sigma::uint32	m_indent;

	osg::ref_ptr<CSulGuiRadioButtonGroup>	m_curRadioButtonGroup;
	osg::ref_ptr<CSulGuiListBox>			m_curListBox;
};

#endif // __SULGUIXML_H__