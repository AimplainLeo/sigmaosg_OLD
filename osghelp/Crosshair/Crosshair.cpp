// Crosshair.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <osgViewer/Viewer>
#include <SigmaUtil/SulGeodeCrosshairs.h>
#include <SigmaUtil/SulTransScreenAlign.h>

#include <osg/matrixtransform>

osg::Node* createScene()
{
	CSulTransScreenAlign* align = new CSulTransScreenAlign( -256, 256, -256, 256 );
	align->addChild( new CSulGeodeCrosshairs( CSulGeodeCrosshairs::TYPE_M99, 512.0f ) );
	return align->getProjection();
}

int _tmain(int argc, _TCHAR* argv[])
{
    // construct the viewer
    osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer;

    // create window
    viewer->setUpViewInWindow( 32, 32, 512, 512 );

	viewer->realize();

    // set the scene-graph data the viewer will render
    viewer->setSceneData( createScene() );

    // execute main loop
    return viewer->run();
}

