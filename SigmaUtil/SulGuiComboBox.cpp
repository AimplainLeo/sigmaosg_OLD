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

	m_rListBox->signalItemClicked.connect( this, &CSulGuiComboBox::onItemClicked );
	m_rListBox->signalItemChanged.connect( this, &CSulGuiComboBox::onItemChanged );
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

void CSulGuiComboBox::setLayer( sigma::uint32 layer )
{
	CSulGuiCanvas::setLayer( layer );
	m_rButton->setLayer( layer+10 );
}

void CSulGuiComboBox::init()
{
	CSulGuiCanvas::init();

	if ( m_rTextBox.valid() )
	{
		m_rTextBox->setWH( getW()-getH(), getH() );
		m_rTextBox->setupEventHandler( getEventHandler() );
		m_rTextBox->init();
		CSulGuiCanvas::addChild( m_rTextBox );
	}
	
	if ( m_rButton.valid() )
	{
		m_rButton->setX( getW()-getH() );
		m_rButton->setWH( getH(), getH() );
		m_rButton->setupEventHandler( getEventHandler() );
		m_rButton->init();
		CSulGuiCanvas::addChild( m_rButton );

		m_rButton->createDownDecal();

		m_rButton->signalClicked.connect( this, &CSulGuiComboBox::onDropDownClick );
	}

	if ( m_rListBox.valid() )
	{
		m_rListBox->setBgColor( osg::Vec4(0.5f,0.5f,0.5f,1.0f) );
		m_rListBox->setY( getH() );
		m_rListBox->setWH( getW()-getH(), 128.0f );
		m_rListBox->setupEventHandler( getEventHandler() );
		m_rListBox->init();
		m_rListBox->show( false );		
		CSulGuiCanvas::addChild( m_rListBox );
	}
}

bool CSulGuiComboBox::addChild( Node *child )
{
	return m_rListBox->addChild( child );
}

void CSulGuiComboBox::onDropDownClick( CSulGuiCanvas* pCanvas )
{
	m_rListBox->toggleShow();

	// HACK: to solve the problem of combobox box vertically aligned and text overlapping
	m_rListBox->setLayer( getLayer()+11 );
}

bool CSulGuiComboBox::addTextItem( const CSulString& s )
{
	return m_rListBox->addTextItem( s );
}

void CSulGuiComboBox::onItemChanged( CSulGuiItem* pItem )
{
	// if item is a textbox
	CSulGuiTextBox* pTextBox = dynamic_cast<CSulGuiTextBox*>(pItem->getCanvas());
	if ( pTextBox )
	{
		m_rTextBox->setText( pTextBox->getText() );
	}

	// if item is an editbox
	CSulGuiEditBox* pEditBox = dynamic_cast<CSulGuiEditBox*>(pItem->getCanvas());
	if ( pEditBox )
	{
		m_rTextBox->setText( pEditBox->getText() );
	}

	// if item is a button
	CSulGuiButton* pButton = dynamic_cast<CSulGuiButton*>(pItem->getCanvas());
	if ( pButton )
	{
		m_rTextBox->setText( pButton->getText() );
	}
}

void CSulGuiComboBox::onItemClicked( CSulGuiItem* pItem )
{
	m_rListBox->toggleShow();
}

void CSulGuiComboBox::setSelectedIndex( sigma::uint32 index )
{
	m_rListBox->setSelectedIndex( index );
}

sigma::int32 CSulGuiComboBox::getSelectedIndex()
{
	return m_rListBox->getSelectedIndex();
}