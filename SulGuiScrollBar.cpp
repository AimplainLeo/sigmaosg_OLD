// SulGuiScrollBar.cpp   ole.r@get2net.dk

#include "stdafx.h"
#include "SulGuiScrollBar.h"
#include "SulSigSlots.h"
#include <iostream>

CSulGuiScrollBar::CSulGuiScrollBar( float x, float y, float w, float h ) :
CSulGuiCanvas( "SCROLLBAR", x, y, w, h )
{
	m_min = 0.0f;
	m_max = 1.0f;
	m_cur = 0.0f;
}

void CSulGuiScrollBar::onButtonDragPositionChanged( float x, float y )
{
	m_cur = (y-getW()) / (m_rangeDrag-getW());
	signalChanged( m_cur );
}

void CSulGuiScrollBar::setupEventHandler( CSulGuiEventHandler* pEventHandler )
{
	CSulGuiCanvas::setupEventHandler( pEventHandler );
}

void CSulGuiScrollBar::init()
{
	CSulGuiCanvas::init();

	float w = getW();
	float h = getH();

	// need top and bottom button
	m_rButtonTop = new CSulGuiButton( 0, 0, w, w );
	m_rButtonTop->setupEventHandler( getEventHandler() );
	m_rButtonTop->init();
	m_rButtonTop->createUpDecal();
	addChild( m_rButtonTop );

	m_rButtonBottom = new CSulGuiButton( 0, h-w, w, w );
	m_rButtonBottom->setupEventHandler( getEventHandler() );
	m_rButtonBottom->init();
	m_rButtonBottom->createDownDecal();
	addChild( m_rButtonBottom );

	// need drag button
	m_rangeDrag = h-w-w/2.0f;
	m_rButtonDrag = new CSulGuiButton( 0, w, w, w/2.0f );
	m_rButtonDrag->setupEventHandler( getEventHandler() );
	m_rButtonDrag->init();
	m_rButtonDrag->allowDrag( 0, 0, w, m_rangeDrag );
	addChild( m_rButtonDrag );

	m_rButtonDrag->signalPositionChanged.connect( this, &CSulGuiScrollBar::onButtonDragPositionChanged );

}
