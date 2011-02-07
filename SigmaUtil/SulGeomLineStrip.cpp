// SulGeomLineStrip.cpp

#include "stdafx.h"
#include "SulGeomLineStrip.h"
#include <osg/geometry>

CSulGeomLineStrip::CSulGeomLineStrip() :
osg::Geode()
{
	initConstructor();
	createDrawable();
}

CSulGeomLineStrip::CSulGeomLineStrip( const sigma::VEC_VEC3& vecVector ) :
osg::Geode(),
m_vecVector( vecVector )
{
	initConstructor();
	createDrawable();
}

void CSulGeomLineStrip::initConstructor()
{
	m_width = 1.0f;
}

void CSulGeomLineStrip::createDrawable()
{
	// we must have atleast two vertices
	if ( m_vecVector.size()<1 )
		return;

	// remove any drawable that might have been created
	if ( getNumDrawables()>0 )
		removeDrawables( 0, getNumDrawables() );

	osg::Geometry* pGeo = new osg::Geometry;
	addDrawable( pGeo );

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
		m_rColors->push_back( osg::Vec4(1,1,1,1.0f) );

		++i;
	}

	pGeo->setVertexArray( verts );
    pGeo->setColorArray( m_rColors );
	pGeo->setColorBinding( osg::Geometry::BIND_PER_VERTEX );

    pGeo->addPrimitiveSet(
        new osg::DrawElementsUInt( 
			GL_LINE_STRIP,
            indices->size(), 
			&(indices->front())
		)
	);

	m_rLineWidth = new osg::LineWidth();
	m_rLineWidth->setWidth( m_width );
	getOrCreateStateSet()->setAttributeAndModes( m_rLineWidth, osg::StateAttribute::ON );
}

void CSulGeomLineStrip::setLines( const sigma::VEC_VEC3& vecVector )
{
	m_vecVector = vecVector;
	createDrawable();
}

void CSulGeomLineStrip::setWidth( float width )
{
	m_width = width;
	m_rLineWidth->setWidth( width );
}
