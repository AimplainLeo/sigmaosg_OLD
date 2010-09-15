// SulGuiXml.h

#ifndef __SULGUIXML_H__
#define __SULGUIXML_H__

#include "SulXmlParser.h"
#include "SulGuiEventHandler.h"
#include "SulGuiRadioButtonGroup.h"
#include "SulGuiListBox.h"
#include "SulGuiComboBox.h"
#include "SulGuiThemeXml.h"

class CSulGuiXml : public CSulXmlParser
{
public:
					CSulGuiXml( osg::Group* pRootGroup, CSulGuiEventHandler* pEventHandler, float viewW, float viewH, CSulGuiThemeXml* pThemeXml );

	void			elementStart( const class CSulString& sName, class CSulXmlAttr* pAttr );

	void			elementEnd( const CSulString& sName );

	osg::Vec2		getNativeDimensions();

	void			loadFinished();

private:
	osg::Group*								m_rRootGroup;
	osg::Group*								m_curGroup;
	osg::ref_ptr<CSulGuiEventHandler>		m_rEventHandler;

	sigma::uint32							m_indent;

	osg::ref_ptr<CSulGuiRadioButtonGroup>	m_curRadioButtonGroup;
	float m_viewW;
	float m_viewH;

	osg::ref_ptr<CSulGuiThemeXml>			m_rThemeXml;

	STACK_GUICOMP							m_vecCompStack;

	float									m_nativeW;
	float									m_nativeH;
};

#endif // __SULGUIXML_H__