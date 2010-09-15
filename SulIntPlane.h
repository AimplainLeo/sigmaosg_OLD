// SulIntPlane.h

#ifndef __SULINTPLANE_H__
#define __SULINTPLANE_H__

#include <osg/linesegment>

bool SulIntPlane( const osg::LineSegment& line, const osg::Plane& plane, osg::Vec3* pOut );
bool SulIntPlane( const osg::Vec3& v0, const osg::Vec3& v1, const osg::Plane& plane, osg::Vec3* pOut );

#endif // __SULINTPLANE_H__