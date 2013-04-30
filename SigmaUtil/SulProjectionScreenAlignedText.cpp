// SulRenderableScreenAlignedText.cpp

#include "stdafx.h"
#include "SulProjectionScreenAlignedText.h"
#include "SulGeode.h"

/*

default ortho:

			+1
			^
			|
	-1   <----->    +1
			|
			v
			-1

*/

CSulProjectionScreenAlignedText::CSulProjectionScreenAlignedText() :
osg::Projection()
{
	m_text = new  osgText::Text;
	m_text->setFont( "arial.ttf" );
	m_text->setCharacterSize( 32.0f );
	m_text->setAxisAlignment( osgText::Text::SCREEN );
	m_text->setCharacterSizeMode( osgText::Text::SCREEN_COORDS );
	m_text->setPosition( osg::Vec3( 0, 0, 0 ) );
    m_text->setColor( osg::Vec4( 1.0f, 1.0f, 1.0f, 1.0f ) );

	CSulGeode* geode = new CSulGeode( m_text );

	osg::MatrixTransform* mtrans = new osg::MatrixTransform;
	mtrans->setReferenceFrame( osg::Transform::ABSOLUTE_RF );
	mtrans->setMatrix( osg::Matrix::identity() );
	mtrans->addChild( geode );
	addChild( mtrans );
	
	/*
	osg::Matrixd m_mOrtho = osg::Matrix::ortho2D( 0, 1, 0, 1 );
	setMatrix( m_mOrtho );	
	*/
}

void CSulProjectionScreenAlignedText::setText( const CSulString& text )
{
	m_text->setText( text ) ;
}

void CSulProjectionScreenAlignedText::setPosition( const osg::Vec2& pos )
{
	m_text->setPosition( osg::Vec3( pos.x(), pos.y(), 0.0f ) );
}