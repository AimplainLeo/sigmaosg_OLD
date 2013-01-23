// SulGeodeParachute.cpp

#include "stdafx.h";
#include "SulGeodeParachute.h"
#include "SulGeomHemiSphere.h"
#include "SulGeomLines.h"
#include <osgDB/ReadFile>
#include <osgDB/FileUtils>

CSulGeodeParachute::CSulGeodeParachute() :
CSulGeode()
{
	float chordLength = 2.5f;
	sigma::uint32 chordCount = 6;

	CSulGeomHemiSphere* geomChute = new CSulGeomHemiSphere(osg::Vec3(0,0,chordLength));
	addDrawable( geomChute );

	/**/
	osg::Texture2D* tex = new osg::Texture2D;
	osg::Image* pImage = osgDB::readImageFile( osgDB::findDataFile("camouflage.tga") );
	tex->setImage( pImage );
	tex->setWrap( osg::Texture::WRAP_S, osg::Texture::REPEAT );
	tex->setWrap( osg::Texture::WRAP_T, osg::Texture::REPEAT );
	geomChute->getOrCreateStateSet()->setTextureAttributeAndModes( 0, tex, osg::StateAttribute::ON );
	/**/

	CSulGeomLines* chords = new CSulGeomLines;

	float rad = (2.0f*osg::PI)/chordCount;
	for ( sigma::uint32 i=0; i<chordCount; i++ )
	{
		float x = cos(rad*i);
		float y = sin(rad*i);
		chords->addLine( osg::Vec3(x,y,chordLength) );
	}

	chords->createDrawable();

	addDrawable( chords );
}