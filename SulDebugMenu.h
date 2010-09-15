// SulDebugMenu.h

#ifndef __SULDEBUGMENU_H__
#define __SULDEBUGMENU_H__

#include "SulDebugValueBase.h"
#include "SulString.h"
#include "SulExport.h"
#include <osg/geode>

class SUL_EXPORT CSulDebugMenu : public osg::Group
{
private:
	typedef std::vector<CSulDebugValueBase*>	VECTOR_VAL;

public:
						CSulDebugMenu();

	void				add( CSulDebugValueBase* pVal );

	std::string			getCurTitle();
	CSulDebugValueBase*	getCurValueObj();

	CSulDebugValueBase*	getValueObj( const std::string& sTitle );

	void				next();
	void				prev();

	virtual void		inc();
	virtual void		dec();

	virtual void		valueChanged( CSulDebugValueBase* pObj );

private:
	osg::ref_ptr<osg::Geode>	m_geode;
	VECTOR_VAL					m_vecVal;
	int							m_iCurSel;
};

#endif // __SULDEBUGMENU_H__