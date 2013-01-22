// SulGeom.cpp

#include "stdafx.h"
#include "SulGeom.h"

CSulGeom::CSulGeom( const osg::Vec4& color ) :
m_color( color )
{
	m_colors = new osg::Vec4Array;
	m_verts = new osg::Vec3Array;
	m_normals = new osg::Vec3Array;

	setColorArray( m_colors );
	setVertexArray( m_verts );
	setNormalArray( m_normals );

	setNormalBinding( osg::Geometry::BIND_PER_VERTEX );
}

void CSulGeom::setColor( const osg::Vec4& color )
{
	m_color = color;
}

void CSulGeom::setColor( float r, float g, float b, float a )
{
	m_color.set( r, g, b, a );
}

void CSulGeom::removeAllPrimitiveSets()
{
	removePrimitiveSet( 0, getNumPrimitiveSets() );
}

void CSulGeom::add( const osg::Vec3& v, const osg::Vec3& n )
{
	m_verts->push_back( v );
	m_normals->push_back( n );
}

void CSulGeom::createDrawable()
{
}