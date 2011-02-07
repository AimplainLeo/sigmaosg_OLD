// SulGuiCounter.cpp

#include "stdafx.h"
#include "SulGuiCounter.h"

CSulGuiCounter::CSulGuiCounter() :
CSulGuiCanvas( "COUNTER" )
{
	m_digits = 1;
}

void CSulGuiCounter::setupTheme( CSulGuiThemeXml* pThemeXml )
{
	CSulGuiCanvas::setupTheme( pThemeXml );
}

void CSulGuiCounter::setupAttr( CSulXmlAttr* pAttr )
{
	CSulGuiCanvas::setupAttr( pAttr );

	if ( pAttr->exist( "digits" ) )	m_digits = pAttr->get( "digits" ).asUint32();
}

void CSulGuiCounter::onClick( CSulGuiCanvas* pCanvas )
{
	float u = pCanvas->getQuad()->getU();

	u += 0.1f;
	if ( u>=1.0f )
		u = 0.0f;

	pCanvas->getQuad()->setUV( u, u+0.1f, 0.0f, 1.0f );
	pCanvas->getQuad()->getDrawable( 0 )->asGeometry()->dirtyDisplayList();
}

void CSulGuiCounter::init()
{
	CSulGuiCanvas::init();

	showCanvas( false );

	sigma::uint32 i;

	for ( i=0; i<m_digits; i++ )
	{
		CSulGuiCanvas* p = new CSulGuiCanvas( "digit" );
		p->setupEventHandler( getEventHandler() );
		p->setImage( "images/counter.png" );
		p->init();		
		p->setXY( i*20.0f, 0.0f );
		p->setWH( 20.0f, 20.0f );
		p->getQuad()->setUV( 0.1f, 1.0f );
		p->signalClicked.connect( this, &CSulGuiCounter::onClick );
		
		addChild( p );
	}

	setWH( m_digits*20.0f, 20.0f );
}

sigma::uint32 CSulGuiCounter::getValue()
{
	sigma::uint32 val = 0;

	sigma::uint32 dig = pow(10.0f,int(m_digits));
	sigma::uint32 count = getNumChildren();
	for ( sigma::uint32 i=0; i<count ; i++ )
	{
		CSulGuiCanvas* p = dynamic_cast<CSulGuiCanvas*>(getChild(i));
		if ( p )
		{
			val += int(p->getQuad()->getU()*dig);
			dig /= 10.0f;
		}
	}	
		
	return val;
}