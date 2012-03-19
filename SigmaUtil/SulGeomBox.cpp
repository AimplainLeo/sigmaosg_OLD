// SulGeomBox.cpp

#include "stdafx.h"
#include "SulGeomBox.h"
#include <osg/geometry>


CSulGeomBox::CSulGeomBox( float extent ) :
CSulGeomGeode()
{
	m_minX = -extent;
	m_maxX =  extent;
	m_minY = -extent;
	m_maxY =  extent;
	m_minZ = -extent;
	m_maxZ =  extent;

	createDrawable();
}

CSulGeomBox::CSulGeomBox( float extentX, float extentY, float extentZ ) :
CSulGeomGeode()
{
	m_minX = -extentX;
	m_maxX =  extentX;
	m_minY = -extentY;
	m_maxY =  extentY;
	m_minZ = -extentZ;
	m_maxZ =  extentZ;

	createDrawable();
}

CSulGeomBox::CSulGeomBox( float minX, float maxX, float minY, float maxY, float minZ, float maxZ ) :
CSulGeomGeode()
{
	m_minX = minX;
	m_maxX = maxX;
	m_minY = minY;
	m_maxY = maxY;
	m_minZ = minZ;
	m_maxZ = maxZ;

	createDrawable();
}

CSulGeomBox::CSulGeomBox( const osg::BoundingBox& bb ) :
CSulGeomGeode()
{
	m_minX = bb.xMin();
	m_maxX = bb.xMax();
	m_minY = bb.yMin();
	m_maxY = bb.yMax();
	m_minZ = bb.zMin();
	m_maxZ = bb.zMax();

	createDrawable();
}

void CSulGeomBox::createDrawable()
{
	osg::Geometry* pGeo = new osg::Geometry;
	addDrawable( pGeo );

	// vertices
	osg::Vec3Array *verts = new osg::Vec3Array;
	verts->push_back(osg::Vec3( m_minX, m_minY,  m_maxZ ));
	verts->push_back(osg::Vec3( m_minX, m_maxY,  m_maxZ));
	verts->push_back(osg::Vec3( m_maxX, m_maxY,  m_maxZ));
	verts->push_back(osg::Vec3( m_maxX, m_minY,  m_maxZ));
	verts->push_back(osg::Vec3( m_minX, m_minY,  m_minZ));
	verts->push_back(osg::Vec3( m_minX, m_maxY,  m_minZ));
	verts->push_back(osg::Vec3( m_maxX, m_maxY,  m_minZ));
	verts->push_back(osg::Vec3( m_maxX, m_minY,  m_minZ));
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