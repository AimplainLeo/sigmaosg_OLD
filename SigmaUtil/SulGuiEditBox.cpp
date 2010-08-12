// SulGuiEditBox.cpp

#include "stdafx.h"
#include "SulGuiEditBox.h"
#include "SulGuiTextUpdateCallback.h"
#include "SulGuiEventHandler.h"

CSulGuiEditBox::CSulGuiEditBox( const CSulString& sText, float x, float y, float w, float h ) :
CSulGuiCanvas( "EDITBOX", x, y, w, h )
{
	m_sText = sText;

	m_bEditActive = false;

	setUpdateCallback( new CSulGuiTextUpdateCallback( this ) );

	float ofsx = 6.0f;
	float ofsy = 6.0f;
	m_rGuiText = new CSulGuiText( sText, 0.0f+ofsx, h-ofsy, h );
	addChild( m_rGuiText );
}

void CSulGuiEditBox::setupEventHandler( CSulGuiEventHandler* pEventHandler )
{
	CSulGuiCanvas::setupEventHandler( pEventHandler );
	addEvent( CSulGuiEventHandler::EVENT_KEYDOWN );	
	addEvent( CSulGuiEventHandler::EVENT_MOUSE_RELEASE );
}

void CSulGuiEditBox::setMouseRelease( bool bInside )
{
	if ( isActive() )
	{
		m_bEditActive = bInside;
		setBorderColor( bInside?osg::Vec4( 1,1,0,1 ):osg::Vec4( 0,0,1,1 ) );
		m_rGuiText->setText( m_sText );
	}
}

void CSulGuiEditBox::setText( const CSulString& sText )
{
	m_sText = sText;
	m_rGuiText->setText( sText );
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

void CSulGuiEditBox::eventKeyDown( sigma::int32 key, sigma::int32 iMod )
{
	CSulGuiCanvas::eventKeyDown( key, iMod );

	if ( isActive() && m_bEditActive )
	{
		// keys to ignore
		if ( key==osgGA::GUIEventAdapter::KEY_Left ||
			key==osgGA::GUIEventAdapter::KEY_Right )
		{
			return;
		}

		if ( iMod==0 && key==osgGA::GUIEventAdapter::KEY_Return )
		{
			setCursor( "" );
			setMouseRelease( false );
			return;
		}

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

		if (	key!=osgGA::GUIEventAdapter::KEY_Shift_L && 
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
