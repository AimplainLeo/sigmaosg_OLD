// SulParticleEmitter.h

#ifndef __SULPARTICLEEMITTER_H__
#define __SULPARTICLEEMITTER_H__

#include "SulParticleCounter.h"
#include <osg/referenced>
#include <osg/vec2>
#include <osg/Math>
#include <osgParticle/range>

class CSulParticleEmitter : public osg::Referenced
{
friend class CSulParticleSystem;

public:
				CSulParticleEmitter();

	// muse be set if a particle is to be created
	void		setParticleSystem( CSulParticleSystem* ps );

	void		setCounter( CSulParticleCounter* counter );

	void		setEndless( bool endless );
	void		setLifeTime( float lifetime );

	void		setThetaRange( float min, float max );
	void		setThetaRange( const osg::Vec2& range );

	void		setPhiRange( float min, float max );
	void		setPhiRange( const osg::Vec2& range );

	void		setSpeedRange( float min, float max );
	void		setSpeedRange( const osg::Vec2& range );

	osg::Vec3	get();

protected:
	virtual void emitParticle();

private:
	// called by SulParticleSystem
	void		update( double dt );

private:
	CSulParticleSystem*					m_ps;
	osg::ref_ptr<CSulParticleCounter>	m_counter;

	double				m_age;
	float				m_lifeTime;
	bool				m_endless;
	osgParticle::rangef	m_rangeTheta;	// around		0 = x axis, pi = -xaxis		(range 0 - 2pi)
	osgParticle::rangef m_rangePhi;		// up/down		0 = top, pi = bottom		(range 0 - pi)
	osgParticle::rangef m_rangeSpeed;
};

#endif // __SULPARTICLEEMITTER_H__