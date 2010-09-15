// SulIntPlane.cpp

#include "stdafx.h"
#include "SulIntPlane.h"

bool SulIntPlane( const osg::LineSegment& line, const osg::Plane& plane, osg::Vec3* pOut )
{
	return SulIntPlane( line.start(), line.end(), plane, pOut );
}

bool SulIntPlane( const osg::Vec3& v0, const osg::Vec3& v1, const osg::Plane& plane, osg::Vec3* pOut )
{
	float u;
	float A, B, C, D;
	float x1, x2, y1, y2, z1, z2;

	A = plane[0];
	B = plane[1];
	C = plane[2];
	D = plane[3];

	x1 = v0.x();
	y1 = v0.y();
	z1 = v0.z();

	x2 = v1.x();
	y2 = v1.y();
	z2 = v1.z();

	u = ( A*x1 + B*y1 + C*z1 + D ) / ( A*(x1-x2) + B*(y1-y2) + C*(z1-z2) );

	if ( pOut )
	{
		(*pOut) = (v0) + (((v1)-(v0))*u);
	}

	if ( u<0.0f || u>1.0f )
	{
		return false;
	}

	return true;
}
