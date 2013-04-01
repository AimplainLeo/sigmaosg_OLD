// SulParticleSystemDebris.cpp

#include "stdafx.h"
#include "SulParticleSystemDebris.h"
#include "SulParticleDebris.h"

CSulParticleSystemDebris::CSulParticleSystemDebris( osgParticle::CenteredPlacer* placer, CSulParticleSystemDataOsg* data ) :
CSulParticleSystem(1)
{
	m_data = data;
	m_placer = placer;
}

void CSulParticleSystemDebris::createParticle( const osg::Vec3& velocity )
{
	CSulParticleDebris* p = new CSulParticleDebris( m_placer, m_data, velocity );
	p->enableDebug( this );
	add( p );
}

void CSulParticleSystemDebris::remitParticle( CSulParticle* p )
{
}
