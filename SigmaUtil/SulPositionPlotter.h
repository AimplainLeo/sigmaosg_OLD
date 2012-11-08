// SulPositionPlotter.h

#ifndef __SULPOSITIONPLOTTER_H__
#define __SULPOSITIONPLOTTER_H__

#include "SulTypes.h"
#include "SulExport.h"

class SUL_EXPORT CSulPositionPlotter : public osg::NodeCallback
{
public:
	CSulPositionPlotter( double timePulse, float dist, osg::Vec3 offset=osg::Vec3(0,0,0) );

	virtual void operator()( osg::Node* node, osg::NodeVisitor* nv );

	sigma::VEC_VEC3& getPositions();

private:
	bool		m_bFirstTime;
	double		m_lastTime;
	double		m_timePulse;
	float		m_dist;
	sigma::VEC_VEC3	m_vecPositions;
	osg::Vec3 m_offset;
};

#endif // __SULPOSITIONPLOTTER_H__