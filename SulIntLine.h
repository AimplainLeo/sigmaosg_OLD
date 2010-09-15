// SulIntLine.h

#ifndef __SULINTLINE_H__
#define __SULINTLINE_H__

#include <osg/linesegment>

bool SulIntLine( const osg::LineSegment& line0, const osg::LineSegment& line1, float* s, float* t );

// returns the distance between the two line segments
float SulIntLine( const osg::LineSegment& line0, const osg::LineSegment& line1 );

#endif // __SULINTLINE_H__