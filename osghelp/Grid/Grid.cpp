// Grid.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <SigmaUtil/SulGeomGrid.h>
#include <SigmaUtil/SulGeomQuad.h>
#include <osgViewer/Viewer>
#include <osg/ShapeDrawable>
#include <osgGA/TrackballManipulator>

#include <osgDB/WriteFile>

float fscale = 10.0f;

osg::Node* createScene()
{
	// using the sigma utility library to create the grid
	CSulGeomGrid* pGeomGrid = new CSulGeomGrid;
	pGeomGrid->Create( osg::Vec3(-2*fscale,-2*fscale,0), 4*fscale, 4*fscale, 1, 1, 5, 5 );

	return pGeomGrid;
}

osg::Node* createSceneQuad()
{
	CSulGeomQuad* pGeom = new CSulGeomQuad( 10 , 10 );
	return pGeom;
}

int _tmain(int argc, _TCHAR* argv[])
{
    // construct the viewer
    osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer;

    // make the viewer create a 512x512 window and position it at 32, 32
    viewer->setUpViewInWindow( 32, 32, 512, 512 );

	// set the scene-graph data the viewer will render
	osg::Node* pScene = createScene();
    viewer->setSceneData( pScene );

	osgDB::writeNodeFile( *pScene, "grid20x20.osg" );

	// execute main loop
	return viewer->run();
}
