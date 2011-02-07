// SulGeomBarChart.cpp

#include "stdafx.h"
#include "SulGeomBarChart.h"

// some properties of the bar
// we need a max length and what this should match in pixels/viewport

CSulGeomBarChart::CSulGeomBarChart() :
osg::Group(),
m_h( 10.0f )
{
}

sigma::uint32 CSulGeomBarChart::createBar( const osg::Vec4& color )
{
	CSulGeomQuad* p = new CSulGeomQuad;
	p->setColor( color );
	addChild( p );

	m_vecBar.push_back( p );

	return m_vecBar.size()-1;
}

void CSulGeomBarChart::setBarSize( sigma::uint32 index, float size )
{
	m_vecBar[index]->setWidth( size );
	m_vecBar[index]->setHeight( m_h );

	// adjust center so the left side is fixed
	m_vecBar[index]->setCenter( osg::Vec3( size/2.0f, index*(m_h+2.0f), 0 ) );
}

