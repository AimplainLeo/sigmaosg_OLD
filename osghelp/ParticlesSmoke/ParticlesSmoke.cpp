// ParticlesSmoke.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <SigmaUtil/SulGeomGrid.h>
#include <SigmaUtil/SulGeode.h>
#include <osgViewer/Viewer>
#include <osgParticle/ParticleSystemUpdater>
#include <osgParticle/ModularEmitter>
#include <osgParticle/RadialShooter>

/*
osg::Node* createScene()
{
	osg::Group* group = new osg::Group;

	// create a simple grid
	CSulGeomGrid* grid = new CSulGeomGrid;
	grid->Create( osg::Vec3(-2,-2,0), 4, 4, 1, 1, 5, 5 );
	group->addChild( grid );

	// particle system
	osgParticle::ParticleSystem* ps = new osgParticle::ParticleSystem;
	ps->setDefaultAttributes("c:/smoke.png", true, false);
	group->addChild( new CSulGeode(ps) );

	// emitter
	osgParticle::ModularEmitter* emitter = new osgParticle::ModularEmitter;
	emitter->setParticleSystem( ps );
    osgParticle::RandomRateCounter *rrc = static_cast<osgParticle::RandomRateCounter *>(emitter->getCounter());
	rrc->setRateRange(3, 5);    // generate n to m particles per second
	group->addChild( emitter );

    osgParticle::RadialShooter *shooter = new osgParticle::RadialShooter;
    shooter->setInitialSpeedRange(0.1, 0.2);
	// give particles a little spin
    shooter->setInitialRotationalSpeedRange(osgParticle::rangev3(
       osg::Vec3(0, 0, -1),
       osg::Vec3(0, 0, 1)
	));    
	emitter->setShooter(shooter);

	// updater
	osgParticle::ParticleSystemUpdater* psu = new osgParticle::ParticleSystemUpdater;
	psu->addParticleSystem(ps);
	group->addChild( psu );

    return group;
}
*/

#include <osgParticle/SmokeTrailEffect>
#include <osgParticle/SmokeEffect>

osg::Node* createScene()
{
	osg::Group* group = new osg::Group;

	// create a simple grid
	CSulGeomGrid* grid = new CSulGeomGrid;
	grid->Create( osg::Vec3(-2,-2,0), 4, 4, 1, 1, 5, 5 );
	group->addChild( grid );

	osg::Vec3 position(0,0,0);
	float scale = 1.0f;
	float intensity = 1.0f;

	//osgParticle::SmokeTrailEffect* smoke =  new osgParticle::SmokeTrailEffect(position, scale, intensity);
	osgParticle::SmokeEffect* smoke =  new osgParticle::SmokeEffect(position, scale, intensity);
	group->addChild( smoke );

    return group;
}

int _tmain(int argc, _TCHAR* argv[])
{
    // construct the viewer
    osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer;

    // make the viewer create a 512x512 window and position it at 32, 32
    viewer->setUpViewInWindow( 32, 32, 512, 512 );

    // set the scene-graph data the viewer will render
    viewer->setSceneData( createScene() );

    // execute main loop
    return viewer->run();
}
