// SulGeom.cpp

#include "stdafx.h"
#include "SulGeom.h"

CSulGeom::CSulGeom( const osg::Vec4& color ) :
m_color( color )
{
	m_colors = new osg::Vec4Array;
	m_verts = new osg::Vec3Array;

	setColorArray( m_colors );
	setVertexArray( m_verts );
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

void CSulGeom::createDrawable()
{
}