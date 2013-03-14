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
	/*
	group->getOrCreateStateSet()->setRenderBinDetails( m_renderBinNum->getUint32(), "DepthSortedBin" );

	osgParticle::Particle pexplosion;
	pexplosion.setLifeTime( m_particleLifeTime->getValue() );
	pexplosion.setSizeRange( osgParticle::rangef(m_particleSizeMin->getValue(), m_particleSizeMax->getValue()) );
	pexplosion.setAlphaRange( osgParticle::rangef(m_particleAlphaMin->getValue(), m_particleAlphaMax->getValue()) );
    pexplosion.setColorRange( osgParticle::rangev4(
        m_particleColorMin->getValue(), 
        m_particleColorMax->getValue())
	);
	pexplosion.setRadius( m_particleRadius->getValue() );
	pexplosion.setMass( m_particleMass->getValue() );
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
	emitter->setNumParticlesToCreateMovementCompensationRatio( m_emitterCompensationRatio->getValue() );
	emitter->setEndless( false );
	emitter->setStartTime( 0 );
	emitter->setLifeTime( m_emitterLifeTime->getValue() );
    emitter->setParticleSystem( ps );
    emitter->setParticleTemplate(pexplosion);

	// counter
    osgParticle::RandomRateCounter* counter = new osgParticle::RandomRateCounter;
    counter->setRateRange( m_rateMin->getValue(), m_rateMax->getValue() );
    emitter->setCounter(counter);

	// sector
    osgParticle::SectorPlacer* placer = new osgParticle::SectorPlacer;
    placer->setCenter( pos );
	placer->setRadiusRange( m_sectorRadiusMin->getValue(), m_sectorRadiusMax->getValue() );
	placer->setPhiRange( 0, 2*osg::PI );    // 360° angle to make a circle
    emitter->setPlacer( placer );

	// shooter
    osgParticle::RadialShooter *shooter = new osgParticle::RadialShooter;
	shooter->setThetaRange( m_shooterThetaMin->getValue(), m_shooterThetaMax->getValue() );		// up/down
	shooter->setPhiRange( m_shooterPhiMin->getValue(), m_shooterPhiMax->getValue() );		// around
	shooter->setInitialSpeedRange( m_shooterInitialSpeedMin->getValue(), m_shooterInitialSpeedMax->getValue() );
	shooter->setInitialRotationalSpeedRange(
		osg::Vec3(0, 0, -1),
		osg::Vec3(0, 0, 1)
	);
    emitter->setShooter(shooter);

	osgParticle::FluidProgram *program = new osgParticle::FluidProgram;
	program->setFluidDensity( m_fluidDensity->getValue() );	
	program->setWind( m_wind->getVec3() );
	program->setParticleSystem(ps);
	group->addChild(program);
    
    osg::Geode *geode = new osg::Geode;
    geode->addDrawable(ps);
	group->addChild( geode );
	*/
	return group;
}

CSulQtPropertySheet* CParticleStandard::createPropertySheet()
{
	CSulQtPropertySheet* propertySheet = new CSulQtPropertySheet( "Standard Particle System" );
/*

	// general
	propertySheet->add( m_renderBinNum = new CSulQtPropString( "RenderBin Num", 3000) );
	propertySheet->add( m_fluidDensity = new CSulQtPropFloat( "Fluid Density", 1.5f) );
	propertySheet->add( m_wind = new CSulQtPropVec3( "Wind", osg::Vec3(0,0,0)) );
	propertySheet->add( m_psTextureFile = new CSulQtPropString( "Texture File", "images/animated_smoke.png") );
	propertySheet->add( m_psEmissive = new CSulQtPropBool( "Emissive", false ) );

	// particles
	propertySheet->add( m_particleTextureTile = new CSulQtPropVec4s( "Particle Texture Tile Range", osg::Vec4s(4,4,0,15)) );
	propertySheet->add( m_particleMass = new CSulQtPropFloat( "Particle Mass", 0.7f ) );
	propertySheet->add( m_particleRadius = new CSulQtPropFloat( "Particle Radius", 0.5f ) );
	propertySheet->add( m_particleLifeTime = new CSulQtPropFloat( "Particle Life Time (sec)", 3.0f ) );
	propertySheet->add( m_particleSizeMin = new CSulQtPropFloat( "Particle Size Min", 0.75f) );
	propertySheet->add( m_particleSizeMax = new CSulQtPropFloat( "Particle Size Max", 3.0f) );
	propertySheet->add( m_particleAlphaMin = new CSulQtPropFloat( "Particle Alpha Min", 0.5f) );
	propertySheet->add( m_particleAlphaMax = new CSulQtPropFloat( "Particle Alpha Max", 1.0f) );
	propertySheet->add( m_particleColorMin = new CSulQtPropVec4( "Particle Color Min", osg::Vec4(1,1,1,1)) );
	propertySheet->add( m_particleColorMax = new CSulQtPropVec4( "Particle Color Max", osg::Vec4(1,1,1,1)) );

	// emitter
	propertySheet->add( m_emitterLifeTime = new CSulQtPropFloat( "Emitter Life Time", 2.5f) );
	propertySheet->add( m_emitterCompensationRatio = new CSulQtPropFloat( "Emitter Movement Compensation Ratio", 1.5f) );

	// rate
	propertySheet->add( m_rateMin = new CSulQtPropFloat( "Rate Min", 10.0f) );
	propertySheet->add( m_rateMax = new CSulQtPropFloat( "Rate Max", 10.0f) );

	// sector
	propertySheet->add( m_sectorRadiusMin = new CSulQtPropFloat( "Sector Radius Min", 0.0f) );
	propertySheet->add( m_sectorRadiusMax = new CSulQtPropFloat( "Sector Radius Max", 0.1f) );

	// shooter
	propertySheet->add( m_shooterThetaMin = new CSulQtPropFloat( "Shooter Theta Min", 0.0f) );
	propertySheet->add( m_shooterThetaMax = new CSulQtPropFloat( "Shooter Theta Max", osg::PI_2) );
	propertySheet->add( m_shooterPhiMin = new CSulQtPropFloat( "Shooter Phi Min", 0.0f) );
	propertySheet->add( m_shooterPhiMax = new CSulQtPropFloat( "Shooter Phi Max", 2.0f*osg::PI) );
	propertySheet->add( m_shooterInitialSpeedMin = new CSulQtPropFloat( "Shooter Initial Speed Min", 0.0f) );
	propertySheet->add( m_shooterInitialSpeedMax = new CSulQtPropFloat( "Shooter Initial Speed Max", 2.0f) );
	*/
	return propertySheet;
}
