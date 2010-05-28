// SulGeomBillboardInstancing.cpp

#include "stdafx.h"
#include "SulGeomBillboardInstancing.h"

CSulGeomBillboardInstancing::CSulGeomBillboardInstancing( Sigma::uint32 numInstances, float sizeMultiplier, float min, float max ) :
CSulGeomBase(),
m_numInstances( numInstances ),
m_sizeMultiplier( sizeMultiplier )
{
}

void CSulGeomBillboardInstancing::createDrawable()
{
	//CSulGeomBase::createGeometry();
	setDrawable( new osg::Geometry );

	const float halfDimX( 0.5f*m_sizeMultiplier );
    const float halfDimZ( 0.5f*m_sizeMultiplier );

	osg::Geometry* pGeo = getDrawable()->asGeometry();
	pGeo->setUseDisplayList( false );
    pGeo->setUseVertexBufferObjects( true );

    osg::Vec3Array* v = new osg::Vec3Array;
    v->resize( 4 );
    pGeo->setVertexArray( v );

    // Geometry for a single quad.
    (*v)[ 0 ] = osg::Vec3( -halfDimX, 0., 0. );
    (*v)[ 1 ] = osg::Vec3( halfDimX, 0., 0. );
    (*v)[ 2 ] = osg::Vec3( halfDimX, 0., halfDimZ*2.0f );
    (*v)[ 3 ] = osg::Vec3( -halfDimX, 0., halfDimZ*2.0f );

	// create color array data (each corner of our triangle will have one color component)
    osg::Vec4Array* pColors = new osg::Vec4Array;
    pColors->push_back( osg::Vec4( 1.0f, 0.0f, 0.0f, 1.0f ) );
    pColors->push_back( osg::Vec4( 0.0f, 1.0f, 0.0f, 1.0f ) );
    pColors->push_back( osg::Vec4( 0.0f, 0.0f, 1.0f, 1.0f ) );
	pColors->push_back( osg::Vec4( 0.0f, 0.0f, 1.0f, 1.0f ) );
    pGeo->setColorArray( pColors );

	// make sure that our geometry is using one color per vertex
    pGeo->setColorBinding( osg::Geometry::BIND_PER_VERTEX );

    osg::Vec2Array* pTexCoords = new osg::Vec2Array( 4 );
    (*pTexCoords)[0].set( 0.0f, 0.0f );
    (*pTexCoords)[1].set( 1.0f, 0.0f );
    (*pTexCoords)[2].set( 1.0f, 1.0f );
    (*pTexCoords)[3].set( 0.0f, 1.0f );
    pGeo->setTexCoordArray( 0, pTexCoords );

    // Use the DrawArraysInstanced PrimitiveSet
    pGeo->addPrimitiveSet( new osg::DrawArrays( GL_QUADS, 0, 4, m_numInstances ) );
}