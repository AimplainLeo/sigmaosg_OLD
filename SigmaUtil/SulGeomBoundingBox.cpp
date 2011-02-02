// SulGeomBoundingBox.cpp

#include "stdafx.h"
#include "SulGeomBoundingBox.h"

CSulGeomBoundingBox::CSulGeomBoundingBox( const osg::BoundingBox& bound ) :
CSulGeomBase()
{
	m_bb = bound;
}

void CSulGeomBoundingBox::createDrawable()
{
	addDrawable( new osg::Geometry );

	osg::Geometry* pGeo = getDrawable()->asGeometry();

	osg::Vec3Array* pVerts = new osg::Vec3Array;
	pGeo->setVertexArray( pVerts );

	pGeo->addPrimitiveSet( AddCornerLines(*pVerts, m_bb, 0,1,2,4) );
	pGeo->addPrimitiveSet( AddCornerLines(*pVerts, m_bb, 1,0,5,3) );
	pGeo->addPrimitiveSet( AddCornerLines(*pVerts, m_bb, 2,0,3,6) );
	pGeo->addPrimitiveSet( AddCornerLines(*pVerts, m_bb, 3,1,2,7) );
	pGeo->addPrimitiveSet( AddCornerLines(*pVerts, m_bb, 4,0,5,6) );
	pGeo->addPrimitiveSet( AddCornerLines(*pVerts, m_bb, 5,1,4,7) );
	pGeo->addPrimitiveSet( AddCornerLines(*pVerts, m_bb, 6,2,4,7) );
	pGeo->addPrimitiveSet( AddCornerLines(*pVerts, m_bb, 7,3,5,6) );

	osg::Vec4Array* pColors = new osg::Vec4Array;
	pColors->push_back( osg::Vec4(1,1,1,1) );
	pGeo->setColorArray( pColors );
	pGeo->setColorBinding( osg::Geometry::BIND_OVERALL );

	pGeo->getOrCreateStateSet()->setMode( GL_LIGHTING, osg::StateAttribute::OFF | osg::StateAttribute::PROTECTED );

	pGeo->setUseDisplayList( false );
}

osg::PrimitiveSet* CSulGeomBoundingBox::AddCornerLines( osg::Vec3Array& verts, const osg::BoundingBox& bound, unsigned i0, unsigned i1, unsigned i2, unsigned i3 )
{
	static float f = 0.3f;

	int n = verts.size();

	osg::DrawElementsUInt* pLines = new osg::DrawElementsUInt(osg::PrimitiveSet::LINES, 0);

	osg::Vec3 p0 = bound.corner(i0);
	osg::Vec3 p1 = bound.corner(i1);
	osg::Vec3 p2 = bound.corner(i2);
	osg::Vec3 p3 = bound.corner(i3);

	verts.push_back(p0);
	verts.push_back( (p1-p0)*f + p0 );
	verts.push_back( (p2-p0)*f + p0 );
	verts.push_back( (p3-p0)*f + p0 );

	pLines->push_back(n + 0);
	pLines->push_back(n + 1);
	pLines->push_back(n + 0);
	pLines->push_back(n + 2);
	pLines->push_back(n + 0);
	pLines->push_back(n + 3);

	return pLines;
}

void CSulGeomBoundingBox::ModifyCornerLines( sigma::uint32 index, osg::Vec3Array* pVerts, const osg::BoundingBox& bound, unsigned i0, unsigned i1, unsigned i2, unsigned i3 )
{
	static float f = 0.3f;

	osg::Vec3 p0 = bound.corner(i0);
	osg::Vec3 p1 = bound.corner(i1);
	osg::Vec3 p2 = bound.corner(i2);
	osg::Vec3 p3 = bound.corner(i3);

	(*pVerts)[index*4+0].set(p0);
	(*pVerts)[index*4+1].set((p1-p0)*f + p0 );
	(*pVerts)[index*4+2].set((p2-p0)*f + p0 );
	(*pVerts)[index*4+3].set((p3-p0)*f + p0 );
}

void CSulGeomBoundingBox::update( const osg::BoundingBox& bound )
{
	osg::Vec3Array* pVerts = dynamic_cast<osg::Vec3Array*>(getDrawable()->asGeometry()->getVertexArray());

	ModifyCornerLines( 0, pVerts, bound, 0,1,2,4 );
	ModifyCornerLines( 1, pVerts, bound, 1,0,5,3 );
	ModifyCornerLines( 2, pVerts, bound, 2,0,3,6 );
	ModifyCornerLines( 3, pVerts, bound, 3,1,2,7 );
	ModifyCornerLines( 4, pVerts, bound, 4,0,5,6 );
	ModifyCornerLines( 5, pVerts, bound, 5,1,4,7 );
	ModifyCornerLines( 6, pVerts, bound, 6,2,4,7 );
	ModifyCornerLines( 7, pVerts, bound, 7,3,5,6 );
}