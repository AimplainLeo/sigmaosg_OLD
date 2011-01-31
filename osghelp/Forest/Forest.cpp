// Forest.cpp : Defines the entry point for the console application.
//

// dot -Tpng -O myfile.dot

#include "stdafx.h"
#include <SigmaUtil/SulGeomGrid.h>
#include <SigmaUtil/SulShadow.h>
#include <SigmaUtil/SulGrass.h>
#include <SigmaUtil/SulGenGrassUpdateCallback.h>
#include <osg/node>
#include <osg/group>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osg/fog>
#include <osg/shapedrawable>
#include <osgShadow/ShadowedScene>
#include <osgShadow/ShadowVolume>
#include <osgShadow/ShadowMap>
#include <osgShadow/ParallelSplitShadowMap>
#include <osgShadow/LightSpacePerspectiveShadowMap>
#include <osgGA/TrackballManipulator>
#include <osg/positionattitudetransform>

#define GRIDSIZE 64.0f
#define SPACING_TREES 10.0f
#define SPACING_GRASS 0.2f

#define LIGHTPOS 30.0f, 30.0f, 30.0f

static int ReceivesShadowTraversalMask = 0x1;
static int CastsShadowTraversalMask = 0x2;

#define ZOFS 4

osg::Geode* createGrid()
{
	CSulGeomGrid* pGeomGrid = new CSulGeomGrid;
    pGeomGrid->Create( osg::Vec3(0,0,0), 10, 10, 1, 1, 5, 5 );

    osg::Geode* pGeode = new osg::Geode;
    pGeode->addDrawable( pGeomGrid->getDrawable() );
	
	return pGeode;
}

osg::Geode* createShapes()
{
	osg::Geode* pGeode = new osg::Geode();

	// we create the simplest form of shapes in OpenSceneGraph
	pGeode->addDrawable( new osg::ShapeDrawable( new osg::Sphere(osg::Vec3(0.0f,0.0f,0.5f+ZOFS),0.5f) ) );
	pGeode->addDrawable( new osg::ShapeDrawable( new osg::Box(osg::Vec3(2.0f,0.0f,1.0f+ZOFS),2.0f) ) );
	pGeode->addDrawable( new osg::ShapeDrawable( new osg::Cone(osg::Vec3(4.0f,0.0f,1.0f+ZOFS),0.5f,3.0f) ) );
	pGeode->addDrawable( new osg::ShapeDrawable( new osg::Cylinder(osg::Vec3(6.0f,0.0f,1.5f+ZOFS),0.5f,3.0f) ) );
	pGeode->addDrawable( new osg::ShapeDrawable( new osg::Capsule(osg::Vec3(8.0f,0.0f,2.0f+ZOFS),0.5f,3.0f) ) );

	return pGeode;
}

int _tmain(int argc, _TCHAR* argv[])
{
    // construct the viewer
    osg::ref_ptr<osgViewer::Viewer> rViewer = new osgViewer::Viewer;

	rViewer->getCamera()->setClearColor( osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f) );
	//rViewer->getCamera()->setClearColor( osg::Vec4(0.0f, 0.0f, 0.0f, 0.0f) );

    // make the viewer create a 800x600 window and position it at 32, 32
    rViewer->setUpViewInWindow( 32, 32, 800, 600 );

	osg::Group* pRoot = new osg::Group;
//	osg::Group* pForest = new osg::Group;

	// fog
	osg::Fog* pFog = new osg::Fog;
	pFog->setDensity( 0.009f );
	pFog->setColor( osg::Vec4( 1.0f, 1.0f, 1.0f, 1.0f ) );
//	pForest->getOrCreateStateSet()->setAttributeAndModes( pFog );
	
	// terrain
	osg::Node* pTerrain = osgDB::readNodeFile("terrain_simple.ive");
//	osg::Node* pTerrain = 0;

/*
	// draw a grid for debugging
	osg::Group* pGroupGrid = new osg::Group;
	pGroupGrid->addChild( createGrid() );
	pGroupGrid->getOrCreateStateSet()->setMode( GL_LIGHTING, osg::StateAttribute::OFF );
	pRoot->addChild( pGroupGrid );
*/

//	pTerrain->getOrCreateStateSet()->setMode( GL_LIGHTING, osg::StateAttribute::OFF );
//	pForest->addChild( pTerrain );
//	pTerrain->setNodeMask( ReceivesShadowTraversalMask );

	// grass
	CSulGrass* pGrass = new CSulGrass;
	pGrass->setReceiveShadow( true );
	pGrass->setTextureUnit_HeightMap( 2 );
	pGrass->setTextureFileName( "grass2.tga" );
	pGrass->setSpacing( SPACING_GRASS );
	pGrass->setGridSize( GRIDSIZE );	

	// trees
	CSulGrass* pTrees = new CSulGrass;
	pTrees->setTextureUnit_HeightMap( 2 );
	pTrees->setTextureFileName( "tree_aligned.png" );
	pTrees->setSizeMultiplier( 8.0f );
	pTrees->setSpacing( SPACING_TREES );
	pTrees->setGridSize( GRIDSIZE );	

	// light
	osg::Vec4 lightpos;
	lightpos.set( LIGHTPOS, 0.0f );
    osg::LightSource* ls = new osg::LightSource;
    ls->getLight()->setPosition(lightpos);	
	pRoot->addChild( ls );

	// create a sphere at light source (so we can see where the light comes from)
	osg::Geode* pGeodeSphere = new osg::Geode;
	osg::Sphere* pSphere = new osg::Sphere( osg::Vec3( LIGHTPOS ), 1.0f );
	osg::TessellationHints* pHint = new osg::TessellationHints;
	pHint->setDetailRatio( 2.0f );
	osg::ShapeDrawable* pShapeDrawable = new osg::ShapeDrawable( pSphere, pHint );
	pGeodeSphere->addDrawable( pShapeDrawable );
	pRoot->addChild( pGeodeSphere );

	////////////////////////////////////////////////
	// shadow
	////////////////////////////////////////////////

	osg::Node* pShapes = createShapes();
//	pRoot->addChild( pShapes );

	pRoot->addChild( pTrees->getPat() );
	pRoot->addChild( pTerrain );

	CSulShadow* pShadow = new CSulShadow( pRoot );
	pShadow->setTextureSize( 8192 );
//	pShadow->useShaderFrag( "terrain.frag" );
//	pShadow->useShaderVert( "terrain.vert" );
	pShadow->useShaderFrag( "grass.frag" );
	pShadow->useShaderVert( "grass.vert" );
	pShadow->setLightPosition( osg::Vec3(LIGHTPOS) );
	pShadow->addCaster( pTrees->getPat() );
//	pShadow->addCaster( pShapes );
	//if ( pTerrain ) pShadow->addReceiver( pTerrain );
	if ( pGrass ) pShadow->addChild( pGrass->getPat() );
	pShadow->init();
	pRoot->addChild( pShadow );

	////////////////////////////////////////////////
	// 
	////////////////////////////////////////////////

	// add the scene
	rViewer->setSceneData( pRoot );

	// setup an event handle for statistics
    rViewer->addEventHandler( new osgViewer::StatsHandler );

	rViewer->setCameraManipulator(new osgGA::TrackballManipulator());


	// GRASS texture height
	{
		CSulGenerateHeightTexture* genTexGrass = new CSulGenerateHeightTexture;

		int grid_size = pGrass->getGridSize();

		genTexGrass->create( 
			pTerrain,
			pRoot,
			0, 0,
			1024,			// texture size
			grid_size		// grid size in meters
		);

		rViewer->realize();
		rViewer->frame();

		pGrass->setHeightTexture( genTexGrass->getTexture() );
		pTrees->setHeightTexture( genTexGrass->getTexture() );

		delete genTexGrass;
	}	

/*

	// TREE texture height
	{
		CSulGenerateHeightTexture* genTexTrees = new CSulGenerateHeightTexture;

		int grid_size = pTrees->getGridSize();

		genTexTrees->create( 
			pTerrain,
			pRoot,
			0, 0,
			1024,			// texture size
			grid_size		// grid size in meters
		);

		rViewer->realize();
		rViewer->frame();

		pTrees->setHeightTexture( genTexTrees->getTexture() );

		delete genTexTrees;
	}	

*/

osgDB::writeNodeFile( *pRoot, "myass.dot" );

    // execute main loop
    return rViewer->run();
}
