// SulGuiCanvas.cpp

#include "stdafx.h"
#include "SulGuiCanvas.h"
#include "SulGuiComp.h"
#include "SulShaderGuiFrame.h"
#include "SulGuiEventHandler.h"

CSulGuiCanvas::CSulGuiCanvas( const CSulString& sCompName ) :
CSulGuiComp( sCompName )
{
	initConstructor();
}

CSulGuiCanvas::CSulGuiCanvas( const CSulString& sCompName, float x, float y ) :
CSulGuiComp( sCompName, x, y )
{
	initConstructor();
}

CSulGuiCanvas::CSulGuiCanvas( const CSulString& sCompName, float x, float y, float w, float h ) :
CSulGuiComp( sCompName, x, y )
{
	initConstructor();
	m_w = w;
	m_h = h;
}

void CSulGuiCanvas::initConstructor()
{
	m_dragDragging = false;
	m_dragAllowed = false;
	m_dragMinX = 0.0f;
	m_dragMaxX = 10000000.0f;		
	m_dragMinY = 0.0f;
	m_dragMaxY = 10000000.0f;
	m_bMouseHover = false;
}

void CSulGuiCanvas::setupTheme( CSulGuiThemeXml* pThemeXml )
{
	CSulGuiComp::setupTheme( pThemeXml );

	m_w = getThemeValue( "w" ).asFloat();
	m_h = getThemeValue( "h" ).asFloat();
}

void CSulGuiCanvas::setupAttr( CSulXmlAttr* pAttr )
{
	CSulGuiComp::setupAttr( pAttr );

	if ( pAttr->exist( "w" ) ) m_w = pAttr->get( "w" ).asFloat();
	if ( pAttr->exist( "h" ) ) m_h = pAttr->get( "h" ).asFloat();
}

void CSulGuiCanvas::init()
{
	CSulGuiComp::init();

	float w = getW();

	m_rQuad = new CSulGeomQuad(
		osg::Vec3( w/2.0f, getH()/2.0f, 0.0f ),
		w, getH() );
	m_rQuad->createUV();
	m_rGeodeQuad = new osg::Geode;
	m_rGeodeQuad->addDrawable( m_rQuad->getDrawable() );

	osg::MatrixTransform::addChild( m_rGeodeQuad );

	// add a shader
	new CSulShaderGuiFrame( m_rGeodeQuad );

	m_rGeodeQuad->getOrCreateStateSet()->addUniform( new osg::Uniform( "cover", 0 ) );
	m_rGeodeQuad->getOrCreateStateSet()->addUniform( m_uniformW = new osg::Uniform( "w", getW() ) );
	m_rGeodeQuad->getOrCreateStateSet()->addUniform( m_uniformH = new osg::Uniform( "h", getH() ) );
	m_rGeodeQuad->getOrCreateStateSet()->addUniform( new osg::Uniform( "border", 4.0f ) );
	m_rGeodeQuad->getOrCreateStateSet()->addUniform( m_uniformBgColor = new osg::Uniform( "bg_color", osg::Vec4(0,0,0,0.2f) ) );
	m_rGeodeQuad->getOrCreateStateSet()->addUniform( m_uniformBorderColor = new osg::Uniform( "border_color", osg::Vec4(0,0,1,1) ) );
}

void CSulGuiCanvas::setDraggable( bool bDraggable )
{
	m_dragAllowed = bDraggable;
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

void CSulGuiCanvas::setWH( float w, float h )
{
	m_w = w;
	m_h = h;

	if ( m_rQuad.valid() )
	{
		m_rQuad->setCenter( osg::Vec3( w/2.0f, h/2.0f, 0.0f ) );
		m_rQuad->setWidth( m_w );
		m_rQuad->setHeight( m_h );
		m_uniformW->set( m_w );
		m_uniformH->set( m_h );
	}
}

float CSulGuiCanvas::getW()
{
	return m_w;
}

float CSulGuiCanvas::getH()
{
	return m_h;
}

bool CSulGuiCanvas::isInside( float x, float y )
{
	return ( x>0 && y>0 && x<getW() && y<getH() )?true:false;
}

void CSulGuiCanvas::eventMousePushed( float x_local, float y_local, float x, float y )
{
	CSulGuiComp::eventMousePushed( x_local, y_local, x, y );

	if ( m_dragAllowed && isInside( x_local, y_local ) )
	{
		m_dragOfsPos = osg::Vec2( x_local, y_local );
		m_dragDragging = true;
	}
}

void CSulGuiCanvas::eventMouseRelease( float x_local, float y_local, float x, float y )
{
	CSulGuiComp::eventMouseRelease( x_local, y_local, x, y );

	// check dragging
	if ( m_dragDragging )
	{
		m_dragDragging = false;
	}

	// something else...
	if ( isInside( x_local, y_local ) )
	{
		signalClicked( this );
		setMouseRelease( true );
	}
	else
	{
		setMouseRelease( false );
	}
}

void CSulGuiCanvas::eventMouseMove( float local_x, float local_y, float x, float y )
{
	CSulGuiComp::eventMouseMove( local_x, local_y, x, y );

	// check hover
	if ( local_x>0 && local_y>0 && local_x<getW() && local_y<getH() )
	{
		if ( !m_bMouseHover )
		{
			m_bMouseHover = true;
			signalHover( true );
		}
	}
	else
	{
		if ( m_bMouseHover )
		{
			m_bMouseHover = false;
			signalHover( false );
		}
	}
}

void CSulGuiCanvas::eventMouseDrag( float local_x, float local_y, float x, float y )
{
	CSulGuiComp::eventMouseDrag( local_x, local_y, x, y );

	// check dragging
	if ( m_dragDragging )
	{
		osg::Vec2 d = osg::Vec2( local_x, local_y ) - m_dragOfsPos;
		osg::Vec2 pos = osg::Vec2( getX(), getY() ) + d;

		// check limits
		if ( pos.x()<m_dragMinX ) pos.x() = m_dragMinX;
		if ( pos.x()>m_dragMaxX ) pos.x() = m_dragMaxX;
		if ( pos.y()<m_dragMinY ) pos.y() = m_dragMinY;
		if ( pos.y()>m_dragMaxY ) pos.y() = m_dragMaxY;
		

		setXY( pos.x(), pos.y() );
	}
}

void CSulGuiCanvas::setMouseRelease( bool bInside )
{
}

void CSulGuiCanvas::setupEventHandler( CSulGuiEventHandler* pEventHandler )
{
	CSulGuiComp::setupEventHandler( pEventHandler );

	addEvent( CSulGuiEventHandler::EVENT_MOUSE_MOVE );	
	addEvent( CSulGuiEventHandler::EVENT_MOUSE_DRAG );	
	addEvent( CSulGuiEventHandler::EVENT_MOUSE_PUSHED );	
	addEvent( CSulGuiEventHandler::EVENT_MOUSE_RELEASE );
}

void CSulGuiCanvas::allowDrag( float minX, float maxX, float minY, float maxY )
{
	m_dragAllowed	= true;
	m_dragMinX		= minX;
	m_dragMaxX		= maxX;
	m_dragMinY		= minY;
	m_dragMaxY		= maxY;
}

