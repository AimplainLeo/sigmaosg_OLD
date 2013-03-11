// SulParticleEmitter.h

#ifndef __SULPARTICLEEMITTER_H__
#define __SULPARTICLEEMITTER_H__

#include <osg/referenced>
#include <osg/vec2>
#include <osg/Math>
#include <osgParticle/range>

class CSulParticleEmitter : public osg::Referenced
{
public:
	CSulParticleEmitter();

	void		setThetaRange( const osg::Vec2& range );
	void		setPhiRange( const osg::Vec2& range );
	void		setSpeedRange( const osg::Vec2& range );

	osg::Vec3	get();

private:
	osgParticle::rangef	m_rangeTheta;	// around		0 = x axis, pi = -xaxis		(range 0 - 2pi)
	osgParticle::rangef m_rangePhi;		// up/down		0 = top, pi = bottom		(range 0 - pi)
	osgParticle::rangef m_rangeSpeed;
};

#endif // __SULPARTICLEEMITTER_H__