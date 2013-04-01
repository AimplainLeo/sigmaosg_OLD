// SulParticleEmitter.cpp

#include "stdafx.h"
#include "SulParticleEmitter.h"
#include "SulParticleCounterOnce.h"
#include "SulParticleSystem.h"

CSulParticleEmitter::CSulParticleEmitter()
{
	// set defaults
	m_age = 0.0;
	m_ps = 0;
	m_lifeTime = 2.0f;
	m_endless = false;
	m_rangeTheta.set( 0, 2*osg::PI );
	m_rangePhi.set( 0, osg::PI_2 );
	m_rangeSpeed.set( 1.0f, 10.0f );

	m_counter = new CSulParticleCounterOnce;
}

void CSulParticleEmitter::setParticleSystem( CSulParticleSystem* ps )
{
	m_ps = ps;
}

void CSulParticleEmitter::setCounter( CSulParticleCounter* counter )
{
	m_counter = counter;
}

void CSulParticleEmitter::setEndless( bool endless )
{
	m_endless = endless;
}

void CSulParticleEmitter::setLifeTime( float lifetime )
{
	m_lifeTime = lifetime;
}

void CSulParticleEmitter::setThetaRange( float min, float max )
{
	setThetaRange( osg::Vec2(min,max) );
}

void CSulParticleEmitter::setThetaRange( const osg::Vec2& range )
{
	m_rangeTheta.set( range.x(), range.y() );
}

void CSulParticleEmitter::setPhiRange( float min, float max )
{
	setPhiRange( osg::Vec2(min, max) );
}

void CSulParticleEmitter::setPhiRange( const osg::Vec2& range )
{
	m_rangePhi.set( range.x(), range.y() );
}

void CSulParticleEmitter::setSpeedRange( float min, float max )
{
	setSpeedRange( osg::Vec2( min, max ) );
}

void CSulParticleEmitter::setSpeedRange( const osg::Vec2& range )
{
	m_rangeSpeed.set( range.x(), range.y() );
}

osg::Vec3 CSulParticleEmitter::get()
{
	osg::Vec3 v;
	
    float theta = m_rangeTheta.get_random();
    float phi = m_rangePhi.get_random();
	float speed = m_rangeSpeed.get_random();

	float x = sinf(phi) * cosf(theta);
	float y = sinf(phi) * sinf(theta);
	float z = cosf(phi);

	v.set(x, y, z);
	v.normalize();
	
	return v * speed;
}

void CSulParticleEmitter::update( double dt )
{
	m_age += dt;

	if ( !m_endless )
	{
		if ( m_age>m_lifeTime )
			return;
	}

	m_counter->update( dt );

	sigma::uint32 numParticles = m_counter->numParticlesToCreate();
	for ( sigma::uint32 i=0; i<numParticles; i++ )
		emitParticle();
}

void CSulParticleEmitter::emitParticle()
{
	m_ps->createParticle( get() );
}
