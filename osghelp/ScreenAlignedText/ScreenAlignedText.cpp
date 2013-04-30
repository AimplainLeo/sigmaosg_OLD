// ScreenAlignedText.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <SigmaUtil/SulProjectionScreenAlignedText.h>
#include <osgViewer/Viewer>
#include <osg/Geometry>
#include <osg/ShapeDrawable>
#include <osg/MatrixTransform>

osg::Node* createScene()
{
	CSulProjectionScreenAlignedText* p = new CSulProjectionScreenAlignedText;
	p->setText( "Hello Screen Aligned Text" );
	p->setPosition( osg::Vec2(0.5,0.3) );
	return p;
}

int _tmain(int argc, _TCHAR* argv[])
{
    // construct the viewer
    osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer;

    // make the viewer create a 512x512 window and position it at 32, 32
    viewer->setUpViewInWindow( 32, 32, 512, 512 );

    // set the scene-graph data the viewer will render
    viewer->setSceneData( createScene() );

    // execute main loop
    return viewer->run();
}
