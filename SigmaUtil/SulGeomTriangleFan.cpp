// SulGeomTriangleFan.cpp

#include "stdafx.h"
#include "SulGeomTriangleFan.h"
#include <osg/geometry>

CSulGeomTriangleFan::CSulGeomTriangleFan( const sigma::VEC_VEC3& vecVector ) :
osg::Geode(),
m_vecVector( vecVector )
{
	createDrawable();
}

void CSulGeomTriangleFan::createDrawable()
{
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
			GL_TRIANGLE_FAN,
            indices->size(), 
			&(indices->front())
		)
	);

}

