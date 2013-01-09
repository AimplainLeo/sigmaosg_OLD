// SulGeomCone.cpp

#include "stdafx.h"
#include "SulGeomCone.h"
#include <osg/cullface>

CSulGeomCone::CSulGeomCone( float len, float radiusBottom, float radiusTop, sigma::uint32 slices, sigma::uint32 stacks ) :
CSulGeode(),
m_len(len),
m_radiusBottom( radiusBottom ),
m_radiusTop( radiusTop ),
m_slices( slices ),
m_colorTop( 1,1,1,1 ),
m_colorBottom( 1,1,1,1 )
{
	createDrawable();

	osg::CullFace* cull = new osg::CullFace;
	cull->setMode( osg::CullFace::FRONT );
	getOrCreateStateSet()->setAttributeAndModes( cull, osg::StateAttribute::ON );
}

void CSulGeomCone::setColor( const osg::Vec4& color )
{
	setColorBottom( color );
	setColorTop( color );
}

void CSulGeomCone::setColorBottom( const osg::Vec4& color )
{
	m_colorBottom = color;
	(*m_colors)[0] = color;
	m_rGeo->dirtyDisplayList();
}

void CSulGeomCone::setColorTop( const osg::Vec4& color )
{
	m_colorTop = color;
	(*m_colors)[1] = color;
	m_rGeo->dirtyDisplayList();
}

void CSulGeomCone::setLength( float len )
{
	m_len = len;
}

void CSulGeomCone::setRadiusBottom( float radius )
{
	m_radiusBottom = radius;
}

void CSulGeomCone::setRadiusTop( float radius )
{
	m_radiusTop = radius;
}

void CSulGeomCone::rebuildDrawable()
{
	removeDrawables( 0, 1 );
	createDrawable();
}

void CSulGeomCone::createDrawable()
{
	m_rGeo = new osg::Geometry;
	addDrawable( m_rGeo );

	// colors
	m_colors = new osg::Vec4Array;
	m_colors->push_back( m_colorBottom );
	m_colors->push_back( m_colorTop );

	// color indices
	osg::UByteArray* colorIndices = new osg::UByteArray();

	// normals
	osg::Vec3Array* normals = new osg::Vec3Array;

	// vertices
	osg::ref_ptr<osg::Vec3Array> m_rVerts = new osg::Vec3Array;
	for ( sigma::uint32 i=0; i<m_slices; i++ )
	{
		double d = ((2.0*osg::PI)/(m_slices-1)) * (double)i;

		double x = cos( d );
		double y = sin( d );

		// vertice calculations
		osg::Vec3 posBottom = osg::Vec3( x*m_radiusBottom, y*m_radiusBottom, 0 );
		osg::Vec3 posTop = osg::Vec3( x*m_radiusTop, y*m_radiusTop, m_len );
		m_rVerts->push_back( posBottom );
		m_rVerts->push_back( posTop );

		// normal calculations
		osg::Vec3 v = posTop-posBottom;
		osg::Vec3 left = v ^ posBottom;
		osg::Vec3 n = left ^ v;
		n.normalize();
		normals->push_back( n ); 
		normals->push_back( n ); 

		// color calculations
		colorIndices->push_back( 1 );
		colorIndices->push_back( 0 );
	}

	m_rGeo->setNormalArray( normals );
	m_rGeo->setNormalBinding( osg::Geometry::BIND_PER_VERTEX );

	m_rGeo->setColorIndices( colorIndices );
	m_rGeo->setColorArray( m_colors );
	m_rGeo->setColorBinding( osg::Geometry::BIND_PER_VERTEX );

	m_rGeo->setVertexArray( m_rVerts );

	// primitive
	m_rGeo->addPrimitiveSet( new osg::DrawArrays(osg::PrimitiveSet::QUAD_STRIP, 0, colorIndices->size() ) );
}