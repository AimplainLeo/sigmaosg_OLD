// SulParticleCounter.h

#ifndef __SULPARTICLECOUNTER_H__
#define __SULPARTICLECOUNTER_H__

#include "SulTypes.h"

class CSulParticleCounter : public osg::Referenced
{
friend class CSulParticleEmitter;

public:
	CSulParticleCounter()
	{
		m_numParticlesToCreate = 0;
	}

	sigma::uint32 numParticlesToCreate()
	{
		return m_numParticlesToCreate;
	}

private:
	// called by SulParticleEmitter
	virtual void update( double dt ) = 0;

protected:
	sigma::uint32	m_numParticlesToCreate;
};

#endif // __SULPARTICLECOUNTER_H__