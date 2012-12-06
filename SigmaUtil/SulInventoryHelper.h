// SulInventoryHelper.h

#ifndef __SULINVENTORYHELPER_H__
#define __SULINVENTORYHELPER_H__

#include "SulInventory.h"
#include "SulInventoryHelperAction.h"
#include "SulExport.h"
#include <osg/timer>
#include <deque>

class SUL_EXPORT CSulInventoryHelper : public osg::NodeCallback
{
public:
	CSulInventoryHelper();

	void push( CSulInventory* inventory, const CSulString& action );

	virtual void operator()( osg::Node* node, osg::NodeVisitor* nv );
	
private:
	typedef std::deque< osg::ref_ptr<CSulInventoryHelperAction> >	VEC_ACTIONS;
	VEC_ACTIONS														m_vecActions;

private:
	bool			m_bFirstInit;
	osg::Timer_t	m_lastTicks;
};

#endif // __SULINVENTORYHELPER_H__