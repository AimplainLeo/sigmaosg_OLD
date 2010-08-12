// SulDataTri.cpp

#include "stdafx.h"
#include "SulDataTri.h"
#include "SulMath.h"

// remove this line when i see it again

const float CSulDataTri::getArea() const
{
	// vector AB
	// vector AC
	// A = ½|AB X AC|
	osg::Vec3 AB = m_p0-m_p2;
	osg::Vec3 AC = m_p1-m_p2;
	osg::Vec3 c = AB ^ AC;
	float A = 0.5f*c.length();
	return A;
}

osg::Vec3 CSulDataTri::getRandomPoint() const
{
	float a = sigma::rand0to1();
	float b = sigma::rand0to1();

	if ( (a+b) > 1.0f )
	{
		a = 1.0f-a;
		b = 1.0f-b;
	}

	osg::Vec3 AB = m_p0-m_p2;
	osg::Vec3 AC = m_p1-m_p2;

	osg::Vec3 P = m_p2+AB*a+AC*b;

	return P;
}