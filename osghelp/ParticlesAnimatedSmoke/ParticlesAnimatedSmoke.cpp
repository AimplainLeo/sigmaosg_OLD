// ParticlesAnimatedSmoke.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <SigmaUtil/SulGeomGrid.h>
#include <SigmaUtil/SulGeode.h>
#include <SigmaUtil/SulScreenAlignedQuad.h>
#include <SigmaUtil/SulManipulatorCamera.h>
#include <osgViewer/Viewer>
#include <osgParticle/particle>
#include <osgParticle/SmokeTrailEffect>
#include <osgParticle/SmokeEffect>
#include <osg/BlendFunc>
#include <osgManipulator/Projector>
#include <osgParticle/RadialShooter>
#include <osgParticle/ModularEmitter>
#include <osgParticle/SectorPlacer>
#include <osgParticle/FluidProgram>
#include <osgParticle/ModularProgram>
#include <osgParticle/AccelOperator>
#include <osgParticle/ParticleSystemUpdater>

// convienence global variable
static osg::Group* group = 0;

osg::Node* createExplosion( const osg::Vec3& pos );

class CInputHandler : public osgGA::GUIEventHandler 
{
public:
	CInputHandler()
	{
		osg::Plane plane( osg::Vec3(0,0,1), osg::Vec3(0,0,0) );
		m_rPlaneProj = new osgManipulator::PlaneProjector;
		m_rPlaneProj->setPlane( plane );
	}

    virtual bool handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa, osg::Object* pObject, osg::NodeVisitor* pNodeVisitor )
    {
        osgViewer::Viewer* pViewer = dynamic_cast<osgViewer::Viewer*>(&aa);
        if ( !pViewer )
        {
            return false;
        }

		if ( ea.getEventType()==osgGA::GUIEventAdapter::PUSH )
        {
            osg::Camera* cam = pViewer->getCamera();

			osg::Vec3d v;
			osgManipulator::PointerInfo pi;
		
			pi.reset();
			pi.setCamera( cam );
			pi.setMousePosition( ea.getX(), ea.getY() );
			m_rPlaneProj->project( pi, v );
			
			group->addChild( createExplosion( v ) );

            return true; // return true, event handled
        }

        return false;
    }

private:
	osg::ref_ptr<osgManipulator::PlaneProjector> m_rPlaneProj;
};

osg::Node* createAnimatedSmoke( const osg::Vec3& pos )
{
	osg::Group* group = new osg::Group;

	osgParticle::Particle pexplosion;
	pexplosion.setLifeTime(10);
    pexplosion.setSizeRange(osgParticle::rangef(0.75f, 3.0f));
    pexplosion.setAlphaRange(osgParticle::rangef(0.0f, 1.0f));
    pexplosion.setColorRange(osgParticle::rangev4(
        osg::Vec4(1, 1, 1, 1), 
        osg::Vec4(1, 1, 1, 1)));
    //pexplosion.setRadius(0.05f);
   // pexplosion.setMass(0.05f);
	pexplosion.setRadius(0.5f);
	 pexplosion.setMass(1/1.5f);
	pexplosion.setTextureTileRange(
		4, 4, 
		0, 15
	);

	// particle system
	osgParticle::ParticleSystem *ps = new osgParticle::ParticleSystem;
	ps->setDefaultAttributes("c:/animated_smoke.png", false, false);
	
    osgParticle::ParticleSystemUpdater *psu = new osgParticle::ParticleSystemUpdater;
    psu->addParticleSystem(ps);
	group->addChild(psu);

	// emitter
    osgParticle::ModularEmitter *emitter = new osgParticle::ModularEmitter;
	group->addChild( emitter );
	emitter->setNumParticlesToCreateMovementCompensationRatio(1.5f);
	emitter->setEndless( false );
	emitter->setStartTime( 0 );
	emitter->setLifeTime( 10 );
    emitter->setParticleSystem(ps);
    emitter->setParticleTemplate(pexplosion);

	// counter
    osgParticle::RandomRateCounter* counter = new osgParticle::RandomRateCounter;
    counter->setRateRange(10, 10);
    emitter->setCounter(counter);

	// sector
    osgParticle::SectorPlacer *placer = new osgParticle::SectorPlacer;
    placer->setCenter( pos );
    placer->setRadiusRange(0, 2.5);
	placer->setPhiRange(0, 2 * osg::PI);    // 360° angle to make a circle
    emitter->setPlacer(placer);

	// shooter
    osgParticle::RadialShooter *shooter = new osgParticle::RadialShooter;
    shooter->setInitialSpeedRange(0, 0);
    shooter->setInitialRotationalSpeedRange(osgParticle::rangev3(
       osg::Vec3(0, 0, -1),
       osg::Vec3(0, 0, 1)));
	shooter->setThetaRange( 0.0f, osg::PI_2 );		// up/down
	shooter->setPhiRange( 0.0f, 2.0f*osg::PI );		// around
    emitter->setShooter(shooter);

	osgParticle::FluidProgram *program = new osgParticle::FluidProgram;
	program->setParticleSystem(ps);
	program->setFluidToAir();
	program->setWind(osg::Vec3(1,0,0));
	group->addChild(program);
    
    osg::Geode *geode = new osg::Geode;
    geode->addDrawable(ps);
	group->addChild( geode );

	return group;
}

osg::Node* createExplosion( const osg::Vec3& pos )
{
	osg::Group* all = new osg::Group;

	all->addChild( createAnimatedSmoke(pos) );

	return all;
}

osg::Node* createScene()
{
	group = new osg::Group;

	// create a simple grid
	CSulGeomGrid* grid = new CSulGeomGrid;
	grid->Create( osg::Vec3(-2,-2,0), 40, 40, 1, 1, 5, 5 );
	group->addChild( grid );

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

    // add the handler to the viewer
    viewer->addEventHandler( new CInputHandler );

	CSulManipulatorCamera* m = new CSulManipulatorCamera;
	m->setHomePosition(
		osg::Vec3(20,20,5),
		osg::Vec3(0,0,0),
		osg::Vec3(0,0,1)
		);
	viewer->setCameraManipulator( m );

    // execute main loop
    return viewer->run();
}
