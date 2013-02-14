// SulParticleSystem.h

#ifndef __SULPARTICLESYSTEM_H__
#define __SULPARTICLESYSTEM_H__

#include "SulParticleUpdater.h"
#include "SulParticleEmitter.h"
#include "SulMath.h"
#include "SulParticle.h"
#include "SulTypes.h"
#include <algorithm>
#include <osg/group>
#include <osg/MatrixTransform>

/*

	CSulParticleSystem
		numParticles	: maximum number of particles the system can work with

*/

class CSulParticleSystem : public osg::MatrixTransform
{
public:
	CSulParticleSystem( sigma::uint32 numParticles )
	{
		m_numParticles = numParticles;
		m_emitterVelocityFlux.set( 10.0f, 10.0f, 10.0f );		// fluxation on velocity when created
		m_emitterVelocity.set( 0,0,3 );		// distance per second
	}

	void init()
	{
		// create default emitter, if one is not supplied
		if ( !m_emitter.valid() )
		{
			m_emitter = new CSulParticleEmitter;
		}

		for ( sigma::uint32 i=0; i<m_numParticles; i++ )
		{
			createParticle( createVelocity() );
		}

		addUpdateCallback( new CSulParticleUpdater( this ) );
	}

	void setEmitter( CSulParticleEmitter* emitter )	
	{
		m_emitter = emitter;
	}
	
	osg::Vec3 createVelocity()
	{
		float x = m_emitterVelocityFlux.x()*sigma::rand0to1() - m_emitterVelocityFlux.x()/2.0f;
		float y = m_emitterVelocityFlux.y()*sigma::rand0to1() - m_emitterVelocityFlux.y()/2.0f;
		float z = m_emitterVelocityFlux.z()*sigma::rand0to1() - m_emitterVelocityFlux.z()/2.0f;
		return m_emitterVelocity + osg::Vec3(
			m_emitterVelocity.x() + x,
			m_emitterVelocity.y() + y,
			m_emitterVelocity.z() + z );
	}

	void add( CSulParticle* p )
	{
		m_vecParticlesAdd.push_back( p );
	}

	void remove( CSulParticle* p )
	{
		m_vecParticlesRemove.push_back( p );
	}

	void update( double dt )
	{
		VEC_PARTICLES::iterator is;
		VEC_PARTICLES::iterator ie;

		// remove particles
		if ( m_vecParticlesRemove.size() )
		{
			is = m_vecParticlesRemove.begin();
			ie = m_vecParticlesRemove.end();

			while ( is!=ie )
			{
				VEC_PARTICLES::iterator i = std::find(m_vecParticles.begin(), m_vecParticles.end(), *is );

				if ( (*i)->getNode() )
					removeChild( (*i)->getNode() );

				m_vecParticles.erase( i );
				++is;
			}

			m_vecParticlesRemove.clear();
		}

		// add particles
		if ( m_vecParticlesAdd.size() )
		{
			is = m_vecParticlesAdd.begin();
			ie = m_vecParticlesAdd.end();

			while ( is!=ie )
			{
				if ( (*is)->getNode() )
					addChild( (*is)->getNode() );

				m_vecParticles.push_back( *is );

				++is;
			}

			m_vecParticlesAdd.clear();
		}
	

		// update particles
		is = m_vecParticles.begin();
		ie = m_vecParticles.end();

		while ( is!=ie )
		{
			(*is)->update( dt );

			// check to see if particle is alive, if not re-emit it
			if ( !(*is)->alive() )
				remitParticle( (*is) );

			++is;
		}
	}

	virtual void remitParticle( CSulParticle* P ) {}

	virtual void createParticle( const osg::Vec3& velocity ) {}

private:
	osg::ref_ptr<CSulParticleEmitter>	m_emitter;

	sigma::uint32	m_numParticles;
	osg::Vec3		m_emitterVelocity;
	osg::Vec3		m_emitterVelocityFlux;
	VEC_PARTICLES	m_vecParticles;

	VEC_PARTICLES	m_vecParticlesAdd;
	VEC_PARTICLES	m_vecParticlesRemove;
};

#endif // __SULPARTICLESYSTEM_H__