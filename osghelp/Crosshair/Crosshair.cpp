// Crosshair.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <osgViewer/Viewer>
#include <SigmaUtil/SulGeodeCrosshairs.h>
#include <SigmaUtil/SulTransScreenAlign.h>
#include <SigmaUtil/SulGeomCone.h>
#include <osg/matrixtransform>

osg::Node* createScene()
{
	// make sure our scope parts are screen aligned
	CSulTransScreenAlign* align = new CSulTransScreenAlign( -256, 256, -256, 256 );

	// create simple scope with a hole in it
	float radiusTop = 256.0f;
	float radiusBottom = 500.0f;
	sigma::uint16 slices = 64;
	CSulGeomCone* cone = new CSulGeomCone( 1, radiusBottom, radiusTop, slices );
	cone->setColor( osg::Vec4(0,0,0,0) );
	align->addChild( cone );

	// create sights and tick marks
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

