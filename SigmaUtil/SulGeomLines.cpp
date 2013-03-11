// SulGeomLines.cpp

#include "stdafx.h"
#include "SulGeomLines.h"

CSulGeomLines::CSulGeomLines( const osg::Vec4& color ) :
CSulGeom( color )
{
	initConstructor();
}

CSulGeomLines::CSulGeomLines( const sigma::VEC_LINESEGMENT& vecLines ) :
CSulGeom(),
m_vecLines( vecLines )
{
	initConstructor();
}

void CSulGeomLines::initConstructor()
{
	createDrawable();
}

void CSulGeomLines::createDrawable()
{
	// we can't create if we don't have any lines
	if ( m_vecLines.size()==0 )
		return;

	removeAllPrimitiveSets();

	sigma::VEC_LINESEGMENT::const_iterator i;
	i = m_vecLines.begin();
	while ( i!=m_vecLines.end() )
	{
		addVC( (*i)->start(), m_color );
		addVC( (*i)->end(), m_color );
		++i;
	}

	addPrimitiveSet( new osg::DrawArrays(osg::PrimitiveSet::LINES, 0, m_vecLines.size()*2 ) );
}

void CSulGeomLines::setLines( const sigma::VEC_LINESEGMENT& vecLines )
{
	m_vecLines = vecLines;
}

void CSulGeomLines::addLine( const osg::Vec3& pos )
{
	osg::LineSegment* ls = new  osg::LineSegment( osg::Vec3(0,0,0), pos );
	m_vecLines.push_back( ls );
}

void CSulGeomLines::addLine( const osg::Vec3& pos0, const osg::Vec3& pos1  )
{
	osg::LineSegment* ls = new  osg::LineSegment( pos0, pos1 );
	m_vecLines.push_back( ls );
}