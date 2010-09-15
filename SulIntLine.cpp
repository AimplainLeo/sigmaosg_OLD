// SulIntLine.cpp

#include "stdafx.h"
#include "SulIntLine.h"

float Determinant( const osg::Matrix& m )
{
	//return m00*m11*m22 + m01*m12*m20 + m02*m10*m21 - m00*m12*m21 - m01*m10*m22 - m02*m11*m20;
	return m(0,0)*m(1,1)*m(2,2) + m(0,1)*m(1,2)*m(2,0) + m(0,2)*m(1,0)*m(2,1) - m(0,0)*m(1,2)*m(2,1) - m(0,1)*m(1,0)*m(2,2) - m(0,2)*m(1,1)*m(2,0);
}

bool SulIntLine( const osg::LineSegment& line0, const osg::LineSegment& line1, float* s, float* t )
{
	osg::Vec3 o1 = line0.start();
	osg::Vec3 o2 = line1.start();
	osg::Vec3 d1 = line0.end()-line0.start();
	osg::Vec3 d2 = line1.end()-line1.start();

	osg::Vec3	v0;
	osg::Vec3	v1;
	osg::Vec3	v2;
	osg::Matrix	m;
	float		det;
	float		len;

	v0 = o2-o1;
	v1 = d2;
	v2 = d1 ^ d2;
	//m.Set( v0, v1, v2 );
	m.set(
		v0.x(), v0.y(), v0.z(), 0.0f,
		v1.x(), v1.y(), v1.z(), 0.0f,
		v2.x(), v2.y(), v2.z(), 0.0f,
		0,0,0,1
		);
	det = Determinant( m );
	len = v2.length();
	if ( s )
	{
		*s = det/(len*len);
	}

	v1 = d1;
	//m.Set( v0, v1, v2 );
	m.set(
		v0.x(), v0.y(), v0.z(), 0.0f,
		v1.x(), v1.y(), v1.z(), 0.0f,
		v2.x(), v2.y(), v2.z(), 0.0f,
		0,0,0,1
		);
	det = Determinant( m );
	if ( t )
	{
		*t = det/(len*len);
	}

	return true;
}


float SulIntLine( const osg::LineSegment& line0, const osg::LineSegment& line1 )
{
	float s;
	float t;

	SulIntLine( line0, line1, &s, &t );

	osg::Vec3 p0 = line0.start();
	osg::Vec3 p1 = line1.start();

	if ( s>1.0f )
		 p0 += (line0.end()-line0.start());
	else if ( s>0.0f )
		p0 += (line0.end()-line0.start())*s;

	if ( t>1.0f )
		p1 += (line1.end()-line1.start());
	else if ( t>0.0f )
		p1 += (line1.end()-line1.start())*t;

	osg::Vec3 vLen = p1-p0;

	return vLen.length();
}

