// SulParticleSystemInterfaceOsg.h

#ifndef __SULPARTICLESYSTEMINTERFACEOSG_H__
#define __SULPARTICLESYSTEMINTERFACEOSG_H__

#include "SulParticleSystemDataOsg.h"
#include "SulParticleSystemDebris.h"

// need to undefine emit because Qt has defined it and conflicts with osg
#undef emit
#include <osgParticle/ParticleSystem>
#include <osgParticle/ModularEmitter>
#include <osgParticle/SectorPlacer>
#include <osgParticle/FluidProgram>
#include <osgDB/FileUtils>
#include <osg/group>

/*
	CSulParticleSystemContainerOsg :

		creates a particle effect defined by the data
*/


class CSulParticleSystemContainerOsg : public osgParticle::ParticleSystem
{
public:
											CSulParticleSystemContainerOsg( CSulParticleSystemDataOsg* data, osg::Group* root );

	void									create( const osg::Vec3& pos );
	void									destroy();

	CSulParticleSystemDataOsg*				getData();
	
public:
	osg::ref_ptr<osg::Group>				m_root;

	CSulParticleSystemDataOsg*				m_data;

	osg::ref_ptr<CSulParticleSystemDebris>	m_debris;
	//osg::ref_ptr<osg::MatrixTransform>		m_debrisMT;

	osg::ref_ptr<osg::MatrixTransform>		m_particleSystemMT;

	osgParticle::Particle*					m_particle;
	osgParticle::ModularEmitter*			m_emitter;
	osgParticle::RandomRateCounter*			m_counterRandomRate;
	osgParticle::SectorPlacer*				m_placer;
	osgParticle::RadialShooter*				m_shooterRadial;
	osgParticle::FluidProgram*				m_programFluid;
};

#endif // __SULPARTICLESYSTEMINTERFACEOSG_H__