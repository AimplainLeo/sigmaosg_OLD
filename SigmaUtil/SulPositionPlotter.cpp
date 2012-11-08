// SulPositionPlotter.cpp

#include "stdafx.h"
#include "SulPositionPlotter.h"

CSulPositionPlotter::CSulPositionPlotter( double timePulse, float dist, osg::Vec3 offset )
{
	m_offset = offset;
	m_timePulse = timePulse;
	m_dist		= dist;
	m_bFirstTime = true;
}

void CSulPositionPlotter::operator()( osg::Node* node, osg::NodeVisitor* nv )
{
	traverse( node, nv );

	double simTime = nv->getFrameStamp()->getSimulationTime();

	if ( m_bFirstTime )
	{
		m_lastTime = simTime;
		m_bFirstTime = false;
		return;
	}

	double deltaTime = simTime-m_lastTime;
	if ( deltaTime>m_timePulse )
	{
		osg::Matrix m;
		m.set( osg::computeLocalToWorld(nv->getNodePath()) );
		osg::Vec3 pos = m.getTrans();

		if ( m_vecPositions.size() )
		{
			osg::Vec3 vDist = pos-m_vecPositions[m_vecPositions.size()-1];
			float distFromLast = vDist.length();

			// only save position if object has moved enough
			if ( distFromLast>m_dist )
				m_vecPositions.push_back( pos+m_offset );
		}
		else
			m_vecPositions.push_back( pos+m_offset );

		m_lastTime = simTime;
	}
}

sigma::VEC_VEC3& CSulPositionPlotter::getPositions()
{
	return m_vecPositions;
}