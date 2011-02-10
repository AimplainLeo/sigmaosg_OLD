// SulGeomBarChart.cpp

#include "stdafx.h"
#include "SulGeomBarChart.h"
#include <osgText/text>

#include <osgText/TextBase>

// some properties of the bar
// we need a max length and what this should match in pixels/viewport

CSulGeomBarChart::CSulGeomBarChart() :
osg::Group(),
m_h( 20.0f )
{
}

sigma::uint32 CSulGeomBarChart::createBar( const CSulString& name, const osg::Vec4& color )
{
	osgText::Text* t = new osgText::Text;
	t->setDataVariance( osg::Object::DYNAMIC );		// FIXME: the code should be able to choose this
	t->setCharacterSize( m_h );
	t->setFont( "Tempest.ttf" );
	t->setText( name );
	t->setPosition( osg::Vec3(5, -(m_vecBar.size()*(m_h+2.0f))-6, 0.0f) );
	t->setColor( osg::Vec4(1,1,1,1) );

	osg::Geode* pGeode = new osg::Geode;
	pGeode->addDrawable( t );

	osg::Matrix m;
	m.makeScale( 1, -1, 1 );
	osg::MatrixTransform* mt = new osg::MatrixTransform;
	mt->setMatrix( m );
	mt->addChild( pGeode );
	addChild( mt );
	

	CSulGeomQuad* p = new CSulGeomQuad;
	p->setColor( color );
	p->setHeight( m_h );
	addChild( p );

	m_vecBar.push_back( p );

	return m_vecBar.size()-1;
}

void CSulGeomBarChart::setBarSize( sigma::uint32 index, float size )
{
	m_vecBar[index]->setWidth( size );

	// adjust center so the left side is fixed
	m_vecBar[index]->setCenter( osg::Vec3( size/2.0f, index*(m_h+2.0f), 0 ) );
}

