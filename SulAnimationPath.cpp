// SulAnimationPath.cpp

#include "stdafx.h"
#include "SulAnimationPath.h"
#include "SulAnimationPathVisitor.h"
#include <osg/NodeVisitor>

CSulAnimationPathCallback::CSulAnimationPathCallback() :
m_latestTime(0.0f),
m_pivotPoint(0.0,0.0,0.0),
m_eMode(MODE_STOP),
m_time(0.0),
m_lastTime(0.0),
m_bNotified(false)
{
}

void CSulAnimationPathCallback::SetMode( EMODE eMode )
{
	m_eMode = eMode;
	m_lastTime = m_latestTime;
	m_bNotified = false;

	if ( m_time>m_animationPath->getLastTime() )
	{
		m_time = m_animationPath->getLastTime();
	}
	else if ( m_time<m_animationPath->getFirstTime() )
	{
		m_time = m_animationPath->getFirstTime();
	}
}

double CSulAnimationPathCallback::GetAnimationTime() 
{
	switch ( m_eMode )
	{
		case MODE_PLAY_FORWARD:
			m_time += (m_latestTime-m_lastTime);
			break;

		case MODE_PLAY_REVERSE:
			m_time -= (m_latestTime-m_lastTime);
			break;
	}

	return m_time;
}

void CSulAnimationPathCallback::SetPath( osg::AnimationPath* path ) 
{ 
	m_animationPath = path; 
}

void CSulAnimationPathCallback::operator()( osg::Node* node, osg::NodeVisitor* nv )
{
	if (	m_animationPath.valid() && 
			nv->getVisitorType()==osg::NodeVisitor::UPDATE_VISITOR && 
			nv->getFrameStamp() )
    {
		double time = nv->getFrameStamp()->getSimulationTime();
		m_lastTime = m_latestTime;
		m_latestTime = time;

		Update( *node );
		UpdateCallbacks();
    }

	NodeCallback::traverse( node, nv );
}

void CSulAnimationPathCallback::Update( osg::Node& node )
{
	osg::AnimationPath::ControlPoint cp;

    if ( m_animationPath->getInterpolatedControlPoint(GetAnimationTime(),cp) )
    {
        CSulAnimationPathVisitor apcv(cp,m_pivotPoint);
        node.accept(apcv);
    }
}

void CSulAnimationPathCallback::UpdateCallbacks()
{
	if ( m_bNotified )
	{
		return;
	}

	EACTION eAction = ACTION_UNKNOWN;

	switch ( m_eMode )
	{
		case MODE_PLAY_FORWARD:
			{
				if ( m_time>m_animationPath->getLastTime() )
				{
					eAction = ACTION_END;
					m_bNotified = true;
				}
			}
			break;

		case MODE_PLAY_REVERSE:
			{
				if ( m_time<m_animationPath->getFirstTime() )
				{
					eAction = ACTION_END;
					m_bNotified = true;
				}
			}
			break;
	}

	if ( m_bNotified )
	{
		MULTIMAP_ANIPATHCALLBACK::iterator i;
		for ( i=m_multimapAniPathCallback.lower_bound(eAction); i!=m_multimapAniPathCallback.upper_bound(eAction); ++i )
		{
			i->second->Call();
		}
	}
}

void CSulAnimationPathCallback::WantCallback( CSulAnimationPathFunctorBase* pFunctor, EACTION eAction )
{
	m_multimapAniPathCallback.insert( MULTIMAP_ANIPATHCALLBACK::value_type(eAction, pFunctor) );
}
