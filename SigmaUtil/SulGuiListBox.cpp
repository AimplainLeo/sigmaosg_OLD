// SulGuiListBox.cpp

#include "stdafx.h"
#include "SulGuiListBox.h"
#include <osg/scissor>
#include <osgManipulator/Selection>

CSulGuiListBox::CSulGuiListBox( float x, float y, float w, float h ) :
CSulGuiCanvas( "LISTBOX", x, y, w, h )
{
	// add a scrollbar
	m_rScrollBar = new CSulGuiScrollBar( w, 0, 32, h );
	addChild( m_rScrollBar );

	m_rScrollBar->signalChanged.connect( this, &CSulGuiListBox::onScrollBarChanged );
}

void CSulGuiListBox::setupView( float w, float h )
{
	CSulGuiCanvas::setupView( w, h );

	float x = getWorldX();
	float y = getWorldY();
	float ww = getW();
	float hh = getH();

	osg::Scissor* pScissor = new osg::Scissor( x, h-(y+hh), ww+32, hh );
	getOrCreateStateSet()->setAttributeAndModes( pScissor, osg::StateAttribute::ON );
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

void CSulGuiListBox::setupEventHandler( class CSulGuiEventHandler* pEventHandler )
{
	CSulGuiCanvas::setupEventHandler( pEventHandler );

	m_rScrollBar->setupEventHandler( pEventHandler );
}

void CSulGuiListBox::addItem( CSulGuiCanvas* pCanvas )
{
	CSulGuiItem* pItem = new CSulGuiItem( pCanvas );

	float h = getTotalItemsHeight();

	m_mapItem[pCanvas] = pItem;

	// we need to set the position of the item in the listbox
	pCanvas->setY( h );
	pCanvas->signalClicked.connect( this, &CSulGuiListBox::onClick );
	pCanvas->setActive( false );
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

