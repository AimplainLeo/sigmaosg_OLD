// SulParticleSystemOsg.cpp

#include "stdafx.h"
#include "SulParticleSystemOsg.h"
#include "SulQtPropertySheet.h"
#include "SulQtPropFloat.h"
#include "SulQtPropVec4.h"
#include "SulQtPropInt32.h"
#include "SulQtPropUint32.h"
#include "SulQtPropString.h"
#include "SulQtPropBool.h"
#include "SulQtPropVec3.h"
#include <osgDB/FileUtils>

CSulParticleSystemOsg::CSulParticleSystemOsg() :
osg::Group()
{
	setDefaultValues();
}

void CSulParticleSystemOsg::setDefaultValues()
{
	// osgParticle::Particle parameter values
	m_particleLifeTime					= 3.0f;
	m_particleSizeMin					= 0.75f;
	m_particleSizeMax					= 3.0f;
	m_particleAlphaMin					= 1.0f;
	m_particleAlphaMax					= 0.0f;
	m_particleColorMin					= osg::Vec4(1,1,1,1);
	m_particleColorMax					= osg::Vec4(1,1,1,1);
	m_particleRadius					= 0.5f;
	m_particleMass						= 0.6f;
	m_particleTextureTileS				= 4;
	m_particleTextureTileT				= 4;
	m_particleTextureTileStart			= 0;
	m_particleTextureTileEnd			= 15;

	// osgParticle::ParticleSystem parameter values
	m_psTextureFile						= "images/animated_smoke.png";
	m_psUseEmissive						= false;
	m_psUseLighting						= false;

	// osgParticle::ModularEmitter parameter values
	m_emitterCompensationRatio			= 1.5f;
	m_emitterEndless					= false;
	m_emitterStartTime					= 0.0f;
	m_emitterLifeTime					= 10.0f;

	// osgParticle::RandomRateCounter parameter values
	m_counterRandomRateMin				= 10.0f;
	m_counterRandomRateMax				= 10.0f;

	// osgParticle::SectorPlacer parameter values
	m_sectorRadiusMin					= 0.0f;
	m_sectorRadiusMax					= 0.1f;
	m_sectorPhiMin						= 0.0f;
	m_sectorPhiMax						= 2*osg::PI;

	// osgParticle::RadialShooter parameter values
	m_shooterRadialThetaMin				= 0.0f;
	m_shooterRadialThetaMax				= 0.4f;
	m_shooterRadialPhiMin				= 0.0f;
	m_shooterRadialPhiMax				= 2*osg::PI;
	m_shooterRadialInitialSpeedMin		= 0.0f;
	m_shooterRadialInitialSpeedMax		= 20.0f;
	m_shooterRadialInitialRotationMin	= osg::Vec3(0,0,-1);
	m_shooterRadialInitialRotationMax	= osg::Vec3(0,0,1);

	// osgParticle::FluidProgram parameter values
	m_programFluidDensity				= 1.2041f;
	m_programFluidWind					= osg::Vec3(0,0,0);
}

QWidget* CSulParticleSystemOsg::createPropertySheet()
{
	QWidget* container = new QWidget;

	QVBoxLayout* layoutPropertySheets = new QVBoxLayout;
	container->setLayout( layoutPropertySheets );

	// particle
	m_psParticles = new CSulQtPropertySheet( "Particle" );
	layoutPropertySheets->addWidget( m_psParticles );
	m_psParticles->add( new CSulQtPropFloat( "Life Time (sec)", m_particleLifeTime ) );
	m_psParticles->add( new CSulQtPropFloat( "Size Min", m_particleSizeMin ) );
	m_psParticles->add( new CSulQtPropFloat( "Size Max", m_particleSizeMax ) );
	m_psParticles->add( new CSulQtPropFloat( "Alpha Min", m_particleAlphaMin ) );
	m_psParticles->add( new CSulQtPropFloat( "Alpha Max", m_particleAlphaMax ) );
	m_psParticles->add( new CSulQtPropVec4( "Color Min", m_particleColorMin ) );
	m_psParticles->add( new CSulQtPropVec4( "Color Max", m_particleColorMax ) );
	m_psParticles->add( new CSulQtPropFloat( "Radius", m_particleRadius ) );
	m_psParticles->add( new CSulQtPropFloat( "Mass", m_particleMass ) );
	m_psParticles->add( new CSulQtPropInt32( "Texture Tile S", m_particleTextureTileS ) );
	m_psParticles->add( new CSulQtPropInt32( "Texture Tile T", m_particleTextureTileT ) );
	m_psParticles->add( new CSulQtPropUint32( "Texture Tile Start", m_particleTextureTileStart ) );
	m_psParticles->add( new CSulQtPropUint32( "Texture Tile End", m_particleTextureTileEnd ) );

	// particle system
	m_psParticleSystem = new CSulQtPropertySheet( "Particle System" );
	layoutPropertySheets->addWidget( m_psParticleSystem );
	m_psParticleSystem->add( new CSulQtPropString( "Texture File", m_psTextureFile ) );
	m_psParticleSystem->add( new CSulQtPropBool( "Emissive", m_psUseEmissive ) );
	m_psParticleSystem->add( new CSulQtPropBool( "Lighting", m_psUseLighting ) );

	// modular emitter
	m_psModularEmitter = new CSulQtPropertySheet( "Emitter: Modular" );
	layoutPropertySheets->addWidget( m_psModularEmitter );
	m_psModularEmitter->add( new CSulQtPropFloat( "Compensation Ratio", m_emitterCompensationRatio ) );
	m_psModularEmitter->add( new CSulQtPropBool( "Endless", m_emitterEndless ) );
	m_psModularEmitter->add( new CSulQtPropFloat( "Start Time", m_emitterStartTime ) );
	m_psModularEmitter->add( new CSulQtPropFloat( "Life Time", m_emitterLifeTime ) );

	// Random Rate Counter
	m_psCounterRandomRate = new CSulQtPropertySheet( "Counter: Random Rate" );
	layoutPropertySheets->addWidget( m_psCounterRandomRate );
	m_psCounterRandomRate->add( new CSulQtPropFloat( "Random Rate Min", m_counterRandomRateMin ) );
	m_psCounterRandomRate->add( new CSulQtPropFloat( "Random Rate Max", m_counterRandomRateMax ) );

	// Sector Placer
	m_psSectorPlacer = new CSulQtPropertySheet( "Sector Placer" );
	layoutPropertySheets->addWidget( m_psSectorPlacer );
	m_psSectorPlacer->add( new CSulQtPropFloat( "Radius Min", m_sectorRadiusMin ) );
	m_psSectorPlacer->add( new CSulQtPropFloat( "Radius Max", m_sectorRadiusMax ) );
	m_psSectorPlacer->add( new CSulQtPropFloat( "Radius Min", m_sectorRadiusMin ) );
	m_psSectorPlacer->add( new CSulQtPropFloat( "Phi Min", m_sectorPhiMin ) );
	m_psSectorPlacer->add( new CSulQtPropFloat( "Phi Max", m_sectorPhiMax ) );

	// Radial Shooter
	m_psShooterRadial = new CSulQtPropertySheet( "Shooter: Radial" );
	layoutPropertySheets->addWidget( m_psShooterRadial );
	m_psShooterRadial->add( new CSulQtPropFloat( "Theta Min", m_shooterRadialThetaMin ) );
	m_psShooterRadial->add( new CSulQtPropFloat( "Theta Max", m_shooterRadialThetaMax ) );
	m_psShooterRadial->add( new CSulQtPropFloat( "Phi Min", m_shooterRadialPhiMin ) );
	m_psShooterRadial->add( new CSulQtPropFloat( "Phi Max", m_shooterRadialPhiMax ) );
	m_psShooterRadial->add( new CSulQtPropFloat( "Initial Speed Min", m_shooterRadialInitialSpeedMin ) );
	m_psShooterRadial->add( new CSulQtPropFloat( "Initial Speed Max", m_shooterRadialInitialSpeedMax ) );
	m_psShooterRadial->add( new CSulQtPropVec3( "Initial Rotation Min", m_shooterRadialInitialRotationMin ) );
	m_psShooterRadial->add( new CSulQtPropVec3( "Initial Rotation Max", m_shooterRadialInitialRotationMax ) );

	// Program Fluid
	m_psProgramFluid = new CSulQtPropertySheet( "Shooter: Radial" );
	layoutPropertySheets->addWidget( m_psProgramFluid );
	m_psProgramFluid->add( new CSulQtPropFloat( "Fluid Density", m_programFluidDensity ) );
	m_psProgramFluid->add( new CSulQtPropVec3( "Wind", m_programFluidWind ) );

	return container;
}

void CSulParticleSystemOsg::updateFromUI()
{
	m_psParticles->updateFromUI();
	m_psParticleSystem->updateFromUI();
	m_psModularEmitter->updateFromUI();
	m_psCounterRandomRate->updateFromUI();
	m_psSectorPlacer->updateFromUI();
	m_psShooterRadial->updateFromUI();
	m_psProgramFluid->updateFromUI();
}

osg::Node* CSulParticleSystemOsg::create( const osg::Vec3& pos )
{
	osg::Group* group = new osg::Group;
//	group->getOrCreateStateSet()->setRenderBinDetails( m_renderBinNum->getUint32(), "DepthSortedBin" );

	m_particle = new osgParticle::Particle;
	m_particle->setLifeTime( m_particleLifeTime );
	m_particle->setSizeRange( osgParticle::rangef(m_particleSizeMin, m_particleSizeMax) );
	m_particle->setAlphaRange( osgParticle::rangef(m_particleAlphaMin, m_particleAlphaMax) );
    m_particle->setColorRange( osgParticle::rangev4(
        m_particleColorMin, 
        m_particleColorMax)
	);
	m_particle->setRadius( m_particleRadius );
	m_particle->setMass( m_particleMass );
	m_particle->setTextureTileRange(
		m_particleTextureTileS, 
		m_particleTextureTileT, 
		m_particleTextureTileStart, 
		m_particleTextureTileEnd 
	);

	// particle system
	m_ps = new osgParticle::ParticleSystem;
	m_ps->setDefaultAttributes( osgDB::findDataFile(m_psTextureFile), m_psUseEmissive, m_psUseLighting );
	
    m_psu = new osgParticle::ParticleSystemUpdater;
    m_psu->addParticleSystem( m_ps );
	group->addChild(m_psu);

	// emitter
    m_emitter = new osgParticle::ModularEmitter;
	group->addChild( m_emitter );
	m_emitter->setReferenceFrame( osgParticle::ParticleProcessor::RELATIVE_RF );
	m_emitter->setNumParticlesToCreateMovementCompensationRatio( m_emitterCompensationRatio );
	m_emitter->setEndless( m_emitterEndless );
	m_emitter->setStartTime( m_emitterStartTime );
	m_emitter->setLifeTime( m_emitterLifeTime );
    m_emitter->setParticleSystem( m_ps );
    m_emitter->setParticleTemplate( *m_particle );

	// counter
    m_counterRandomRate = new osgParticle::RandomRateCounter;
    m_counterRandomRate->setRateRange( m_counterRandomRateMin, m_counterRandomRateMax );
    m_emitter->setCounter( m_counterRandomRate );

	// sector
    m_placer = new osgParticle::SectorPlacer;
    m_placer->setCenter( pos );
	m_placer->setRadiusRange( m_sectorRadiusMin, m_sectorRadiusMax );
	m_placer->setPhiRange( m_sectorPhiMin, m_sectorPhiMax );    // 360° angle to make a circle
    m_emitter->setPlacer( m_placer );

	// shooter
    m_shooterRadial = new osgParticle::RadialShooter;
	m_shooterRadial->setThetaRange( m_shooterRadialThetaMin, m_shooterRadialThetaMax );		// up/down
	m_shooterRadial->setPhiRange( m_shooterRadialPhiMin, m_shooterRadialPhiMax );		// around
	m_shooterRadial->setInitialSpeedRange( m_shooterRadialInitialSpeedMin, m_shooterRadialInitialSpeedMax );
	m_shooterRadial->setInitialRotationalSpeedRange(
		m_shooterRadialInitialRotationMin,
		m_shooterRadialInitialRotationMax
	);
    m_emitter->setShooter( m_shooterRadial );

	m_programFluid = new osgParticle::FluidProgram;
	m_programFluid->setFluidDensity( m_programFluidDensity );	
	m_programFluid->setWind( m_programFluidWind );
	m_programFluid->setParticleSystem( m_ps );
	group->addChild( m_programFluid );
    
    osg::Geode *geode = new osg::Geode;
    geode->addDrawable( m_ps );
	group->addChild( geode );

	return group;
}


