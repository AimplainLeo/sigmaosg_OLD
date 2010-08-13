// SulGuiComboBox.cpp

#include "stdafx.h"
#include "SulGuiComboBox.h"


CSulGuiComboBox::CSulGuiComboBox( float x, float y, float w, float h ) :
CSulGuiCanvas( "COMBOBOX", x, y, w, h )
{
	// combobox is going to be built with an textbox, listbox, button

	m_rTextBox = new CSulGuiTextBox( 0, 0 );

	m_rButton = new CSulGuiButton;
}

void CSulGuiComboBox::setupTheme( CSulGuiThemeXml* pThemeXml )
{
	CSulGuiCanvas::setupTheme( pThemeXml );

	m_rTextBox->setupTheme( pThemeXml );
	m_rButton->setupTheme( pThemeXml );
}

void CSulGuiComboBox::setupAttr( CSulXmlAttr* pAttr )
{
	CSulGuiCanvas::setupAttr( pAttr );
}

void CSulGuiComboBox::init()
{
	CSulGuiCanvas::init();

	m_rTextBox->init();
	addChild( m_rTextBox );

	m_rButton->setX( getW() );
	m_rButton->setWH( getH(), getH() );
	m_rButton->init();
	addChild( m_rButton );
}