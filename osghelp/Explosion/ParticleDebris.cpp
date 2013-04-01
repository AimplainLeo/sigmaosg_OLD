// ParticleDebris.cpp

#include "stdafx.h"
#include "ParticleDebris.h"
#include <osgParticle/ParticleSystemUpdater>

CParticleDebris::CParticleDebris( 
	const osg::Vec3& velocity,
	float debrisPosOffsetMin,
	float debrisPosOffsetMax,
	float particleMass,
	float particleLifeTime,
	float particleSizeMin,
	float particleSizeMax,
	float particleAlphaMin,
	float particleAlphaMax,
	float emitterLifeTime,
	float rateMin,
	float rateMax
) :
CSulParticle( 2.0f, velocity )
{
	m_particleMass		= particleMass;
	m_particleLifeTime	= particleLifeTime;
	m_particleSizeMin	= particleSizeMin;
	m_particleSizeMax	= particleSizeMax;
	m_particleAlphaMin	= particleAlphaMin;
	m_particleAlphaMax	= particleAlphaMax;
	m_emitterLifeTime	= emitterLifeTime;
	m_rateMin			= rateMin;
	m_rateMax			= rateMax;

	m_group = new osg::MatrixTransform;

	m_group->addChild( myAnimatedBurningSmoke( osg::Vec3(0,0,0) , 5000 ) );
	//m_group->addChild( mycreateFireBall( osg::Vec3(0,0,0) , 5000 ) );
		
	m_group->setDataVariance( osg::Object::DYNAMIC );

	/*
	osg::Vec4 color( sigma::rand0to1(),sigma::rand0to1(),sigma::rand0to1(),1 );
	CSulGeomSphere* sphere = new CSulGeomSphere( 0.2f );
	sphere->setColor( color );
	m_group->addChild( sphere );
	*/
}


osg::Node* CParticleDebris::myAnimatedBurningSmoke( const osg::Vec3& pos, int binNum )
{
	osg::Group* group = new osg::Group;
	group->getOrCreateStateSet()->setRenderBinDetails( binNum, "DepthSortedBin" );

	osgParticle::Particle pexplosion;
	pexplosion.setLifeTime(m_particleLifeTime);
    pexplosion.setSizeRange(osgParticle::rangef(m_particleSizeMin, m_particleSizeMax));
    pexplosion.setAlphaRange(osgParticle::rangef(m_particleAlphaMin, m_particleAlphaMax));
    pexplosion.setColorRange(osgParticle::rangev4(
        osg::Vec4(1, 1, 1, 1), 
        osg::Vec4(1, 1, 1, 1))
	);
	pexplosion.setRadius(0.5f);
	 pexplosion.setMass(m_particleMass);
	pexplosion.setTextureTileRange(
		4, 4, 
		0, 15
	);

	// particle system
	osgParticle::ParticleSystem *ps = new osgParticle::ParticleSystem;
	ps->setDefaultAttributes( osgDB::findDataFile("images/animated_smoke.png"), false, false);
	
    osgParticle::ParticleSystemUpdater *psu = new osgParticle::ParticleSystemUpdater;
    psu->addParticleSystem(ps);
	group->addChild(psu);

	// emitter
    osgParticle::ModularEmitter *emitter = new osgParticle::ModularEmitter;
	group->addChild( emitter );
	emitter->setReferenceFrame( osgParticle::ParticleProcessor::RELATIVE_RF );
	emitter->setNumParticlesToCreateMovementCompensationRatio(1.5f);
	emitter->setEndless( false );
	emitter->setStartTime( 0 );
	emitter->setLifeTime( m_emitterLifeTime );
    emitter->setParticleSystem(ps);
    emitter->setParticleTemplate(pexplosion);

	// counter
    osgParticle::RandomRateCounter* counter = new osgParticle::RandomRateCounter;
    counter->setRateRange(m_rateMin, m_rateMax);
    emitter->setCounter(counter);

	// sector
    m_placer = new osgParticle::SectorPlacer;
    m_placer->setCenter( pos );
    m_placer->setRadiusRange(0, .1);
	m_placer->setPhiRange(0, 2 * osg::PI);    // 360° angle to make a circle
    emitter->setPlacer(m_placer);

	// shooter
    osgParticle::RadialShooter *shooter = new osgParticle::RadialShooter;
	shooter->setThetaRange( 0.0f, osg::PI_2 );		// up/down
	shooter->setPhiRange( 0.0f, 2.0f*osg::PI );		// around
	shooter->setInitialSpeedRange( 0, 2 );
	shooter->setInitialRotationalSpeedRange(
		osg::Vec3(0, 0, -1),
		osg::Vec3(0, 0, 1)
	);
    emitter->setShooter(shooter);

	osgParticle::FluidProgram *program = new osgParticle::FluidProgram;
	program->setFluidDensity(1.5f);	
	program->setParticleSystem(ps);
	group->addChild(program);
    
    osg::Geode *geode = new osg::Geode;
    geode->addDrawable(ps);
	group->addChild( geode );

	return group;
}

osgParticle::SmokeEffect* CParticleDebris::mycreateFireBall( const osg::Vec3& pos, int binNum )
{
	osgParticle::SmokeEffect* smoke =  new osgParticle::SmokeEffect(false);
	smoke->getOrCreateStateSet()->setRenderBinDetails( binNum, "DepthSortedBin" );
	smoke->setPosition( pos );
	smoke->setIntensity( 20.0f );
	smoke->setScale( 0.8f );
	smoke->setParticleDuration( 3.0f );
	smoke->setEmitterDuration( 2.5f );
	smoke->buildEffect();

	osgParticle::Particle& ptemplate = smoke->getParticleSystem()->getDefaultParticleTemplate();
	ptemplate.setAlphaRange(osgParticle::rangef(1.0f, -1.0f));

	osgParticle::ModularEmitter* me = dynamic_cast<osgParticle::ModularEmitter*>(smoke->getEmitter());
	me->setReferenceFrame( osgParticle::ParticleProcessor::RELATIVE_RF );

	osgParticle::RadialShooter* shooter = dynamic_cast<osgParticle::RadialShooter*>(me->getShooter());
	osgParticle::rangef r = shooter->getThetaRange();

	// I think osg has reversed the mening of theta and phi
	shooter->setThetaRange( 0.0f, osg::PI_2 );		// up/down
	shooter->setPhiRange( 0.0f, 2.0f*osg::PI );		// around
	shooter->setInitialSpeedRange( 0, 2 );
	shooter->setInitialRotationalSpeedRange(
		osg::Vec3(0, 0, -1),
		osg::Vec3(0, 0, 1)
	);

	/*
	osgParticle::SectorPlacer* placer = dynamic_cast<osgParticle::SectorPlacer*>(me->getPlacer());
	placer->setRadiusRange(0.0f,0.01f);
	*/
	m_placer = dynamic_cast<osgParticle::SectorPlacer*>(me->getPlacer());
	m_placer->setRadiusRange(0.0f,0.01f);

	osgParticle::FluidProgram* fluid = dynamic_cast<osgParticle::FluidProgram*>(smoke->getProgram());
	fluid->setFluidDensity(1.0f);	
//	fluid->setWind( wind );

	smoke->getParticleSystem()->setDefaultAttributes( osgDB::findDataFile("images/myexp.png") );

	return smoke;
}

