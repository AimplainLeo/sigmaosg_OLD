// SulGuiProfiler.h

#ifndef __SULGUIPROFILER_H__
#define __SULGUIPROFILER_H__

#include "SulGuiCanvas.h"
#include "SulProfiler.h"

class SUL_EXPORT CSulGuiProfiler : public CSulGuiCanvas
{
public:
	CSulGuiProfiler();
	CSulGuiProfiler( CSulProfiler* pProfiler );

	virtual void	setupAttr( CSulXmlAttr* pAttr );

	virtual void	init();

	void			setProfiler( CSulProfiler* pProfiler );

	virtual class CSulGuiProfiler*			asProfiler()		{ return this; }

private:
	osg::ref_ptr<CSulProfiler>		m_rProfiler;

};

#endif // __SULGUIPROFILER_H__