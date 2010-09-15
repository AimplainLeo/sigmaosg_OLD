// SulGeomLineList.cpp

#include "stdafx.h"
#include "SulGeomLineList.h"

CSulGeomLineList::CSulGeomLineList( const sigma::VEC_LINESEGMENT& vecLines ) :
CSulGeomBase(),
m_vecLines( vecLines )
{
}

void CSulGeomLineList::createDrawable()
{
	setDrawable( new osg::Geometry );

	osg::Vec3Array* verts = new osg::Vec3Array;
	osg::ref_ptr<osg::UIntArray> indices = new osg::UIntArray;	
	m_rColors = new osg::Vec4Array;

	sigma::uint32 count = 0;
	sigma::VEC_LINESEGMENT::const_iterator i;
	i = m_vecLines.begin();
	while ( i!=m_vecLines.end() )
	{
		verts->push_back( (*i)->start() );
		verts->push_back( (*i)->end() );

		indices->push_back( count++ );
		indices->push_back( count++ );

		m_rColors->push_back( osg::Vec4(1,1,0,1.0f) );
		m_rColors->push_back( osg::Vec4(1,1,0,1.0f) );

		++i;
	}

	getDrawable()->asGeometry()->setVertexArray( verts );
    getDrawable()->asGeometry()->setColorArray( m_rColors );
	getDrawable()->asGeometry()->setColorBinding( osg::Geometry::BIND_PER_VERTEX );

    getDrawable()->asGeometry()->addPrimitiveSet(
        new osg::DrawElementsUInt( 
			GL_LINES,
            indices->size(), 
			&(indices->front())
		)
	);
}

void CSulGeomLineList::setColor( const osg::Vec4& color )
{
	setColor( color.r(), color.g(), color.b(), color.a() );
}

void CSulGeomLineList::setColor( float r, float g, float b, float a )
{
	if ( !m_rColors.valid() )
	{
		createDrawable();
	}

	sigma::uint32 count = m_vecLines.size()*2;
	for ( sigma::uint32 i=0; i<count; i++  )
	{
		(*m_rColors)[i].set( r, g, b, a );
	}

	getDrawable()->asGeometry()->dirtyDisplayList();
}