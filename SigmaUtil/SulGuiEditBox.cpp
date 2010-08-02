// SulGuiEditBox.cpp

#include "stdafx.h"
#include "SulGuiEditBox.h"
#include "SulGuiTextUpdateCallback.h"

CSulGuiEditBox::CSulGuiEditBox( const CSulString& sText, float x, float y, float w, float h ) :
CSulGuiCanvas( x, y, w, h )
{
	m_sText = sText;

	m_bActive = false;

	addEvents( EVENT_KEYDOWN );	
	addEvents( EVENT_MOUSE_RELEASE );
	setUpdateCallback( new CSulGuiTextUpdateCallback( this ) );

	float ofsx = 6.0f;
	float ofsy = 6.0f;
	m_rGuiText = new CSulGuiText( sText, 0.0f+ofsx, h-ofsy, h );
	addChild( m_rGuiText );
}

void CSulGuiEditBox::setMouseRelease( bool bInside )
{
	m_bActive = bInside;
	setBorderColor( bInside?osg::Vec4( 1,1,0,1 ):osg::Vec4( 0,0,1,1 ) );
	m_rGuiText->setText( m_sText );
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
	if ( m_bActive )
	{
		m_sCursor = sCursor;
		m_rGuiText->setText( m_sText+m_sCursor );
	}
}

void CSulGuiEditBox::eventKeyDown( Sigma::int32 key, Sigma::int32 iMod )
{
	CSulGuiCanvas::eventKeyDown( key, iMod );

	if ( m_bActive )
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
