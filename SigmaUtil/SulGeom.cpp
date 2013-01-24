// SulGeom.cpp

#include "stdafx.h"
#include "SulGeom.h"

CSulGeom::CSulGeom( const osg::Vec4& color ) :
m_color( color )
{
	m_colors = new osg::Vec4Array;
	m_verts = new osg::Vec3Array;
	m_normals = new osg::Vec3Array;

	setVertexArray( m_verts );
	
	m_texCoords = new osg::Vec2Array;
	//setTexCoordArray( 0, m_texCoords );

	//setNormalBinding( osg::Geometry::BIND_PER_VERTEX );	// note: this will crash the render if no normals are given and you use this method
}

void CSulGeom::useColorArray()
{
	setColorArray( m_colors );
}

void CSulGeom::useNormalArray()
{
	setNormalArray( m_normals );
}

void CSulGeom::useTextureCoordArray()
{
	setTexCoordArray( 0, m_texCoords );
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

void CSulGeom::addV( const osg::Vec3& v )
{
	m_verts->push_back( v );
}

void CSulGeom::addVN( const osg::Vec3& v, const osg::Vec3& n )
{
	m_verts->push_back( v );
	m_normals->push_back( n );
}

void CSulGeom::addVC( const osg::Vec3& v, const osg::Vec4& c )
{
	m_verts->push_back( v );
	m_colors->push_back( c );
}

void CSulGeom::addVNC( const osg::Vec3& v, const osg::Vec3& n, const osg::Vec4& c )
{
	m_verts->push_back( v );
	m_normals->push_back( n );
	m_colors->push_back( c );
}

void CSulGeom::addVNT( const osg::Vec3& v, const osg::Vec3& n, const osg::Vec2& t )
{
	m_verts->push_back( v );
	m_normals->push_back( n );
	m_texCoords->push_back( t );
}

sigma::uint32 CSulGeom::getNumVertices()
{
	return m_verts->size();
}

void CSulGeom::createDrawable()
{
}

void CSulGeom::transformVerts( const osg::Matrix& m )
{
	osg::Vec3Array::iterator i = m_verts->begin();
	osg::Vec3Array::iterator ie = m_verts->end();

	while ( i!=ie )
	{
		(*i) = (*i)*m;
		++i;
	}
}

void CSulGeom::transformNormals( const osg::Matrix& m )
{
	osg::Vec3Array::iterator i = m_normals->begin();
	osg::Vec3Array::iterator ie = m_normals->end();

	while ( i!=ie )
	{
		(*i) = (*i)*m;
		++i;
	}
}

void CSulGeom::applyMatrix( const osg::Matrix& m )
{
	transformVerts( m );
	transformNormals( m );
}