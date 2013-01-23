// SulGeomHemiSphere.cpp

#include "stdafx.h"
#include "SulGeomHemiSphere.h"

CSulGeomHemiSphere::CSulGeomHemiSphere( const osg::Vec3& offset, float radius, sigma::uint32 sides, sigma::uint32 segments ) :
CSulGeom()
{
	m_offset = offset;
	m_radius = radius;
	m_sides = sides;
	m_segments = segments;
	createDrawable();
	setNormalBinding( osg::Geometry::BIND_PER_VERTEX );
}

CSulGeomHemiSphere::CSulGeomHemiSphere( float radius, sigma::uint32 sides, sigma::uint32 segments ) :
CSulGeom()
{
	m_radius = radius;
	m_sides = sides;
	m_segments = segments;
	createDrawable();
	setNormalBinding( osg::Geometry::BIND_PER_VERTEX );
}


#include <osg/texgen>
#include <osg/TexEnv>

void CSulGeomHemiSphere::createDrawable()
{
	useColorArray();
	useNormalArray();
	useTextureCoordArray();
	
	sigma::uint32 i;
	sigma::uint32 s;
	float r = m_radius;

	float radSeg = (osg::PI_2)/(float)(m_segments);
	float rad = (2.0f*osg::PI)/(float)(m_sides);

	float test = 0.0f;

	for ( s=0; s<m_segments; s++ )
	{
		for ( i=0; i<=m_sides; i++ )
		{
			// this will ensure we start and end with the same vertices
			sigma::uint32 ii = i<m_sides?i:0;
			float z0 = cos( radSeg*s );
			float x0 = cos( rad*ii ) * sin( radSeg*s );
			float y0 = sin( rad*ii ) * sin( radSeg*s );

			float z1 = cos( radSeg*(s+1) );
			float x1 = cos( rad*ii ) * sin( radSeg*(s+1) );
			float y1 = sin( rad*ii ) * sin( radSeg*(s+1) );

			// sphere texture mapping
			float t0 = asin(x0)/osg::PI + 0.5;
			float s0 = asin(y0)/osg::PI + 0.5;
			float t1 = asin(x1)/osg::PI + 0.5;
			float s1 = asin(y1)/osg::PI + 0.5;

			addVNT( 
				osg::Vec3(x0*r, y0*r, z0*r - s*test)+m_offset, 
				osg::Vec3(x0, y0, z0),
				osg::Vec2(s0,t0)
			);

			addVNT( 
				osg::Vec3(x1*r, y1*r, z1*r - s*test )+m_offset, 
				osg::Vec3(x1, y1, z1),
				osg::Vec2(s1,t1)
			);

		}

		sigma::uint32 count = 2+m_sides*2;
		sigma::uint32 start = s*(count);
		addPrimitiveSet( new osg::DrawArrays(osg::PrimitiveSet::QUAD_STRIP, start, count ) );
	}

	/*
	osg::TexGen* texgen = new osg::TexGen;
	texgen->setMode(osg::TexGen::SPHERE_MAP);
	getOrCreateStateSet()->setTextureAttributeAndModes(0,texgen,osg::StateAttribute::ON);
	*/

	/*
 osg::TexEnv* texenv = new osg::TexEnv;
   texenv->setMode( osg::TexEnv::DECAL );
   texenv->setMode( osg::TexEnv::BLEND );
   texenv->setColor( osg::Vec4(0.6f,0.6f,0.6f,0.2f) );
   getOrCreateStateSet()->setTextureAttribute( 0, texenv );
   */
}
