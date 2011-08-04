// SulGeomQuadTiles.cpp

#include "stdafx.h"
#include "SulGeomQuadTiles.h"
#include "SulTypes.h"
#include <osg/geometry>

CSulGeomQuadTiles::CSulGeomQuadTiles( 
	const osg::Vec3& vCenter, 
	float w, float h,						// total width and height with all quads
	sigma::uint32 tileX, sigma::uint32 tileY,				// number of tiles x and y with in w and h
	bool bFadeEdges,
	EPLANE ePlane ) :
osg::Geode(),
m_vCenter(vCenter),
m_w(w),
m_h(h),
m_tileX(tileX),
m_tileY(tileY),
m_bFadeEdges(bFadeEdges),
m_ePlane(ePlane)
{
	createDrawable();
}

sigma::uint32 CSulGeomQuadTiles::getTileX()
{
	return m_tileX;
}

sigma::uint32 CSulGeomQuadTiles::getTileY()
{
	return m_tileY;
}

float CSulGeomQuadTiles::getWidth()
{
	return m_w;
}

float CSulGeomQuadTiles::getHeight()
{
	return m_h;
}

void CSulGeomQuadTiles::createDrawable()
{
	m_rGeo = new osg::Geometry;
	addDrawable( m_rGeo );
	createTiles();
}

void CSulGeomQuadTiles::createTiles()
{
	////////////////////////////////////////////////
	// create vertice coordinates
	////////////////////////////////////////////////
	
	osg::Vec3Array* pVerts = new osg::Vec3Array;

	// calc vert count x and y
	sigma::uint32 vertCountX = (m_tileX+1)*(m_tileX+1);
	sigma::uint32 vertCountY = (m_tileY+1)*(m_tileY+1);

	float dx = m_w/m_tileX;
	float dy = m_h/m_tileY;

	sigma::uint32 x, y;
	for ( y=0; y<vertCountY; y++ )
	{
		for ( x=0; x<vertCountX; x++ )
		{
			osg::Vec3 v( 
				-m_w/2.0f + x*dx + m_vCenter.x(), 
				-m_h/2.0f + y*dy + m_vCenter.y(), 
				m_vCenter.z() );

			pVerts->push_back( v );
		}
	}

	m_rGeo->setVertexArray( pVerts );

	////////////////////////////////////////////////
	// create vertice indices
	////////////////////////////////////////////////

	osg::UByteArray* pCoordIndices = new osg::UByteArray;

	for ( y=0; y<m_tileY; y++ )
	{
		for ( x=0; x<m_tileX; x++ )
		{
			pCoordIndices->push_back( x     + (y+1)*vertCountX );
			pCoordIndices->push_back( x     + y*vertCountX );
			pCoordIndices->push_back( (x+1) + y*vertCountX );
			pCoordIndices->push_back( (x+1) + (y+1)*vertCountX );
		}
	}

	m_rGeo->setVertexIndices( pCoordIndices );

	////////////////////////////////////////////////
	// texture coordinates
	////////////////////////////////////////////////

	m_texCoords = new osg::Vec2Array( m_tileX*m_tileY*4 );

	sigma::uint32 numTexCoordsPerRow = m_tileX*4;

	dx = 1.0f/m_tileX;
	dy = 1.0f/m_tileX;

	for ( y=0; y<m_tileY; y++ )
	{
		for ( x=0; x<m_tileX; x++ )
		{
			(*m_texCoords)[ 4*x + numTexCoordsPerRow*y + 0].set( dx*x,		dy*(y+1) );
			(*m_texCoords)[ 4*x + numTexCoordsPerRow*y + 1].set( dx*x,		dy*y );
			(*m_texCoords)[ 4*x + numTexCoordsPerRow*y + 2].set( dx*(x+1),	dy*y );
			(*m_texCoords)[ 4*x + numTexCoordsPerRow*y + 3].set( dx*(x+1),	dy*(y+1) );
		}
	}

	m_rGeo->setTexCoordArray( 0, m_texCoords );

	////////////////////////////////////////////////
	// colors
	////////////////////////////////////////////////

	m_colors = new osg::Vec4Array;
	//m_colors->push_back(osg::Vec4(0.2f,0.2f,0.4f,0.0f)); // fog color - alpha=0
	//m_colors->push_back(osg::Vec4(1.0f,0.0f,0.0f,1.0f)); // fog color - alpha=0

	//m_colors->push_back(osg::Vec4(0.5f,0.5f,0.5f,0.0f)); // fog color - alpha=0

	//m_colors->push_back(osg::Vec4(1.0f,0.0f,0.0f,1.0f)); // fog color - alpha=0

	m_colors->push_back(osg::Vec4(0.2f,0.2f,0.4f,0.0f)); // fog color - alpha=0
	m_colors->push_back(osg::Vec4(1.0f,1.0f,1.0f,1.f));  // black    - alpha=1

	osg::UByteArray* colorIndices = new osg::UByteArray();

	for ( y=0; y<m_tileY; y++ )
	{
		for ( x=0; x<m_tileX; x++ )
		{
			sigma::uint32 cX;
			sigma::uint32 cY;

			cX = x;
			cY = y+1;
			colorIndices->push_back( cX==0||cX==m_tileX||cY==0||cY==m_tileY?(m_bFadeEdges?0:1):1 );

			cX = x;
			cY = y;
			colorIndices->push_back( cX==0||cX==m_tileX||cY==0||cY==m_tileY?(m_bFadeEdges?0:1):1 );

			cX = x+1;
			cY = y;
			colorIndices->push_back( (cX==0||cX==m_tileX||cY==0||cY==m_tileY)?(m_bFadeEdges?0:1):1 );

			cX = x+1;
			cY = y+1;
			colorIndices->push_back( cX==0||cX==m_tileX||cY==0||cY==m_tileY?(m_bFadeEdges?0:1):1 );
		}
	}

	m_rGeo->setColorIndices( colorIndices );
	m_rGeo->setColorArray( m_colors );
	m_rGeo->setColorBinding( osg::Geometry::BIND_PER_VERTEX );
	
	////////////////////////////////////////////////
	// normals
	////////////////////////////////////////////////

	osg::Vec3Array* normals = new osg::Vec3Array;
	normals->push_back(osg::Vec3(0.0f,0.0f,1.0f)); // set up a single normal for the plane

	m_rGeo->setNormalArray( normals );
	m_rGeo->setNormalBinding( osg::Geometry::BIND_OVERALL );

	m_rGeo->addPrimitiveSet( new osg::DrawArrays(osg::PrimitiveSet::QUADS, 0, pCoordIndices->size()) );
}