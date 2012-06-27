// GodRays.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "DebugMenu.h"
#include <SigmaUtil/SulScreenAlignedQuad.h>
#include <SigmaUtil/SulTexCam.h>
#include <SigmaUtil/SulNodeCallbackCameraSync.h>
#include <SigmaUtil/SulPostFilterGodRays.h>
#include <SigmaUtil/SulDebugMenuKeyboardHandler.h>
#include <osgViewer/Viewer>
#include <osg/ShapeDrawable>
#include <osgDB/FileUtils>

#define FS (true)

osg::Group* createLightSource()
{
	osg::Group* pGroup = new osg::Group;

	CSulScreenAlignedQuad* pQuad = new CSulScreenAlignedQuad(
		osg::Vec3(400,300,0),
		800, 600,
		800, 600
	);
	pQuad->setTexture( "sun.tga" );
	pGroup->addChild( pQuad );

	pQuad->getGroup()->getOrCreateStateSet()->setRenderBinDetails( -100, "RenderBin" );

	return pGroup;
}

osg::Group* createScene()
{
	osg::Group* pGroup = new osg::Group;

	osg::Geode* pGeode = new osg::Geode;
    pGeode->addDrawable( new osg::ShapeDrawable( new osg::Sphere(osg::Vec3(0.0f,0.0f,0.0f),0.5f) ) );
    pGeode->addDrawable( new osg::ShapeDrawable( new osg::Box(osg::Vec3(2.0f,0.0f,0.0f),2.0f) ) );
    pGeode->addDrawable( new osg::ShapeDrawable( new osg::Cone(osg::Vec3(4.0f,0.0f,0.0f),0.5f,3.0f) ) );
    pGeode->addDrawable( new osg::ShapeDrawable( new osg::Cylinder(osg::Vec3(6.0f,0.0f,0.5f),0.5f,3.0f) ) );
    pGeode->addDrawable( new osg::ShapeDrawable( new osg::Capsule(osg::Vec3(8.0f,0.0f,1.0f),0.5f,3.0f) ) );
	pGroup->addChild( pGeode );

	return pGroup;
}

osg::Group* createGodRays( osgViewer::Viewer* pViewer, osg::Node* pRender_occluding, osg::Node* pRender_normal )
{
	unsigned int w = FS?800:400;
	unsigned int h = FS?600:300;

	osg::Group* pGroup = new osg::Group;
	
	// render scene to texture (occluding objects, meaning objects that block light)
	CSulTexCam* pRTT_occluding = new CSulTexCam( w, h, CSulTexCam::HDR );
	pRTT_occluding->setUpdateCallback( new CSulNodeCallbackCameraSync( pViewer->getCamera() ) );
	pRTT_occluding->addChild(	pRender_occluding );
	pGroup->addChild( pRTT_occluding );

	// render scene to texture (normal with everything)
	CSulTexCam* pRTT_normal = new CSulTexCam( w, h, CSulTexCam::HDR );
	pRTT_normal->setUpdateCallback( new CSulNodeCallbackCameraSync( pViewer->getCamera() ) );
	pRTT_normal->addChild(	pRender_normal );
	pGroup->addChild( pRTT_normal );

	// have a quad to visualize occluding texture
	CSulScreenAlignedQuad* pQuad_occluding = new CSulScreenAlignedQuad(
		osg::Vec3(700,450,0),
		200, 150,
		800, 600
	);
	pQuad_occluding->setTexture( pRTT_occluding->getTexture() );
	pGroup->addChild( pQuad_occluding );

	// postfilter here
	CSulPostFilterGodRays* pGodRays = new CSulPostFilterGodRays;
	pGodRays->in( pRTT_normal->getTexture(), pRTT_occluding->getTexture() );
	pGroup->addChild( pGodRays );

	// have a quad to visualize texture normal
	CSulScreenAlignedQuad* pQuad_normal = new CSulScreenAlignedQuad(
		osg::Vec3(700,450-180,0),
		200, 150,
		800, 600
	);
	pQuad_normal->setTexture( pRTT_normal->getTexture() );
	pGroup->addChild( pQuad_normal );


	// have a quad to visualize texture (godrays)
	CSulScreenAlignedQuad* pQuad= new CSulScreenAlignedQuad(
		osg::Vec3(w/2,h/2,0),
		w, h,
		800, 600
	);
	pQuad->setTexture( pGodRays->out() );
	pGroup->addChild( pQuad );

	// some debug stuff
	osg::ref_ptr<CDebugMenu> rDebugMenu = new CDebugMenu( pGodRays );
	osg::ref_ptr<CSulDebugMenuKeyboardHandler> rKeyboardHandler = new CSulDebugMenuKeyboardHandler( rDebugMenu );
    pViewer->addEventHandler( rKeyboardHandler );
	pGroup->addChild( rDebugMenu );

	return pGroup;
}

int _tmain(int argc, _TCHAR* argv[])
{
	osgDB::FilePathList filepath;

	char* ptr = getenv( "SIGMAOSG_DATA_PATH" );
	if ( ptr )
	{
		osgDB::convertStringPathIntoFilePathList( ptr, filepath );
		osgDB::appendPlatformSpecificResourceFilePaths(filepath);
		osgDB::setDataFilePathList(filepath);
	}

    // construct the viewer
    osg::ref_ptr<osgViewer::Viewer> rViewer = new osgViewer::Viewer;

	rViewer->setThreadingModel( osgViewer::ViewerBase::SingleThreaded );

    rViewer->setUpViewInWindow( 32, 32, 800, 600 );

	// objects in scene (also used for occluding)
	osg::Group* pScene_objects = new osg::Group;
	pScene_objects->addChild( createScene() );
		
	// scene as it should look like (normal)
	osg::Group* pScene = new osg::Group;
	pScene->addChild( createLightSource() );
	pScene->addChild( pScene_objects );
		
	osg::Group* pRoot = new osg::Group;
	pRoot->addChild( pScene );
	pRoot->addChild( pScene_objects );
	pRoot->addChild( createGodRays( rViewer, pScene_objects, pScene ) );

	rViewer->setSceneData( pRoot );

    // execute main loop
    return rViewer->run();

}
