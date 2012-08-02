// SulGeomGrid.cpp

#include "stdafx.h"
#include "SulGeomGrid.h"
#include "SulTypes.h"
#include <osg/geode>

CSulGeomGrid::CSulGeomGrid() :
osg::Geode(), 
m_colorGrid( 1.0f, 1.0f, 1.0f, 1.0f ),
m_colorGridDiv( 0.5f, 0.5f, 0.5f, 1.0f )
{
	createDrawable();
}

void CSulGeomGrid::createDrawable()
{
	m_rGeo = new osg::Geometry;
	addDrawable( m_rGeo );

	m_rGeo->setVertexArray( new osg::Vec3Array );
    m_rGeo->setColorArray( new osg::Vec4Array );
    m_rGeo->setColorIndices( new osg::TemplateIndexArray<unsigned int, osg::Array::UIntArrayType, 4, 4> );
	m_rGeo->setColorBinding( osg::Geometry::BIND_PER_VERTEX );

    m_rPrimitiveSet = new osg::DrawElementsUInt( osg::PrimitiveSet::LINES, 0 );
    m_rGeo->addPrimitiveSet( m_rPrimitiveSet.get() );
}

void CSulGeomGrid::Create( const osg::Vec3& vPos, float fW, float fH, float fCellW, float fCellH, float fSubDivX, float fSubDivY )
{
	osg::Vec3 v0;
	osg::Vec3 v1;
	sigma::uint32 count = 0;
	float x, y;

	osg::Vec3Array* pVerts = dynamic_cast<osg::Vec3Array*>(m_rGeo->getVertexArray());
	osg::Vec4Array* pColors = dynamic_cast<osg::Vec4Array*>(m_rGeo->getColorArray());
	osg::TemplateIndexArray<unsigned int, osg::Array::UIntArrayType, 4, 4>* pColorIndices = 
		dynamic_cast<osg::TemplateIndexArray<unsigned int, osg::Array::UIntArrayType, 4, 4>*>(m_rGeo->getColorIndices());

	for ( x=0; x<fW+1.0f; x++ )
	{
		v0 = vPos+osg::Vec3( x*fCellW, 0.0f, 0.0f );
		v1 = vPos+osg::Vec3( x*fCellW, fH*fCellW, 0.0f );
		pVerts->push_back( v0 );
		pVerts->push_back( v1 );
	    pColors->push_back( m_colorGrid );
		pColors->push_back( m_colorGrid );
		pColorIndices->push_back( count );
		m_rPrimitiveSet->push_back( count++ );
		pColorIndices->push_back( count );
		m_rPrimitiveSet->push_back( count++ );
	}
	
	for ( y=0; y<fH+1.0f; y++ )
	{
		v0 = vPos+osg::Vec3( 0.0f, y*fCellH, 0.0f );
		v1 = vPos+osg::Vec3( fW*fCellW, y*fCellH, 0.0f );
		pVerts->push_back( v0 );
		pVerts->push_back( v1 );
	    pColors->push_back( m_colorGrid );
		pColors->push_back( m_colorGrid );
		pColorIndices->push_back( count );
		m_rPrimitiveSet->push_back( count++ );
		pColorIndices->push_back( count );
		m_rPrimitiveSet->push_back( count++ );
	}

	// subdiv
	for ( x=0; x<fW; x++ )
	{
		float sx = fCellW/fSubDivX;

		for ( float xx=1; xx<fSubDivX; xx++ )
		{
			v0 = vPos+osg::Vec3( x*fCellW+sx*xx, 0.0f, 0.0f );
			v1 = vPos+osg::Vec3( x*fCellW+sx*xx, fH*fCellW, 0.0f );
			pVerts->push_back( v0 );
			pVerts->push_back( v1 );
			pColors->push_back( m_colorGridDiv );
			pColors->push_back( m_colorGridDiv );
			pColorIndices->push_back( count );
			m_rPrimitiveSet->push_back( count++ );
			pColorIndices->push_back( count );
			m_rPrimitiveSet->push_back( count++ );
		}

	}
	// subdiv
	for ( y=0; y<fH; y++ )
	{
		float sy = fCellH/fSubDivY;

		for ( float yy=1; yy<fSubDivY; yy++ )
		{
			v0 = vPos+osg::Vec3( 0.0f, y*fCellH+sy*yy, 0.0f );
			v1 = vPos+osg::Vec3( fW*fCellW, y*fCellH+sy*yy, 0.0f );
			pVerts->push_back( v0 );
			pVerts->push_back( v1 );
			pColors->push_back( m_colorGridDiv );
			pColors->push_back( m_colorGridDiv );
			pColorIndices->push_back( count );
			m_rPrimitiveSet->push_back( count++ );
			pColorIndices->push_back( count );
			m_rPrimitiveSet->push_back( count++ );
		}
	}

	m_rGeo->getOrCreateStateSet()->setMode( GL_LIGHTING, osg::StateAttribute::OFF );
}

void CSulGeomGrid::setColor( const osg::Vec4& color )
{
	m_colorGrid = color;
	m_colorGridDiv = color;

	if ( m_rGeo.valid() )
	{
		// first half of the colors are for the grid (last half subgrid)
		osg::Vec4Array* c = dynamic_cast<osg::Vec4Array*>(m_rGeo->getColorArray());

		sigma::uint32 s = c->size();

		for ( sigma::uint32 i=0; i<s; i++ )
		{
			(*c)[i] = color;
		}

		m_rGeo->dirtyDisplayList();
	}
}

void CSulGeomGrid::SetGridColor( const osg::Vec4& color )
{
	m_colorGrid = color;
}

void CSulGeomGrid::SetGridDivColor( const osg::Vec4& color )
{
	m_colorGridDiv = color;
}




