// SulParticleSystemContainerOsg.cpp

#include "stdafx.h"
#include "SulParticleSystemContainerOsg.h"
//#include <osg/Export>

CSulParticleSystemContainerOsg::CSulParticleSystemContainerOsg( CSulParticleSystemDataOsg* data, osg::Group* root ) :
osgParticle::ParticleSystem()
{
	m_root = root;
	m_data = data;
}

CSulParticleSystemDataOsg* CSulParticleSystemContainerOsg::getData()
{
	return m_data;
}

void CSulParticleSystemContainerOsg::create( const osg::Vec3& pos )
{
	m_particleSystemMT = new osg::MatrixTransform;
	osg::Matrix m;
	m.setTrans( pos );
	m_particleSystemMT->setMatrix( m );
	m_particleSystemMT->getOrCreateStateSet()->setRenderBinDetails( m_data->m_renderBinNum, "DepthSortedBin" );
	m_root->addChild( m_particleSystemMT );

// only support in osg version 3
#if OPENSCENEGRAPH_MAJOR_VERSION > 2
	std::map<CSulString,osgParticle::ParticleSystem::SortMode> mapSortMode;
	mapSortMode["NO_SORT"] = osgParticle::ParticleSystem::NO_SORT;
	mapSortMode["SORT_FRONT_TO_BACK"] = osgParticle::ParticleSystem::SORT_FRONT_TO_BACK;
	mapSortMode["SORT_BACK_TO_FRONT"] = osgParticle::ParticleSystem::SORT_BACK_TO_FRONT;
	setSortMode( mapSortMode[m_data->m_psSortMode] );
#endif
	
	setDefaultAttributes( osgDB::findDataFile(m_data->m_psTextureFile), m_data->m_psUseEmissive, m_data->m_psUseLighting );

	m_particle = new osgParticle::Particle;
	m_particle->setLifeTime( m_data->m_particleLifeTime );
	m_particle->setSizeRange( osgParticle::rangef(m_data->m_particleSizeMin, m_data->m_particleSizeMax) );
	m_particle->setAlphaRange( osgParticle::rangef(m_data->m_particleAlphaMin, m_data->m_particleAlphaMax) );
	m_particle->setColorRange( osgParticle::rangev4(
		m_data->m_particleColorMin, 
		m_data->m_particleColorMax)
	);
	m_particle->setRadius( m_data->m_particleRadius );
	m_particle->setMass( m_data->m_particleMass );
// only support in osg version 3
#if OPENSCENEGRAPH_MAJOR_VERSION > 2

	m_particle->setTextureTileRange(
		m_data->m_particleTextureTileS, 
		m_data->m_particleTextureTileT, 
		m_data->m_particleTextureTileStart, 
		m_data->m_particleTextureTileEnd 
	);
#endif

	// emitter
	m_emitter = new osgParticle::ModularEmitter;
	m_emitter->setReferenceFrame( osgParticle::ParticleProcessor::RELATIVE_RF );
//	m_emitter->setReferenceFrame( osgParticle::ParticleProcessor::ABSOLUTE_RF );
	m_emitter->setNumParticlesToCreateMovementCompensationRatio( m_data->m_emitterCompensationRatio );
	m_emitter->setEndless( m_data->m_emitterEndless );
	m_emitter->setStartTime( m_data->m_emitterStartTime );
	m_emitter->setLifeTime( m_data->m_emitterLifeTime );
	m_emitter->setParticleSystem( this );
	m_emitter->setParticleTemplate( *m_particle );
	m_particleSystemMT->addChild( m_emitter );
	
	// counter
	m_counterRandomRate = new osgParticle::RandomRateCounter;
	m_counterRandomRate->setRateRange( m_data->m_counterRandomRateMin, m_data->m_counterRandomRateMax );
	m_emitter->setCounter( m_counterRandomRate );

	// sector
	m_placer = new osgParticle::SectorPlacer;
	m_placer->setRadiusRange( m_data->m_sectorRadiusMin, m_data->m_sectorRadiusMax );
	m_placer->setPhiRange( m_data->m_sectorPhiMin, m_data->m_sectorPhiMax );    // 360° angle to make a circle
	m_emitter->setPlacer( m_placer );

	////////////////////////////
	// shooter
	////////////////////////////
	float min, max;

	m_shooterRadial = new osgParticle::RadialShooter;

	// theta
	min = m_data->m_shooterRadialThetaMin;
	max = m_data->m_shooterRadialThetaMax;
	if ( m_data->m_shooterRadialThetaRandomLock )
	{
		min = max = osgParticle::rangef( min, max ).get_random();
	}
	m_shooterRadial->setThetaRange( min, max );		// up/down

	// phi
	min = m_data->m_shooterRadialPhiMin;
	max = m_data->m_shooterRadialPhiMax;
	if ( m_data->m_shooterRadialPhiRandomLock )
	{
		min = max = osgParticle::rangef( min, max ).get_random();
	}
	m_shooterRadial->setPhiRange( min, max );		// around

	// speed
	min = m_data->m_shooterRadialInitialSpeedMin;
	max = m_data->m_shooterRadialInitialSpeedMax;
	if ( m_data->m_shooterRadialInitialSpeedRandomLock )
	{
		min = max = osgParticle::rangef( min, max ).get_random();
	}
	m_shooterRadial->setInitialSpeedRange( min, max );

	// rotational speed
	osg::Vec3 minV = m_data->m_shooterRadialInitialRotationMin;
	osg::Vec3 maxV = m_data->m_shooterRadialInitialRotationMax;
	if ( m_data->m_shooterRadialInitialRotationRandomLock )
	{
		minV = maxV = osgParticle::rangev3( minV, maxV ).get_random();
	}
	m_shooterRadial->setInitialRotationalSpeedRange( minV, maxV );

	m_emitter->setShooter( m_shooterRadial );

	m_programFluid = new osgParticle::FluidProgram;
	m_programFluid->setFluidDensity( m_data->m_programFluidDensity );
	m_programFluid->setFluidViscosity( m_data->m_programFluidViscosity );
	m_programFluid->setWind( m_data->m_programFluidWind );
	m_programFluid->setParticleSystem( this );
	m_particleSystemMT->addChild( m_programFluid );
	    
	osg::Geode *geode = new osg::Geode;
	geode->addDrawable( this );
	m_particleSystemMT->addChild( geode );

	// create the debris system here
	if ( m_data->m_debrisEnabled )
	{
		m_debris = new CSulParticleSystemDebris( m_placer, m_data );

		m_debris->getEmitter()->setSpeedRange( m_data->m_debrisSpeedMin, m_data->m_debrisSpeedMax );
		m_debris->getEmitter()->setPhiRange( m_data->m_debrisPhiMin, m_data->m_debrisPhiMax );
		m_debris->getEmitter()->setThetaRange( m_data->m_debrisThetaMin, m_data->m_debrisThetaMax );
		m_debris->getEmitter()->setEndless( m_data->m_debrisEndless );
		m_debris->getEmitter()->setLifeTime( m_data->m_debrisLifeTime );

		m_debris->init();

		m_particleSystemMT->addChild( m_debris );
	}
}

void CSulParticleSystemContainerOsg::destroy()
{
	m_root->removeChild( m_particleSystemMT );
}

