// SulParticleCounterOnce.h

#ifndef __SULPARTICLECOUNTERONCE_H__
#define __SULPARTICLECOUNTERONCE_H__

#include "SulParticleCounter.h"

class CSulParticleCounterOnce : public CSulParticleCounter
{
public:
	CSulParticleCounterOnce();

protected:
	void update( double dt );

private:
	bool	m_bCalledOnce;
};

#endif // __SULPARTICLECOUNTERONCE_H__