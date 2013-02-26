// SulParticleEmitter.cpp

#include "stdafx.h"
#include "SulParticleEmitter.h"

CSulParticleEmitter::CSulParticleEmitter()
{
	// set defaults
	m_rangeTheta.set( 0, 2*osg::PI );
	m_rangePhi.set( 0, osg::PI_2 );
}

void CSulParticleEmitter::setThetaRange( const osg::Vec2& range )
{
	m_rangeTheta = range;
}

void CSulParticleEmitter::setPhiRange( const osg::Vec2& range )
{
	m_rangePhi = range;
}
