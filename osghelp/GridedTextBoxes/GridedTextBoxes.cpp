// GridedTextBoxes.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <osgViewer/Viewer>
#include <osg/ShapeDrawable>
#include <SigmaUtil/SulPlasma.h>
#include <SigmaUtil/SulGeomGrid.h>
#include <SigmaUtil/SulGeomGriddedText.h>
#include <osg/MatrixTransform>
#include <osgText/Text>

#include <SigmaUtil/SulGeodeCrosshairs.h>
#include <SigmaUtil/SulGeodeCircle.h>

osg::Node* createScene()
{
	osg::Group* group = new osg::Group;

	CSulGeomGriddedText* gt = new CSulGeomGriddedText( 5, 2, 32, 32, 512, 512 );
	group->addChild( gt );
 
	gt->setText( "WAIT", 0, CSulGeomGriddedText::ALIGN_LEFT );
	gt->setText( "123", 1, CSulGeomGriddedText::ALIGN_RIGHT );


	// test crosshairs here too (should rename this project to something else)
	CSulGeodeCircle* p = new CSulGeodeCircle( 10 );
	group->addChild( p );

	CSulGeodeCrosshairs* crosshair = new CSulGeodeCrosshairs( CSulGeodeCrosshairs::TYPE_M99, 10.0f );
	group->addChild( crosshair );

	return group;
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
