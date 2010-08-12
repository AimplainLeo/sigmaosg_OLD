// SulGuiText.cpp

#include "stdafx.h"
#include "SulGuiText.h"

CSulGuiText::CSulGuiText( const CSulString& sText, float x, float y, float size ) :
CSulGuiComp( "TEXT", x, y )
{
	m_sText = sText;
	m_size = size;
}

void CSulGuiText::setupAttr( CSulXmlAttr* pAttr )
{
	CSulGuiComp::setupAttr( pAttr );

	m_size = getThemeValue( "font_size" ).asFloat();

	if ( pAttr->exist( "font_size" ) ) m_size = pAttr->get( "font_size" ).asFloat();
}

void CSulGuiText::init()
{
	m_rText = new osgText::Text;
	m_rText->setDataVariance( osg::Object::DYNAMIC );		// FIXME: the code should be able to choose this
	m_rText->setCharacterSize( m_size );
	m_rText->setFont( "Tempest.ttf" );
	m_rText->setColor( osg::Vec4(1,1,1,1) );
	m_rText->setText( m_sText );	

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

	setText( m_sText );
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

