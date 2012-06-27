// PostFilter_NightVision.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <osgViewer/Viewer>
#include <SigmaUtil/SulTexCam.h>
#include <SigmaUtil/SulTestGeoms.h>
#include <SigmaUtil/SulScreenAlignedQuad.h>
#include <SigmaUtil/SulPostFilterNightVision.h>

osg::Node* createScene( osgViewer::Viewer* viewer )
{
	osg::Group* group = new osg::Group;

	// create some geometry, so we can see the effect
	CSulTestGeoms* geoms = new CSulTestGeoms;
	group->addChild( geoms );

	// render the objects to a texture
	CSulTexCam* rtt = new CSulTexCam( 320, 240 );
	rtt->addChild( geoms );
	group->addChild( rtt );

	// show the result of the rtt
	CSulScreenAlignedQuad* quadRTT = new CSulScreenAlignedQuad( viewer, rtt->getTexture(0),	0, 0, 320, 240 );
	group->addChild( quadRTT );
	
	// apply a postfilter to the rtt texture
	CSulPostFilterNightVision* postfilter = new CSulPostFilterNightVision;
	postfilter->in( rtt->getTexture(), 32 );
	group->addChild( postfilter );

	// show the result of the postfilter
	CSulScreenAlignedQuad* quadPostFilter = new CSulScreenAlignedQuad( viewer, postfilter->out(), 400, 0, 320, 240 );
	group->addChild( quadPostFilter );

	return group;
}

int _tmain(int argc, _TCHAR* argv[])
{
    osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer;

	viewer->setUpViewInWindow( 32, 32, 1024, 800 );

    // set the scene-graph data the viewer will render
	osg::Node* root = createScene( viewer );
	viewer->setSceneData( root );

    // execute main loop
	return viewer->run();
}

