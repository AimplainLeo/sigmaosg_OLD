// SulPositionPlotterManager.h

#ifndef __SULPOSITIONPLOTTERMANAGER_H__
#define __SULPOSITIONPLOTTERMANAGER_H__

#include "SulExport.h"
#include "SulPositionPlotter.h"

class SUL_EXPORT CSulPositionPlotterManager : public osg::Referenced
{
private:
	typedef std::vector< CSulPositionPlotter* > VEC_POSITIONPLOTTER;

public:
	void	add( CSulPositionPlotter* plotterPosition );

	osg::Node*		createPath();

private:
	VEC_POSITIONPLOTTER		m_vecPositionPlotter;
};

#endif // __SULPOSITIONPLOTTERMANAGER_H__