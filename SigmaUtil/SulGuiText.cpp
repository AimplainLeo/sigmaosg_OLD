// SulGuiText.cpp

#include "stdafx.h"
#include "SulGuiText.h"

CSulGuiText::CSulGuiText( const CSulString& sName, float x, float y, float w, float h, float size ) :
CSulGuiComp( sName, x, y, w, h )
{
	m_rText = new osgText::Text;
	m_rText->setDataVariance( osg::Object::DYNAMIC );		// FIXME: the code should be able to choose this
	m_rText->setCharacterSize( size );
	m_rText->setFont( "Tempest.ttf" );
	m_rText->setColor( osg::Vec4(1,1,1,1) );
	m_rText->setText( sName );	

	m_rText->setPosition( osg::Vec3( 0, 0, 0) );

	//m_rText->setMaximumWidth( w );
	//m_rText->setMaximumWidth( h );
	//m_rText->setLayout( osgText::Text::RIGHT_TO_LEFT );
	//m_rText->setAlignment( osgText::Text::RIGHT_BASE_LINE );

	// because of the coordinate system starting at top, left the text is drawn upside down
	// to compensate for this I reverse the Y-Axis by using a negative scaling of one
	osg::Matrix m;
	m.makeScale( 1, -1, 1 );
	osg::MatrixTransform* p = new osg::MatrixTransform;
	p->setMatrix( m );

	osg::Geode* pGeode = new osg::Geode;
	pGeode->addDrawable( m_rText );
	p->addChild( pGeode );

	addChild( p );
}

void CSulGuiText::setName( const CSulString& sName )
{
	m_rText->setText( sName );
}