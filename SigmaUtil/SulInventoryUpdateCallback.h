// SulInventoryUpdateCallback.h

#ifndef __SULINVENTORYUPDATECALLBACK_H__
#define __SULINVENTORYUPDATECALLBACK_H__

#include <osg/timer>

class CSulInventoryUpdateCallback : public osg::NodeCallback
{
public:
	CSulInventoryUpdateCallback( CSulInventory* inventory )
	{
		m_inventory = inventory;
		m_bFirstInit = true;
	}

	virtual void operator()( osg::Node* node, osg::NodeVisitor* nv )
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

		m_inventory->update( dt );
	}

private:
	bool			m_bFirstInit;
	osg::Timer_t	m_lastTicks;
	CSulInventory*	m_inventory;
};

#endif // __SULINVENTORYUPDATECALLBACK_H__