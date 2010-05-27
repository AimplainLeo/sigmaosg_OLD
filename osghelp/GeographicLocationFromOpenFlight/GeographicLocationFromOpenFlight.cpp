// GeographicLocationFromOpenFlight.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <osg/node>
#include <osg/group>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgDB/ReadFile>
#include <osgSim/GeographicLocation>


int _tmain(int argc, _TCHAR* argv[])
{
    // construct the viewer
    osg::ref_ptr<osgViewer::Viewer> rViewer = new osgViewer::Viewer;

    // make the viewer create a 800x600 window and position it at 32, 32
    rViewer->setUpViewInWindow( 32, 32, 800, 600 );

	osg::Group* pGroup = new osg::Group;
	osg::Node* node = osgDB::readNodeFile( "target.flt" );
	pGroup->addChild( node );

	rViewer->setSceneData( pGroup );

	osgSim::GeographicLocation* loc = dynamic_cast<osgSim::GeographicLocation*>(node->getUserData());

    // execute main loop
    return rViewer->run();
}


