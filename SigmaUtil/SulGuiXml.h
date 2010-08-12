// SulGuiXml.h

#ifndef __SULGUIXML_H__
#define __SULGUIXML_H__

#include "SulXmlParser.h"
#include "SulGuiEventHandler.h"
#include "SulGuiRadioButtonGroup.h"
#include "SulGuiListBox.h"
#include "SulGuiThemeXml.h"

class CSulGuiXml : public CSulXmlParser
{
public:
					CSulGuiXml( osg::Group* pRootGroup, CSulGuiEventHandler* pEventHandler, float viewW, float viewH, CSulGuiThemeXml* pThemeXml );

	void			elementStart( const class CSulString& sName, class CSulXmlAttr* pAttr );

	void			elementEnd( const CSulString& sName );

	CSulXmlAttr*	get( const CSulString& sTag );

private:
	osg::Group*								m_rRootGroup;
	osg::Group*								m_curGroup;
	osg::ref_ptr<CSulGuiEventHandler>		m_rEventHandler;

	sigma::uint32							m_indent;

	osg::ref_ptr<CSulGuiRadioButtonGroup>	m_curRadioButtonGroup;
	osg::ref_ptr<CSulGuiListBox>			m_curListBox;
	float m_viewW;
	float m_viewH;

	osg::ref_ptr<CSulGuiThemeXml>			m_rThemeXml;
};

#endif // __SULGUIXML_H__