// SulGeomCrosshairSimple.cpp

#include "stdafx.h"
#include "SulGeomCrosshairSimple.h"

CSulGeomCrosshairSimple::CSulGeomCrosshairSimple( float centerOffset, float len, const osg::Vec4& color ) :
CSulGeomLines( color ),
m_centerOffset( centerOffset ),
m_lenTop( len ),
m_lenBottom( len ),
m_lenLeft( len ),
m_lenRight( len )
{
	createDrawable();
}

void CSulGeomCrosshairSimple::createDrawable()
{
	sigma::VEC_LINESEGMENT vecLine;

	vecLine.push_back( new osg::LineSegment(osg::Vec3(0,m_centerOffset,0), osg::Vec3(0,m_centerOffset+m_lenTop,0)) );			// top
	vecLine.push_back( new osg::LineSegment(osg::Vec3(0,-m_centerOffset,0), osg::Vec3(0,-(m_lenBottom+m_centerOffset),0)) );	// bottom
	vecLine.push_back( new osg::LineSegment(osg::Vec3(-m_centerOffset,0,0), osg::Vec3(-(m_lenLeft+m_centerOffset),0,0)) );		// left
	vecLine.push_back( new osg::LineSegment(osg::Vec3(m_centerOffset,0,0), osg::Vec3(m_centerOffset+m_lenRight,0,0)) );			// right

	setLines( vecLine );

	CSulGeomLines::createDrawable();
}