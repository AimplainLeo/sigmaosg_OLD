// SulParticleSystemInterfaceOsg.h

#ifndef __SULPARTICLESYSTEMINTERFACEOSG_H__
#define __SULPARTICLESYSTEMINTERFACEOSG_H__

#include "SulParticleSystemDataOsg.h"
#include <osgParticle/ParticleSystem>
#include <osgParticle/ModularEmitter>
#include <osgParticle/SectorPlacer>
#include <osgParticle/FluidProgram>
#include <osgDB/FileUtils>


class CSulParticleSystemContainerOsg : public osgParticle::ParticleSystem
{
public:
	CSulParticleSystemContainerOsg( CSulParticleSystemDataOsg* data, osg::Group* root )
	{
		std::map<CSulString,osgParticle::ParticleSystem::SortMode> mapSortMode;
		mapSortMode["NO_SORT"] = osgParticle::ParticleSystem::NO_SORT;
		mapSortMode["SORT_FRONT_TO_BACK"] = osgParticle::ParticleSystem::SORT_FRONT_TO_BACK;
		mapSortMode["SORT_BACK_TO_FRONT"] = osgParticle::ParticleSystem::SORT_BACK_TO_FRONT;

		setDefaultAttributes( osgDB::findDataFile(data->m_psTextureFile), data->m_psUseEmissive, data->m_psUseLighting );
		setSortMode( mapSortMode[data->m_psSortMode] );

		m_particle = new osgParticle::Particle;
		m_particle->setLifeTime( data->m_particleLifeTime );
		m_particle->setSizeRange( osgParticle::rangef(data->m_particleSizeMin, data->m_particleSizeMax) );
		m_particle->setAlphaRange( osgParticle::rangef(data->m_particleAlphaMin, data->m_particleAlphaMax) );
		m_particle->setColorRange( osgParticle::rangev4(
			data->m_particleColorMin, 
			data->m_particleColorMax)
		);
		m_particle->setRadius( data->m_particleRadius );
		m_particle->setMass( data->m_particleMass );
		m_particle->setTextureTileRange(
			data->m_particleTextureTileS, 
			data->m_particleTextureTileT, 
			data->m_particleTextureTileStart, 
			data->m_particleTextureTileEnd 
		);

		// emitter
		m_emitter = new osgParticle::ModularEmitter;
		m_emitter->setReferenceFrame( osgParticle::ParticleProcessor::RELATIVE_RF );
		m_emitter->setNumParticlesToCreateMovementCompensationRatio( data->m_emitterCompensationRatio );
		m_emitter->setEndless( data->m_emitterEndless );
		m_emitter->setStartTime( data->m_emitterStartTime );
		m_emitter->setLifeTime( data->m_emitterLifeTime );
		m_emitter->setParticleSystem( this );
		m_emitter->setParticleTemplate( *m_particle );

		// counter
		m_counterRandomRate = new osgParticle::RandomRateCounter;
		m_counterRandomRate->setRateRange( data->m_counterRandomRateMin, data->m_counterRandomRateMax );
		m_emitter->setCounter( m_counterRandomRate );

		// sector
		m_placer = new osgParticle::SectorPlacer;
		m_placer->setRadiusRange( data->m_sectorRadiusMin, data->m_sectorRadiusMax );
		m_placer->setPhiRange( data->m_sectorPhiMin, data->m_sectorPhiMax );    // 360° angle to make a circle
		m_emitter->setPlacer( m_placer );

		// shooter
		m_shooterRadial = new osgParticle::RadialShooter;
		m_shooterRadial->setThetaRange( data->m_shooterRadialThetaMin, data->m_shooterRadialThetaMax );		// up/down
		m_shooterRadial->setPhiRange( data->m_shooterRadialPhiMin, data->m_shooterRadialPhiMax );		// around
		m_shooterRadial->setInitialSpeedRange( data->m_shooterRadialInitialSpeedMin, data->m_shooterRadialInitialSpeedMax );
		m_shooterRadial->setInitialRotationalSpeedRange(
			data->m_shooterRadialInitialRotationMin,
			data->m_shooterRadialInitialRotationMax
		);
		m_emitter->setShooter( m_shooterRadial );

		m_programFluid = new osgParticle::FluidProgram;
		m_programFluid->setFluidDensity( data->m_programFluidDensity );	
		m_programFluid->setWind( data->m_programFluidWind );
		m_programFluid->setParticleSystem( this );
	    
		osg::Geode *geode = new osg::Geode;
		geode->addDrawable( this );
		root->addChild( geode );
	}

public:
	osgParticle::Particle*				m_particle;
	osgParticle::ModularEmitter*		m_emitter;
	osgParticle::RandomRateCounter*		m_counterRandomRate;
	osgParticle::SectorPlacer*			m_placer;
	osgParticle::RadialShooter*			m_shooterRadial;
	osgParticle::FluidProgram*			m_programFluid;
};

#endif // __SULPARTICLESYSTEMINTERFACEOSG_H__