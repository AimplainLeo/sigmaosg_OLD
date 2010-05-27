// RadarSim.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <osgViewer/Viewer>
#include <osg/ShapeDrawable>
#include <osg/positionattitudetransform>
#include <osg/texture2d>
#include <stdlib.h>

#define SIMWATER ( 2000.0f )
#define MAX_SHIPS ( 100 )

// row, col
static float simtable[2][2] =
{
	{0.0f,	10.0f},
	{10.0f,	20.0f}
};

// [start angle][end angle][depth-start][depth-end][depth-start][depth-end][depth-start][depth-end]

osg::Node* CreateScene()
{
	osg::Group* pGroup = new osg::Group;


	return pGroup;
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


