// SulParticleEmitter.h

#ifndef __SULPARTICLEEMITTER_H__
#define __SULPARTICLEEMITTER_H__

#include <osg/referenced>
#include <osg/vec2>

class CSulParticleEmitter : public osg::Referenced
{
public:
	CSulParticleEmitter();

	void setThetaRange( const osg::Vec2& range );
	void setPhiRange( const osg::Vec2& range );

private:
	osg::Vec2	m_rangeTheta;	// around		0 = x axis, pi = -xaxis		(range 0 - 2pi)
	osg::Vec2	m_rangePhi;		// up/down		0 = top, pi = bottom		(range 0 - pi)
};

#endif // __SULPARTICLEEMITTER_H__