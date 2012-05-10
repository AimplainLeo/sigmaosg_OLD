// SulParticleUpdater.cpp

#include "stdafx.h"
#include "SulParticleUpdater.h"
#include "SulParticleSystem.h"

void CSulParticleUpdater::operator()( osg::Node* node, osg::NodeVisitor* nv )
{
	traverse( node, nv );

	if ( m_bFirstInit )
	{
		m_bFirstInit = false;
		m_lastTicks = osg::Timer::instance()->tick();
		return;
	}

	double ticks = osg::Timer::instance()->tick();
	double dt = osg::Timer::instance()->delta_s( m_lastTicks, ticks );
	m_lastTicks = ticks;

	m_ps->update( dt );
}
