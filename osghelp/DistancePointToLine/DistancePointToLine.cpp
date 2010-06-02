// DistancePointToLine.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <SigmaUtil/SulDistancePointAndLine.h>
#include <osg/notify>

int _tmain(int argc, _TCHAR* argv[])
{
	double point_x = -100.0;
	double point_y = -100.0;
	double x0 = -50.0;
	double y0 = -50.0;
	double x1 = -50.0;
	double y1 = -100.0;

	double len = sulDistancePointAndLine( point_x, point_y, x0, y0, x1, y1 );

	// expect distance is 50
	osg::notify(osg::NOTICE) << "LEN = " << len << std::endl;

	return 0;
}

