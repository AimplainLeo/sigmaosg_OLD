// SulGeomPlane.h

#ifndef __SULGEOMPLANE_H__
#define __SULGEOMPLANE_H__

#include "SulGeomQuad.h"

class CSulGeomPlane : public CSulGeomQuad
{
public:
					CSulGeomPlane( const osg::Plane& plane, float extent );

					CSulGeomPlane( const osg::Plane& plane, float extent, const osg::Vec3& pos );

private:
	osg::Plane		m_plane;
	float			m_extent;
};

#endif // __SULGEOMPLANE_H__