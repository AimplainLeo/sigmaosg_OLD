// SulGeomHeightField.cpp

#include "stdafx.h"
#include "SulGeomHeightField.h"
#include <osg/geometry>

////////////////////////////////////////////////////////////////////////////////
//
//		-------
//		|\ II |
//		| \	  |
//		|  \  |
//		| I \ |
//		|    \|
//		-------
//
////////////////////////////////////////////////////////////////////////////////

CSulGeomHeightField::CSulGeomHeightField() :
osg::Geode()
{
}

void CSulGeomHeightField::Create( CSulArray2D<float>* pArray2D, float fScaleX, float fScaleY, float fScaleH )
{
	osg::Geometry* pGeo = new osg::Geometry;
	addDrawable( pGeo );

	m_w = pArray2D->GetWidth();
	m_h = pArray2D->GetHeight();
	m_fScaleX = fScaleX;
	m_fScaleY = fScaleY;
	m_fScaleH = fScaleH;

	// create vertices
	m_rVerts = new osg::Vec3Array;
	sigma::uint32 x, y;
	for ( y=0; y<m_h; y++ )
	{
		for ( x=0; x<m_w; x++ ) 
		{
			float h = pArray2D->Get( x, y );
			m_rVerts->push_back( osg::Vec3( x*fScaleX, y*fScaleY, h*fScaleH ) );
		}
	}
	pGeo->setVertexArray( m_rVerts.get() );
    
	// create triangle faces
	osg::DrawElementsUInt* pPrimitiveSet = new osg::DrawElementsUInt( osg::PrimitiveSet::TRIANGLES, 0 );
	for ( y=0; y<m_h-1; y++ )
	{
		for ( x=0; x<m_w-1; x++ ) 
		{
			sigma::uint32 ofs = x+y*m_w;
			sigma::uint32 ofsY = (y+1)*m_w;

			// triange part I
			pPrimitiveSet->push_back( ofs+0 );
			pPrimitiveSet->push_back( x+ofsY+0 );
			pPrimitiveSet->push_back( x+ofsY+1 );

			// triange part II
			pPrimitiveSet->push_back( ofs+0 );
			pPrimitiveSet->push_back( x+ofsY+1 );
			pPrimitiveSet->push_back( ofs+1 );
		}
	}
	pGeo->addPrimitiveSet( pPrimitiveSet );

	// color
	osg::Vec4Array* pColors = new osg::Vec4Array;
    pColors->push_back( osg::Vec4( 1.0f, 1.0f, 1.0f, 1.0f ) );
    pGeo->setColorArray( pColors );
	pGeo->setColorBinding( osg::Geometry::BIND_OVERALL );

	// normals
	osg::Vec3Array* pNormals = new osg::Vec3Array;
	for ( y=0; y<m_h; y++ )
	{
		for ( x=0; x<m_w; x++ ) 
		{
			//osg::Vec3 v = (*m_rVerts)[0];
			//pNormals->push_back( osg::Vec3(0.0f,-1.0f,0.0f) );
			pNormals->push_back( GetNormal( x, y ) );
		}
	}
	pGeo->setNormalArray( pNormals );
	pGeo->setNormalBinding( osg::Geometry::BIND_PER_VERTEX );

	/////////////////////////////////////////////////////////////////////////////////////////
	// test code, can we add lines to draw normals so we can visually see what is happening
	/////////////////////////////////////////////////////////////////////////////////////////
	osg::DrawElementsUInt* pPrimitiveSet2 = new osg::DrawElementsUInt( osg::PrimitiveSet::LINES, 0 );
	pNormals->getNumElements();
	pGeo->addPrimitiveSet( pPrimitiveSet2 );
}

float CSulGeomHeightField::GetHeight( sigma::int32 x, sigma::int32 y ) const
{
	return (*m_rVerts)[x+y*m_w].z();
}

osg::Vec3 CSulGeomHeightField::GetNormal( sigma::int32 x, sigma::int32 y ) const 
{
	float h0 = GetHeight( x, y );
	float h1 = GetHeight( x, (y-1)%m_h );
	float h2 = GetHeight( (x-1)%m_w, y );
	float h3 = GetHeight( x, (y+1)%m_h );
	float h4 = GetHeight( (x-1)%m_w, y );

	float nx = (0.5f*(h4-h2))/m_fScaleX;
	float ny = (0.5f*(h3-h1))/m_fScaleY;

	osg::Vec3 n(-nx,-ny,1.0f);
    n.normalize();
	return n;
}

