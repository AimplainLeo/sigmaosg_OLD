// SulDebugValueBase.cpp

#include "stdafx.h"
#include "SulDebugValueBase.h"

CSulDebugValueBase::CSulDebugValueBase( const std::string& sTitle ) :
m_sTitle( sTitle )
{
	m_textTitle = new  osgText::Text;
	m_textValue = new  osgText::Text;
	
	m_textTitle->setColor( osg::Vec4( 1.0f, 1.0f, 0.0f, 1.0f ) );
	m_textValue->setColor( osg::Vec4( 1.0f, 1.0f, 0.0f, 1.0f ) );
	
	m_textTitle->setText( sTitle );
	m_textValue->setText( "[Not Set]" );

	m_textTitle->setBackdropType( osgText::Text::OUTLINE );
	m_textValue->setBackdropType( osgText::Text::OUTLINE );
}

void CSulDebugValueBase::setTextPosition( float x, float y )
{
	m_textTitle->setPosition( osg::Vec3( x, y, 0.0f ) );
	m_textValue->setPosition( osg::Vec3( x+128.0f, y, 0.0f ) );
}

void CSulDebugValueBase::setTextColor( const osg::Vec4& c )
{
	m_textTitle->setColor( c );
	m_textValue->setColor( c );
}

void CSulDebugValueBase::setTextValue( const std::string& s )
{
	m_textValue->setText( s );
}

osgText::Text* CSulDebugValueBase::getTextTitleObj()
{
	return m_textTitle;
}

osgText::Text* CSulDebugValueBase::getTextValueObj()
{
	return m_textValue;
}

const std::string& CSulDebugValueBase::getTitle()
{
	return m_sTitle;
}