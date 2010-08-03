// SulGuiXml.cpp

#include "stdafx.h"
#include "SulGuiXml.h"
#include "SulXmlAttr.h"
#include "SulGuiCanvas.h"
#include "SulGuiButton.h"
#include "SulGuiText.h"
#include "SulGuiEditBox.h"
#include "SulGuiCheckBox.h"
#include "SulGuiRadioButton.h"

CSulGuiXml::CSulGuiXml( osg::Group* pRootGroup, CSulGuiEventHandler* pEventHandler )
{
	m_indent = 0;
	m_rEventHandler = pEventHandler;
	m_rRootGroup = pRootGroup;
	m_curGroup = pRootGroup;
}

void CSulGuiXml::elementStart( const CSulString& sName, CSulXmlAttr* pAttr )
{
	if ( sName=="CANVAS" )
	{
		CSulGuiComp* p = new CSulGuiCanvas(
			pAttr->get( "x" ).asFloat(),
			pAttr->get( "y" ).asFloat(),
			pAttr->get( "w" ).asFloat(),
			pAttr->get( "h" ).asFloat()
		);

		p->setLayer( m_indent );

		m_rEventHandler->addComp( p );

		m_curGroup->addChild( p );
		m_curGroup = p;
		m_indent++;
	}

	if ( sName=="BUTTON" )
	{
		CSulGuiComp* p = new CSulGuiButton(
			pAttr->get( "text" ),
			pAttr->get( "x" ).asFloat(),
			pAttr->get( "y" ).asFloat(),
			pAttr->get( "w" ).asFloat(),
			pAttr->get( "h" ).asFloat()
		);

		p->setLayer( m_indent );

		m_rEventHandler->addComp( p );

		m_curGroup->addChild( p );
		m_curGroup = p;
		m_indent++;
	}

	if ( sName=="TEXT" )
	{
		CSulGuiComp* p = new CSulGuiText(
			pAttr->get( "text" ),
			pAttr->get( "x" ).asFloat(),
			pAttr->get( "y" ).asFloat(),
			pAttr->get( "size" ).asFloat()
		);

		p->setLayer( m_indent );

		m_rEventHandler->addComp( p );

		m_curGroup->addChild( p );
		m_curGroup = p;
		m_indent++;
	}

	if ( sName=="EDITBOX" )
	{
		CSulGuiComp* p = new CSulGuiEditBox(
			pAttr->get( "text" ),
			pAttr->get( "x" ).asFloat(),
			pAttr->get( "y" ).asFloat(),
			pAttr->get( "w" ).asFloat(),
			pAttr->get( "h" ).asFloat()
		);

		p->setLayer( m_indent );

		m_rEventHandler->addComp( p );

		m_curGroup->addChild( p );
		m_curGroup = p;
		m_indent++;
	}

	if ( sName=="CHECKBOX" )
	{
		CSulGuiComp* p = new CSulGuiCheckBox(
			pAttr->get( "text" ),
			pAttr->get( "x" ).asFloat(),
			pAttr->get( "y" ).asFloat(),
			pAttr->get( "w" ).asFloat(),
			pAttr->get( "h" ).asFloat()
		);

		p->setLayer( m_indent );

		m_rEventHandler->addComp( p );

		m_curGroup->addChild( p );
		m_curGroup = p;
		m_indent++;
	}

	if ( sName=="RADIOBUTTON" )
	{
		CSulGuiComp* p = new CSulGuiRadioButton(
			pAttr->get( "text" ),
			pAttr->get( "x" ).asFloat(),
			pAttr->get( "y" ).asFloat(),
			pAttr->get( "w" ).asFloat(),
			pAttr->get( "h" ).asFloat()
		);

		p->setLayer( m_indent );

		m_rEventHandler->addComp( p );

		m_curGroup->addChild( p );
		m_curGroup = p;
		m_indent++;

		m_curRadioButtonGroup->addRadioButton( dynamic_cast<CSulGuiRadioButton*>(p) );
	}

	if ( sName=="RADIOBUTTON_GROUP" )
	{
		m_curRadioButtonGroup = new CSulGuiRadioButtonGroup(
			pAttr->get( "x" ).asFloat(),
			pAttr->get( "y" ).asFloat()
		);

		m_curRadioButtonGroup->setLayer( m_indent );

		m_rEventHandler->addComp( m_curRadioButtonGroup );

		m_curGroup->addChild( m_curRadioButtonGroup );
		m_curGroup = m_curRadioButtonGroup;
		m_indent++;
	}

	// we assume only element tags from now on (there is no smart error checking here yet!)
	m_mapAttr[sName] = pAttr;
}

void CSulGuiXml::elementEnd( const CSulString& sName )
{
	if ( 
		sName=="CANVAS" || 
		sName=="BUTTON" || 
		sName=="TEXT" || 
		sName=="EDITBOX" || 
		sName=="CHECKBOX" || 
		sName=="RADIOBUTTON" ||
		sName=="RADIOBUTTON_GROUP"
	)
	{
		m_curGroup = m_curGroup->getParent( 0 );
		m_indent--;
	}
}

CSulXmlAttr* CSulGuiXml::get( const CSulString& sTag ) 
{
	MAP_ATTR::const_iterator i;

	i = m_mapAttr.find( sTag );
	if ( i!=m_mapAttr.end() )
	{
		return m_mapAttr[sTag].get();
	}

	return 0;
}

