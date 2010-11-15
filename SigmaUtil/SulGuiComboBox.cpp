// SulGuiComboBox.cpp

#include "stdafx.h"
#include "SulGuiComboBox.h"
#include "SulGuiTextBox.h"
#include "SulGuiEditBox.h"

CSulGuiComboBox::CSulGuiComboBox( float x, float y, float w, float h ) :
CSulGuiCanvas( "COMBOBOX", x, y, w, h )
{
	// combobox is going to be built with an textbox, listbox, button
	m_rTextBox = new CSulGuiTextBox( 0, 0 );
	m_rButton = new CSulGuiButton;
	m_rListBox = new CSulGuiListBox;

	m_rListBox->signalItemClicked.connect( this, &CSulGuiComboBox::itemClicked );
}

void CSulGuiComboBox::setupEventHandler( CSulGuiEventHandler* pEventHandler )
{
	CSulGuiCanvas::setupEventHandler( pEventHandler );
}

void CSulGuiComboBox::setupTheme( CSulGuiThemeXml* pThemeXml )
{
	CSulGuiCanvas::setupTheme( pThemeXml );

	if ( m_rTextBox.valid() )	m_rTextBox->setupTheme( pThemeXml );
	if ( m_rButton.valid() )	m_rButton->setupTheme( pThemeXml );
	if ( m_rListBox.valid() )	m_rListBox->setupTheme( pThemeXml );
}

void CSulGuiComboBox::setupAttr( CSulXmlAttr* pAttr )
{
	CSulGuiCanvas::setupAttr( pAttr );
}

void CSulGuiComboBox::init()
{
	CSulGuiCanvas::init();

	if ( m_rTextBox.valid() )
	{
		m_rTextBox->setWH( getW(), getH() );
		m_rTextBox->setupEventHandler( getEventHandler() );
		m_rTextBox->init();
		osg::MatrixTransform::addChild( m_rTextBox );
	}
	
	if ( m_rButton.valid() )
	{
		m_rButton->setX( getW() );
		m_rButton->setWH( getH(), getH() );
		m_rButton->setupEventHandler( getEventHandler() );
		m_rButton->init();
		osg::MatrixTransform::addChild( m_rButton );

		m_rButton->createDownDecal();

		m_rButton->signalClicked.connect( this, &CSulGuiComboBox::onDropDownClick );
	}

	if ( m_rListBox.valid() )
	{
		m_rListBox->setY( getH() );
		m_rListBox->setWH( getW(), 128.0f );
		m_rListBox->setupEventHandler( getEventHandler() );
		m_rListBox->init();
		m_rListBox->show( false );		
		osg::MatrixTransform::addChild( m_rListBox );
	}
}

bool CSulGuiComboBox::addChild( Node *child )
{
	return m_rListBox->addChild( child );
}

void CSulGuiComboBox::onDropDownClick( CSulGuiCanvas* pCanvas )
{
	m_rListBox->toggleShow();
}

// FIXME: this is not working???
bool CSulGuiComboBox::addTextItem( const CSulString& s )
{
	CSulGuiTextBox* p = new CSulGuiTextBox( 0, 0 );
	p->setupEventHandler( getEventHandler() );
	p->init();
	p->useShaderBorder( false );
	p->useShaderBackground( false );
	p->setText( s );
	return addChild( p );
}

void CSulGuiComboBox::itemClicked( CSulGuiItem* pItem )
{
	CSulGuiTextBox* pTextBox = dynamic_cast<CSulGuiTextBox*>(pItem->getCanvas());
	if ( pTextBox )
	{
		m_rTextBox->setText( pTextBox->getText() );
	}

	CSulGuiEditBox* pEditBox = dynamic_cast<CSulGuiEditBox*>(pItem->getCanvas());
	if ( pEditBox )
	{
		m_rTextBox->setText( pEditBox->getText() );
	}

	CSulGuiButton* pButton = dynamic_cast<CSulGuiButton*>(pItem->getCanvas());
	if ( pButton )
	{
		m_rTextBox->setText( pButton->getText() );
	}

	m_rListBox->toggleShow();
}