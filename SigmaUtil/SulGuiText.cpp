// SulGuiText.cpp

#include "stdafx.h"
#include "SulGuiText.h"

CSulGuiText::CSulGuiText( const CSulString& sText, float x, float y, float size, CSulString font ) :
CSulGuiComp( "TEXT", x, y )
{
	m_font = font;
	m_sText = sText;
	m_size = size;
	m_color.set( 1,1,1,1 );
}

void CSulGuiText::setupTheme( CSulGuiThemeXml* pThemeXml )
{
	CSulGuiComp::setupTheme( pThemeXml );

	m_size = getThemeValue( "font_size" ).asFloat();
}

void CSulGuiText::setupAttr( CSulXmlAttr* pAttr )
{
	CSulGuiComp::setupAttr( pAttr );

	if ( pAttr->exist( "font_size" ) )	m_size = pAttr->get( "font_size" ).asFloat();
	if ( pAttr->exist( "font" ) )		m_font = pAttr->get( "font" );
	if ( pAttr->exist( "color" ) )		m_color	= pAttr->get( "color" ).asVec4();
}

void CSulGuiText::init()
{
	CSulGuiComp::init();

	m_rText = new osgText::Text;
	m_rText->setDataVariance( osg::Object::DYNAMIC );		// FIXME: the code should be able to choose this
	m_rText->setCharacterSize( m_size );
	if ( !m_font.empty() ) 
		m_rText->setFont( m_font );
	m_rText->setColor( m_color );
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

void CSulGuiText::setColor( const osg::Vec4& color )
{
	m_color = color;
	if ( m_rText.valid() )
	{
		m_rText->setColor( color );
	}
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

void CSulGuiText::setFontSize( float size )
{
	m_size = size;
	if ( m_rText.valid() )
	{
		m_rText->setCharacterSize( size );
	}
}