// BarChart.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <SigmaUtil/SulGeomBarChart.h>
#include <osgViewer/Viewer>
#include <osg/ShapeDrawable>
 
osg::Node* CreateScene()
{
	CSulGeomBarChart* p = new CSulGeomBarChart;
	
	p->createBar( "test1" );
	p->createBar( "test2" );
	p->createBar( "test3" );

	p->setBarSize( 0, 100 );
	p->setBarSize( 1, 200 );
	p->setBarSize( 2, 300 );

	// turn off lighting
	p->getOrCreateStateSet()->setMode( GL_LIGHTING, osg::StateAttribute::OFF );

	return p;
}

int _tmain(int argc, _TCHAR* argv[])
{
    // construct the viewer
    osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer;

    // make the viewer create window and position it
    viewer->setUpViewInWindow( 32, 32, 512, 512 );

    // set the scene-graph data the viewer will render
    viewer->setSceneData( CreateScene() );

    // execute main loop
    return viewer->run();
}

