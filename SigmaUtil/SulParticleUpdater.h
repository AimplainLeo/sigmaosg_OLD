// SulParticleUpdater.h

#ifndef __SULPARTICLEUPDATER_H__
#define __SULPARTICLEUPDATER_H__

#include "SulExport.h"
#include <osg/timer>

class SUL_EXPORT CSulParticleUpdater : public osg::NodeCallback
{
public:
	CSulParticleUpdater( class CSulParticleSystem* ps )
	{
		m_bFirstInit = true;
		m_ps = ps;
	}

	virtual void operator()( osg::Node* node, osg::NodeVisitor* nv );

private:
	class CSulParticleSystem*	m_ps;
	bool						m_bFirstInit;
	osg::Timer_t				m_lastTicks;
};

#endif // __SULPARTICLEUPDATER_H__