// SulGuiAlign.cpp

#include "stdafx.h"
#include "SulGuiAlign.h"
#include "SulGuiEventHandler.h"

CSulGuiAlign::CSulGuiAlign() :
CSulGuiCanvas( "ALIGN" )
{
	setName( "CSulGuiCanvas" );

	m_eAlignV = V_TOP;
	m_eAlignH = H_LEFT;
	m_xOfs = 0.0f;
	m_yOfs = 0.0f;
}

void CSulGuiAlign::setupEventHandler( CSulGuiEventHandler* pEventHandler )
{
	CSulGuiCanvas::setupEventHandler( pEventHandler );

	pEventHandler->signalViewResize.connect( this, &CSulGuiAlign::onViewResize );
}

void CSulGuiAlign::setupAttr( CSulXmlAttr* pAttr )
{
	CSulGuiCanvas::setupAttr( pAttr );

	if ( pAttr->exist( "align_h" ) )
	{
		CSulString s = pAttr->get( "align_h" );
		if ( s=="left" )	m_eAlignH = H_LEFT;
		if ( s=="right" )	m_eAlignH = H_RIGHT;
		if ( s=="center" )	m_eAlignH = H_CENTER;
	}
	
	if ( pAttr->exist( "align_v" ) )
	{
		CSulString s = pAttr->get( "align_v" );
		if ( s=="top" )		m_eAlignV = V_TOP;
		if ( s=="bottom" )	m_eAlignV = V_BOTTOM;
		if ( s=="center" )	m_eAlignV = V_CENTER;
	}

	if ( pAttr->exist( "x_ofs" ) )
	{
		m_xOfs = pAttr->get( "x_ofs" ).asFloat();
	}

	if ( pAttr->exist( "y_ofs" ) )
	{
		m_yOfs = pAttr->get( "y_ofs" ).asFloat();
	}
}

void CSulGuiAlign::init()
{
	CSulGuiCanvas::init();

	showCanvas( false );
}

/*
void CSulGuiAlign::onViewResize( float w, float h )
{
	CSulGuiCanvas* p = dynamic_cast<CSulGuiCanvas*>(getParent(0));
	setW( p?p->getW():w );
	setH( p?p->getH():h );
		
	// reposition children horizontal
	float acc_w = 0.0f;
	float acc_h = 0.0f;
	sigma::uint32 count = getNumChildren();
	for ( sigma::uint32 i=0; i<count ; i++ )
	{
		CSulGuiCanvas* p = dynamic_cast<CSulGuiCanvas*>(getChild(i));
		if ( p )
		{
			if ( m_eAlignH==RIGHT )
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

			if ( m_eAlignV==BOTTOM )
			{
				acc_h += p->getH();
				float yy = getH()-acc_h;
				p->setY( yy-m_yOfs );
			}
			else 
			{
				float yy = acc_h;
				p->setY( yy+m_yOfs );	
				acc_h += p->getH();
			}
		}
	}
}
*/


/*
void CSulGuiAlign::onViewResize( float w, float h )
{
	CSulGuiCanvas* p = dynamic_cast<CSulGuiCanvas*>(getParent(0));
	setW( p?p->getW():w );
	setH( p?p->getH():h );
		
	sigma::uint32 count = getNumChildren();
	for ( sigma::uint32 i=0; i<count ; i++ )
	{
		CSulGuiCanvas* p = dynamic_cast<CSulGuiCanvas*>(getChild(i));
		if ( p )
		{
			if ( m_eAlignH==RIGHT )
			{
				float xx = getW() - (p->getW()+p->getAttrX());
				p->setX( xx );
			}

			if ( m_eAlignV==BOTTOM )
			{
				float yy = getH() - (p->getH()+p->getAttrY());
				p->setY( yy );
			}
		}
	}
}
*/

void CSulGuiAlign::update()
{
	sigma::uint32 count = getNumChildren();
	for ( sigma::uint32 i=0; i<count ; i++ )
	{
		CSulGuiCanvas* p = dynamic_cast<CSulGuiCanvas*>(getChild(i));
		if ( p )
		{
			if ( m_eAlignH==H_CENTER )
			{
				float xx = (getW()/2.0f - p->getW()/2.0) + p->getX();

				// we write directly to the matrixtransform
				osg::Matrix m;
				m = p->getMatrix();
				osg::Vec3 pos = m.getTrans();
				pos.x() = xx;
				m.setTrans( pos );
				p->setMatrix( m );	
			}

			if ( m_eAlignH==H_RIGHT )
			{
				float xx = getW() - (p->getW()+p->getX());

				// we write directly to the matrixtransform
				osg::Matrix m;
				m = p->getMatrix();
				osg::Vec3 pos = m.getTrans();
				pos.x() = xx;
				m.setTrans( pos );
				p->setMatrix( m );	
			}

			if ( m_eAlignV==V_CENTER )
			{
				float yy = (getH()/2.0f - p->getH()/2.0) + p->getY();

				// we write directly to the matrixtransform
				osg::Matrix m;
				m = p->getMatrix();
				osg::Vec3 pos = m.getTrans();
				pos.y() = yy;
				m.setTrans( pos );
				p->setMatrix( m );	
			}

			if ( m_eAlignV==V_BOTTOM )
			{
				float yy = getH() - (p->getH()+p->getY());

				// we write directly to the matrixtransform
				osg::Matrix m;
				m = p->getMatrix();
				osg::Vec3 pos = m.getTrans();
				pos.y() = yy;
				m.setTrans( pos );
				p->setMatrix( m );	
			}
		}
	}
}

void CSulGuiAlign::onViewResize( float w, float h )
{
	CSulGuiCanvas* p = dynamic_cast<CSulGuiCanvas*>(getParent(0));
	setW( p?p->getW():w );
	setH( p?p->getH():h );

	update();
}
