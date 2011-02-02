// SulGeomTriangleFan.cpp

#include "stdafx.h"
#include "SulGeomTriangleFan.h"

CSulGeomTriangleFan::CSulGeomTriangleFan( const sigma::VEC_VEC3& vecVector ) :
CSulGeomBase(),
m_vecVector( vecVector )
{
}

void CSulGeomTriangleFan::createDrawable()
{
	addDrawable( new osg::Geometry );

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

	getDrawable()->asGeometry()->setVertexArray( verts );
    getDrawable()->asGeometry()->setColorArray( m_rColors );
	getDrawable()->asGeometry()->setColorBinding( osg::Geometry::BIND_PER_VERTEX );

    getDrawable()->asGeometry()->addPrimitiveSet(
        new osg::DrawElementsUInt( 
			GL_TRIANGLE_FAN,
            indices->size(), 
			&(indices->front())
		)
	);

}

