// StreakingEffect.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <SigmaUtil/SulTexCam.h>
#include <SigmaUtil/SulScreenAlignedQuad.h>
#include <SigmaUtil/SulNodeCallbackCameraSync.h>
#include <SigmaUtil/SulPostFilterStreaking.h>
#include <osgViewer/Viewer>
#include <osg/ShapeDrawable>
#include <osgDB/FileUtils>

osg::Node* CreateScene( osgViewer::Viewer* pViewer )
{
	osg::Group* pGroup = new osg::Group;

	osg::Geode* pGeode = new osg::Geode();

	// we create the simplest form of shapes in OpenSceneGraph
    pGeode->addDrawable( new osg::ShapeDrawable( new osg::Sphere(osg::Vec3(0.0f,0.0f,0.0f),0.5f) ) );
	pGroup->addChild( pGeode );

	// render scene to texture
	CSulTexCam* pRTT = new CSulTexCam( 400, 300, CSulTexCam::HDR );
	pRTT->setUpdateCallback( new CSulNodeCallbackCameraSync( pViewer->getCamera() ) );
	pRTT->addChild(	pGeode );
	pGroup->addChild( pRTT );

	// streaking effect
	CSulPostFilterStreaking* pStreaking = new CSulPostFilterStreaking;
	pStreaking->in( pRTT->getTexture() );
	pGroup->addChild( pStreaking );

	// have a quad to visualize texture
	CSulScreenAlignedQuad* pQuad= new CSulScreenAlignedQuad(
		osg::Vec3(200,150,0),
		400, 300,
		800, 600
	);
	pQuad->setTexture( pStreaking->out() );
	pGroup->addChild( pQuad );

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

	osg::Node* pScene = CreateScene( rViewer );
	rViewer->setSceneData( pScene );

	// set background black for easier viewing of streaks
	rViewer->getCamera()->setClearColor( osg::Vec4(0,0,0,1) );

    // execute main loop
    return rViewer->run();

}

