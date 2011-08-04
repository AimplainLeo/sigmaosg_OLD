// SulGeomCone.cpp

#include "stdafx.h"
#include "SulGeomCone.h"

CSulGeomCone::CSulGeomCone( float len, float radiusBottom, float radiusTop, sigma::uint32 slices, sigma::uint32 stacks ) :
m_len(len),
m_radiusBottom( radiusBottom ),
m_radiusTop( radiusTop ),
m_slices( slices )
{
	createDrawable();
}

void CSulGeomCone::createDrawable()
{
	m_rGeo = new osg::Geometry;
	addDrawable( m_rGeo );

	// vertices
	osg::ref_ptr<osg::Vec3Array> m_rVerts = new osg::Vec3Array;
	for ( sigma::uint32 i=0; i<m_slices; i++ )
	{
		double d = ((2.0*osg::PI)/(m_slices-1)) * (double)i;

		double x = cos( d );
		double y = sin( d );

		osg::Vec3 posBottom = osg::Vec3( x*m_radiusBottom, y*m_radiusBottom, 0 );
		osg::Vec3 posTop = osg::Vec3( x*m_radiusTop, y*m_radiusTop, m_len );
		
		m_rVerts->push_back( posBottom );
		m_rVerts->push_back( posTop );
	}

	// coords indices
	osg::UByteArray* pCoordIndices = new osg::UByteArray;
	for ( sigma::uint32 i=0; i<m_slices*2; i++ )	
		pCoordIndices->push_back( i );

	// colors
	osg::ref_ptr<osg::Vec4Array> m_colors = new osg::Vec4Array;
	m_colors->push_back(osg::Vec4(1.0f,1.0f,1.0f,1.0f));

	// color indices
	osg::UByteArray* colorIndices = new osg::UByteArray();
	for ( sigma::uint32 i=0; i<m_slices*2; i++ )	
		pCoordIndices->push_back( 0 );

	// normals
	osg::Vec3Array* normals = new osg::Vec3Array;
	normals->push_back(osg::Vec3(0.0f,0.0f,1.0f)); // set up a single normal for the plane

	m_rGeo->setColorIndices( colorIndices );
	m_rGeo->setColorArray( m_colors );
	m_rGeo->setVertexArray( m_rVerts );
	m_rGeo->setNormalArray( normals );
	m_rGeo->setNormalBinding( osg::Geometry::BIND_OVERALL );


	// primitive
	/*
    osg::DrawElementsUInt* pPrimitiveSet = new osg::DrawElementsUInt( osg::PrimitiveSet::QUAD_STRIP, 0 );
    pPrimitiveSet->push_back( 3 );
    pPrimitiveSet->push_back( 2 );
    pPrimitiveSet->push_back( 1 );
    pPrimitiveSet->push_back( 0 );
    m_rGeo->addPrimitiveSet( pPrimitiveSet );
	*/

	m_rGeo->addPrimitiveSet( new osg::DrawArrays(osg::PrimitiveSet::QUAD_STRIP, 0, pCoordIndices->size()/2.0 ) );

}