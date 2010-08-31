// SulGuiAlignH.cpp

#include "stdafx.h"
#include "SulGuiAlignH.h"
#include "SulGuiEventHandler.h"
#include <assert.h>

CSulGuiAlignH::CSulGuiAlignH() :
CSulGuiCanvas( "HALIGN" )
{
	m_eAlign = LEFT;
	m_xOfs = 0.0f;
}

void CSulGuiAlignH::setupEventHandler( CSulGuiEventHandler* pEventHandler )
{
	CSulGuiCanvas::setupEventHandler( pEventHandler );

	pEventHandler->signalViewResize.connect( this, &CSulGuiAlignH::onViewResize );
}

void CSulGuiAlignH::setupAttr( CSulXmlAttr* pAttr )
{
	CSulGuiCanvas::setupAttr( pAttr );

	if ( pAttr->exist( "align" ) )
	{
		CSulString s = pAttr->get( "align" );
		if ( s=="left" ) m_eAlign = LEFT;
		if ( s=="right" ) m_eAlign = RIGHT;
	}

	if ( pAttr->exist( "x_ofs" ) )
	{
		m_xOfs = pAttr->get( "x_ofs" ).asFloat();
	}
}

void CSulGuiAlignH::init()
{
	CSulGuiCanvas::init();

}

void CSulGuiAlignH::onViewResize( float w, float h )
{
	CSulGuiCanvas* p = dynamic_cast<CSulGuiCanvas*>(getParent(0));
	setW( p?p->getW():w );
		
	// reposition children
	float acc_w = 0.0f;
	sigma::uint32 count = getNumChildren();
	for ( sigma::uint32 i=0; i<count ; i++ )
	{
		CSulGuiCanvas* p = dynamic_cast<CSulGuiCanvas*>(getChild(i));
		if ( p )
		{
			if ( m_eAlign==RIGHT )
			{
				acc_w += p->getW();
				float xx = getW()-acc_w;
				p->setX( xx-m_xOfs );
			}
			else 
			{
				float xx = acc_w;
				p->setX( xx+m_xOfs );	
				acc_w += p->getW();
			}
		}
	}
}