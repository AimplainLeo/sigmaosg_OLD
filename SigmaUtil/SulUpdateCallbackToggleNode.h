// SulUpdateCallbackToggleNode.h

#ifndef __SULUPDATECALLBACKTOGGLENODE_H__
#define __SULUPDATECALLBACKTOGGLENODE_H__

#include <osg/timer>

class CSulUpdateCallbackToggleNode : public osg::NodeCallback
{
public:
	CSulUpdateCallbackToggleNode( osg::Node* pNodeToToggle, double secondsToToggle )
	{
		m_secondsToToggle = secondsToToggle;
		m_startTicks = osg::Timer::instance()->tick();
		m_rNodeToToggle = pNodeToToggle;
		m_elapsedTime = 0.0;
	}

	// if pulse = 2 then the laser will pulse 2x a second
	void setPulseFrequency( double pulse )
	{
		if ( pulse==0 )
			m_rNodeToToggle->setNodeMask( 0xFFFFFFFF );

		m_secondsToToggle = (1.0/pulse)/2.0;
	}

	virtual void operator()( osg::Node* node, osg::NodeVisitor* nv )
    {
		traverse( node, nv );

		if ( m_secondsToToggle!=0.0 )
		{
			double ticks = osg::Timer::instance()->tick();
			double dt = osg::Timer::instance()->delta_s(m_startTicks, ticks);
			m_startTicks = ticks;

			m_elapsedTime += dt;
			if ( m_elapsedTime>m_secondsToToggle )
			{
				if ( m_rNodeToToggle->getNodeMask() )
					m_rNodeToToggle->setNodeMask( 0 );
				else
					m_rNodeToToggle->setNodeMask( 0xFFFFFFFF );

				m_elapsedTime = 0.0;
			}
		}
	}

private:
	osg::ref_ptr<osg::Node> m_rNodeToToggle;
	osg::Timer_t			m_startTicks;
	double					m_elapsedTime;
	double					m_secondsToToggle;
};

#endif // __SULUPDATECALLBACKTOGGLENODE_H__