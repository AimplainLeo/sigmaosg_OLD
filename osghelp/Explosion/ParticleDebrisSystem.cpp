// ParticleDebris.cpp

#include "stdafx.h"
#include "ParticleDebrisSystem.h"
#include "ParticleDebris.h"

#define NUM_PARTICLES (10)

CParticleDebrisSystem::CParticleDebrisSystem() :
CSulParticleSystem( NUM_PARTICLES )
{
}

void CParticleDebrisSystem::createParticle( const osg::Vec3& velocity ) 
{
	add( new CParticleDebris( velocity ) );
}

void CParticleDebrisSystem::remitParticle( CSulParticle* p )
{
//	p->reset();
//	p->setVelocity( createVelocity() );	
}
