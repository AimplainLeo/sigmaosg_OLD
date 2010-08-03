// SulGuiCanvas.cpp

#include "stdafx.h"
#include "SulGuiCanvas.h"
#include "SulGuiComp.h"
#include "SulShaderGuiFrame.h"

CSulGuiCanvas::CSulGuiCanvas( float x, float y, float w, float h ) :
CSulGuiComp( x, y )
{
	m_bMouseHover = false;

	m_w = w;
	m_h = h;

	m_rQuad = new CSulGeomQuad(
		osg::Vec3( getW()/2.0f, getH()/2.0f, 0.0f ),
		getW(), getH() );
	m_rQuad->createUV();
	m_rGeodeQuad = new osg::Geode;
	m_rGeodeQuad->addDrawable( m_rQuad->getDrawable() );

	addChild( m_rGeodeQuad );

	// add a shader
	new CSulShaderGuiFrame( m_rGeodeQuad );

	m_rGeodeQuad->getOrCreateStateSet()->addUniform( new osg::Uniform( "w", getW() ) );
	m_rGeodeQuad->getOrCreateStateSet()->addUniform( new osg::Uniform( "h", getH() ) );
	m_rGeodeQuad->getOrCreateStateSet()->addUniform( new osg::Uniform( "border", 4.0f ) );
	m_rGeodeQuad->getOrCreateStateSet()->addUniform( m_uniformBgColor = new osg::Uniform( "bg_color", osg::Vec4(0,0,0,0.2f) ) );
	m_rGeodeQuad->getOrCreateStateSet()->addUniform( m_uniformBorderColor = new osg::Uniform( "border_color", osg::Vec4(0,0,1,1) ) );
}

void CSulGuiCanvas::showCanvas( bool bShow )
{
	m_rGeodeQuad->setNodeMask( bShow?0xFFFFFFFF:0 );
}

void CSulGuiCanvas::setBgColor( const osg::Vec4& c )
{
	m_uniformBgColor->set( c );
}

void CSulGuiCanvas::setBorderColor( const osg::Vec4& c )
{
	m_uniformBorderColor->set( c );
}

float CSulGuiCanvas::getW()
{
	return m_w;
}

float CSulGuiCanvas::getH()
{
	return m_h;
}

void CSulGuiCanvas::eventMouseMove( float local_x, float local_y, float x, float y )
{
	CSulGuiComp::eventMouseMove( local_x, local_y, x, y );

	if ( local_x>0 && local_y>0 && local_x<getW() && local_y<getH() )
	{
		if ( !m_bMouseHover )
		{
			setMouseHover( true );
		}
	}
	else
	{
		if ( m_bMouseHover )
		{
			setMouseHover( false );
		}
	}
}

void CSulGuiCanvas::setMouseHover( bool bHover )
{
	m_bMouseHover = bHover;
}

void CSulGuiCanvas::eventMouseRelease( float x_local, float y_local, float x, float y )
{
	CSulGuiComp::eventMouseRelease( x_local, y_local, x, y );

	if ( x_local>0 && y_local>0 && x_local<getW() && y_local<getH() )
	{
		setMouseRelease( true );
	}
	else
	{
		setMouseRelease( false );
	}
}

void CSulGuiCanvas::setMouseRelease( bool bInside )
{
}


