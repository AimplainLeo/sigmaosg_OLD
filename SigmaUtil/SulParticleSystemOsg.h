// SulParticleSystemOsg.h

#ifndef __SULPARTICLESYSTEMOSG_H__
#define __SULPARTICLESYSTEMOSG_H__

#include "SulTypes.h"
#include "SulString.h"
#include "SulQtPropertySheet.h"
#include <osgParticle/Particle>
#include <osgParticle/ParticleSystem>
#include <osgParticle/ParticleSystemUpdater>
#include <osgParticle/ModularEmitter>
#include <osgParticle/SectorPlacer>
#include <osgParticle/FluidProgram>
#include <QtGui/qwidget>

class CSulParticleSystemOsg : public osg::Group
{
public:
	CSulParticleSystemOsg();

	void setDefaultValues();

	osg::Node* create( const osg::Vec3& pos );

	// some Qt stuff
	void updateFromUI();
	QWidget* createPropertySheet();

private:
	osgParticle::Particle*				m_particle;
	osgParticle::ParticleSystem*		m_ps;
	osgParticle::ParticleSystemUpdater*	m_psu;
	osgParticle::ModularEmitter*		m_emitter;
	osgParticle::RandomRateCounter*		m_counterRandomRate;
	osgParticle::SectorPlacer*			m_placer;
	osgParticle::RadialShooter*			m_shooterRadial;
	osgParticle::FluidProgram*			m_programFluid;

	//////////////////////////////////////////////////////////////////////////////////////
	// basic idea; you can setup all the values to be used by the particle system
	// it then will create that particle system at the specified position. This allows
	// us to create many particle systems using this as a template parameter list.
	//////////////////////////////////////////////////////////////////////////////////////

	CSulQtPropertySheet*		m_psParticles;
	CSulQtPropertySheet*		m_psParticleSystem;
	CSulQtPropertySheet*		m_psModularEmitter;
	CSulQtPropertySheet*		m_psCounterRandomRate;
	CSulQtPropertySheet*		m_psSectorPlacer;
	CSulQtPropertySheet*		m_psShooterRadial;
	CSulQtPropertySheet*		m_psProgramFluid;

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


#endif // __SULPARTICLESYSTEMOSG_H__