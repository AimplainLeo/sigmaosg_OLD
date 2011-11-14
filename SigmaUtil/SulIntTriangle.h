// SulIntTriangle.h

#ifndef __SULINTTRIANGLE_H__
#define __SULINTTRIANGLE_H__

#include "SulDataTri.h"
#include <osg/linesegment>

bool sulIntTriangle( const osg::LineSegment& line, const CSulDataTri& tri, osg::Vec3& vHit, bool treatLineSegmentAsInfinity=false, float Epsilon=0.0001f );

bool sulIntTriangle( const osg::LineSegment& line, const CSulDataTri& tri, float Epsilon=0.0001f );

#endif // __SULINTTRIANGLE_H__