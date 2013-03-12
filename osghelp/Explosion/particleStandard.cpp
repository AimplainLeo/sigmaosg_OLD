// particleStandard.cpp

#include "stdafx.h"
#include "particleStandard.h"

#include <osg/group>
#include <osgParticle\particle>
#include <osgParticle\range>
#include <osgParticle\particlesystem>
#include <osgParticle\ParticleSystemUpdater>
#include <osgParticle\ModularEmitter>
#include <osgParticle\SectorPlacer>
#include <osgParticle\FluidProgram>
#include <osgDB/FileUtils>

CParticleStandard::CParticleStandard() :
CParticleBase()
{
}

osg::Node* CParticleStandard::create( const osg::Vec3& pos )
{
	osg::Group* group = new osg::Group;
	group->getOrCreateStateSet()->setRenderBinDetails( m_renderBinNum->getUint32(), "DepthSortedBin" );

	osgParticle::Particle pexplosion;
	pexplosion.setLifeTime( m_particleLifeTime->getFloat() );
	pexplosion.setSizeRange( osgParticle::rangef(m_particleSizeMin->getFloat(), m_particleSizeMax->getFloat()) );
	pexplosion.setAlphaRange( osgParticle::rangef(m_particleAlphaMin->getFloat(), m_particleAlphaMax->getFloat()) );
    pexplosion.setColorRange( osgParticle::rangev4(
        m_particleColorMin->getVec4(), 
        m_particleColorMax->getVec4())
	);
	pexplosion.setRadius( m_particleRadius->getFloat() );
	pexplosion.setMass( m_particleMass->getFloat() );
	pexplosion.setTextureTileRange(
		m_particleTextureTile->getVec4s().x(), 
		m_particleTextureTile->getVec4s().y(), 
		m_particleTextureTile->getVec4s().z(), 
		m_particleTextureTile->getVec4s().w() 
	);

	// particle system
	osgParticle::ParticleSystem *ps = new osgParticle::ParticleSystem;
	ps->setDefaultAttributes( osgDB::findDataFile(m_psTextureFile->getString()), m_psEmissive->getState(), false);
	
    osgParticle::ParticleSystemUpdater *psu = new osgParticle::ParticleSystemUpdater;
    psu->addParticleSystem(ps);
	group->addChild(psu);

	// emitter
    osgParticle::ModularEmitter *emitter = new osgParticle::ModularEmitter;
	group->addChild( emitter );
	emitter->setReferenceFrame( osgParticle::ParticleProcessor::RELATIVE_RF );
	emitter->setNumParticlesToCreateMovementCompensationRatio( 1.5f );
	emitter->setEndless( false );
	emitter->setStartTime( 0 );
	emitter->setLifeTime( m_emitterLifeTime->getFloat() );
    emitter->setParticleSystem( ps );
    emitter->setParticleTemplate(pexplosion);

	// counter
    osgParticle::RandomRateCounter* counter = new osgParticle::RandomRateCounter;
    counter->setRateRange( m_rateMin->getFloat(), m_rateMax->getFloat() );
    emitter->setCounter(counter);

	// sector
    osgParticle::SectorPlacer* placer = new osgParticle::SectorPlacer;
    placer->setCenter( pos );
	placer->setRadiusRange( m_sectorRadiusMin->getFloat(), m_sectorRadiusMax->getFloat() );
	placer->setPhiRange( 0, 2*osg::PI );    // 360° angle to make a circle
    emitter->setPlacer( placer );

	// shooter
    osgParticle::RadialShooter *shooter = new osgParticle::RadialShooter;
	shooter->setThetaRange( m_shooterThetaMin->getFloat(), m_shooterThetaMax->getFloat() );		// up/down
	shooter->setPhiRange( m_shooterPhiMin->getFloat(), m_shooterPhiMax->getFloat() );		// around
	shooter->setInitialSpeedRange( m_shooterInitialSpeedMin->getFloat(), m_shooterInitialSpeedMax->getFloat() );
	shooter->setInitialRotationalSpeedRange(
		osg::Vec3(0, 0, -1),
		osg::Vec3(0, 0, 1)
	);
    emitter->setShooter(shooter);

	osgParticle::FluidProgram *program = new osgParticle::FluidProgram;
	program->setFluidDensity( m_fluidDensity->getFloat() );	
	program->setParticleSystem(ps);
	group->addChild(program);
    
    osg::Geode *geode = new osg::Geode;
    geode->addDrawable(ps);
	group->addChild( geode );

	return group;
}

CPropertySheet* CParticleStandard::createPropertySheet()
{
	CPropertySheet* propertySheet = new CPropertySheet( "Standard Particle System" );

	// general
	propertySheet->add( m_renderBinNum = new CPropString( "RenderBin Num", 3000) );
	propertySheet->add( m_fluidDensity = new CPropFloat( "Fluid Density", 1.5f) );
	propertySheet->add( m_psTextureFile = new CPropString( "Texture File", "images/animated_smoke.png") );
	propertySheet->add( m_psEmissive = new CPropBool( "Emissive", false ) );

	// particles
	propertySheet->add( m_particleTextureTile = new CPropVec4s( "Particle Texture Tile Range", osg::Vec4s(4,4,0,15)) );
	propertySheet->add( m_particleMass = new CPropFloat( "Particle Mass", 0.7f ) );
	propertySheet->add( m_particleRadius = new CPropFloat( "Particle Radius", 0.5f ) );
	propertySheet->add( m_particleLifeTime = new CPropFloat( "Particle Life Time (sec)", 3.0f ) );
	propertySheet->add( m_particleSizeMin = new CPropFloat( "Particle Size Min", 0.75f) );
	propertySheet->add( m_particleSizeMax = new CPropFloat( "Particle Size Max", 3.0f) );
	propertySheet->add( m_particleAlphaMin = new CPropFloat( "Particle Alpha Min", 0.5f) );
	propertySheet->add( m_particleAlphaMax = new CPropFloat( "Particle Alpha Max", 1.0f) );
	propertySheet->add( m_particleColorMin = new CPropVec4( "Particle Color Min", osg::Vec4(1,1,1,1)) );
	propertySheet->add( m_particleColorMax = new CPropVec4( "Particle Color Max", osg::Vec4(1,1,1,1)) );

	// emitter
	propertySheet->add( m_emitterLifeTime = new CPropFloat( "Emitter Life Time", 2.5f) );

	// rate
	propertySheet->add( m_rateMin = new CPropFloat( "Rate Min", 10.0f) );
	propertySheet->add( m_rateMax = new CPropFloat( "Rate Max", 10.0f) );

	// sector
	propertySheet->add( m_sectorRadiusMin = new CPropFloat( "Sector Radius Min", 0.0f) );
	propertySheet->add( m_sectorRadiusMax = new CPropFloat( "Sector Radius Max", 0.1f) );

	// shooter
	propertySheet->add( m_shooterThetaMin = new CPropFloat( "Shooter Theta Min", 0.0f) );
	propertySheet->add( m_shooterThetaMax = new CPropFloat( "Shooter Theta Max", osg::PI_2) );
	propertySheet->add( m_shooterPhiMin = new CPropFloat( "Shooter Phi Min", 0.0f) );
	propertySheet->add( m_shooterPhiMax = new CPropFloat( "Shooter Phi Max", 2.0f*osg::PI) );
	propertySheet->add( m_shooterInitialSpeedMin = new CPropFloat( "Shooter Initial Speed Min", 0.0f) );
	propertySheet->add( m_shooterInitialSpeedMax = new CPropFloat( "Shooter Initial Speed Max", 2.0f) );

	return propertySheet;
}
