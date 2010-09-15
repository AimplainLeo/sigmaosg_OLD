// SulIntTriangle.cpp

#include "stdafx.h"
#include "SulIntTriangle.h"

bool sulIntTriangle( const osg::LineSegment& line, const CSulDataTri& tri, osg::Vec3& vHit, float Epsilon )
{
	osg::Vec3    u, v, n;				// triangle vectors
    osg::Vec3    dir, w0, w;			// ray vectors
    float		 r, a, b;				// params to calc ray-plane intersect

    // get triangle edge vectors and plane normal
    u = tri.m_p1 - tri.m_p0;
    v = tri.m_p2 - tri.m_p0;
    n = u ^ v;							// cross product
	float len = n.length();
    if ( len>-Epsilon && len<Epsilon )		// triangle is degenerate
        return false;					// do not deal with this case

	dir = line.end() - line.start();	        // ray direction vector
	w0 = line.start() - tri.m_p0;
    a = - ( n * w0 );
    b = n * dir;
    if ( fabs(b)<Epsilon )
	{ 
		return false;
    }

    // get intersect point of ray with triangle plane
    r = a / b;
	/*
    if ( r<0.0f )						// ray goes away from triangle
        return false;					// => no intersect
		*/
    // for a segment, also test if (r > 1.0) => no intersect

    vHit = line.start() + (dir*r);           // intersect point of ray and plane
 
    // is I inside T?
    float    uu, uv, vv, wu, wv, D;
    uu = u * u;
    uv = u * v;
    vv = v * v;
    w = vHit - tri.m_p0;
    wu = w * u;
    wv = w * v;
    D = uv * uv - uu * vv;

    // get and test parametric coords
    float s, t;
    s = (uv * wv - vv * wu) / D;
    if (s < 0.0 || s > 1.0)        // I is outside T
        return false;

	t = (uv * wu - uu * wv) / D;
    if (t < 0.0 || (s + t) > 1.0)  // I is outside T
        return false;

	return true;

}

bool sulIntTriangle( const osg::LineSegment& line, const CSulDataTri& tri, float Epsilon )
{
	osg::Vec3 vHit;
	return sulIntTriangle( line, tri, vHit, Epsilon );
}