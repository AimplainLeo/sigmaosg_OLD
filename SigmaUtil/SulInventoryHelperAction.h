// SulInventoryHelperAction.h

#ifndef __SULINVENTORYHELPERACTION_H__
#define __SULINVENTORYHELPERACTION_H__

class CSulInventoryHelperAction : public osg::Referenced
{
public:
	CSulInventoryHelperAction( CSulInventory* inventory, const CSulString& action )
	{
		m_inventory = inventory;
		m_action = action;
		m_timeStart = m_time = 0.2;
	}

	// returns true when this action is done and should be deleted
	bool update( double dt )
	{
		m_time -= dt;

		if ( m_time<0.0 )
		{
			if ( m_action=="hide" )
				m_inventory->hide();

			return true;
		}

		if ( m_action=="show" )
			m_inventory->show();

		// goes from 1 to 0
		double ntime = m_time/m_timeStart;

		if ( m_action=="hide" )
			m_inventory->setDistanceFactor( ntime );

		if ( m_action=="show" )
			m_inventory->setDistanceFactor( 1.0 - ntime );

		return false;
	}

private:
	osg::ref_ptr<CSulInventory>	m_inventory;
	CSulString					m_action;
	double						m_time;
	double						m_timeStart;
};

#endif // __SULINVENTORYHELPERACTION_H__