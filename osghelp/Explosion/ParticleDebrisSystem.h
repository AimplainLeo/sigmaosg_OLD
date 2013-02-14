// ParticleDebris.h

#ifndef __PARTICLEDEBRISSYSTEM_H__
#define __PARTICLEDEBRISSYSTEM_H__

#include <SigmaUtil\SulParticleSystem.h>

class CParticleDebrisSystem : public CSulParticleSystem
{
public:
	CParticleDebrisSystem();

protected:
	void createParticle( const osg::Vec3& velocity );
	void remitParticle( CSulParticle* p );
};


#endif // __PARTICLEDEBRISSYSTEM_H__