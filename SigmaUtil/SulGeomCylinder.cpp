// SulGeomCylinder.cpp

#include "stdafx.h"
#include "SulGeomCylinder.h"

CSulGeomCylinder::CSulGeomCylinder( float radius, float length, sigma::uint32 segments ) :
CSulGeom()
{
	// setup geometry data definitions
	useColorArray();
	useNormalArray();
	setNormalBinding( osg::Geometry::BIND_PER_VERTEX );

	m_radius = radius;
	m_length = length;
	m_color = osg::Vec4( 1,0,0,1);
	m_segments = segments;
	createDrawable();
}

void CSulGeomCylinder::createDrawable()
{
	CSulGeom::createDrawable();

	removeAllPrimitiveSets();

	for ( sigma::uint32 i=0; i<=m_segments; i++ )
	{
		// make sure we start and end at the smae position
		sigma::uint32 ii = i<m_segments?i:0;

		double d = ((2.0*osg::PI)/(m_segments)) * (double)ii;
		double x = cos( d );
		double y = sin( d );

		addVNC( 
			osg::Vec3( x*m_radius, y*m_radius, 0.0f ),
			osg::Vec3( x, y, 0.0f ),
			osg::Vec4( m_color )
		);

		addVNC( 
			osg::Vec3( x*m_radius, y*m_radius, m_length ),
			osg::Vec3( x, y, 0.0f ),
			osg::Vec4( m_color )
		);
	}

	sigma::uint32 count = 2+m_segments*2;
	addPrimitiveSet( new osg::DrawArrays(osg::PrimitiveSet::QUAD_STRIP, 0, count ) );
}