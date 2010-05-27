// ShipShapes.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <osgViewer/Viewer>
#include <osg/ShapeDrawable>
#include <osg/positionattitudetransform>
#include <osgDB/ReadFile>
#include <osg/texture2d>
#include <stdlib.h>

#define SIMWATER ( 2000.0f )
#define MAX_SHIPS ( 100 )

// create 3 statesets
// create cube for osghelp


osg::Node* CreateShip()
{
	float fLength = 20.0f + rand() % 300;

	// ship base
	osg::Geode* pGeode = new osg::Geode();

	// create random texture (the red channel represents the reflection intensity of the ships)
	static osg::Image* pImage32 = 0;
	static osg::Image* pImage128 = 0;
	static osg::Image* pImage255 = 0;

	if ( !pImage32 ) pImage32 = osgDB::readImageFile( "red32.tga" );
	if ( !pImage128 ) pImage128 = osgDB::readImageFile( "red128.tga" );
	if ( !pImage255 ) pImage255 = osgDB::readImageFile( "red255.tga" );

	osg::Texture2D* pTex = new osg::Texture2D;
	switch ( rand() % 3 )
	{
		case 0:
			pTex->setImage( pImage32 );
			break;
		case 1:
			pTex->setImage( pImage128 );
			break;
		case 2:
			pTex->setImage( pImage255 );
			break;
	}
	
	osg::StateSet* pStateSet = pGeode->getOrCreateStateSet();
    pStateSet->setTextureAttributeAndModes( 0, pTex, osg::StateAttribute::ON );

	float length = fLength;
	float width = fLength/3.0f;
	float height = (fLength/15.0f);
	float zpos = height/2.0f;
    pGeode->addDrawable( new osg::ShapeDrawable( new osg::Box(osg::Vec3(0.0f,0.0f,zpos),length,width,height) ) );
	zpos+= height/2.0f;

	// decks
	int iDecks = rand() % 3;
	for ( int i=0; i<iDecks; i++ )
	{
		float f = (float)rand()/(float)RAND_MAX;
		length *= 0.9f;
		height *= 0.5f+((float)rand()/(float)RAND_MAX)*0.7f;
		width *= 0.8f;
		zpos += height/2.0f;
		pGeode->addDrawable( new osg::ShapeDrawable( new osg::Box(osg::Vec3(0.0f,0.0f,zpos),length,width,height) ) );
		zpos += height/2.0f;
	}

	// chimeys, towers
	int iTowers = rand() % (1+((int)(length/40.0f)));
	float len_tmp = length;
	float xofs = length/(1.0f+(float)iTowers);
	width = width/2.0f;
	length = width;
	for ( int i=0; i<iTowers; i++ )
	{
		float xpos = xofs*(i+1) - len_tmp/2.0f;
		height = fLength/10.0f;
		zpos += height/2.0f;
		pGeode->addDrawable( new osg::ShapeDrawable( new osg::Box(osg::Vec3(xpos,0.0f,zpos),length,width,height) ) );
		zpos -= height/2.0f;
	}

	osg::PositionAttitudeTransform* pPat = new osg::PositionAttitudeTransform;
	int max = SIMWATER - 500;
	float x = ( rand() % max ) - max/2.0f;
	float y = ( rand() % max ) - max/2.0f;
    pPat->setPosition( osg::Vec3(x, y, 0) );    
	pPat->setAttitude( 
		osg::Quat( (((float)rand()/(float)RAND_MAX))*osg::PI*2.0f, osg::Vec3(0,0,1) ) 
		);
	pPat->addChild( pGeode );

	return pPat;
}

osg::Node* CreateShip2()
{
	float fLength = 20.0f + rand() % 300;

	// ship base
	osg::Geode* pGeode = new osg::Geode();

	// create random texture (the red channel represents the reflection intensity of the ships)
	static osg::Image* pImage32 = 0;
	static osg::Image* pImage128 = 0;
	static osg::Image* pImage255 = 0;

	if ( !pImage32 ) pImage32 = osgDB::readImageFile( "red32.tga" );
	if ( !pImage128 ) pImage128 = osgDB::readImageFile( "red128.tga" );
	if ( !pImage255 ) pImage255 = osgDB::readImageFile( "red255.tga" );

	osg::Texture2D* pTex = new osg::Texture2D;
	switch ( rand() % 3 )
	{
		case 0:
			pTex->setImage( pImage32 );
			break;
		case 1:
			pTex->setImage( pImage128 );
			break;
		case 2:
			pTex->setImage( pImage255 );
			break;
	}
	
	osg::StateSet* pStateSet = pGeode->getOrCreateStateSet();
    pStateSet->setTextureAttributeAndModes( 0, pTex, osg::StateAttribute::ON );

	float length = fLength;
	float width = fLength/3.0f;
	float height = (fLength/15.0f);
	float zpos = height/2.0f;
    pGeode->addDrawable( new osg::ShapeDrawable( new osg::Box(osg::Vec3(0.0f,0.0f,zpos),length,width,height) ) );
	zpos+= height/2.0f;

	// decks
	int iDecks = rand() % 3;
	for ( int i=0; i<iDecks; i++ )
	{
		float f = (float)rand()/(float)RAND_MAX;
		length *= 0.9f;
		height *= 0.5f+((float)rand()/(float)RAND_MAX)*0.7f;
		width *= 0.8f;
		zpos += height/2.0f;
		pGeode->addDrawable( new osg::ShapeDrawable( new osg::Box(osg::Vec3(0.0f,0.0f,zpos),length,width,height) ) );
		zpos += height/2.0f;
	}

	// chimeys, towers
	int iTowers = rand() % (1+((int)(length/40.0f)));
	float len_tmp = length;
	float xofs = length/(1.0f+(float)iTowers);
	width = width/2.0f;
	length = width;
	for ( int i=0; i<iTowers; i++ )
	{
		float xpos = xofs*(i+1) - len_tmp/2.0f;
		height = fLength/10.0f;
		zpos += height/2.0f;
		pGeode->addDrawable( new osg::ShapeDrawable( new osg::Box(osg::Vec3(xpos,0.0f,zpos),length,width,height) ) );
		zpos -= height/2.0f;
	}

	osg::PositionAttitudeTransform* pPat = new osg::PositionAttitudeTransform;
	int max = SIMWATER - 500;
	float x = ( rand() % max ) - max/2.0f;
	float y = ( rand() % max ) - max/2.0f;
    pPat->setPosition( osg::Vec3(x, y, 0) );    
	pPat->setAttitude( 
		osg::Quat( (((float)rand()/(float)RAND_MAX))*osg::PI*2.0f, osg::Vec3(0,0,1) ) 
		);
	pPat->addChild( pGeode );

	return pPat;
}

osg::Node* CreateScene()
{
	osg::Group* pGroup = new osg::Group;

	osg::Geode* pGeode = new osg::Geode();

	// create 500km x 500km plane to simulate water area
    pGeode->addDrawable( new osg::ShapeDrawable( new osg::Box(osg::Vec3(0.0f,0.0f,-1.0f),SIMWATER,SIMWATER,2.0f) ) );
	pGroup->addChild( pGeode );

	for ( int i=0; i<MAX_SHIPS; i++ )
	{
		pGroup->addChild( CreateShip() );
	}


	return pGroup;
}

int _tmain(int argc, _TCHAR* argv[])
{
    // construct the viewer
    osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer;

    // make the viewer create a 512x512 window and position it at 32, 32
    viewer->setUpViewInWindow( 32, 32, 512, 512 );

    // set the scene-graph data the viewer will render
    viewer->setSceneData( CreateScene() );

    // execute main loop
    return viewer->run();
}


