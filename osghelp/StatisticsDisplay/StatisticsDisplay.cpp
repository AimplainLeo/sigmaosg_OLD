// StatisticsDisplay.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <osgViewer/Viewer>
#include <osg/ShapeDrawable>

#include <osgViewer/ViewerEventHandlers>
 
osg::Node* CreateScene()
{
    osg::Geode* pGeode = new osg::Geode();
    pGeode->addDrawable( new osg::ShapeDrawable( new osg::Sphere(osg::Vec3(0.0f,0.0f,0.0f),0.5f) ) );
    return pGeode;
}

int _tmain(int argc, _TCHAR* argv[])
{
    // construct the viewer
    osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer;

    // make the viewer create a 512x512 window and position it at 32, 32
    viewer->setUpViewInWindow( 32, 32, 512, 512 );

    // set the scene-graph data the viewer will render
    viewer->setSceneData( CreateScene() );

	// setup an event handle for statistics
	viewer->addEventHandler( new osgViewer::StatsHandler );

    // execute main loop
    return viewer->run();
}

