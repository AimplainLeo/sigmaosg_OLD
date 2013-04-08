// SulParticleSystemOsg.cpp

#include "stdafx.h"
#include "SulParticleSystemOsg.h"

CSulParticleSystemOsg::CSulParticleSystemOsg() :
osg::Group()
{
	m_psu = new osgParticle::ParticleSystemUpdater;
	addChild( m_psu );
}


void CSulParticleSystemOsg::removeAll()
{
	// remove from updater
	VEC_PARTICLESYSTEMCONTAINEROSG::iterator i = m_vecParticleSystemContainer.begin();
	VEC_PARTICLESYSTEMCONTAINEROSG::iterator ie = m_vecParticleSystemContainer.end();
	while ( i!=ie )
	{
		CSulParticleSystemContainerOsg* psContainer = (*i);
		psContainer->destroy();
		m_psu->removeParticleSystem( psContainer );
		++i;
	}

	m_vecParticleSystemContainer.clear();
}


void CSulParticleSystemOsg::create( CSulParticleSystemContainerOsg* psContainer, const osg::Vec3& pos )
{
	psContainer->create( pos );
	m_psu->addParticleSystem( psContainer );	

	m_vecParticleSystemContainer.push_back( psContainer );
}

