// Billboard with SigmaUtil.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <SigmaUtil/SulGeomQuad.h>
#include <osgViewer/Viewer>

osg::Node* createScene()
{
	CSulGeomQuad* geom = new CSulGeomQuad( 1, 1, CSulGeomQuad::PLANE_XZ );
	geom->setTexture( "osghelptex.tga" );

	osg::Billboard* billboard = new osg::Billboard;
	billboard->setMode( osg::Billboard::POINT_ROT_EYE );
	billboard->addDrawable( geom );
	
	return billboard;
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

	// execute main loop
	return viewer->run();
}
