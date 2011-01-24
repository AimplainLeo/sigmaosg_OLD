// SulGuiEditBox.cpp

#include "stdafx.h"
#include "SulGuiEditBox.h"
#include "SulGuiTextUpdateCallback.h"
#include "SulGuiEventHandler.h"

CSulGuiEditBox::CSulGuiEditBox( const CSulString& sText, float x, float y, float w, float h, float fontSize ) :
CSulGuiCanvas( "EDITBOX", x, y, w, h )
{
	m_max = 1024;
	m_fontSize = fontSize;
	m_sText = sText;

	m_bEditActive = false;
}

void CSulGuiEditBox::setupTheme( CSulGuiThemeXml* pThemeXml )
{
	CSulGuiCanvas::setupTheme( pThemeXml );

	m_fontSize	= getThemeValue( "font_size" ).asFloat();
	m_ofs_x		= getThemeValue( "ofs_x" ).asFloat();
	m_ofs_y		= getThemeValue( "ofs_y" ).asFloat();
}

void CSulGuiEditBox::setupAttr( CSulXmlAttr* pAttr )
{
	CSulGuiCanvas::setupAttr( pAttr );

	if ( pAttr->exist( "font_size" ) )	m_fontSize  = pAttr->get( "font_size" ).asFloat();
	if ( pAttr->exist( "ofs_x" ) )		m_ofs_x		= pAttr->get( "ofs_x" ).asFloat();
	if ( pAttr->exist( "ofs_y" ) )		m_ofs_x		= pAttr->get( "ofs_y" ).asFloat();
	if ( pAttr->exist( "max" ) )		m_max		= pAttr->get( "max" ).asUint32();
}

void CSulGuiEditBox::setupEventHandler( CSulGuiEventHandler* pEventHandler )
{
	CSulGuiCanvas::setupEventHandler( pEventHandler );

	pEventHandler->signalKeyDown.connect( this, &CSulGuiEditBox::onKeyDown );

	pEventHandler->signalMouseRelease.connect( this, &CSulGuiEditBox::onMouseRelease );
}

void CSulGuiEditBox::init()
{
	CSulGuiCanvas::init();

	setUpdateCallback( new CSulGuiTextUpdateCallback( this ) );

	float h = getH();

	m_rGuiText = new CSulGuiText( m_sText, 0.0f+m_ofs_x, h-m_ofs_y, m_fontSize );
	m_rGuiText->init();
	m_rGuiText->setText( m_sText );
	addChild( m_rGuiText );
}

void CSulGuiEditBox::onMouseRelease( float x, float y )
{
	if ( isActive() )
	{
		bool bInside = isInsideWorld( x, y );
		m_bEditActive = bInside;
		setBorderColor( bInside?osg::Vec4( 1,1,0,1 ):osg::Vec4( 0,0,1,1 ) );
		m_rGuiText->setText( m_sText );

		if ( m_bEditActive )
			setCursor( "|" );
	}

}

void CSulGuiEditBox::setText( const CSulString& sText )
{
	m_sText = sText;
	if ( m_rGuiText.valid() )
	{
		m_rGuiText->setText( sText+m_sCursor );
	}
}

const CSulString& CSulGuiEditBox::getText() const
{
	return m_sText;
}

void CSulGuiEditBox::setCursor( const CSulString& sCursor )
{
	if ( m_bEditActive )
	{
		m_sCursor = sCursor;
		m_rGuiText->setText( m_sText+m_sCursor );
	}
}

void CSulGuiEditBox::onKeyDown( sigma::int32 key, sigma::int32 iMod )
{
	if ( isActive() && m_bEditActive )
	{
		// keys to ignore
		if ( key==osgGA::GUIEventAdapter::KEY_Left ||
			key==osgGA::GUIEventAdapter::KEY_Right )
		{
			return;
		}

		// user presses [return], we end input now
		if ( iMod==0 && key==osgGA::GUIEventAdapter::KEY_Return )
		{
			setCursor( "" );
			setMouseRelease( false );
			return;
		}

		// delete
		if ( key==osgGA::GUIEventAdapter::KEY_BackSpace )
		{
			CSulString sText = getText();
			if ( sText.size() )
			{
				sText = sText.substr(0, sText.size()-1);
				setText( sText );
			}
			return;
		}

		bool bCheck = getText().size()<m_max?true:false;

		if (	bCheck &&
				key!=osgGA::GUIEventAdapter::KEY_Shift_L && 
				key!=osgGA::GUIEventAdapter::KEY_Shift_R && 
				key!=osgGA::GUIEventAdapter::KEY_Control_L &&
				key!=osgGA::GUIEventAdapter::KEY_Control_R 
			)
		{
			CSulString s = getText();
			s += key;
			setText( s );		
		}
	}
}

