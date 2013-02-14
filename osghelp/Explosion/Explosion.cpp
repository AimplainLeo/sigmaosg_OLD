// Explosion.cpp : Defines the entry point for the console application
//

#include "stdafx.h"
#include "ParticleDebrisSystem.h"
#include <SigmaUtil/SulGeomGrid.h>
#include <SigmaUtil/SulGeode.h>
#include <SigmaUtil/SulScreenAlignedQuad.h>
#include <SigmaUtil/SulCameraManipulatorDebugger.h>
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
#include <osgParticle/ParticleSystemUpdater>
#include <osgParticle/ConnectedParticleSystem>

// convienence global variable
static osg::Group* group = 0;
static osg::Vec3 wind( 1,0,0 );

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


osg::Node* createFireBall( const osg::Vec3& pos, int binNum )
{
	osgParticle::SmokeEffect* smoke =  new osgParticle::SmokeEffect(false);
	smoke->getOrCreateStateSet()->setRenderBinDetails( binNum, "DepthSortedBin" );
	smoke->setPosition( pos );
	smoke->setIntensity( 50.0f );
	smoke->setScale( 1.5f );
	smoke->setParticleDuration( 1.0f );
	smoke->setEmitterDuration( 0.5f );
	smoke->buildEffect();

	osgParticle::Particle& ptemplate = smoke->getParticleSystem()->getDefaultParticleTemplate();
	ptemplate.setAlphaRange(osgParticle::rangef(1.0f, -1.0f));

	osgParticle::ModularEmitter* me = dynamic_cast<osgParticle::ModularEmitter*>(smoke->getEmitter());

	osgParticle::RadialShooter* shooter = dynamic_cast<osgParticle::RadialShooter*>(me->getShooter());
	osgParticle::rangef r = shooter->getThetaRange();

	// I think osg has reversed the mening of theta and phi
	shooter->setThetaRange( 0.0f, osg::PI_2 );		// up/down
	shooter->setPhiRange( 0.0f, 2.0f*osg::PI );		// around
	shooter->setInitialSpeedRange( 0, 4 );
	shooter->setInitialRotationalSpeedRange(
		osg::Vec3(0, 0, -1),
		osg::Vec3(0, 0, 1)
	);

	osgParticle::SectorPlacer* placer = dynamic_cast<osgParticle::SectorPlacer*>(me->getPlacer());
	placer->setRadiusRange(0.0f,2.5f);

	osgParticle::FluidProgram* fluid = dynamic_cast<osgParticle::FluidProgram*>(smoke->getProgram());
	fluid->setFluidDensity(1.0f);	
	fluid->setWind( wind );

	smoke->getParticleSystem()->setDefaultAttributes( "c:/myexp.png" );

	return smoke;
}


osg::Node* createAnimatedBurningSmoke( const osg::Vec3& pos, int binNum )
{
	osg::Group* group = new osg::Group;
	group->getOrCreateStateSet()->setRenderBinDetails( binNum, "DepthSortedBin" );

	osgParticle::Particle pexplosion;
	pexplosion.setLifeTime(10);
    pexplosion.setSizeRange(osgParticle::rangef(0.75f, 3.0f));
    pexplosion.setAlphaRange(osgParticle::rangef(0.0f, 1.0f));
    pexplosion.setColorRange(osgParticle::rangev4(
        osg::Vec4(1, 1, 1, 1), 
        osg::Vec4(1, 1, 1, 1))
	);
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
	program->setWind(wind);
	group->addChild(program);
    
    osg::Geode *geode = new osg::Geode;
    geode->addDrawable(ps);
	group->addChild( geode );

	return group;
}


osg::Node* createDarkSmoke( const osg::Vec3& pos )
{
	osgParticle::SmokeEffect* smoke =  new osgParticle::SmokeEffect(false);
	smoke->setTextureFileName( "c:/smoke.png" );
	smoke->getOrCreateStateSet()->setRenderBinDetails( 10000, "DepthSortedBin" );
	smoke->setPosition( pos );
	smoke->setIntensity( 50.0f );
	smoke->setScale( 1.5f );
	smoke->setParticleDuration( 20.5f );
	smoke->setEmitterDuration( 2.0f );
	smoke->buildEffect();
	/*
	// make smoke dark
	osgParticle::Particle& ptemplate = smoke->getParticleSystem()->getDefaultParticleTemplate();
	ptemplate.setColorRange(osgParticle::rangev4(
                            osg::Vec4(0.5f, 0.5f, 0.5f, 1.0f), 
                            osg::Vec4(0.2f, 0.2f, 0.2f, 0.5f)));
	

	// make smoke disapear downwards
	//ptemplate.setAlphaRange(osgParticle::rangef(1.0f, -1.0f));
	ptemplate.setAlphaRange(osgParticle::rangef(0.2f, 1.0f));
	*/
	osgParticle::ModularEmitter* me = dynamic_cast<osgParticle::ModularEmitter*>(smoke->getEmitter());

	osgParticle::RadialShooter* shooter = dynamic_cast<osgParticle::RadialShooter*>(me->getShooter());
	osgParticle::rangef r = shooter->getThetaRange();

	// I think osg has reversed the mening of theta and phi
	shooter->setThetaRange( 0.0f, osg::PI_2 );		// up/down
	shooter->setPhiRange( 0.0f, 2.0f*osg::PI );		// around
	shooter->setInitialSpeedRange( 0, 4 );

	osgParticle::SectorPlacer* placer = dynamic_cast<osgParticle::SectorPlacer*>(me->getPlacer());
	placer->setRadiusRange(0.0f,2.0f);

	osgParticle::FluidProgram* fluid = dynamic_cast<osgParticle::FluidProgram*>(smoke->getProgram());
	fluid->setFluidDensity(1.0f);	

	//smoke->getParticleSystem()->setDefaultAttributes( "Images/smoke.rgb" );

	return smoke;
}

osg::Node* createDarkSmoke2( const osg::Vec3& pos, int binNum )
{
	osgParticle::SmokeEffect* smoke =  new osgParticle::SmokeEffect(false);
	smoke->setTextureFileName( "c:/smoke.png" );
	smoke->getOrCreateStateSet()->setRenderBinDetails( binNum, "DepthSortedBin" );
	smoke->setPosition( pos );
	smoke->setIntensity( 50.0f );
	smoke->setScale( 1.0f );
	smoke->setParticleDuration( 5 );
	smoke->setEmitterDuration( 2.0f );
	smoke->buildEffect();

	// make smoke dark
	osgParticle::Particle& ptemplate = smoke->getParticleSystem()->getDefaultParticleTemplate();
	ptemplate.setColorRange(osgParticle::rangev4(
                            osg::Vec4(0.5f, 0.5f, 0.5f, 1.0f), 
                            osg::Vec4(0.2f, 0.2f, 0.2f, 0.5f)));
	

	// make smoke disapear downwards
	//ptemplate.setAlphaRange(osgParticle::rangef(1.0f, -1.0f));
	ptemplate.setAlphaRange(osgParticle::rangef(0.2f, 1.0f));

	osgParticle::ModularEmitter* me = dynamic_cast<osgParticle::ModularEmitter*>(smoke->getEmitter());

	osgParticle::RadialShooter* shooter = dynamic_cast<osgParticle::RadialShooter*>(me->getShooter());
	osgParticle::rangef r = shooter->getThetaRange();

	// I think osg has reversed the mening of theta and phi
	shooter->setThetaRange( 0.0f, osg::PI_2 );		// up/down
	shooter->setPhiRange( 0.0f, 2.0f*osg::PI );		// around
	shooter->setInitialSpeedRange( 0, 4 );

	osgParticle::SectorPlacer* placer = dynamic_cast<osgParticle::SectorPlacer*>(me->getPlacer());
	placer->setRadiusRange(0.0f,3.0f);

	osgParticle::FluidProgram* fluid = dynamic_cast<osgParticle::FluidProgram*>(smoke->getProgram());
	fluid->setFluidDensity(1.0f);	
	fluid->setWind( wind );

	//smoke->getParticleSystem()->setDefaultAttributes( "Images/smoke.rgb" );
	//smoke->getParticleSystem()->setDefaultAttributes( "c:/smoke.png" );

	return smoke;
}

osg::Node* createLightSmoke( const osg::Vec3& pos )
{
	// dark smoke
	float scaleSmoke = 2.5f;
	float intensitySmoke = 50.0f;
	osgParticle::SmokeEffect* smoke =  new osgParticle::SmokeEffect(pos, scaleSmoke, intensitySmoke);
	smoke->setParticleDuration( 1.5f );
	smoke->setEmitterDuration( 2.0f );
	smoke->getOrCreateStateSet()->setRenderBinDetails( 5000, "DepthSortedBin" );
	return smoke;
}

osg::Node* createAnimatedInitialSmoke( const osg::Vec3& pos, int binNum )
{
	osg::Group* group = new osg::Group;
	group->getOrCreateStateSet()->setRenderBinDetails( binNum, "DepthSortedBin" );

	osgParticle::Particle pexplosion;
	pexplosion.setLifeTime(5);
    pexplosion.setSizeRange(osgParticle::rangef(0.75f, 3.0f));
    pexplosion.setAlphaRange(osgParticle::rangef(1.0f, 0.0f));
    pexplosion.setColorRange(osgParticle::rangev4(
        osg::Vec4(1, 1, 1, 1), 
        osg::Vec4(1, 1, 1, 1))
	);
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
	emitter->setLifeTime( 5 );
    emitter->setParticleSystem(ps);
    emitter->setParticleTemplate(pexplosion);

	// counter
    osgParticle::RandomRateCounter* counter = new osgParticle::RandomRateCounter;
    counter->setRateRange(50, 50);
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
	program->setWind(wind);
	group->addChild(program);
    
    osg::Geode *geode = new osg::Geode;
    geode->addDrawable(ps);
	group->addChild( geode );

	return group;
}

osg::Node* createImpactRing( const osg::Vec3& pos, int binNum )
{
	osg::MatrixTransform* mt = new osg::MatrixTransform;

	mt->getOrCreateStateSet()->setMode( GL_BLEND, osg::StateAttribute::ON );

	osg::Matrix m;
	m.makeTranslate( pos );
	mt->setMatrix( m );

	CSulGeomQuad* quad = new CSulGeomQuad;
	quad->setTexture( "c:/smoke.png" );

	CSulGeode* geode = new CSulGeode( quad );
	mt->addChild( geode );

	return mt;
}

osg::Node* createFlyingDebris_old( const osg::Vec3& pos, int binNum )
{
	osg::Group* group = new osg::Group;
	group->getOrCreateStateSet()->setRenderBinDetails( binNum, "DepthSortedBin" );

	osgParticle::Particle pexplosion;
	pexplosion.setLifeTime(3);
    pexplosion.setSizeRange(osgParticle::rangef(0.3f, 0.0f));
    pexplosion.setAlphaRange(osgParticle::rangef(1.0f, 1.0f));
    pexplosion.setColorRange(osgParticle::rangev4(
        osg::Vec4(1, 1, 1, 1), 
        osg::Vec4(1, 1, 1, 1))
	);
	pexplosion.setRadius(0.5f);
	 pexplosion.setMass(1/1.2f);
	pexplosion.setTextureTileRange(
		4, 4, 
		0, 15
	);

	// particle system
	osgParticle::ParticleSystem *ps = new osgParticle::ConnectedParticleSystem;
	ps->setDefaultAttributes("c:/animated_smoke.png", false, false);
	
    osgParticle::ParticleSystemUpdater *psu = new osgParticle::ParticleSystemUpdater;
    psu->addParticleSystem(ps);
	group->addChild(psu);

	// emitter
    osgParticle::ModularEmitter *emitter = new osgParticle::ModularEmitter;
	emitter->setNumParticlesToCreateMovementCompensationRatio(100);
	group->addChild( emitter );
	//emitter->setNumParticlesToCreateMovementCompensationRatio(1.5f);
	emitter->setEndless( false );
	emitter->setStartTime( 0 );
	emitter->setLifeTime( 3 );
    emitter->setParticleSystem(ps);
    emitter->setParticleTemplate(pexplosion);

	// counter
    osgParticle::RandomRateCounter* counter = new osgParticle::RandomRateCounter;
    counter->setRateRange(100, 100);
    emitter->setCounter(counter);

	// sector
    osgParticle::SectorPlacer *placer = new osgParticle::SectorPlacer;
    placer->setCenter( pos );
    placer->setRadiusRange(0, 0);
	placer->setPhiRange(0, 0);    // 360° angle to make a circle
    emitter->setPlacer(placer);

	// shooter
    osgParticle::RadialShooter *shooter = new osgParticle::RadialShooter;
    shooter->setInitialSpeedRange(10, 10);
    shooter->setInitialRotationalSpeedRange(osgParticle::rangev3(
       osg::Vec3(0, 0, -5),
       osg::Vec3(0, 0, 5)));
	shooter->setThetaRange( 1.5, 1.5 );		// up/down
	shooter->setPhiRange( 1.5, 1.5 );		// around
    emitter->setShooter(shooter);

	osgParticle::FluidProgram *program = new osgParticle::FluidProgram;
	program->setParticleSystem(ps);
	program->setFluidToAir();
	//program->setWind(wind);
	group->addChild(program);
    
    osg::Geode *geode = new osg::Geode;
    geode->addDrawable(ps);
	group->addChild( geode );

	return group;
}

osg::Node* createFlyingDebris_2( const osg::Vec3& pos, int binNum )
{
	osg::Group* group = new osg::Group;
	group->getOrCreateStateSet()->setRenderBinDetails( binNum, "DepthSortedBin" );


	float scale = 1.0f;
	float intensity = 1.0f;
	osgParticle::SmokeTrailEffect* p = new osgParticle::SmokeTrailEffect( pos, scale, intensity );
	group->addChild( p );


	osgParticle::ModularEmitter* emitter = dynamic_cast<osgParticle::ModularEmitter*>(p->getEmitter());
	osgParticle::RadialShooter* shooter = dynamic_cast<osgParticle::RadialShooter*>(emitter->getShooter());

	shooter->setThetaRange( 0, 2*osg::PI );
	shooter->setPhiRange(  0, 2*osg::PI );

	return group;
}

osg::Node* createFlyingDebris( const osg::Vec3& pos, int binNum )
{
	osg::Group* group = new osg::Group;
	group->getOrCreateStateSet()->setRenderBinDetails( binNum, "DepthSortedBin" );

	CParticleDebrisSystem* p = new CParticleDebrisSystem;
	p->init();
	osg::Matrix m;
	m.makeTranslate( pos );
	p->setMatrix( m );
	group->addChild( p );

	return group;
}


osg::Node* createExplosion( const osg::Vec3& pos )
{
	osg::Group* all = new osg::Group;
	
	// problem with impact is that it needs to conform to the ground shape (have to think about this)
	//all->addChild( createImpactRing(pos, 3000 ) );

	/*
	all->addChild( createFireBall(pos, 4000 ) );
	all->addChild( createAnimatedInitialSmoke(pos, 5000) );
	all->addChild( createAnimatedBurningSmoke(pos, 6000 ) );
	*/
	

	all->addChild( createFlyingDebris( pos, 3000 ) );
	
	

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

	CSulCameraManipulatorDebugger* m = new CSulCameraManipulatorDebugger;
	m->setHomePosition(
		osg::Vec3(20,20,5),
		osg::Vec3(0,0,0),
		osg::Vec3(0,0,1)
		);
	viewer->setCameraManipulator( m );

    // execute main loop
    return viewer->run();
}
