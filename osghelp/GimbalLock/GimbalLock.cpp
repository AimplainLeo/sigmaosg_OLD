// GimbalLock.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <osgViewer/Viewer>
#include <osgDB/ReadFile>
#include <osg/animationpath>
#include <osg/matrixtransform>

osg::Node* CreateScenetmp()
{
	// load our pyramid
	osg::Node* pLoadedModel = osgDB::readNodeFile( "pyramid.obj" );

	// use AnimationPathCallback to demostrate Callback usage
	osg::MatrixTransform* pMatTrans = new osg::MatrixTransform;
	pMatTrans->addChild( pLoadedModel );
	pMatTrans->setUpdateCallback( new osg::AnimationPathCallback(osg::Vec3(0, 0, 0), osg::Z_AXIS, osg::inDegrees(45.0f)) );

	return pMatTrans;
}

osg::Node* CreateScene()
{
	osg::Node* pLoadedModel = osgDB::readNodeFile( "pyramid.obj" );
	
	osg::MatrixTransform* pMatTrans = new osg::MatrixTransform;
	pMatTrans->addChild( pLoadedModel );

	osg::Matrix m = osg::Matrix::rotate( osg::PI_2, osg::Vec3(0,0,1) );
	
	m(0,0) = 0.0f;
	m(1,1) = 0.0f;

	osg::Matrix m1 = osg::Matrix::rotate( osg::PI_2, osg::Vec3(1,0,0) );
	m = m1*m;

	pMatTrans->setMatrix( m );

	return pMatTrans;
}

int _tmain(int argc, _TCHAR* argv[])
{
	// construct the viewer
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer;

	// make the viewer create a 512x512 window and position it at 32, 32
	viewer->setUpViewInWindow( 32, 32, 512, 512 );

	// set the scene-graph data the viewer will render
	osg::Node* pNode = CreateScene();
	if ( pNode )
	{
		viewer->setSceneData( pNode );
	}

	// execute main loop
	return viewer->run();
}