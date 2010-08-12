// SulDataShapeMask.h

#ifndef __SULDATASHAPEMASK_H__
#define __SULDATASHAPEMASK_H__

#include "SulString.h"
#include <osg/node>
#include <osg/PositionAttitudeTransform>
#include <osgSim/GeographicLocation>
#include <vector>

class CSulDataShapeMask
{
public:
	CSulDataShapeMask( const CSulString& file, float lineDist ) :
		m_file( file ),
		m_lineDist( lineDist )
	{
	}

	void process()
	{
/*
		CSulNodeVisitorExtractLines myLines;
		node->accept( myLines );
		const sigma::VEC_LINESEGMENT& lines_org = myLines.getLineSegmentList();
		sigma::VEC_LINESEGMENT::const_iterator ii;

		// we need to minize the number of lines, so we only have those in the boundingbox area
		CSulClipLines clipLines0( lines_org, m_vecPlane[0] );
		CSulClipLines clipLines1( clipLines0.getLinesList(), m_vecPlane[1] );
		CSulClipLines clipLines2( clipLines1.getLinesList(), m_vecPlane[2] );
		CSulClipLines clipLines3( clipLines2.getLinesList(), m_vecPlane[3] );
		CSulClipLines clipLines4( clipLines3.getLinesList(), m_vecPlane[4] );
		CSulClipLines clipLines5( clipLines4.getLinesList(), m_vecPlane[5] );
*/
	}

public:
	CSulString										m_file;	
	float											m_lineDist;
	osg::ref_ptr<osg::PositionAttitudeTransform>	m_node;
	osgSim::GeographicLocation*						loc;

	class CSulClipLines*							m_clipLines;	// the lines that are clipped to the bounding box
};

typedef std::vector<CSulDataShapeMask>	VEC_DATASHAPEMASK;

#endif // __SULDATASHAPEMASK_H__