// ObjectTransformation.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <osgViewer/Viewer>
#include <osgDB/ReadFile>
#include <osg/positionattitudetransform>
#include <osg/matrixtransform>

osg::Node* CreateScene()
{
	osg::Group* pGroup = new osg::Group;

	// load sphere
	osg::Node* pLoadedModel = osgDB::readNodeFile( "sphere.obj" );
	pGroup->addChild( pLoadedModel );

	// load cylinder
	pLoadedModel = osgDB::readNodeFile( "cylinder.obj" );

	// move cylinder to the left of the sphere using osg::PositionAttitudeTransform
	osg::ref_ptr<osg::PositionAttitudeTransform> rPat = new osg::PositionAttitudeTransform;
	rPat->setPosition( osg::Vec3(-1100, 0, 0) );
	rPat->addChild( pLoadedModel );
	pGroup->addChild( rPat.get() );

	// load pyramid
	pLoadedModel = osgDB::readNodeFile( "pyramid.obj" );

	// move pyramid to the right of the sphere using osg::TransformMatrix
	osg::MatrixTransform* pMatTrans = new osg::MatrixTransform;
	pMatTrans->addChild( pLoadedModel );

	osg::Matrix m;
	m.setTrans( 1100, 0, 0 );
	pMatTrans->setMatrix( m );

	pGroup->addChild( pMatTrans );

	return pGroup;
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