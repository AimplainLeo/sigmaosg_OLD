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
#include "SulGuiItem.h"
#include "SulGuiScrollBar.h"

CSulGuiXml::CSulGuiXml( osg::Group* pRootGroup, CSulGuiEventHandler* pEventHandler )
{
	m_indent = 0;
	m_rEventHandler = pEventHandler;
	m_rRootGroup = pRootGroup;
	m_curGroup = pRootGroup;
}

void CSulGuiXml::elementStart( const CSulString& sName, CSulXmlAttr* pAttr )
{
	CSulGuiComp* pComp = 0;

	if ( sName=="CANVAS" )
	{
		pComp = new CSulGuiCanvas(
			pAttr->get( "x" ).asFloat(),
			pAttr->get( "y" ).asFloat(),
			pAttr->get( "w" ).asFloat(),
			pAttr->get( "h" ).asFloat()
		);
	}

	if ( sName=="BUTTON" )
	{
		pComp = new CSulGuiButton(
			pAttr->get( "text" ),
			pAttr->get( "x" ).asFloat(),
			pAttr->get( "y" ).asFloat(),
			pAttr->get( "w" ).asFloat(),
			pAttr->get( "h" ).asFloat()
		);
	}

	if ( sName=="TEXT" )
	{
		pComp = new CSulGuiText(
			pAttr->get( "text" ),
			pAttr->get( "x" ).asFloat(),
			pAttr->get( "y" ).asFloat(),
			pAttr->get( "size" ).asFloat()
		);
	}

	if ( sName=="EDITBOX" )
	{
		pComp = new CSulGuiEditBox(
			pAttr->get( "text" ),
			pAttr->get( "x" ).asFloat(),
			pAttr->get( "y" ).asFloat(),
			pAttr->get( "w" ).asFloat(),
			pAttr->get( "h" ).asFloat()
		);
	}

	if ( sName=="CHECKBOX" )
	{
		pComp = new CSulGuiCheckBox(
			pAttr->get( "text" ),
			pAttr->get( "x" ).asFloat(),
			pAttr->get( "y" ).asFloat(),
			pAttr->get( "w" ).asFloat(),
			pAttr->get( "h" ).asFloat()
		);
	}

	if ( sName=="RADIOBUTTON" )
	{
		pComp = new CSulGuiRadioButton(
			pAttr->get( "text" ),
			pAttr->get( "x" ).asFloat(),
			pAttr->get( "y" ).asFloat(),
			pAttr->get( "w" ).asFloat(),
			pAttr->get( "h" ).asFloat()
		);

		m_curRadioButtonGroup->addRadioButton( dynamic_cast<CSulGuiRadioButton*>(pComp) );
	}

	if ( sName=="RADIOBUTTON_GROUP" )
	{
		pComp = new CSulGuiRadioButtonGroup(
			pAttr->get( "x" ).asFloat(),
			pAttr->get( "y" ).asFloat()
		);

		m_curRadioButtonGroup = dynamic_cast<CSulGuiRadioButtonGroup*>(pComp);
	}

	if ( sName=="LISTBOX" )
	{
		pComp = new CSulGuiListBox(
			pAttr->get( "x" ).asFloat(),
			pAttr->get( "y" ).asFloat(),
			pAttr->get( "w" ).asFloat(),
			pAttr->get( "h" ).asFloat()
		);

		m_curListBox = dynamic_cast<CSulGuiListBox*>(pComp);
	}

	if ( sName=="ITEM" )
	{
		pComp = new CSulGuiItem(
			pAttr->get( "text" )
		);

		m_curListBox->addItem( dynamic_cast<CSulGuiItem*>(pComp) );
	}

	if ( sName=="SCROLLBAR" )
	{
		pComp = new CSulGuiScrollBar(
			pAttr->get( "x" ).asFloat(),
			pAttr->get( "y" ).asFloat(),
			pAttr->get( "w" ).asFloat(),
			pAttr->get( "h" ).asFloat()
		);
	}

	if ( pComp )
	{
		pComp->setupEventHandler( m_rEventHandler );
		pComp->setLayer( m_indent );
		m_curGroup->addChild( pComp );
		m_curGroup = pComp;
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
		sName=="RADIOBUTTON_GROUP" ||
		sName=="LISTBOX" ||
		sName=="ITEM" ||
		sName=="SCROLLBAR"
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

