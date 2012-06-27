// MultiCameraDepthBuffer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <SigmaUtil/SulTestGeoms.h>
#include <SigmaUtil/SulViewRTT.h>
#include <SigmaUtil/SulScreenAlignedQuad.h>
#include <osgViewer/Viewer>
#include <osg/drawable>

osg::Node* createScene( osgViewer::Viewer* viewer )
{
	osg::Group* root = new osg::Group;

	// test geoms
	CSulTestGeoms* geoms = new CSulTestGeoms;
	root->addChild( geoms );

	// render test geoms to a texture and show it
	CSulTexCam* rtt = new CSulTexCam( 1200, 800 );
//	rtt->addChild( geoms );
	root->addChild( rtt );

	CSulScreenAlignedQuad* quadRTT = new CSulScreenAlignedQuad( viewer, rtt->getTexture(0),	0, 0, 200, 150 );
	quadRTT->getOrCreateStateSet()->setMode( GL_DEPTH_TEST, osg::StateAttribute::OFF );
	root->addChild( quadRTT );

	// put a quad bar in the way
	rtt->addChild( new CSulScreenAlignedQuad( viewer, 0, 0, 300, 1200, 300 ) );

	// render the depthbuffer to a texture and show it
	osg::Camera* camMain = viewer->getCamera();
	osg::ref_ptr<osg::Texture2D> tex = new osg::Texture2D;
	tex->setTextureSize( 1200, 800 );
	tex->setInternalFormat( GL_DEPTH_COMPONENT );
	tex->setResizeNonPowerOfTwoHint( false );
	camMain->attach( osg::Camera::DEPTH_BUFFER, tex );

	CSulScreenAlignedQuad* quadDepth = new CSulScreenAlignedQuad( viewer, tex,	400 );
	quadDepth->getOrCreateStateSet()->setMode( GL_DEPTH_TEST, osg::StateAttribute::OFF );
	root->addChild( quadDepth );

	// attach the depthbuffer image to the rtt cam
	rtt->attach( osg::Camera::DEPTH_BUFFER, tex );

	camMain->setClearMask( GL_COLOR_BUFFER_BIT );

	camMain->setComputeNearFarMode( osg::Camera::DO_NOT_COMPUTE_NEAR_FAR );
	camMain->setProjectionMatrixAsPerspective( 45.0f, 1, 0.1, 100 );

	rtt->setComputeNearFarMode( osg::Camera::DO_NOT_COMPUTE_NEAR_FAR );
	rtt->setProjectionMatrixAsPerspective( 45.0f, 1, 0.1, 100 );


	return root;
}

osg::Node* createScene2( osgViewer::Viewer* viewer )
{
	osg::Group* root = new osg::Group;

	sigma::uint32 w = 200;
	sigma::uint32 h = 150;

	// test geoms
	CSulTestGeoms* geoms = new CSulTestGeoms;
	root->addChild( geoms );

	// render test geoms to a texture and show it
	CSulTexCam* rtt2 = new CSulTexCam( w, h );
	rtt2->addChild( new CSulScreenAlignedQuad( w, h, 0, 0, h/2, w, 50 ) );		// bar over
	root->addChild( rtt2 );
	root->addChild( new CSulScreenAlignedQuad( viewer, rtt2->getTexture(0),	w, 0, w, h ) );

	// render test geoms to a texture and show it
	CSulTexCam* rtt3 = new CSulTexCam( w, h );
	rtt3->addChild( geoms );
	root->addChild( rtt3 );
	root->addChild( new CSulScreenAlignedQuad( viewer, rtt3->getTexture(0),	w*3, 0, w, h ) );


	osg::ref_ptr<osg::Texture2D> tex = new osg::Texture2D;
	tex->setTextureSize( w, h );
	tex->setInternalFormat( GL_DEPTH_COMPONENT );
	tex->setResizeNonPowerOfTwoHint( false );
	rtt2->attach( osg::Camera::DEPTH_BUFFER, tex );
	rtt3->attach( osg::Camera::DEPTH_BUFFER, tex );
	root->addChild( new CSulScreenAlignedQuad( viewer, tex,	w*2, 0, w, h ) );

	rtt3->setClearMask( GL_COLOR_BUFFER_BIT );


	return root;
}


int _tmain(int argc, _TCHAR* argv[])
{
    // construct the viewer
    osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer;

    // make the viewer and create window
    viewer->setUpViewInWindow( 32, 32, 1200, 800 );

    // set the scene-graph data the viewer will render
    viewer->setSceneData( createScene2(viewer) );

    // execute main loop
    return viewer->run();
}

