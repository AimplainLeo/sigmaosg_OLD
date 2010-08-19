// SulGuiComp.h

#include "stdafx.h"
#include "SulGuiComp.h"
#include "SulGuiEventHandler.h"
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
	m_bActive = true;
}

void CSulGuiComp::init()
{
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
	if ( pAttr->exist( "x" ) ) setX( pAttr->get( "x" ).asFloat() );
	if ( pAttr->exist( "y" ) ) setY( pAttr->get( "y" ).asFloat() );
}

void CSulGuiComp::setupTheme( CSulGuiThemeXml* pThemeXml )
{
	m_rThemeXml = pThemeXml;
}

void CSulGuiComp::setupView( float w, float h )
{
}

void CSulGuiComp::setupEventHandler( CSulGuiEventHandler* pEventHandler )
{
	m_pEventHandler = pEventHandler;
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
	osg::NodePath pathToRoot;
	osgManipulator::computeNodePathToRoot( *this, pathToRoot );
	osg::Matrix m = osg::computeLocalToWorld( pathToRoot );
	osg::Vec3 pos = m.getTrans();
	return pos.x();
}

float CSulGuiComp::getWorldY()
{
	osg::NodePath pathToRoot;
	osgManipulator::computeNodePathToRoot( *this, pathToRoot );
	osg::Matrix m = osg::computeLocalToWorld( pathToRoot );
	osg::Vec3 pos = m.getTrans();
	return pos.y();
}

void CSulGuiComp::show( bool bShow )
{
	setNodeMask( bShow?0xFFFFFFFF:0 );
}

void CSulGuiComp::toggleShow()
{
	int i = getNodeMask();
	setNodeMask( i?0:0xFFFFFFFF );
}

void CSulGuiComp::setLayer( sigma::uint32 layer )
{
	getOrCreateStateSet()->setRenderBinDetails( layer, "DepthSortedBin" );
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