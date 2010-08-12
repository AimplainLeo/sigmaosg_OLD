// SulGeomTriangleList.cpp

#include "stdafx.h"
#include "SulGeomTriangleList.h"

CSulGeomTriangleList::CSulGeomTriangleList( const sigma::VEC_TRI& vecTri ) :
CSulGeomBase(),
m_vecTri( vecTri )
{
}

void CSulGeomTriangleList::createDrawable()
{
	setDrawable( new osg::Geometry );

	osg::Vec3Array *verts = new osg::Vec3Array;
    osg::ref_ptr<osg::UIntArray> indices = new osg::UIntArray;
	m_rColors = new osg::Vec4Array;

	float c = 1.0f;
	sigma::uint32 count = 0;
	sigma::VEC_TRI::iterator i;	// FIXME: bet there is a good cpy stl thingy that can do this
	i = m_vecTri.begin();
	while ( i!=m_vecTri.end() )
	{
		verts->push_back( i->m_p0 );
		verts->push_back( i->m_p1 );
		verts->push_back( i->m_p2 );

		indices->push_back( count++ );
		indices->push_back( count++ );
		indices->push_back( count++ );

		m_rColors->push_back( osg::Vec4(c,c,c,1.0f) );
		m_rColors->push_back( osg::Vec4(c,c,c,1.0f) );
		m_rColors->push_back( osg::Vec4(c,c,c,1.0f) );

		++i;
	}
	getDrawable()->asGeometry()->setVertexArray( verts );

    getDrawable()->asGeometry()->setColorArray( m_rColors.get() );
	getDrawable()->asGeometry()->setColorBinding( osg::Geometry::BIND_PER_VERTEX );

    getDrawable()->asGeometry()->addPrimitiveSet(
        new osg::DrawElementsUInt( 
			GL_TRIANGLES,
            indices->size(), 
			&(indices->front())
		)
	);
}

void CSulGeomTriangleList::setColor( const osg::Vec4& color )
{
	setColor( color.r(), color.g(), color.b(), color.a() );
}

void CSulGeomTriangleList::setColor( float r, float g, float b, float a )
{
	if ( !m_rColors.valid() )
	{
		createDrawable();
	}

	sigma::uint32 count = m_vecTri.size()*3;
	for ( sigma::uint32 i=0; i<count; i++  )
	{
		(*m_rColors)[i].set( r, g, b, a );
	}

	getDrawable()->asGeometry()->dirtyDisplayList();
}