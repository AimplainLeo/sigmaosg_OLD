// DeferredRenderingMultiView.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <SigmaUtil/SulTestGeoms.h>
#include <SigmaUtil/SulViewRTT.h>
#include <SigmaUtil/SulDeferredCamera.h>
#include <SigmaUtil/SulProgramShaders.h>
#include <osgViewer/Viewer>
#include <osg/drawable>
#include <osg/MatrixTransform>

osg::Node* createSpotLight( CSulLightManager* lm, osg::Vec3& pos, osg::Vec3& dir, osg::Vec4& color )
{
	osg::MatrixTransform* mt = new osg::MatrixTransform;

	osg::Matrix mPos;
	mPos.setTrans( pos );

	osg::Matrix mRot;
	mRot.makeRotate( osg::Z_AXIS, dir );

	osg::Matrix m = mRot * mPos;

	mt->setMatrix( m );

	CSulLightSpot* spot = new CSulLightSpot( lm );
	spot->setDirection( osg::Vec3( 0, 0, 1 ) );
	spot->setDiffuseColor( color );
	spot->setConstantAtt( 0.0005f );
	spot->setLinearAtt( 0.0f );
	spot->setQuadraticAtt( 0.25f );
	spot->setEnabled( true );
	mt->addChild( spot );

	osg::Geode* pGeode = new osg::Geode();
    pGeode->addDrawable( new osg::ShapeDrawable( new osg::Cylinder(osg::Vec3(0.0f,0.0f,0.0f),0.5f,3.0f) ) );
	mt->addChild( pGeode );

	return mt;
}

osg::Node* createScene( osgViewer::Viewer* viewer )
{
	osg::Group* root = new osg::Group;

	// light manager
	CSulLightManager* lm = new CSulLightManager;

	// create test geoms
	CSulTestGeoms* geoms = new CSulTestGeoms;
	root->addChild( geoms );
	lm->addChild( geoms );

	// floor
	osg::Geode* pGeodeFloor = new osg::Geode;
    pGeodeFloor->addDrawable( new osg::ShapeDrawable( new osg::Box(osg::Vec3(0.0f,0.0f,0.0f), 50.0f, 50.0f, 0.1f ) ) );
	lm->addChild( pGeodeFloor );

	lm->addChild( 
		createSpotLight( lm, osg::Vec3(0,0,5), osg::Vec3( 0, 0, -1 ), osg::Vec4(0,1,0,1) )
	);

	// test light
	CSulLightPoint* light = new CSulLightPoint( lm );
	light->setPosition( osg::Vec3(6.0f,0.0f,2.5f) );
	light->setDiffuseColor( osg::Vec4(1,0,0,1) );
	light->setConstantAtt( 0.0005f );
	light->setLinearAtt( 0.0f );
	light->setQuadraticAtt( 0.25f );
	light->setEnabled( true );
	lm->addChild( light );

	// deferred rendering
	CSulDeferredCamera* d1 = new CSulDeferredCamera( lm );

	d1->getOrCreateStateSet()->setAttribute( new CSulProgramShaders( "shaders/sulDeferredRendering.frag,shaders/sulDeferredRendering.vert" ) );

	// we add shaders to the final composite camera, because it is up to the application how to handle the deferred rendering
	d1->getCompositeCamera()->getOrCreateStateSet()->setAttribute( new CSulProgramShaders( 
		"mycomposite.frag,"
		"mycomposite.vert,"
		"shaders/sulFuncLightPoint.frag,"
		"shaders/sulFuncLightSpot.frag"
	) );
	d1->addChild( lm );
	root->addChild( d1 );

	// show texture results
	root->addChild( new CSulScreenAlignedQuad( viewer, d1->getTexture(0),	0 ) );
	root->addChild( new CSulScreenAlignedQuad( viewer, d1->getTexture(1),	200 ) );
	root->addChild( new CSulScreenAlignedQuad( viewer, d1->getTexture(2),	400 ) );
	root->addChild( new CSulScreenAlignedQuad( viewer, d1->getFinalTexture(),	1000 ) );

	return root;
}

int _tmain(int argc, _TCHAR* argv[])
{
    // construct the viewer
    osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer;

    // make the viewer and create window
    viewer->setUpViewInWindow( 32, 32, 1200, 800 );

    // set the scene-graph data the viewer will render
    viewer->setSceneData( createScene(viewer) );

	viewer->setThreadingModel( osgViewer::ViewerBase::SingleThreaded );

    // execute main loop
    return viewer->run();
}

