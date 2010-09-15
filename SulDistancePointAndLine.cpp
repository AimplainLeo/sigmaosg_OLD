	// SulDistancePointAndLine.cpp

#include "stdafx.h"
#include "SulDistancePointAndLine.h"

double sulDistancePointAndLine( double point_x, double point_y, double x0, double y0, double x1, double y1 )
{
	osg::Vec2 v0( x0, y0 );
	osg::Vec2 v1( x1, y1 );
	osg::Vec2 p( point_x, point_y );

	osg::Vec2 v = v1-v0;
	osg::Vec2 w = p-v0;
	
	double c1 = w * v;
	if ( c1<=0.0 )
	{
		osg::Vec2 vlen = p-v0;
		return vlen.length();
	}

	double c2 = v * v;
	if ( c2 <= c1 )
	{
		osg::Vec2 vlen = p-v1;
		return vlen.length();
	}

	double b = c1/c2;
	osg::Vec2 Pb = v0 + v*b;
	osg::Vec2 vlen = p-Pb;
	return vlen.length();	
}