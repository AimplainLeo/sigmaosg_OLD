// SulIntersectGraphMouse.h

#ifndef __SULINTERSECTGRAPHMOUSE_H__
#define __SULINTERSECTGRAPHMOUSE_H__

#include "SulExport.h"
#include "SulIntersectGraph.h"

class SUL_EXPORT CSulIntersectGraphMouse : public CSulIntersectGraph
{
public:
								CSulIntersectGraphMouse( osg::Camera* pCam );

	void						setCamera( osg::Camera* pCam );

	// x, y: mouse normalized positions -1 to +1
	bool						update( float x, float y );

private:
	osg::ref_ptr<osg::Camera>	m_rCam;
};

#endif // __SULINTERSECTGRAPHMOUSE_H__