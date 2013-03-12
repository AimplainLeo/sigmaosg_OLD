// ParticleDebris.cpp

#include "stdafx.h"
#include "ParticleDebrisSystem.h"
#include "ParticleDebris.h"

CParticleDebrisSystem::CParticleDebrisSystem( 
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
) :
CSulParticleSystem( debrisCount )
{
	m_debrisPosOffsetMin = debrisPosOffsetMin;
	m_debrisPosOffsetMax = debrisPosOffsetMax;

	CSulParticleEmitter* emitter = getEmitter();

	emitter->setSpeedRange( osg::Vec2(debrisSpeedMin, debrisSpeedMax) );
	emitter->setThetaRange( osg::Vec2(debrisThetaMin, debrisThetaMax) );
	emitter->setPhiRange( osg::Vec2(debrisPhiMin, debrisPhiMax) );

	m_particleMass		= particleMass;
	m_particleLifeTime	= particleLifeTime;
	m_particleSizeMin	= particleSizeMin;
	m_particleSizeMax	= particleSizeMax;
	m_particleAlphaMin	= particleAlphaMin;
	m_particleAlphaMax	= particleAlphaMax;
	
	m_emitterLifeTime	= emitterLifeTime;
	m_rateMin			= rateMin;
	m_rateMax			= rateMax;
}

void CParticleDebrisSystem::createParticle( const osg::Vec3& velocity ) 
{
	add( new CParticleDebris( 
		velocity,
		m_debrisPosOffsetMin,
		m_debrisPosOffsetMax,
		m_particleMass,
		m_particleLifeTime,
		m_particleSizeMin,
		m_particleSizeMax,
		m_particleAlphaMin,
		m_particleAlphaMax,
		m_emitterLifeTime,
		m_rateMin,
		m_rateMax
	));
}

void CParticleDebrisSystem::remitParticle( CSulParticle* p )
{
//	p->reset();
//	p->setVelocity( createVelocity() );	
}
