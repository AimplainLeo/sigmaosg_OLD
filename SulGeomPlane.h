// SulGeomPlane.h

#ifndef __SULGEOMPLANE_H__
#define __SULGEOMPLANE_H__

#include "SulGeomQuad.h"

class CSulGeomPlane : public CSulGeomQuad
{
public:
					CSulGeomPlane( const osg::Plane& plane, float extent );

private:
	osg::Plane		m_plane;
	float			m_extent;
};

#endif // __SULGEOMPLANE_H__