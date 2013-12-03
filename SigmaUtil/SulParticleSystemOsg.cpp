// SulParticleSystemOsg.cpp

#include "stdafx.h"
#include "SulParticleSystemOsg.h"
#include "SulParticleSystemCleanUpUpdater.h"

CSulParticleSystemOsg::CSulParticleSystemOsg() :
osg::Group()
{
	m_psu = new osgParticle::ParticleSystemUpdater;
	addChild( m_psu );

	// we use the cleanUp updator to remove dead particle systems
	addUpdateCallback( new CSulParticleSystemCleanUpUpdater(this) );
}

CSulParticleSystemOsg::~CSulParticleSystemOsg()
{
	removeAll();
}

bool CSulParticleSystemOsg::isAlive()
{
	VEC_PARTICLESYSTEMCONTAINEROSG::iterator i = m_vecParticleSystemContainer.begin();
	VEC_PARTICLESYSTEMCONTAINEROSG::iterator ie = m_vecParticleSystemContainer.end();
	while ( i!=ie )
	{
		CSulParticleSystemContainerOsg* psContainer = (*i);
		if ( psContainer->m_emitter->isAlive() || !psContainer->areAllParticlesDead() )
			return true;

		++i;
	}

	return false;
}


void CSulParticleSystemOsg::removeAll()
{
	// remove from updater
	VEC_PARTICLESYSTEMCONTAINEROSG::iterator i = m_vecParticleSystemContainer.begin();
	VEC_PARTICLESYSTEMCONTAINEROSG::iterator ie = m_vecParticleSystemContainer.end();
	while ( i!=ie )
	{
		CSulParticleSystemContainerOsg* psContainer = (*i);

		removeChild( psContainer->getGeode() );

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

	addChild( psContainer->getGeode() );
}

void CSulParticleSystemOsg::setWind( const osg::Vec3& wind )
{
	VEC_PARTICLESYSTEMCONTAINEROSG::iterator i = m_vecParticleSystemContainer.begin();
	VEC_PARTICLESYSTEMCONTAINEROSG::iterator ie = m_vecParticleSystemContainer.end();
	while ( i!=ie )
	{
		CSulParticleSystemContainerOsg* psContainer = (*i);
		psContainer->setWind( wind );
		++i;
	}
}