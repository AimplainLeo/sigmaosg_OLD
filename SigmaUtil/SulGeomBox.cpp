// SulGeomBox.cpp

#include "stdafx.h"
#include "SulGeomBox.h"
#include <osg/geometry>


CSulGeomBox::CSulGeomBox( float extent ) :
CSulGeomGeode(),
m_extentX( extent ),
m_extentY( extent ),
m_extentZ( extent )
{
	createDrawable();
}

CSulGeomBox::CSulGeomBox( float extentX, float extentY, float extentZ ) :
CSulGeomGeode(),
m_extentX( extentX ),
m_extentY( extentY ),
m_extentZ( extentZ )
{
	createDrawable();
}

void CSulGeomBox::createDrawable()
{
	osg::Geometry* pGeo = new osg::Geometry;
	addDrawable( pGeo );

	// vertices
	osg::Vec3Array *verts = new osg::Vec3Array;
	verts->push_back(osg::Vec3(-m_extentX, -m_extentY,  m_extentZ));
	verts->push_back(osg::Vec3(-m_extentX,  m_extentY,  m_extentZ));
	verts->push_back(osg::Vec3( m_extentX,  m_extentY,  m_extentZ));
	verts->push_back(osg::Vec3( m_extentX, -m_extentY,  m_extentZ));
	verts->push_back(osg::Vec3(-m_extentX, -m_extentY, -m_extentZ));
	verts->push_back(osg::Vec3(-m_extentX,  m_extentY, -m_extentZ));
	verts->push_back(osg::Vec3( m_extentX,  m_extentY, -m_extentZ));
	verts->push_back(osg::Vec3( m_extentX, -m_extentY, -m_extentZ));
	pGeo->setVertexArray( verts );

    // texture coordinates
    osg::Vec3Array *coords = new osg::Vec3Array;
    coords->push_back(osg::Vec3(-1,  1, -1));
    coords->push_back(osg::Vec3(-1, -1, -1));
    coords->push_back(osg::Vec3( 1, -1, -1));
    coords->push_back(osg::Vec3( 1,  1, -1));
    coords->push_back(osg::Vec3(-1,  1,  1));
    coords->push_back(osg::Vec3(-1, -1,  1));
    coords->push_back(osg::Vec3( 1, -1,  1));
    coords->push_back(osg::Vec3( 1,  1,  1));
	pGeo->setTexCoordArray( 0, coords );

    osg::ref_ptr<osg::UIntArray> indices = new osg::UIntArray;

    // Front face
    indices->push_back(0);
    indices->push_back(1);
    indices->push_back(2);
    indices->push_back(3);

    // Back face
    indices->push_back(4);
    indices->push_back(5);
    indices->push_back(6);
    indices->push_back(7);

    // Right face
    indices->push_back(6);
    indices->push_back(7);
    indices->push_back(3);
    indices->push_back(2);

    // Left face
    indices->push_back(0);
    indices->push_back(1);
    indices->push_back(5);
    indices->push_back(4);

    // Top face
    indices->push_back(1);
    indices->push_back(2);
    indices->push_back(6);
    indices->push_back(5);

    // Bottom face
    indices->push_back(0);
    indices->push_back(3);
    indices->push_back(7);
    indices->push_back(4);

    pGeo->addPrimitiveSet(
        new osg::DrawElementsUInt(GL_QUADS,
            indices->size(), &(indices->front())));
}
