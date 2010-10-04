// SulGuiTextBox.cpp

#include "stdafx.h"
#include "SulGuiTextBox.h"

CSulGuiTextBox::CSulGuiTextBox( float x, float y ) :
CSulGuiCanvas( "TEXTBOX", x, y )
{
	initConstructor();
}

CSulGuiTextBox::CSulGuiTextBox( const CSulString& sText, float x, float y, float w, float h, float fontSize ) :
CSulGuiCanvas( "TEXTBOX", x, y, w, h )
{
	m_fontSize = fontSize;
	m_sText = sText;
	initConstructor();
}

void CSulGuiTextBox::initConstructor()
{
	m_font = "Tempest.ttf";
	m_ofs_x = 0.0f;
	m_ofs_y = 0.0f;
	m_color.set( 1,1,1,1 );
}

void CSulGuiTextBox::setupTheme( CSulGuiThemeXml* pThemeXml )
{
	CSulGuiCanvas::setupTheme( pThemeXml );

	m_fontSize	= getThemeValue( "font_size" ).asFloat();
	m_ofs_x		= getThemeValue( "ofs_x" ).asFloat();
	m_ofs_y		= getThemeValue( "ofs_y" ).asFloat();
	m_color		= getThemeValue( "color" ).asVec4();
}

void CSulGuiTextBox::setupAttr( CSulXmlAttr* pAttr )
{
	CSulGuiCanvas::setupAttr( pAttr );

	if ( pAttr->exist( "font_size" ) )	m_fontSize	= pAttr->get( "font_size" ).asFloat();
	if ( pAttr->exist( "ofs_x" ) )		m_ofs_x		= pAttr->get( "ofs_x" ).asFloat();
	if ( pAttr->exist( "ofs_y" ) )		m_ofs_x		= pAttr->get( "ofs_y" ).asFloat();
	if ( pAttr->exist( "font" ) )		m_font		= pAttr->get( "font" );
	if ( pAttr->exist( "color" ) )		m_color		= pAttr->get( "color" ).asVec4();
}

void CSulGuiTextBox::init()
{
	CSulGuiCanvas::init();

	float h = getH();

	m_rText = new CSulGuiText( m_sText, 0+m_ofs_x, h-m_ofs_y, m_fontSize, m_font );
	m_rText->setColor( m_color );
	m_rText->init();
	addChild( m_rText );

	useShaderTexture( false );
	useShaderBorder( false );
	useShaderBackground( false );
}

void CSulGuiTextBox::setText( const CSulString& text )
{
	m_rText->setText( text );
}