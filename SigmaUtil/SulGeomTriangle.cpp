// SulGeomTriangle.cpp

#include "stdafx.h"
#include "SulGeomTriangle.h"
#include <osg/geometry>

CSulGeomTriangle::CSulGeomTriangle( float d ) :
osg::Geode(),
m_type( CSulGeomTriangle::EQUILATERAL ),
m_distance( d )
{
}

CSulGeomTriangle::CSulGeomTriangle( const osg::Vec3& p0, const osg::Vec3& p1, const osg::Vec3& p2 ) :
osg::Geode(),
m_type( CSulGeomTriangle::ARBITRARY ),
m_p0(p0),
m_p1(p1),
m_p2(p2)
{
	createDrawable();
}

void CSulGeomTriangle::createDrawable()
{
	m_rGeo = new osg::Geometry;
	addDrawable( m_rGeo );

	switch ( m_type )
	{
		case EQUILATERAL:
			{
				// equilateral triangle angle from center
				float a = (2.0f*osg::PI)/3.0f;
				float s = osg::PI+osg::PI_2;

				m_p0 = osg::Vec3( cos(s), sin(s), 0.0f );
				m_p1 = osg::Vec3( cos(s+a), sin(s+a), 0.0f );
				m_p2 = osg::Vec3( cos(s+a+a), sin(s+a+a), 0.0f );
			}
			break;

		case ARBITRARY:
			{
			}
			break;
	}

	// vertices
	m_verts = new osg::Vec3Array;
	m_verts->push_back( m_p0 );
	m_verts->push_back( m_p1 );
	m_verts->push_back( m_p2 );
	m_rGeo->setVertexArray( m_verts );

    osg::ref_ptr<osg::UIntArray> indices = new osg::UIntArray;
    indices->push_back(0);
    indices->push_back(1);
    indices->push_back(2);

	m_rColors = new osg::Vec4Array;
	float f = 1.0f;
	m_rColors->push_back( osg::Vec4(f,f,f,1.0f) );
	m_rColors->push_back( osg::Vec4(f,f,f,1.0f) );
	m_rColors->push_back( osg::Vec4(f,f,f,1.0f) );
    m_rGeo->setColorArray( m_rColors.get() );
	m_rGeo->setColorBinding( osg::Geometry::BIND_PER_VERTEX );

    m_rGeo->addPrimitiveSet(
        new osg::DrawElementsUInt( 
			GL_TRIANGLES,
            indices->size(), 
			&(indices->front())
		)
	);

}

void CSulGeomTriangle::setColor( float r, float g, float b, float a )
{
	if ( !m_rColors.valid() )
	{
		createDrawable();
	}

	(*m_rColors)[0].set( r, g, b, a );
	(*m_rColors)[1].set( r, g, b, a );
	(*m_rColors)[2].set( r, g, b, a );
	m_rGeo->dirtyDisplayList();
}

void CSulGeomTriangle::setVertices( const osg::Vec3& p0, const osg::Vec3& p1, const osg::Vec3& p2 )
{
	if ( !m_verts.valid() )
	{
		createDrawable();
	}

	(*m_verts)[0] = p0;
	(*m_verts)[1] = p1;
	(*m_verts)[2] = p2;
	m_rGeo->dirtyDisplayList();
}

const CSulDataTri	CSulGeomTriangle::getTriangle() const
{
	CSulDataTri tri( m_p0, m_p1, m_p2 );
	return tri;
}