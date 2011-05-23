// SulGuiCanvas.cpp

#include "stdafx.h"
#include "SulGuiCanvas.h"
#include "SulGuiComp.h"
#include "SulShaderGuiFrame.h"
#include "SulGuiEventHandler.h"
#include "SulNodePath.h"
#include <osgManipulator/Selection>

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
	setName( "CSulGuiCanvas" );

	m_cBg.set( 0,0,0, 0.2f );
	m_dragDragging = false;
	m_dragAllowed = false;
	m_dragMinX = 0.0f;
	m_dragMaxX = 10000000.0f;		
	m_dragMinY = 0.0f;
	m_dragMaxY = 10000000.0f;
	m_bMouseHover = false;
	m_bShowCanvas	= true;
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

	if ( pAttr->exist( "w" ) )				m_w = pAttr->get( "w" ).asFloat();
	if ( pAttr->exist( "h" ) )				m_h = pAttr->get( "h" ).asFloat();
	if ( pAttr->exist( "img" ) )			m_img = pAttr->get( "img" );
	if ( pAttr->exist( "show_canvas" ) )	m_bShowCanvas = pAttr->get( "show_canvas" ).asBool();
}

// FIXME: not all components need all these events!!!
void CSulGuiCanvas::setupEventHandler( CSulGuiEventHandler* pEventHandler )
{
	CSulGuiComp::setupEventHandler( pEventHandler );

	pEventHandler->signalMouseMove.connect( this, &CSulGuiCanvas::onMouseMove );
	pEventHandler->signalMouseDrag.connect( this, &CSulGuiCanvas::onMouseDrag );
	pEventHandler->signalMousePush.connect( this, &CSulGuiCanvas::onMousePush );
	pEventHandler->signalMouseRelease.connect( this, &CSulGuiCanvas::onMouseRelease );
	pEventHandler->signalViewResize.connect( this, &CSulGuiCanvas::onViewResize );
}

void CSulGuiCanvas::init()
{
	CSulGuiComp::init();

	float w = getW();

	m_rQuad = new CSulGeomQuad(
		osg::Vec3( w/2.0f, getH()/2.0f, 0.0f ),
		w, getH() );
	m_rQuad->createUV();

	osg::MatrixTransform::addChild( m_rQuad );

	// add a shader (perhaps we should move this to xml manager so we only have one shader)
	new CSulShaderGuiFrame( m_rQuad );

	m_rQuad->getOrCreateStateSet()->addUniform( m_uniformUseTexture = new osg::Uniform( "use_texture", 0 ) );
	m_rQuad->getOrCreateStateSet()->addUniform( m_uniformUseCover = new osg::Uniform( "use_cover", 1 ) );
	m_rQuad->getOrCreateStateSet()->addUniform( m_uniformUseBorder = new osg::Uniform( "use_border", 1 ) );
	m_rQuad->getOrCreateStateSet()->addUniform( m_uniformUseBackground = new osg::Uniform( "use_background", 1 ) );

	m_rQuad->getOrCreateStateSet()->addUniform( new osg::Uniform( "cover", 0 ) );
	m_rQuad->getOrCreateStateSet()->addUniform( m_uniformW = new osg::Uniform( "w", getW() ) );
	m_rQuad->getOrCreateStateSet()->addUniform( m_uniformH = new osg::Uniform( "h", getH() ) );
	m_rQuad->getOrCreateStateSet()->addUniform( new osg::Uniform( "border", 2.0f ) );
	m_rQuad->getOrCreateStateSet()->addUniform( m_uniformBgColor = new osg::Uniform( "bg_color", m_cBg ) );
	m_rQuad->getOrCreateStateSet()->addUniform( m_uniformBorderColor = new osg::Uniform( "border_color", osg::Vec4(0,0,1,1) ) );

	if ( !m_img.empty() )
	{
		setImage( m_img );
	}

	showCanvas( m_bShowCanvas );

	// FIXME: this should be an option for the user (not all gui components need mouserelease)
	getEventHandler()->wantEvent( this, this, CSulGuiEventHandler::EVENT_MOUSERELEASE ); 
}

void CSulGuiCanvas::useShaderTexture( bool bUse )
{
	m_uniformUseTexture->set( bUse?1:0 );
}

void CSulGuiCanvas::useShaderCover( bool bUse )
{
	m_uniformUseCover->set( bUse?1:0 );
}

void CSulGuiCanvas::useShaderBorder( bool bUse )
{
	m_uniformUseBorder->set( bUse?1:0 );
}

void CSulGuiCanvas::useShaderBackground( bool bUse )
{
	m_uniformUseBackground->set( bUse?1:0 );
}

void CSulGuiCanvas::setImage( const CSulString& imgFile )
{
	m_img = imgFile;

	m_rQuad->setTexture( m_img, 0 );
	m_uniformUseTexture->set( 1 );
	
	if ( m_w==0.0f || m_h==0.0f )
	{
		osg::Image* pImage = m_rQuad->getImage();
		setWH( pImage->s(), pImage->t() );
	}
}

void CSulGuiCanvas::setTexture( osg::Texture2D* pTex )
{
	m_rQuad->setTexture( pTex );
	m_uniformUseTexture->set( 1 );
}


CSulGuiCanvas* CSulGuiCanvas::asCanvas()
{
	return this;
}

void CSulGuiCanvas::setDraggable( bool bDraggable )
{
	m_dragAllowed = bDraggable;
}

void CSulGuiCanvas::showCanvas( bool bShow )
{
	if ( bShow==m_bShowCanvas )
		return;

	m_bShowCanvas = bShow;

	if ( m_bShowCanvas )
	{
		osg::MatrixTransform::addChild( m_rQuad );
	}
	else
	{
		osg::MatrixTransform::removeChild( m_rQuad );
	}
}

void CSulGuiCanvas::setBgColor( const osg::Vec4& c )
{
	m_cBg = c;

	if ( m_uniformBgColor.valid() )
	{
		m_uniformBgColor->set( c );
	}
}

const osg::Vec4& CSulGuiCanvas::getBgColor() const
{
	return m_cBg;
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

void CSulGuiCanvas::setW( float w )
{
	m_w = w;
	if ( m_rQuad.valid() )
	{
		m_rQuad->setCenter( osg::Vec3( m_w/2.0f, m_h/2.0f, 0.0f ) );
		m_rQuad->setWidth( m_w );
		m_uniformW->set( m_w );
	}
}

void CSulGuiCanvas::setH( float h )
{
	m_h = h;
	if ( m_rQuad.valid() )
	{
		m_rQuad->setCenter( osg::Vec3( m_w/2.0f, m_h/2.0f, 0.0f ) );
		m_rQuad->setHeight( m_h );
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

// uses local coordinates
bool CSulGuiCanvas::isInside( const osg::Vec2& vLocal )
{
	return isInside( vLocal.x(), vLocal.y() );
}

bool CSulGuiCanvas::isInside( float x, float y )
{
	return ( x>0 && y>0 && x<getW() && y<getH() )?true:false;
}

bool CSulGuiCanvas::isInsideWorld( const osg::Vec2& vWorld )
{
	osg::Vec2 local = getLocal( vWorld );
	return isInside( local );
}

bool CSulGuiCanvas::isInsideWorld( float xWorld, float yWorld )
{
	return isInsideWorld( osg::Vec2(xWorld, yWorld) );
}

void CSulGuiCanvas::setMouseRelease( bool bInside )
{
}

void CSulGuiCanvas::allowDrag( float minX, float maxX, float minY, float maxY )
{
	m_dragAllowed	= true;
	m_dragMinX		= minX;
	m_dragMaxX		= maxX;
	m_dragMinY		= minY;
	m_dragMaxY		= maxY;
}

bool isInsideChildren( float x, float y, CSulGuiCanvas* pCanvas )
{
	sigma::uint32 count = pCanvas->getNumChildren();

	for ( sigma::uint32 i=0; i<count; i++ )
	{
		CSulGuiCanvas* pChild = dynamic_cast<CSulGuiCanvas*>(pCanvas->getChild( i ));

		if ( pChild )
		{
			// are we inside this child?
			osg::NodePath path;
			sulNodePath( *pChild, path, 0, true );
			osg::Matrix m = osg::computeLocalToWorld( path );
			float local_x = x-m.getTrans().x();
			float local_y = y-m.getTrans().y();
			if ( pChild->isInside( local_x, local_y ) )
			{
				return true;
			}

			if ( isInsideChildren( x, y, pChild ) )
			{
				return true;
			}
		}
	}
	
	return false;
}

void CSulGuiCanvas::onMousePush( float x, float y )
{
	// calc local positions
	osg::NodePath path;
	sulNodePath( *this, path, 0, true );
	osg::Matrix m = osg::computeLocalToWorld( path );
	float local_x = x-m.getTrans().x();
	float local_y = y-m.getTrans().y();

	bool bEditMode = false;
	if ( isEditMode() )
	{
		bEditMode = !isInsideChildren( x, y, this );
	}

	if ( (bEditMode || m_dragAllowed) && isInside( local_x, local_y ) )
	{
		m_dragOfsPos = osg::Vec2( local_x, local_y );
		m_dragDragging = true;
	}
}

bool CSulGuiCanvas::eventMouseRelease( CSulGuiCanvas* pCanvas, float local_x, float local_y, float x, float y )
{
	if ( !isVisible() )
	{
		return false;
	}

	if ( pCanvas==this )
	{
		// check dragging
		if ( m_dragDragging )
		{
			m_dragDragging = false;
		}

		signalClicked( this );
		signalClickedExt( this, local_x, local_y, x, y );
		setMouseRelease( true );
		return true;
	}

	return false;
}

void CSulGuiCanvas::onMouseRelease( float x, float y )
{
return;
	// calc local positions
	osg::NodePath path;
	sulNodePath( *this, path, 0, true );
	osg::Matrix m = osg::computeLocalToWorld( path );
	float local_x = x-m.getTrans().x();
	float local_y = y-m.getTrans().y();

	// check dragging
	if ( m_dragDragging )
	{
		m_dragDragging = false;
	}

	// something else...
	if ( isInside( local_x, local_y ) )
	{
		signalClicked( this );
		setMouseRelease( true );
	}
	else
	{
		setMouseRelease( false );
	}

	// edit mode info stuff
	if ( isEditMode() &&  isInside( local_x, local_y ) && !isInsideChildren( x, y, this ) )
	{
		osg::notify(osg::ALWAYS) << "GUI EDIT: x = " << getX() << " , y = " << getY() << std::endl;
	}

}

void CSulGuiCanvas::onMouseMove( float x, float y )
{
	// calc local positions
	osg::NodePath path;
	sulNodePath( *this, path, 0, true );
	osg::Matrix m = osg::computeLocalToWorld( path );
	float local_x = x-m.getTrans().x();
	float local_y = y-m.getTrans().y();

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

void CSulGuiCanvas::onMouseDrag( float x, float y )
{
	// calc local positions
	osg::NodePath path;
	sulNodePath( *this, path, 0, true );
	osg::Matrix m = osg::computeLocalToWorld( path );
	float local_x = x-m.getTrans().x();
	float local_y = y-m.getTrans().y();

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

CSulGeomQuad* CSulGuiCanvas::getQuad()
{
	return m_rQuad;
}

void CSulGuiCanvas::onViewResize( float w, float h  )
{
return;
	osg::Vec2 d = getNativeDimensions();

	float ww = m_w * (d.x()/w);
	float hh = m_h * (d.y()/h);

	m_rQuad->setCenter( osg::Vec3( ww/2.0f, hh/2.0f, 0.0f ) );

	m_rQuad->setWidth( ww );
	m_rQuad->setHeight( hh );

	//osg::notify(osg::NOTICE) << "ww= " << ww << "   hh= " << hh << std::endl;
	osg::notify(osg::NOTICE) << "m_w= " << m_w << "   m_h= " << m_h << std::endl;
}

