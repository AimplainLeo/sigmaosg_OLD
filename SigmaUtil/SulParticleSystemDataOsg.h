// SulParticleSystemDataOsg.h

#ifndef __SULPARTICLESYSTEMDATAOSG_H__
#define __SULPARTICLESYSTEMDATAOSG_H__

#include "SulString.h"

class CSulParticleSystemDataOsg
{
public:
	bool save( const CSulString& file );
	bool load( const CSulString& file );

	// osgParticle::Particle parameter values
	float						m_particleLifeTime;
	float						m_particleSizeMin;
	float						m_particleSizeMax;
	float						m_particleAlphaMin;
	float						m_particleAlphaMax;
	osg::Vec4					m_particleColorMin;
	osg::Vec4					m_particleColorMax;
	float						m_particleRadius;
	float						m_particleMass;
	sigma::int32				m_particleTextureTileS;
	sigma::int32				m_particleTextureTileT;
	sigma::uint32				m_particleTextureTileStart;
	sigma::uint32				m_particleTextureTileEnd;

	// osgParticle::ParticleSystem parameter values
	CSulString					m_psTextureFile;
	bool						m_psUseEmissive;
	bool						m_psUseLighting;
	CSulString					m_psSortMode;

	// osgParticle::ModularEmitter parameter values
	float						m_emitterCompensationRatio;
	bool						m_emitterEndless;
	float						m_emitterStartTime;
	float						m_emitterLifeTime;

	// osgParticle::RandomRateCounter parameter values
	float						m_counterRandomRateMin;
	float						m_counterRandomRateMax;

	// osgParticle::SectorPlacer parameter values
	float						m_sectorRadiusMin;
	float						m_sectorRadiusMax;
	float						m_sectorPhiMin;
	float						m_sectorPhiMax;

	// osgParticle::RadialShooter parameter values
	float						m_shooterRadialThetaMin;
	float						m_shooterRadialThetaMax;
	float						m_shooterRadialPhiMin;
	float						m_shooterRadialPhiMax;
	float						m_shooterRadialInitialSpeedMin;
	float						m_shooterRadialInitialSpeedMax;
	osg::Vec3					m_shooterRadialInitialRotationMin;
	osg::Vec3					m_shooterRadialInitialRotationMax;

	// osgParticle::FluidProgram parameter values
	float						m_programFluidDensity;
	osg::Vec3					m_programFluidWind;
};

#endif // __SULPARTICLESYSTEMDATAOSG_H__