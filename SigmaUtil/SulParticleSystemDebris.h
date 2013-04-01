// SulParticleSystemDebris.h

#ifndef __SULPARTICLESYSTEMDEBRIS_H__
#define __SULPARTICLESYSTEMDEBRIS_H__

#include "SulParticleSystem.h"
#include "SulParticleSystemDataOsg.h"
#include <osgParticle\centeredplacer>

class CSulParticleSystemDebris : public CSulParticleSystem
{
public:
	CSulParticleSystemDebris( osgParticle::CenteredPlacer* placer, CSulParticleSystemDataOsg* data );

protected:
	void createParticle( const osg::Vec3& velocity );
	void remitParticle( CSulParticle* p );

private:
	CSulParticleSystemDataOsg*					m_data;
	osg::ref_ptr< osgParticle::CenteredPlacer>	m_placer;
};


#endif // __SULPARTICLESYSTEMDEBRIS_H__