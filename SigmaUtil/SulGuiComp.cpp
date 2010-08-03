// SulGuiComp.h

#include "stdafx.h"
#include "SulGuiComp.h"
#include <osg/matrix>

CSulGuiComp::CSulGuiComp( float x, float y )
{
	m_events = 0;

	setXY( x, y );
}

void CSulGuiComp::setXY( float x, float y )
{
	osg::Matrix m;
	m.setTrans( x, y, 0.0f );
	setMatrix( m );	
}

float CSulGuiComp::getX()
{
	return 0.0f; // FIXME, should return local coordinates
}

float CSulGuiComp::getY()
{
	return 0.0f; // fix me, should return local coordinates
}

void CSulGuiComp::addEvents( Sigma::uint32 events )
{
	m_events |= events;
}

Sigma::uint32 CSulGuiComp::getEvents()
{
	return m_events;
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

