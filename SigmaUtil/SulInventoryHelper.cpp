// SulInventoryHelper.cpp

#include "stdafx.h"
#include "SulInventoryHelper.h"

CSulInventoryHelper::CSulInventoryHelper()
{
	m_bFirstInit = true; 
}

void CSulInventoryHelper::push( CSulInventory* inventory, const CSulString& action )
{
	CSulInventoryHelperAction* helperAction = new CSulInventoryHelperAction( inventory, action );
	m_vecActions.push_front( helperAction );
}

void CSulInventoryHelper::operator()( osg::Node* node, osg::NodeVisitor* nv )
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

	if ( m_vecActions.size() )
	{
		CSulInventoryHelperAction* helperAction = m_vecActions[m_vecActions.size()-1];
		if ( helperAction->update( dt ) )
			m_vecActions.pop_back();
	}

}
