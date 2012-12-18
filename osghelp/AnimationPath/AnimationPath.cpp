// AnimationPath.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <osgViewer/Viewer>
#include <osg/animationpath>
#include <osg/matrixtransform>
#include <osg/ShapeDrawable>

osg::Node* createScene()
{
    osg::Geode* geode = new osg::Geode();
    geode->addDrawable( new osg::ShapeDrawable( new osg::Box(osg::Vec3(0.0f,0.0f,0.0f),2.0f) ) );

	osg::MatrixTransform* mt = new osg::MatrixTransform;
    mt->addChild( geode );
    
	osg::ref_ptr<osg::AnimationPath> path = new osg::AnimationPath;
	path->setLoopMode( osg::AnimationPath::SWING );
	osg::AnimationPath::ControlPoint pointA(osg::Vec3(-1,0,0));
	osg::AnimationPath::ControlPoint pointB(osg::Vec3( 1,0,0));
	path->insert( 0.0f, pointA );
	path->insert( 2.0f, pointB );

	osg::ref_ptr<osg::AnimationPathCallback> cb = new osg::AnimationPathCallback( path );
	mt->setUpdateCallback( cb );

    return mt;
}

int _tmain(int argc, _TCHAR* argv[])
{
    // construct the viewer
    osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer;

    // make the viewer create a window and position it
	viewer->setUpViewInWindow( 32, 32, 512, 512 );

    // set the scene-graph data the viewer will render
    viewer->setSceneData( createScene() );

    // execute main loop
    return viewer->run();
}