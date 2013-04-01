// SulParticleCounterOnce.cpp

#include "stdafx.h"
#include "SulParticleCounterOnce.h"


CSulParticleCounterOnce::CSulParticleCounterOnce() :
CSulParticleCounter()
{
	m_bCalledOnce = false;
}

void CSulParticleCounterOnce::update( double dt )
{
	m_numParticlesToCreate = 0;

	if ( m_bCalledOnce )
		return;

	m_numParticlesToCreate = 1;
	m_bCalledOnce = true;
}