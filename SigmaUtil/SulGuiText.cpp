// SulGuiText.cpp

#include "stdafx.h"
#include "SulGuiText.h"

CSulGuiText::CSulGuiText( const CSulString& sText, float x, float y, float size ) :
CSulGuiComp( x, y )
{
	m_rText = new osgText::Text;
	m_rText->setDataVariance( osg::Object::DYNAMIC );		// FIXME: the code should be able to choose this
	m_rText->setCharacterSize( size );
	m_rText->setFont( "Tempest.ttf" );
	m_rText->setColor( osg::Vec4(1,1,1,1) );
	m_rText->setText( sText );	

	//m_rText->setAlignment( osgText::TextBase::LEFT_CENTER );

	//m_rText->setPosition( osg::Vec3( 10, -getH()/2.0f, 0 ) );

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

	setName( sText );
}

void CSulGuiText::setText( const CSulString& sText )
{
	m_sText = sText;
	m_rText->setText( m_sText );
}

const CSulString& CSulGuiText::getText() const
{
	return m_sText;
}

osgText::Text* CSulGuiText::getTextObject()
{
	return m_rText;
}

