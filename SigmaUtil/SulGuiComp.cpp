// SulGuiComp.h

#include "stdafx.h"
#include "SulGuiComp.h"
#include "SulGuiEventHandler.h"
#include "SulNodePath.h"
#include <osg/matrix>
#include <osgManipulator/Selection>

CSulGuiComp::CSulGuiComp( const CSulString& sCompName )
{
	initConstructor();
	m_sCompName = sCompName;
}

CSulGuiComp::CSulGuiComp( const CSulString& sCompName, float x, float y )
{
	initConstructor();
	m_sCompName = sCompName;
	setXY( x, y );
}

void CSulGuiComp::initConstructor()
{
	m_pParentLast	= 0;
	m_layer			= 0;
	m_bEditMode		= false;
	m_bActive		= true;
	m_renderbinNum	= 0;
	m_attrX			= 0.0f;
	m_attrY			= 0.0f;
	m_attrValid		= false;
	m_bShow			= true;
}

void CSulGuiComp::init()
{
	if ( m_attrValid )
	{
		setX( m_attrX );
		setY( m_attrY ); 
	}

	show( m_bShow );
}

bool CSulGuiComp::addChild( Node *child )
{
	bool bRet = osg::MatrixTransform::addChild( child );

	CSulGuiComp* p = dynamic_cast<CSulGuiComp*>(child);
	if ( p )
	{
		p->setLayer( getLayer()+10 );
	}

	return bRet;
}

const CSulString& CSulGuiComp::getCompName() const
{
	return m_sCompName;
}

void CSulGuiComp::setEditMode( bool bEdit )
{
	m_bEditMode = bEdit;
}

bool CSulGuiComp::isEditMode()
{
	return m_bEditMode;
}

void CSulGuiComp::setActive( bool bActive )
{
	m_bActive = bActive;
}

bool CSulGuiComp::isActive()
{
	return m_bActive;
}

CSulString CSulGuiComp::getThemeValue( const CSulString& attr )
{
	return m_rThemeXml->getValue( m_sCompName, attr );
}

void CSulGuiComp::setupAttr( CSulXmlAttr* pAttr )
{
	if ( pAttr->exist( "id" ) )			m_id = pAttr->get( "id" );
	if ( pAttr->exist( "x" ) )			m_attrX = pAttr->get( "x" ).asFloat();
	if ( pAttr->exist( "y" ) )			m_attrY = pAttr->get( "y" ).asFloat();
	if ( pAttr->exist( "renderbin" ) )	m_renderbinNum = pAttr->get( "renderbin" ).asInt32();

	if ( pAttr->exist( "x" ) || pAttr->exist( "y" ) )
	{
		m_attrValid = true;
	}
}

void CSulGuiComp::setupTheme( CSulGuiThemeXml* pThemeXml )
{
	m_rThemeXml = pThemeXml;
}

CSulGuiThemeXml* CSulGuiComp::getTheme()
{
	return m_rThemeXml;
}

void CSulGuiComp::setupEventHandler( CSulGuiEventHandler* pEventHandler )
{
	m_pEventHandler = pEventHandler;

	pEventHandler->signalNativeDimensionsChanged.connect( this, &CSulGuiComp::onNativeDimensionsChanged );
}

float CSulGuiComp::getAttrX()
{
	return m_attrX;
}

float CSulGuiComp::getAttrY()
{
	return m_attrY;
}

void CSulGuiComp::setXY( float x, float y )
{
	osg::Matrix m;
	m.setTrans( x, y, 0.0f );
	setMatrix( m );	

	signalPositionChanged( x, y );
}

void CSulGuiComp::setX( float x )
{
	osg::Matrix m;
	m = getMatrix();
	osg::Vec3 pos = m.getTrans();
	pos.x() = x;
	m.setTrans( pos );
	setMatrix( m );
}

void CSulGuiComp::setY( float y )
{
	osg::Matrix m;
	m = getMatrix();
	osg::Vec3 pos = m.getTrans();
	pos.y() = y;
	m.setTrans( pos );
	setMatrix( m );
}

float CSulGuiComp::getX()
{
	osg::Matrix m;
	m = getMatrix();
	osg::Vec3 pos = m.getTrans();
	return pos.x();
}

float CSulGuiComp::getY()
{
	osg::Matrix m;
	m = getMatrix();
	osg::Vec3 pos = m.getTrans();
	return pos.y();
}

float CSulGuiComp::getWorldX()
{
	osg::NodePath path;
	sulNodePath( *this, path, 0, true );
	osg::Matrix m = osg::computeLocalToWorld( path );
	osg::Vec3 pos = m.getTrans();
	return pos.x();
}

float CSulGuiComp::getWorldY()
{
	osg::NodePath path;
	sulNodePath( *this, path, 0, true );
	osg::Matrix m = osg::computeLocalToWorld( path );
	osg::Vec3 pos = m.getTrans();
	return pos.y();
}

osg::Vec2 CSulGuiComp::getLocal( const osg::Vec2& vWorld )
{
	return getLocal( vWorld.x(), vWorld.y() );
}

osg::Vec2 CSulGuiComp::getLocal( float xWorld, float yWorld )
{
	osg::NodePath path;
	sulNodePath( *this, path, 0, true );
	osg::Matrix m = osg::computeLocalToWorld( path );
	osg::Vec2 v;
	v.x() = xWorld-m.getTrans().x();
	v.y() = yWorld-m.getTrans().y();
	return v;
}

void CSulGuiComp::show( bool bShow )
{
	m_bShow = bShow;

	if ( m_bShow )
	{
		if ( m_pParentLast )
			m_pParentLast->addChild( this );

		m_pParentLast = 0;
	}
	else
	{
		if ( getNumParents() )
		{
			m_pParentLast = getParent(0)->asGroup();
			m_pParentLast->removeChild( this );
		}
	}

	// FIXME: don't use nodemasks
	//setNodeMask( bShow?0xFFFFFFFF:0 );
}

void CSulGuiComp::toggleShow()
{
	m_bShow = !m_bShow;

	// FIXME: don't use nodemasks
	/*
	int i = getNodeMask();
	setNodeMask( i?0:0xFFFFFFFF );
	*/
}

bool CSulGuiComp::isVisible()
{
	return m_bShow;

	/*
	int i = getNodeMask();
	return i==0?false:true;
	*/
}

void CSulGuiComp::setLayer( sigma::uint32 layer )
{
	m_layer = layer;
	getOrCreateStateSet()->setRenderBinDetails( m_renderbinNum?m_renderbinNum:layer, "DepthSortedBin" );

	// set childrens new layer if any
	sigma::uint32 numChildren = getNumChildren();
	for ( sigma::uint32 i = 0; i<numChildren; i++ )
	{
		CSulGuiComp* p = dynamic_cast<CSulGuiComp*>(getChild( i ));
		if ( p )
		{
			p->setLayer( layer+10 );
		}
	}
}

sigma::uint32 CSulGuiComp::getLayer()
{
	return m_layer;
}
 
void CSulGuiComp::eventMouseMove( float mouse_local_x, float mouse_local_y, float mouse_x, float mouse_y )
{
}

void CSulGuiComp::eventMouseDrag( float mouse_local_x, float mouse_local_y, float mouse_x, float mouse_y )
{
}

void CSulGuiComp::eventMousePushed( float x_local, float y_local, float x, float y )
{
}

void CSulGuiComp::eventMouseRelease( float x_local, float y_local, float x, float y )
{
}

void CSulGuiComp::eventKeyDown( sigma::int32 key, sigma::int32 iMod )
{
}

CSulGuiEventHandler* CSulGuiComp::getEventHandler()
{
	return m_pEventHandler;
}

const CSulString& CSulGuiComp::getId() const
{
	return m_id;
}

osg::Vec2 CSulGuiComp::getNativeDimensions()
{
	return m_nativeDimensions;
}

void CSulGuiComp::onNativeDimensionsChanged( float w, float h )
{
	m_nativeDimensions.set( w, h );
}

