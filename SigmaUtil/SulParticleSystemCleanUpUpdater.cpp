// SulParticleSystemCleanUpUpdater.cpp

#include "stdafx.h"
#include "SulParticleSystemCleanUpUpdater.h"
#include "SulParticleSystemOsg.h"

CSulParticleSystemCleanUpUpdater::CSulParticleSystemCleanUpUpdater( CSulParticleSystemOsg* ps )
{
	m_ps = ps;
}

void CSulParticleSystemCleanUpUpdater::operator()( osg::Node* node, osg::NodeVisitor* nv )
{
	if ( nv->getVisitorType()==osg::NodeVisitor::UPDATE_VISITOR && nv->getFrameStamp() )
    {
		if ( !m_ps->isAlive() )
			m_ps->removeAll();
    }

	NodeCallback::traverse( node, nv );
}
