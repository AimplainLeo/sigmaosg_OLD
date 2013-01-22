// SulGeomHemiSphere.cpp

#include "stdafx.h"
#include "SulGeomHemiSphere.h"

CSulGeomHemiSphere::CSulGeomHemiSphere( float radius, sigma::uint32 sides, sigma::uint32 segments ) :
CSulGeom()
{
	m_radius = radius;
	m_sides = sides;
	m_segments = segments;
	createDrawable();
}

void CSulGeomHemiSphere::createDrawable()
{
	float lats		= (float)m_segments;
	float longs		= (float)m_sides;
	float fRadius	= m_radius;

	int i, j;	
	for ( i=1; i<=lats; i++ ) 
	{
		float lat0 = osg::PI * ( (float) (i - 1) / (lats*2.0f));
		float z0  = sin(lat0);
		float zr0 = cos(lat0);

		float lat1 = osg::PI * ( (float) i / (lats*2.0f));
		float z1  = sin(lat1);
		float zr1 = cos(lat1);

		for ( j=0; j<=longs; j++ ) 
		{
			float lng = 2.0f * osg::PI * (float) (j - 1) / longs;
			float x = cos(lng);
			float y = sin(lng);

			add( 
				osg::Vec3(x*zr0*fRadius, z0*fRadius, y*zr0*fRadius), 
				osg::Vec3(x*zr0, z0, y*zr0) 
			);

			add( 
				osg::Vec3(x*zr1*fRadius, z1*fRadius, y*zr1*fRadius), 
				osg::Vec3( x*zr1, z1, y*zr1 )
			);
		}
	}

	addPrimitiveSet( new osg::DrawArrays(osg::PrimitiveSet::QUAD_STRIP, 0, m_verts->size() ) );

	CSulGeom::createDrawable();
}