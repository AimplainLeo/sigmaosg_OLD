// SulGeomTickMarksHorizontal.cpp

#include "stdafx.h"
#include "SulGeomTickMarksHorizontal.h"

CSulGeomTickMarksHorizontal::CSulGeomTickMarksHorizontal( sigma::uint32 numTicks, float tickSpacing, float tickHeight, float ofsY, const osg::Vec4& color ) :
CSulGeomLines( color ),
m_numTicks( numTicks ),
m_tickSpacing( tickSpacing ),
m_tickHeight( tickHeight ),
m_ofsY( ofsY )
{
	createDrawable();
}

void CSulGeomTickMarksHorizontal::createDrawable()
{
	sigma::VEC_LINESEGMENT vecLine;

	for ( sigma::uint32 i=0; i<m_numTicks; i++ )
	{
		float x = i*m_tickSpacing;
		float x_sub = x-m_tickSpacing/2.0f;

		// left side
		vecLine.push_back( 
			new osg::LineSegment(
				osg::Vec3(-x,m_ofsY,0), 
				osg::Vec3(-x,m_ofsY+m_tickHeight,0)
		));

		// left side (sub tick)
		vecLine.push_back( 
			new osg::LineSegment(
				osg::Vec3(-x_sub,m_ofsY,0), 
				osg::Vec3(-x_sub,m_ofsY+m_tickHeight/2.0f,0)
		));

		// right side
		vecLine.push_back( 
			new osg::LineSegment(
				osg::Vec3(x,m_ofsY,0), 
				osg::Vec3(x,m_ofsY+m_tickHeight,0)
		));

		// right side (sub tick)
		vecLine.push_back( 
			new osg::LineSegment(
				osg::Vec3(x_sub,m_ofsY,0), 
				osg::Vec3(x_sub,m_ofsY+m_tickHeight/2.0f,0)
		));
	}

	setLines( vecLine );

	CSulGeomLines::createDrawable();
}