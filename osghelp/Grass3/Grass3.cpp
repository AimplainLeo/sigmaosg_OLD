// Grass3.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "input.h"
#include <SigmaUtil/SulGrass.h>
#include <osg/node>
#include <osg/group>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgDB/ReadFile>

int _tmain(int argc, _TCHAR* argv[])
{
    // construct the viewer
    osg::ref_ptr<osgViewer::Viewer> rViewer = new osgViewer::Viewer;

    // make the viewer create a 800x600 window and position it at 32, 32
    rViewer->setUpViewInWindow( 32, 32, 800, 600 );

	osg::Group* pGroup = new osg::Group;

	// terrain
	osg::Node* terrain = osgDB::readNodeFile("terrain_simple.ive");
	terrain->getOrCreateStateSet()->setMode( GL_LIGHTING, osg::StateAttribute::OFF );
	pGroup->addChild( terrain );

	// grass
	CSulGrass* pGrass = new CSulGrass;
	pGrass->setGridSize( 16 );
	pGroup->addChild( pGrass->getGeode() );

	rViewer->setSceneData( pGroup );

	// add the handler to the viewer
    osg::ref_ptr<CInput> rKeyboardHandler = new CInput( terrain, pGroup, pGrass );
	rViewer->addEventHandler( rKeyboardHandler.get() );

	// setup an event handle for statistics
    rViewer->addEventHandler( new osgViewer::StatsHandler );

    // execute main loop
    return rViewer->run();
}

