// SulGuiTextBox.cpp

#include "stdafx.h"
#include "SulGuiTextBox.h"

CSulGuiTextBox::CSulGuiTextBox( const CSulString& sText, float x, float y, float w, float h, float size ) :
CSulGuiCanvas( "TEXTBOX", x, y, w, h )
{
	m_size = size;
	m_sText = sText;
}

void CSulGuiTextBox::setupAttr( CSulXmlAttr* pAttr )
{
	CSulGuiCanvas::setupAttr( pAttr );

	m_size = getThemeValue( "size" ).asFloat();
	m_ofs_x = getThemeValue( "ofs_x" ).asFloat();
	m_ofs_y = getThemeValue( "ofs_y" ).asFloat();

	if ( pAttr->exist( "size" ) ) m_size = pAttr->get( "size" ).asFloat();
	if ( pAttr->exist( "ofs_x" ) ) m_ofs_x = pAttr->get( "ofs_x" ).asFloat();
	if ( pAttr->exist( "ofs_y" ) ) m_ofs_x = pAttr->get( "ofs_y" ).asFloat();
}

void CSulGuiTextBox::init()
{
	CSulGuiCanvas::init();

	float h = getH();

	m_rText = new CSulGuiText( m_sText, 0+m_ofs_x, h-m_ofs_y, m_size );
	addChild( m_rText );
}