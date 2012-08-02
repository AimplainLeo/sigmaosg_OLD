// SulGeomLineStrip.cpp

#include "stdafx.h"
#include "SulGeomLineStrip.h"

CSulGeomLineStrip::CSulGeomLineStrip( const osg::Vec4& color ) :
CSulGeom(),
m_color( color )
{
	initConstructor();
	createDrawable();
}

CSulGeomLineStrip::CSulGeomLineStrip( const sigma::VEC_VEC3& vecVector ) :
CSulGeom()
{
	initConstructor();
	createDrawable();
}

void CSulGeomLineStrip::initConstructor()
{
	m_width = 1.0f;

	m_rLineWidth = new osg::LineWidth();
	m_rLineWidth->setWidth( m_width );
	getOrCreateStateSet()->setAttributeAndModes( m_rLineWidth, osg::StateAttribute::ON );
}

void CSulGeomLineStrip::createDrawable()
{
	// we must have atleast two vertices
	if ( m_vecVector.size()<1 )
		return;

	removeAllPrimitiveSets();

	osg::Vec3Array* verts = new osg::Vec3Array;
	osg::ref_ptr<osg::UIntArray> indices = new osg::UIntArray;	
	m_rColors = new osg::Vec4Array;

	sigma::uint32 count = 0;
	sigma::VEC_VEC3::const_iterator i;
	i = m_vecVector.begin();
	while ( i!=m_vecVector.end() )
	{
		verts->push_back( (*i) );
		indices->push_back( count++ );
		m_rColors->push_back( m_color );

		++i;
	}

	setVertexArray( verts );
    setColorArray( m_rColors );
	setColorBinding( osg::Geometry::BIND_PER_VERTEX );

    addPrimitiveSet(
        new osg::DrawElementsUInt( 
			GL_LINE_STRIP,
            indices->size(), 
			&(indices->front())
		)
	);
}

void CSulGeomLineStrip::setLines( const sigma::VEC_VEC3& vecVector )
{
	m_vecVector = vecVector;
}

void CSulGeomLineStrip::setWidth( float width )
{
	m_width = width;
	m_rLineWidth->setWidth( width );
}
