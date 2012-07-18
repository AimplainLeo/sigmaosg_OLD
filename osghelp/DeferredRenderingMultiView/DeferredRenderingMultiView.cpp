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
#include <osgParticle/Particle>
#include <osgParticle/ParticleSystem>
#include <osgParticle/ParticleSystemUpdater>
#include <osgParticle/ModularEmitter>

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

	// spot light
	osg::Node* spotLight = createSpotLight( lm, osg::Vec3(0,0,5), osg::Vec3( 0, 0, -1 ), osg::Vec4(0,1,0,1) );
	spotLight->setNodeMask( 1 );
	lm->addChild( spotLight );

	// point light
	CSulLightPoint* light = new CSulLightPoint( lm );
	light->setNodeMask( 1 );
	light->setPosition( osg::Vec3(6.0f,0.0f,2.5f) );
	light->setDiffuseColor( osg::Vec4(1,0,0,1) );
	light->setConstantAtt( 0.0005f );
	light->setLinearAtt( 0.0f );
	light->setQuadraticAtt( 0.25f );
	light->setEnabled( true );
	lm->addChild( light );

	// deferred rendering
	CSulDeferredCamera* d1 = new CSulDeferredCamera( lm );
	d1->setCullMask( 1 );

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

	///////////////////////////////
	// setup up view number two
	///////////////////////////////

	// deferred rendering
	CSulDeferredCamera* d2 = new CSulDeferredCamera( lm );
	d2->setCullMask( 2 );

	d2->getOrCreateStateSet()->setAttribute( new CSulProgramShaders( "shaders/sulDeferredRendering.frag,shaders/sulDeferredRendering.vert" ) );

	// we add shaders to the final composite camera, because it is up to the application how to handle the deferred rendering
	d2->getCompositeCamera()->getOrCreateStateSet()->setAttribute( new CSulProgramShaders( 
		"mycomposite.frag,"
		"mycomposite.vert,"
		"shaders/sulFuncLightPoint.frag,"
		"shaders/sulFuncLightSpot.frag"
	) );
	d2->addChild( lm );
	root->addChild( d2 );

	root->addChild( new CSulScreenAlignedQuad( viewer, d2->getFinalTexture(),	1000, 300 ) );

	return root;
}

osgParticle::ParticleSystem *create_simple_particle_system( osg::Group *root )
{
    osgParticle::ParticleSystem *ps = new osgParticle::ParticleSystem;
    ps->setDefaultAttributes("", false, false);

    osgParticle::ModularEmitter *emitter = new osgParticle::ModularEmitter;
    emitter->setParticleSystem(ps);
	osg::MatrixTransform* mt = new osg::MatrixTransform;
	osg::Matrix m;
	m.setTrans( 6.0, 0.0, 2.0 );
	mt->setMatrix( m );
	mt->addChild( emitter );
    root->addChild(mt);

    osgParticle::RandomRateCounter *rrc = 
        static_cast<osgParticle::RandomRateCounter *>(emitter->getCounter());
    rrc->setRateRange(20, 30);    // generate 20 to 30 particles per second


    osg::Geode *geode = new osg::Geode;    
    geode->addDrawable(ps);
    root->addChild(geode);

    return ps;
}

osg::Node* createSceneTestParticles( osgViewer::Viewer* viewer )
{
	osg::Group* root = new osg::Group;

	// light manager
	CSulLightManager* lm = new CSulLightManager;

	// create test geoms
	CSulTestGeoms* geoms = new CSulTestGeoms;
	//root->addChild( geoms );
	lm->addChild( geoms );


	// create particle system
	osgParticle::ParticleSystem *ps1 = create_simple_particle_system(lm);
    osgParticle::ParticleSystemUpdater *psu = new osgParticle::ParticleSystemUpdater;
    psu->addParticleSystem(ps1);
    root->addChild(psu);


	// floor
	osg::Geode* pGeodeFloor = new osg::Geode;
    pGeodeFloor->addDrawable( new osg::ShapeDrawable( new osg::Box(osg::Vec3(0.0f,0.0f,0.0f), 50.0f, 50.0f, 0.1f ) ) );
	lm->addChild( pGeodeFloor );

	// spot light
	osg::Node* spotLight = createSpotLight( lm, osg::Vec3(0,0,5), osg::Vec3( 0, 0, -1 ), osg::Vec4(0,1,0,1) );
	lm->addChild( spotLight );

	// point light
	CSulLightPoint* light = new CSulLightPoint( lm );
	light->setPosition( osg::Vec3(6.0f,0.0f,10.0f) );
	light->setDiffuseColor( osg::Vec4(1,0,0,1) );
	light->setConstantAtt( 0.0005f );
	light->setLinearAtt( 0.0f );
	light->setQuadraticAtt( 0.025f );
	light->setEnabled( true );
	lm->addChild( light );

	// deferred rendering
	CSulDeferredCamera* d1 = new CSulDeferredCamera( lm );

	d1->setReferenceFrame( osg::Transform::ABSOLUTE_RF );
	d1->setViewMatrixAsLookAt( osg::Vec3(30,30,30), osg::Vec3(0,0,0), osg::Z_AXIS );
	d1->setProjectionMatrixAsPerspective( 45, 1, 0.1, 1000 );

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
	root->addChild( new CSulScreenAlignedQuad( viewer, d1->getFinalTexture(),	800, 0, 400, 300 ) );

	///////////////////////////////
	// setup up view number two
	///////////////////////////////

	// deferred rendering
	CSulDeferredCamera* d2 = new CSulDeferredCamera( lm );
	
	d2->getOrCreateStateSet()->setAttribute( new CSulProgramShaders( "shaders/sulDeferredRendering.frag,shaders/sulDeferredRendering.vert" ) );

	// we add shaders to the final composite camera, because it is up to the application how to handle the deferred rendering
	d2->getCompositeCamera()->getOrCreateStateSet()->setAttribute( new CSulProgramShaders( 
		"mycomposite.frag,"
		"mycomposite.vert,"
		"shaders/sulFuncLightPoint.frag,"
		"shaders/sulFuncLightSpot.frag"
	) );
	d2->addChild( lm );
	root->addChild( d2 );

	root->addChild( new CSulScreenAlignedQuad( viewer, d2->getFinalTexture(),	800, 300, 400, 300 ) );

	return root;
}

osg::Node* createOpenLightTest( osgViewer::Viewer* viewer )
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

	osg::LightSource* ls = new osg::LightSource;
	osg::Light &light = *(ls->getLight());

	light.setLightNum( 3 );				// simulate sun

	light.setAmbient( osg::Vec4(0,0,0,0) );
	light.setDiffuse( osg::Vec4(1,1,0,1) );
	
	light.setPosition( osg::Vec4( 0,0,1,0) );

	lm->addChild( ls );
	//root->addChild( ls );
	//d1->addChild( ls );

	return root;
}

int _tmain(int argc, _TCHAR* argv[])
{
    // construct the viewer
    osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer;

    // make the viewer and create window
    viewer->setUpViewInWindow( 32, 32, 1200, 800 );

    // set the scene-graph data the viewer will render
//    viewer->setSceneData( createOpenLightTest(viewer) );
//	viewer->setSceneData( createScene(viewer) );
	viewer->setSceneData( createSceneTestParticles(viewer) );
	
	viewer->setThreadingModel( osgViewer::ViewerBase::SingleThreaded );

	// turn off headlights
	viewer->setLightingMode( osg::View::NO_LIGHT );

    // execute main loop
    return viewer->run();
}

