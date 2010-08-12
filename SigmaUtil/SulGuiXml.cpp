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
#include "SulGuiComboBox.h"
#include "SulGuiTextBox.h"

CSulGuiXml::CSulGuiXml( osg::Group* pRootGroup, CSulGuiEventHandler* pEventHandler, float viewW, float viewH, CSulGuiThemeXml* pThemeXml )
{
	m_rThemeXml = pThemeXml;
	m_viewW = viewW;
	m_viewH = viewH;
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
			"CANVAS",
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

	if ( sName=="SCROLLBAR" )
	{
		pComp = new CSulGuiScrollBar(
			pAttr->get( "x" ).asFloat(),
			pAttr->get( "y" ).asFloat(),
			pAttr->get( "w" ).asFloat(),
			pAttr->get( "h" ).asFloat()
		);
	}

	if ( sName=="COMBOBOX" )
	{
		pComp = new CSulGuiComboBox(
			pAttr->get( "x" ).asFloat(),
			pAttr->get( "y" ).asFloat(),
			pAttr->get( "w" ).asFloat(),
			pAttr->get( "h" ).asFloat()
		);
	}

	if ( sName=="TEXTBOX" )
	{
		pComp = new CSulGuiTextBox(
			pAttr->get( "text" ),
			pAttr->get( "x" ).asFloat(),
			pAttr->get( "y" ).asFloat(),
			pAttr->get( "w" ).asFloat(),
			pAttr->get( "h" ).asFloat(),
			pAttr->get( "size" ).asFloat()
		);
	}

	if ( sName=="COMP" )
	{
		pComp = new CSulGuiComp(
			"COMP",
			pAttr->get( "x" ).asFloat(),
			pAttr->get( "y" ).asFloat()
		);
	}


	if ( pComp )
	{
		CSulGuiListBox* pListBox = dynamic_cast<CSulGuiListBox*>(m_curGroup);
		if ( pListBox )
		{
			pListBox->addItem( dynamic_cast<CSulGuiCanvas*>(pComp) );
		}

		m_curGroup->addChild( pComp );
		pComp->setupTheme( m_rThemeXml );
		pComp->setupAttr( pAttr );
		pComp->setupView( m_viewW, m_viewH );
		pComp->setupEventHandler( m_rEventHandler );
		pComp->setLayer( m_indent );
		pComp->init();
		m_curGroup = pComp;
		m_indent++;
	}
}

void CSulGuiXml::elementEnd( const CSulString& sName )
{
	if ( 
		sName=="CANVAS" || 
		sName=="BUTTON" || 
		sName=="TEXT" || 
		sName=="TEXTBOX" || 
		sName=="EDITBOX" || 
		sName=="CHECKBOX" || 
		sName=="RADIOBUTTON" ||
		sName=="RADIOBUTTON_GROUP" ||
		sName=="LISTBOX" ||
		sName=="SCROLLBAR" ||
		sName=="COMBOBOX" ||
		sName=="COMP"
	)
	{
		m_curGroup = m_curGroup->getParent( 0 );
		m_indent--;
	}
}

