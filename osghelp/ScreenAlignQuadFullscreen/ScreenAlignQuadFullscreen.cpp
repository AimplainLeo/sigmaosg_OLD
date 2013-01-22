// ScreenAlignQuadFullscreen.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <osgViewer/Viewer>
#include <SigmaUtil/SulScreenAlignedQuad.h>
#include <SigmaUtil/SulGeomQuad.h>
#include <SigmaUtil/SulTexCam.h>
#include <SigmaUtil/SulTestGeoms.h>
#include <SigmaUtil/SulViewQuad.h>

osg::Node* createScene1()
{
	CSulScreenAlignedQuad* p = new CSulScreenAlignedQuad;
	p->setTexture( "osghelptex.tga" );
	return p;
}

osg::Node* createScene2()
{
	osg::Group* group = new osg::Group;

	// creates a unit length quad
	CSulGeomQuad* quad = new CSulGeomQuad(2,2);
	quad->setTexture( "osghelptex.tga" );

	// render objects to a texture
	CSulTexCam* texCam = new CSulTexCam;
	texCam->addChild( new CSulGeode(quad) );
	group->addChild( texCam );

	// let's see the rendered texture
	CSulScreenAlignedQuad* p = new CSulScreenAlignedQuad;
	p->setTexture( texCam->getTexture() );
	group->addChild( p );

	return group;
}

osg::Node* createScene3()
{
	// deprecated
	CSulViewQuad* quad = new CSulViewQuad;
	quad->getGeom()->setTexture( "osghelptex.tga" );
	return quad;
}

int _tmain(int argc, _TCHAR* argv[])
{
    // construct the viewer
    osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer;

    // create view
    viewer->setUpViewInWindow( 32, 32, 512, 512 );

    // set the scene-graph data the viewer will render
    viewer->setSceneData( createScene3() );

    // execute main loop
    return viewer->run();
}
