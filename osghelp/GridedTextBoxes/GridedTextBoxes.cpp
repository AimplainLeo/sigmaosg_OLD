// GridedTextBoxes.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <osgViewer/Viewer>
#include <SigmaUtil/SulGeomGriddedText.h>

osg::Node* createScene()
{
	CSulGeomGriddedText* gt = new CSulGeomGriddedText( 5, 2, 32, 32, 512, 512 );
	gt->setText( "WAIT", 0, CSulGeomGriddedText::ALIGN_LEFT );
	gt->setText( "123", 1, CSulGeomGriddedText::ALIGN_RIGHT );
	return gt;
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
