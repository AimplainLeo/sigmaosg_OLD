// SulGuiComp.h

#include "stdafx.h"
#include "SulGuiComp.h"
#include "SulGuiEventHandler.h"
#include <osg/matrix>

CSulGuiComp::CSulGuiComp( float x, float y )
{
	setXY( x, y );
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

void CSulGuiComp::show( bool bShow )
{
	setNodeMask( bShow?0xFFFFFFFF:0 );
}

void CSulGuiComp::addEvent( Sigma::uint32 eventType )
{
	m_pEventHandler->addEvent( this, eventType );
}

void CSulGuiComp::setLayer( Sigma::uint32 layer )
{
	getOrCreateStateSet()->setRenderBinDetails( layer, "DepthSortedBin" );
}

void CSulGuiComp::eventMouseMove( float mouse_local_x, float mouse_local_y, float mouse_x, float mouse_y )
{
}

void CSulGuiComp::eventMouseRelease( float x_local, float y_local, float x, float y )
{
}

void CSulGuiComp::eventKeyDown( Sigma::int32 key, Sigma::int32 iMod )
{
}

