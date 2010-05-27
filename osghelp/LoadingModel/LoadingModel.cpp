// LoadingModel.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <osgViewer/Viewer>
#include <osgDB/ReadFile>

osg::Node* CreateScene()
{
	// load model
	osg::Node* pLoadedModel = osgDB::readNodeFile( "torusknot.obj" );
	return pLoadedModel;
}

int _tmain(int argc, _TCHAR* argv[])
{
	// construct the viewer
	osg::ref_ptr<osgViewer::Viewer> rViewer = new osgViewer::Viewer;

	// make the viewer create a 512x512 window and position it at 32, 32
	rViewer->setUpViewInWindow( 32, 32, 512, 512 );

	// set the scene-graph data the viewer will render
	rViewer->setSceneData( CreateScene() );

	// execute main loop
	return rViewer->run();
}
