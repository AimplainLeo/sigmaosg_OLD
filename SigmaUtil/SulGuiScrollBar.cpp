// SulGuiScrollBar.cpp

#include "stdafx.h"
#include "SulGuiScrollBar.h"
#include "SulGuiButtonMediator.h"

CSulGuiScrollBar::CSulGuiScrollBar( float x, float y, float w, float h ) :
CSulGuiCanvas( x, y, w, h )
{
	// need top and bottom button
	m_rButtonTop = new CSulGuiButton( 0, 0, w, w );
	addChild( m_rButtonTop );

	m_rButtonBottom = new CSulGuiButton( 0, h-w, w, w );
	addChild( m_rButtonBottom );

	// need drag button
	m_rButtonDrag = new CSulGuiButton( 0, w, w, w/2.0f );
	addChild( m_rButtonDrag );
}

void CSulGuiScrollBar::setupEventHandler( CSulGuiEventHandler* pEventHandler )
{
	CSulGuiCanvas::setupEventHandler( pEventHandler );

	m_rButtonTop->setupEventHandler( pEventHandler );
	m_rButtonBottom->setupEventHandler( pEventHandler );
	m_rButtonDrag->setupEventHandler( pEventHandler );

	m_rButtonTop->addMediator( new CSulGuiButtonMediator<CSulGuiScrollBar>(this) );
}

void CSulGuiScrollBar::buttonClick( CSulGuiButton* pButton )
{
	
}

// prehaps a generic drag comp with limits would be better than a hardcoded one???

