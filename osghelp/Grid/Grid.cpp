// Grid.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <SigmaUtil/SulGeomGrid.h>
#include <osgViewer/Viewer>
#include <osg/ShapeDrawable>
#include <osgGA/TrackballManipulator>

osg::Node* CreateScene()
{
	// using the sigma utility library to create the grid
	CSulGeomGrid* pGeomGrid = new CSulGeomGrid;
	pGeomGrid->Create( osg::Vec3(-2,-2,0), 4, 4, 1, 1, 5, 5 );

	osg::Geode* pGeode = new osg::Geode;
	pGeode->addDrawable( pGeomGrid->GetGeometry() );

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
