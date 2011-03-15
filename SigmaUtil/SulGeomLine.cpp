// SulGeomLine.cpp

#include "stdafx.h"
#include "SulGeomLine.h"
#include <osg/geometry>

CSulGeomLine::CSulGeomLine( const osg::LineSegment& line ) :
osg::Geode(),
m_v0( line.start() ),
m_v1( line.end() )
{
	createDrawable();
}

CSulGeomLine::CSulGeomLine( const osg::Vec3& v0, const osg::Vec3& v1 ) :
osg::Geode(),
m_v0(v0),
m_v1(v1)
{
	createDrawable();
}

void CSulGeomLine::createDrawable()
{
	m_rGeo = new osg::Geometry;
	addDrawable( m_rGeo );

	// vertices
	osg::Vec3Array* verts = new osg::Vec3Array;
	verts->push_back( m_v0 );
	verts->push_back( m_v1 );
	m_rGeo->setVertexArray( verts );

    osg::ref_ptr<osg::UIntArray> indices = new osg::UIntArray;
    indices->push_back(0);
    indices->push_back(1);

	m_rColors = new osg::Vec4Array;
	m_rColors->push_back( osg::Vec4(1,1,0,1.0f) );
	m_rColors->push_back( osg::Vec4(1,1,0,1.0f) );
    m_rGeo->setColorArray( m_rColors );
	m_rGeo->setColorBinding( osg::Geometry::BIND_PER_VERTEX );

    m_rGeo->addPrimitiveSet(
        new osg::DrawElementsUInt( 
			GL_LINES,
            indices->size(), 
			&(indices->front())
		)
	);

	m_rLineWidth = new osg::LineWidth();
	m_rLineWidth->setWidth( 1.0f );
	m_rGeo->getOrCreateStateSet()->setAttributeAndModes( m_rLineWidth, osg::StateAttribute::ON );
}

void CSulGeomLine::setWidth( float width )
{
	m_rLineWidth->setWidth( width );
}

void CSulGeomLine::setColor( const osg::Vec4& color )
{
	(*m_rColors)[0].set( color.r(),color.g(),color.b(),color.a() );
	(*m_rColors)[1].set( color.r(),color.g(),color.b(),color.a() );

	m_rGeo->dirtyDisplayList();
}
