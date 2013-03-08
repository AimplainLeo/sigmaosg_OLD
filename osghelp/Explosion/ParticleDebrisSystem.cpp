// ParticleDebris.cpp

#include "stdafx.h"
#include "ParticleDebrisSystem.h"
#include "ParticleDebris.h"

CParticleDebrisSystem::CParticleDebrisSystem( sigma::uint32 numParticles ) :
CSulParticleSystem( numParticles )
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
