// LensFlare.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <SigmaUtil/SulScreenAlignedQuad.h>
#include <SigmaUtil/SulTexCam.h>
#include <SigmaUtil/SulNodeCallbackCameraSync.h>
#include <SigmaUtil/SulPostFilterGhost.h>
#include <osgViewer/Viewer>
#include <osg/ShapeDrawable>
#include <osgDB/FileUtils>

osg::Group* CreateScene( osgViewer::Viewer* pViewer )
{
	osg::Group* pGroup = new osg::Group;

	// some object
	osg::Geode* pGeode = new osg::Geode();
	pGeode->addDrawable( new osg::ShapeDrawable( new osg::Sphere(osg::Vec3(0.0f,0.0f,0.0f),0.5f) ) );
	pGroup->addChild( pGeode );

	// render scene to texture
	CSulTexCam* pRTT = new CSulTexCam( 400, 300, CSulTexCam::HDR );
	pRTT->setUpdateCallback( new CSulNodeCallbackCameraSync( pViewer->getCamera() ) );
	pRTT->addChild(	pGeode );
	pGroup->addChild( pRTT );

	// lens flare here
	CSulPostFilterGhost* pPostFilterGhost = new CSulPostFilterGhost;
	pPostFilterGhost->in( pRTT->getTexture() );
	pGroup->addChild( pPostFilterGhost );

	// have a quad to visualize texture
	CSulScreenAlignedQuad* pQuad= new CSulScreenAlignedQuad(
		osg::Vec3(200,150,0),
		400, 300,
		800, 600
	);
	pQuad->setTexture( pPostFilterGhost->out() );
	pGroup->addChild( pQuad->getProjection() );

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

	osg::Group* pScene = CreateScene( rViewer );
	rViewer->setSceneData( pScene );

    // execute main loop
    return rViewer->run();
}

