// sulDistancePointAndTriangle2d.cpp

#include "stdafx.h"
#include "sulDistancePointAndTriangle2d.h"
#include "sulDistancePointAndLine.h"

double sulDistancePointAndTriangle2d( const osg::Vec3& point, const CSulDataTri& tri )
{
	double d0 = sulDistancePointAndLine( point.x(), point.y(), tri.m_p0.x(), tri.m_p0.y(), tri.m_p1.x(), tri.m_p1.y()  );
	double d1 = sulDistancePointAndLine( point.x(), point.y(), tri.m_p1.x(), tri.m_p1.y(), tri.m_p2.x(), tri.m_p2.y()  );
	double d2 = sulDistancePointAndLine( point.x(), point.y(), tri.m_p2.x(), tri.m_p2.y(), tri.m_p0.x(), tri.m_p0.y()  );

	double dNew = osg::minimum( d0, d1 );
	double dOut = osg::minimum( dNew, d2 );

	return dOut;
}
