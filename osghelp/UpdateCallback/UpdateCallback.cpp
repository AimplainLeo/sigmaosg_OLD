// UpdateCallback.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <osgViewer/Viewer>
#include <osgDB/ReadFile>
#include <osg/AnimationPath>
#include <osg/MatrixTransform>

osg::Node* CreateScene()
{
	// load our pyramid
	osg::Node* pLoadedModel = osgDB::readNodeFile( "pyramid.obj" );

	// use AnimationPathCallback to demostrate Callback usage
	osg::MatrixTransform* pMatTrans = new osg::MatrixTransform;
	pMatTrans->addChild( pLoadedModel );
	pMatTrans->setUpdateCallback( new osg::AnimationPathCallback(osg::Vec3(0, 0, 0), osg::Z_AXIS, osg::inDegrees(45.0f)) );

	return pMatTrans;
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