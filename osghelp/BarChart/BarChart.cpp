// BarChart.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <SigmaUtil/SulGeomBarChart.h>
#include <osgViewer/Viewer>
#include <osg/ShapeDrawable>
 
osg::Node* CreateScene()
{
	CSulGeomBarChart* p = new CSulGeomBarChart;
	
	p->createBar();
	p->createBar();
	p->createBar();

	p->setBarSize( 0, 100 );
	p->setBarSize( 1, 200 );
	p->setBarSize( 2, 300 );

	osg::Geode* pGeode = p->createGeode();

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

    // execute main loop
    return viewer->run();
}

