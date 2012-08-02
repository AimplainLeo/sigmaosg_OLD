// SulGeomCrosshairs.cpp

#include "stdafx.h"
#include "SulGeodeCrosshairs.h"
#include "SulGeomCircle.h"
#include "SulGeomCrosshairSimple.h"
#include "SulGeomTickMarksHorizontal.h"
#include "SulString.h"
#include <osgText/Text>

// fixme : marks and text are not scaled... so it's kinda hardcoded right now

CSulGeodeCrosshairs::CSulGeodeCrosshairs( ETYPE type, float scale, const osg::Vec4& color ) :
CSulGeode(),
m_type( type ),
m_scale( scale ),
m_color( color )
{
	initConstructor();
}

void CSulGeodeCrosshairs::initConstructor()
{
	switch ( m_type )
	{
		case TYPE_M99:
		{
			// create middle circle sight
			CSulGeomCircle*	geomCircle = new CSulGeomCircle( 0.1f*m_scale, m_color );
			addDrawable( geomCircle );

			// create cross
			CSulGeomCrosshairSimple* geomCrosshair = new CSulGeomCrosshairSimple( 0.2f*m_scale, m_scale, m_color );
			addDrawable( geomCrosshair );

			// create tick marks
			CSulGeomTickMarksHorizontal* geomTickMarks = new CSulGeomTickMarksHorizontal( 5, 40, 20, -75 );
			addDrawable( geomTickMarks );

			for ( sigma::uint32 i=0; i<5; i++ )
			{
				float x = i*40;
				createTextNumber( x, -63-32, i*10 );
				createTextNumber( -x, -63-32, i*10 );
			}

		}
		break;
	};
}

void CSulGeodeCrosshairs::createTextNumber( float x, float y, sigma::int32 numText )
{
	osgText::Text* pText = new  osgText::Text;
	pText->setFont( "fonts/DroidSansMono.ttf" );
	pText->setAlignment( osgText::TextBase::CENTER_CENTER );
	pText->setCharacterSize( 32.0f );
	pText->setAxisAlignment( osgText::Text::SCREEN );
	pText->setCharacterSizeMode( osgText::Text::SCREEN_COORDS );
	pText->setPosition( osg::Vec3( x,y,0 ) );
	pText->setColor( osg::Vec4( 0.0f, 0.0f, 0.0f, 1.0f ) );
	pText->setText( CSulString( numText ) );
	addDrawable( pText );
}

