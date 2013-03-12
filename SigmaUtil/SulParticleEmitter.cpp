// SulParticleEmitter.cpp

#include "stdafx.h"
#include "SulParticleEmitter.h"

CSulParticleEmitter::CSulParticleEmitter()
{
	// set defaults
	m_rangeTheta.set( 0, 2*osg::PI );
	m_rangePhi.set( 0, osg::PI_2 );
	m_rangeSpeed.set( 1.0f, 10.0f );
}

void CSulParticleEmitter::setThetaRange( const osg::Vec2& range )
{
	m_rangeTheta.set( range.x(), range.y() );
}

void CSulParticleEmitter::setPhiRange( const osg::Vec2& range )
{
	m_rangePhi.set( range.x(), range.y() );
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
