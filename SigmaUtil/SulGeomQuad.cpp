// SulGeomQuad.cpp

#include "stdafx.h"
#include "SulGeomQuad.h"
#include <osgDB/ReadFile>
#include <osgDB/FileUtils>

CSulGeomQuad::CSulGeomQuad( float w, float h, EPLANE ePlane ) :
m_vCenter(0,0,0),
m_w(w),
m_h(h),
m_ePlane(ePlane)
{
}

CSulGeomQuad::CSulGeomQuad( const osg::Vec3& vCenter, float w, float h, EPLANE ePlane ) :
m_vCenter(vCenter),
m_w(w),
m_h(h),
m_ePlane(ePlane)
{
}

void CSulGeomQuad::createDrawable()
{
	setDrawable( new osg::Geometry );
	create();
}

void CSulGeomQuad::create()
{
	Create( m_vCenter, m_w, m_h, m_ePlane );
}

void CSulGeomQuad::Create( const osg::Vec3& vCenter, float w, float h, EPLANE ePlane )
{
    // add 4 vertices creating a quad
    m_rVerts = new osg::Vec3Array;
	switch ( ePlane )
	{
		case PLANE_XZ:
			{
				osg::Vec3 vHalf( w/2.0f, 0, h/2.0f );
				m_rVerts->push_back( vCenter+(osg::Vec3( 0, 0, 0 )-vHalf) );
				m_rVerts->push_back( vCenter+(osg::Vec3( w, 0, 0 )-vHalf) );
				m_rVerts->push_back( vCenter+(osg::Vec3( w, 0, h )-vHalf) );
				m_rVerts->push_back( vCenter+(osg::Vec3( 0, 0, h )-vHalf) );
			}
			break;

		case PLANE_XY:
			{
				osg::Vec3 vHalf( w/2.0f, h/2.0f, 0 );
				m_rVerts->push_back( vCenter+(osg::Vec3( 0, 0, 0 )-vHalf) );
				m_rVerts->push_back( vCenter+(osg::Vec3( w, 0, 0 )-vHalf) );
				m_rVerts->push_back( vCenter+(osg::Vec3( w, h, 0 )-vHalf) );
				m_rVerts->push_back( vCenter+(osg::Vec3( 0, h, 0 )-vHalf) );
			}
			break;
	}
    getDrawable()->asGeometry()->setVertexArray( m_rVerts );

    // create a quad primitive set
    osg::DrawElementsUInt* pPrimitiveSet = new osg::DrawElementsUInt( osg::PrimitiveSet::QUADS, 0 );
    pPrimitiveSet->push_back( 3 );
    pPrimitiveSet->push_back( 2 );
    pPrimitiveSet->push_back( 1 );
    pPrimitiveSet->push_back( 0 );
    getDrawable()->asGeometry()->addPrimitiveSet( pPrimitiveSet );

	m_rColors = new osg::Vec4Array;
	float f = 1.0f;
	m_rColors->push_back( osg::Vec4(f,f,f,1.0f) );
	m_rColors->push_back( osg::Vec4(f,f,f,1.0f) );
	m_rColors->push_back( osg::Vec4(f,f,f,1.0f) );
	m_rColors->push_back( osg::Vec4(f,f,f,1.0f) );
    getDrawable()->asGeometry()->setColorArray( m_rColors.get() );
	getDrawable()->asGeometry()->setColorBinding( osg::Geometry::BIND_PER_VERTEX );
}

void CSulGeomQuad::setColor( osg::Vec4& c )
{
	setColor( c.r(), c.g(), c.b(), c.a() );
}

void CSulGeomQuad::setColor( float r, float g, float b, float a )
{
	if ( !m_rColors.valid() )
	{
		createDrawable();
	}

	(*m_rColors)[0].set( r, g, b, a );
	(*m_rColors)[1].set( r, g, b, a );
	(*m_rColors)[2].set( r, g, b, a );
	(*m_rColors)[3].set( r, g, b, a );
	getDrawable()->asGeometry()->dirtyDisplayList();
}

const osg::Vec4& CSulGeomQuad::getColor( sigma::uint32 index )
{
	return (*m_rColors)[index];
}

void CSulGeomQuad::createUV()
{
	if ( !m_rUV.valid() )
	{
		m_rUV = new osg::Vec2Array;

		m_rUV->push_back(osg::Vec2(0, 0));
		m_rUV->push_back(osg::Vec2(1, 0));
		m_rUV->push_back(osg::Vec2(1, 1));
		m_rUV->push_back(osg::Vec2(0, 1));

		getDrawable()->asGeometry()->setTexCoordArray( 0, m_rUV );
	}
}

void CSulGeomQuad::setTexture( osg::Image* pImage, GLint internalFormat, sigma::uint32 unit )
{
	createUV();

	osg::Texture2D* pTex = new osg::Texture2D;
	pTex->setInternalFormat( internalFormat );	
	pTex->setResizeNonPowerOfTwoHint( false );
	pTex->setFilter( osg::Texture::MIN_FILTER,osg::Texture::LINEAR );
	pTex->setFilter( osg::Texture::MAG_FILTER,osg::Texture::LINEAR );
	pTex->setImage( pImage );

	m_mapTex[unit] = pTex;

    getDrawable()->asGeometry()->getOrCreateStateSet()->setTextureAttributeAndModes( unit, pTex, osg::StateAttribute::ON );
}

void CSulGeomQuad::setTexture( osg::Texture2D* pTex, sigma::uint32 unit )
{
	createUV();

	m_mapTex[unit] = pTex;

    getDrawable()->asGeometry()->getOrCreateStateSet()->setTextureAttributeAndModes( unit, pTex, osg::StateAttribute::ON );
}

void CSulGeomQuad::setTexture( const CSulString& file, sigma::uint32 unit )
{
	osg::Texture2D* pTex = new osg::Texture2D;
    osg::Image* pImage = osgDB::readImageFile( osgDB::findDataFile(file.c_str()) );
    pTex->setImage( pImage );
    getDrawable()->asGeometry()->getOrCreateStateSet()->setTextureAttributeAndModes( unit, pTex, osg::StateAttribute::ON );
	m_mapTex[unit] = pTex;
}

osg::Texture2D* CSulGeomQuad::getTexture( sigma::uint32 unit )
{
	return m_mapTex[unit];
}

void CSulGeomQuad::setUV( float uv )
{
	setUV( uv, uv );
}

void CSulGeomQuad::setUV( float u, float v )
{
	(*m_rUV)[0].set( 0, 0 );
	(*m_rUV)[1].set( u, 0 );
	(*m_rUV)[2].set( u, v );
	(*m_rUV)[3].set( 0, v );
}

void CSulGeomQuad::setWidth( float w )
{
	m_w = w;
	calcVertPositions();
}

void CSulGeomQuad::setHeight( float h )
{
	m_h = h;
	calcVertPositions();
}

float CSulGeomQuad::getWidth()
{
	return m_w;
}

float CSulGeomQuad::getHeight()
{
	return m_h;
}

void CSulGeomQuad::calcVertPositions()
{
	if ( !m_rVerts.valid() )
	{
		return;
	}

	switch ( m_ePlane )
	{
		case PLANE_XZ:
			{
				osg::Vec3 vHalf( m_w/2.0f, 0, m_h/2.0f );

				(*m_rVerts)[0].set( m_vCenter+(osg::Vec3( 0, 0, 0 )-vHalf) );
				(*m_rVerts)[1].set( m_vCenter+(osg::Vec3( m_w, 0, 0 )-vHalf) );
				(*m_rVerts)[2].set( m_vCenter+(osg::Vec3( m_w, 0, m_h )-vHalf) );
				(*m_rVerts)[3].set( m_vCenter+(osg::Vec3( 0, 0, m_h )-vHalf) );
			}
			break;

		case PLANE_XY:
			{
				osg::Vec3 vHalf( m_w/2.0f, m_h/2.0f, 0 );
				(*m_rVerts)[0].set( m_vCenter+(osg::Vec3( 0, 0, 0 )-vHalf) );
				(*m_rVerts)[1].set( m_vCenter+(osg::Vec3( m_w, 0, 0 )-vHalf) );
				(*m_rVerts)[2].set( m_vCenter+(osg::Vec3( m_w, m_h, 0 )-vHalf) );
				(*m_rVerts)[3].set( m_vCenter+(osg::Vec3( 0, m_h, 0 )-vHalf) );
			}
			break;
	}

	getDrawable()->asGeometry()->dirtyBound();
}