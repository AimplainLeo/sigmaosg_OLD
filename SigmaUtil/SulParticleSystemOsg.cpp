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
#include "SulQtPropCombobox.h"
#include <osgDB/FileUtils>

CSulParticleSystemOsg::CSulParticleSystemOsg() :
osg::Group()
{
	m_data = new CSulParticleSystemDataOsg;
	m_psu = new osgParticle::ParticleSystemUpdater;
	addChild( m_psu );

	setDefaultValues();
}

void CSulParticleSystemOsg::setDefaultValues()
{
	// osgParticle::Particle parameter values
	m_data->m_particleLifeTime					= 3.0f;
	m_data->m_particleSizeMin					= 0.75f;
	m_data->m_particleSizeMax					= 3.0f;
	m_data->m_particleAlphaMin					= 1.0f;
	m_data->m_particleAlphaMax					= 0.0f;
	m_data->m_particleColorMin					= osg::Vec4(1,1,1,1);
	m_data->m_particleColorMax					= osg::Vec4(1,1,1,1);
	m_data->m_particleRadius					= 0.5f;
	m_data->m_particleMass						= 0.6f;
	m_data->m_particleTextureTileS				= 4;
	m_data->m_particleTextureTileT				= 4;
	m_data->m_particleTextureTileStart			= 0;
	m_data->m_particleTextureTileEnd			= 15;

	// osgParticle::ParticleSystem parameter values
	m_data->m_psTextureFile						= "images/animated_smoke.png";
	m_data->m_psUseEmissive						= false;
	m_data->m_psUseLighting						= false;

	// osgParticle::ModularEmitter parameter values
	m_data->m_emitterCompensationRatio			= 1.5f;
	m_data->m_emitterEndless					= false;
	m_data->m_emitterStartTime					= 0.0f;
	m_data->m_emitterLifeTime					= 10.0f;

	// osgParticle::RandomRateCounter parameter values
	m_data->m_counterRandomRateMin				= 10.0f;
	m_data->m_counterRandomRateMax				= 10.0f;

	// osgParticle::SectorPlacer parameter values
	m_data->m_sectorRadiusMin					= 0.0f;
	m_data->m_sectorRadiusMax					= 0.1f;
	m_data->m_sectorPhiMin						= 0.0f;
	m_data->m_sectorPhiMax						= 2*osg::PI;

	// osgParticle::RadialShooter parameter values
	m_data->m_shooterRadialThetaMin				= 0.0f;
	m_data->m_shooterRadialThetaMax				= 0.4f;
	m_data->m_shooterRadialPhiMin				= 0.0f;
	m_data->m_shooterRadialPhiMax				= 2*osg::PI;
	m_data->m_shooterRadialInitialSpeedMin		= 0.0f;
	m_data->m_shooterRadialInitialSpeedMax		= 20.0f;
	m_data->m_shooterRadialInitialRotationMin	= osg::Vec3(0,0,-1);
	m_data->m_shooterRadialInitialRotationMax	= osg::Vec3(0,0,1);

	// osgParticle::FluidProgram parameter values
	m_data->m_programFluidDensity				= 1.2041f;
	m_data->m_programFluidWind					= osg::Vec3(0,0,0);
}

QWidget* CSulParticleSystemOsg::createPropertySheet()
{
	QWidget* container = new QWidget;

	QVBoxLayout* layoutPropertySheets = new QVBoxLayout;
	container->setLayout( layoutPropertySheets );

	// particle
	m_psParticles = new CSulQtPropertySheet( "Particle" );
	layoutPropertySheets->addWidget( m_psParticles );
	m_psParticles->add( new CSulQtPropFloat( "Life Time (sec)", m_data->m_particleLifeTime ) );
	m_psParticles->add( new CSulQtPropFloat( "Size Min", m_data->m_particleSizeMin ) );
	m_psParticles->add( new CSulQtPropFloat( "Size Max", m_data->m_particleSizeMax ) );
	m_psParticles->add( new CSulQtPropFloat( "Alpha Min", m_data->m_particleAlphaMin ) );
	m_psParticles->add( new CSulQtPropFloat( "Alpha Max", m_data->m_particleAlphaMax ) );
	m_psParticles->add( new CSulQtPropVec4( "Color Min", m_data->m_particleColorMin ) );
	m_psParticles->add( new CSulQtPropVec4( "Color Max", m_data->m_particleColorMax ) );
	m_psParticles->add( new CSulQtPropFloat( "Radius", m_data->m_particleRadius ) );
	m_psParticles->add( new CSulQtPropFloat( "Mass", m_data->m_particleMass ) );
	m_psParticles->add( new CSulQtPropInt32( "Texture Tile S", m_data->m_particleTextureTileS ) );
	m_psParticles->add( new CSulQtPropInt32( "Texture Tile T", m_data->m_particleTextureTileT ) );
	m_psParticles->add( new CSulQtPropUint32( "Texture Tile Start", m_data->m_particleTextureTileStart ) );
	m_psParticles->add( new CSulQtPropUint32( "Texture Tile End", m_data->m_particleTextureTileEnd ) );

	// particle system
	m_psParticleSystem = new CSulQtPropertySheet( "Particle System" );
	layoutPropertySheets->addWidget( m_psParticleSystem );
	m_psParticleSystem->add( new CSulQtPropCombobox( "Sort Mode", m_data->m_psSortMode, "NO_SORT,SORT_FRONT_TO_BACK,SORT_BACK_TO_FRONT" ) );
	m_psParticleSystem->add( new CSulQtPropString( "Texture File", m_data->m_psTextureFile ) );
	m_psParticleSystem->add( new CSulQtPropBool( "Emissive", m_data->m_psUseEmissive ) );
	m_psParticleSystem->add( new CSulQtPropBool( "Lighting", m_data->m_psUseLighting ) );

	// modular emitter
	m_psModularEmitter = new CSulQtPropertySheet( "Emitter: Modular" );
	layoutPropertySheets->addWidget( m_psModularEmitter );
	m_psModularEmitter->add( new CSulQtPropFloat( "Compensation Ratio", m_data->m_emitterCompensationRatio ) );
	m_psModularEmitter->add( new CSulQtPropBool( "Endless", m_data->m_emitterEndless ) );
	m_psModularEmitter->add( new CSulQtPropFloat( "Start Time", m_data->m_emitterStartTime ) );
	m_psModularEmitter->add( new CSulQtPropFloat( "Life Time", m_data->m_emitterLifeTime ) );

	// Random Rate Counter
	m_psCounterRandomRate = new CSulQtPropertySheet( "Counter: Random Rate" );
	layoutPropertySheets->addWidget( m_psCounterRandomRate );
	m_psCounterRandomRate->add( new CSulQtPropFloat( "Random Rate Min", m_data->m_counterRandomRateMin ) );
	m_psCounterRandomRate->add( new CSulQtPropFloat( "Random Rate Max", m_data->m_counterRandomRateMax ) );

	// Sector Placer
	m_psSectorPlacer = new CSulQtPropertySheet( "Sector Placer" );
	layoutPropertySheets->addWidget( m_psSectorPlacer );
	m_psSectorPlacer->add( new CSulQtPropFloat( "Radius Min", m_data->m_sectorRadiusMin ) );
	m_psSectorPlacer->add( new CSulQtPropFloat( "Radius Max", m_data->m_sectorRadiusMax ) );
	m_psSectorPlacer->add( new CSulQtPropFloat( "Radius Min", m_data->m_sectorRadiusMin ) );
	m_psSectorPlacer->add( new CSulQtPropFloat( "Phi Min", m_data->m_sectorPhiMin ) );
	m_psSectorPlacer->add( new CSulQtPropFloat( "Phi Max", m_data->m_sectorPhiMax ) );

	// Radial Shooter
	m_psShooterRadial = new CSulQtPropertySheet( "Shooter: Radial" );
	layoutPropertySheets->addWidget( m_psShooterRadial );
	m_psShooterRadial->add( new CSulQtPropFloat( "Theta Min", m_data->m_shooterRadialThetaMin ) );
	m_psShooterRadial->add( new CSulQtPropFloat( "Theta Max", m_data->m_shooterRadialThetaMax ) );
	m_psShooterRadial->add( new CSulQtPropFloat( "Phi Min", m_data->m_shooterRadialPhiMin ) );
	m_psShooterRadial->add( new CSulQtPropFloat( "Phi Max", m_data->m_shooterRadialPhiMax ) );
	m_psShooterRadial->add( new CSulQtPropFloat( "Initial Speed Min", m_data->m_shooterRadialInitialSpeedMin ) );
	m_psShooterRadial->add( new CSulQtPropFloat( "Initial Speed Max", m_data->m_shooterRadialInitialSpeedMax ) );
	m_psShooterRadial->add( new CSulQtPropVec3( "Initial Rotation Min", m_data->m_shooterRadialInitialRotationMin ) );
	m_psShooterRadial->add( new CSulQtPropVec3( "Initial Rotation Max", m_data->m_shooterRadialInitialRotationMax ) );

	// Program Fluid
	m_psProgramFluid = new CSulQtPropertySheet( "Program: Fluid" );
	layoutPropertySheets->addWidget( m_psProgramFluid );
	m_psProgramFluid->add( new CSulQtPropFloat( "Density",m_data-> m_programFluidDensity ) );
	m_psProgramFluid->add( new CSulQtPropVec3( "Wind", m_data->m_programFluidWind ) );

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

void CSulParticleSystemOsg::updateToUI()
{
	m_psParticles->updateToUI();
	m_psParticleSystem->updateToUI();
	m_psModularEmitter->updateToUI();
	m_psCounterRandomRate->updateToUI();
	m_psSectorPlacer->updateToUI();
	m_psShooterRadial->updateToUI();
	m_psProgramFluid->updateToUI();
}

void CSulParticleSystemOsg::placeParticleSystem( const osg::Vec3& pos, CSulParticleSystemContainerOsg* p )
{
	addChild( p->m_emitter );
	addChild( p->m_programFluid );
	
	p->m_placer->setCenter( pos );

	// particle system updater
    m_psu->addParticleSystem( p );	
}

CSulParticleSystemContainerOsg* CSulParticleSystemOsg::createParticleSystem()
{
	CSulParticleSystemContainerOsg* p = new CSulParticleSystemContainerOsg( m_data, this );
	return p;
}

CSulParticleSystemDataOsg* CSulParticleSystemOsg::getData()
{
	return m_data;
}

/*
void CSulParticleSystemOsg::create( const osg::Vec3& pos )
{
	// particle system
	osgParticle::ParticleSystem*		m_ps;
	m_ps = new osgParticle::ParticleSystem;
	m_ps->setDefaultAttributes( osgDB::findDataFile(m_psTextureFile), m_psUseEmissive, m_psUseLighting );

	// particle system updater
    m_psu->addParticleSystem( m_ps );	

	osgParticle::Particle*				m_particle;
	osgParticle::ModularEmitter*		m_emitter;
	osgParticle::RandomRateCounter*		m_counterRandomRate;
	osgParticle::SectorPlacer*			m_placer;
	osgParticle::RadialShooter*			m_shooterRadial;
	osgParticle::FluidProgram*			m_programFluid;

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

	// emitter
    m_emitter = new osgParticle::ModularEmitter;
	addChild( m_emitter );
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
	addChild( m_programFluid );
    
    osg::Geode *geode = new osg::Geode;
    geode->addDrawable( m_ps );
	addChild( geode );
}

*/
