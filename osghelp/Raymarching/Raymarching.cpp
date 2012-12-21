// Raymarching.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <osgViewer/Viewer>
#include <SigmaUtil/SulScreenAlignedQuad.h>
#include <SigmaUtil/SulProgramShaders.h>

osg::Node* createScene1()
{
	CSulScreenAlignedQuad* p = new CSulScreenAlignedQuad;
	p->setTexture( "osghelptex.tga" );

	p->getOrCreateStateSet()->setAttribute( 
		new CSulProgramShaders( 
			"test.frag" 
		) 
	);

	return p;
}

int _tmain(int argc, _TCHAR* argv[])
{
    // construct the viewer
    osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer;

    // create view
    viewer->setUpViewInWindow( 32, 32, 512, 512 );

    // set the scene-graph data the viewer will render
    viewer->setSceneData( createScene1() );

    // execute main loop
    return viewer->run();
}


