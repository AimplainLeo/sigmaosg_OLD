// ParticleDebris.h

#ifndef __PARTICLEDEBRISSYSTEM_H__
#define __PARTICLEDEBRISSYSTEM_H__

#include <SigmaUtil\SulParticleSystem.h>

class CParticleDebrisSystem : public CSulParticleSystem
{
public:
	CParticleDebrisSystem( 
		sigma::uint32 debrisCount,
		float debrisPosOffsetMin,
		float debrisPosOffsetMax,
		float debrisSpeedMin,
		float debrisSpeedMax,
		float debrisThetaMin,
		float debrisThetaMax,
		float debrisPhiMin,
		float debrisPhiMax,
		float particleMass,
		float particleLifeTime,
		float particleSizeMin,
		float particleSizeMax,
		float particleAlphaMin,
		float particleAlphaMax,
		float emitterLifeTime,
		float rateMin,
		float rateMax
 );

protected:
	void createParticle( const osg::Vec3& velocity );
	void remitParticle( CSulParticle* p );

private:
	float	m_debrisPosOffsetMin;
	float	m_debrisPosOffsetMax;
	float	m_particleMass;
	float	m_particleLifeTime;
	float	m_particleSizeMin;
	float	m_particleSizeMax;
	float	m_emitterLifeTime;
	float	m_particleAlphaMin;
	float	m_particleAlphaMax;
	float	m_rateMin;
	float	m_rateMax;
};


#endif // __PARTICLEDEBRISSYSTEM_H__