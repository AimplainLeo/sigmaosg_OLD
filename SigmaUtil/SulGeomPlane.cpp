// SulGeomPlane.cpp

#include "stdafx.h"
#include "SulGeomPlane.h"

CSulGeomPlane::CSulGeomPlane( const osg::Plane& plane, float extent ) :
CSulGeomQuad( osg::Vec3(0,0,0), extent, extent ),
m_plane( plane ),
m_extent( extent )
{
}

CSulGeomPlane::CSulGeomPlane( const osg::Plane& plane, float extent, const osg::Vec3& pos ) :
CSulGeomQuad( pos, extent, extent ),
m_plane( plane ),
m_extent( extent )
{
}
