// SulGeodeParachute.cpp

#include "stdafx.h";
#include "SulGeodeParachute.h"
#include "SulGeomHemiSphere.h"
#include "SulGeomLines.h"
#include "SulGeomCylinder.h"
#include <osgDB/ReadFile>
#include <osgDB/FileUtils>

CSulGeodeParachute::CSulGeodeParachute() :
CSulGeode()
{
	float chordLength = 2.5f;
	sigma::uint32 chordCount = 6;
	float chuteRadius = 1.0f;

	CSulGeomHemiSphere* geomChute = new CSulGeomHemiSphere( osg::Vec3(0,0,chordLength), chuteRadius );
	addDrawable( geomChute );

	/**/
	osg::Texture2D* tex = new osg::Texture2D;
	osg::Image* pImage = osgDB::readImageFile( osgDB::findDataFile("camouflage.tga") );
	tex->setImage( pImage );
	tex->setWrap( osg::Texture::WRAP_S, osg::Texture::REPEAT );
	tex->setWrap( osg::Texture::WRAP_T, osg::Texture::REPEAT );
	geomChute->getOrCreateStateSet()->setTextureAttributeAndModes( 0, tex, osg::StateAttribute::ON );
	/**/

	// calc actual length of chord
	float l = sqrt(chuteRadius*chuteRadius+chordLength*chordLength);

	float rad = (2.0f*osg::PI)/chordCount;
	for ( sigma::uint32 i=0; i<chordCount; i++ )
	{
		float x = cos(rad*i);
		float y = sin(rad*i);

		// we need to rotate
		CSulGeomCylinder* chord = new CSulGeomCylinder( 0.01f, l, 6 );
		osg::Matrix m;
		osg::Vec3 pos = osg::Vec3(x,y,chordLength);
		pos.normalize();
		m.makeRotate(
			osg::Vec3(0,0,1),
			pos
		);
		chord->applyMatrix( m );
		addDrawable( chord );
	}
}