// SulGuiListBox.cpp

#include "stdafx.h"
#include "SulGuiListBox.h"
#include <osg/scissor>
#include <osgManipulator/Selection>

CSulGuiListBox::CSulGuiListBox( float x, float y, float w, float h ) :
CSulGuiCanvas( "LISTBOX", x, y, w, h )
{
	m_clipPadding = 2.0f;
}

void CSulGuiListBox::setupView( float w, float h )
{
	CSulGuiCanvas::setupView( w, h );

	m_viewW = w;
	m_viewH = h;

	updateClipping();
}

void CSulGuiListBox::setupAttr( CSulXmlAttr* pAttr )
{
	CSulGuiCanvas::setupAttr( pAttr );

	m_clipPadding = getThemeValue( "clip_padding" ).asFloat();
	m_itemOfsX = getThemeValue( "item_ofs_x" ).asFloat();

	if ( pAttr->exist( "clip_padding" ) )	m_clipPadding = pAttr->get( "clip_padding" ).asFloat();
	if ( pAttr->exist( "item_ofs_x" ) )	m_itemOfsX = pAttr->get( "item_ofs_x" ).asFloat();
	
}

void CSulGuiListBox::setupEventHandler( class CSulGuiEventHandler* pEventHandler )
{
	CSulGuiCanvas::setupEventHandler( pEventHandler );

	m_rScrollBar->setupEventHandler( pEventHandler );
}

void CSulGuiListBox::init()
{
	CSulGuiCanvas::init();

	float w = getW();
	float h = getH();

	// add a scrollbar
	m_rScrollBar = new CSulGuiScrollBar( w, 0, 32, h );
	m_rScrollBar->init();
	addChild( m_rScrollBar );

	m_rScrollBar->signalChanged.connect( this, &CSulGuiListBox::onScrollBarChanged );
}

float CSulGuiListBox::getTotalItemsHeight()
{
	MAP_GUIITEM::iterator i, e;
	i = m_mapItem.begin();	
	e = m_mapItem.end();
	float h = 0.0f;

	while( i!=e )
	{
		h += i->first->getH();		
		++i;
	}

	return h;
}

void CSulGuiListBox::onScrollBarChanged( float val )
{
	float totalHeight = getTotalItemsHeight()-getH();
	
	float realPosition = totalHeight*val;

	// reposition items
	MAP_GUIITEM::iterator i, e;
	i = m_mapItem.begin();	
	e = m_mapItem.end();

	float h = 0.0f;

	while( i!=e )
	{
		i->first->setY( -realPosition+h );
		h += i->first->getH();
		++i;
	}
}

void CSulGuiListBox::addItem( CSulGuiCanvas* pCanvas )
{
	CSulGuiItem* pItem = new CSulGuiItem( pCanvas );

	float h = getTotalItemsHeight();

	m_mapItem[pCanvas] = pItem;

	// we need to set the position of the item in the listbox
	pCanvas->setXY( m_itemOfsX, h );
	pCanvas->signalClicked.connect( this, &CSulGuiListBox::onClick );
	pCanvas->setActive( false );

	updateClipping();
}

void CSulGuiListBox::onClick( CSulGuiCanvas* pItem )
{
	// find item in our list
	MAP_GUIITEM::iterator iFound = m_mapItem.find( pItem );
	if ( iFound!=m_mapItem.end() )
	{
		iFound->second->toggleSelect();
	}

}

void CSulGuiListBox::updateClipping()
{
	float x = getWorldX()+m_clipPadding;
	float y = getWorldY()+m_clipPadding;
	float ww = getW()-m_clipPadding*2.0f;
	float hh = getH()-m_clipPadding*2.0f;

	osg::Scissor* pScissor = new osg::Scissor( x, m_viewH-(y+hh), ww+32, hh );

	MAP_GUIITEM::iterator i, e;
	i = m_mapItem.begin();	
	e = m_mapItem.end();

	while( i!=e )
	{
		i->first->getOrCreateStateSet()->setAttributeAndModes( pScissor, osg::StateAttribute::ON );
		++i;
	}
}