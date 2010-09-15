// SulGeomGrid.cpp

#include "stdafx.h"
#include "SulGeomGrid.h"
#include "SulTypes.h"

// FIXME: why does createGeometry have a pGeo when the base class has this?? mistake perhaps

CSulGeomGrid::CSulGeomGrid() :
CSulGeomBase(), 
m_colorGrid( 1.0f, 1.0f, 1.0f, 1.0f ),
m_colorGridDiv( 0.5f, 0.5f, 0.5f, 1.0f )
{
}

void CSulGeomGrid::createDrawable()
{
	//CSulGeomBase::createGeometry();
	setDrawable( new osg::Geometry );

	osg::Geometry* pGeo = getDrawable()->asGeometry();

	pGeo->setVertexArray( new osg::Vec3Array );
    pGeo->setColorArray( new osg::Vec4Array );
    pGeo->setColorIndices( new osg::TemplateIndexArray<unsigned int, osg::Array::UIntArrayType, 4, 4> );
	pGeo->setColorBinding( osg::Geometry::BIND_PER_VERTEX );

    m_rPrimitiveSet = new osg::DrawElementsUInt( osg::PrimitiveSet::LINES, 0 );
    pGeo->addPrimitiveSet( m_rPrimitiveSet.get() );
}

void CSulGeomGrid::Create( const osg::Vec3& vPos, float fW, float fH, float fCellW, float fCellH, float fSubDivX, float fSubDivY )
{
	osg::Vec3 v0;
	osg::Vec3 v1;
	sigma::uint32 count = 0;
	float x, y;

	osg::Vec3Array* pVerts = dynamic_cast<osg::Vec3Array*>(getDrawable()->asGeometry()->getVertexArray());
	osg::Vec4Array* pColors = dynamic_cast<osg::Vec4Array*>(getDrawable()->asGeometry()->getColorArray());
	osg::TemplateIndexArray<unsigned int, osg::Array::UIntArrayType, 4, 4>* pColorIndices = 
		dynamic_cast<osg::TemplateIndexArray<unsigned int, osg::Array::UIntArrayType, 4, 4>*>(getDrawable()->asGeometry()->getColorIndices());

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

	// FIXME: it's not the SulGeomGrid that should decide if lighting should be on or off!!!
	getDrawable()->asGeometry()->getOrCreateStateSet()->setMode( GL_LIGHTING, osg::StateAttribute::OFF );
}

void CSulGeomGrid::SetGridColor( const osg::Vec4& vColor )
{
	m_colorGrid = vColor;
}

void CSulGeomGrid::SetGridDivColor( const osg::Vec4& vColor )
{
	m_colorGridDiv = vColor;
}




