// SulIntersectGraph.h

#ifndef __SULINTERSECTGRAPH_H__
#define __SULINTERSECTGRAPH_H__

#include "SulExport.h"
#include <osg/Referenced>
#include <osgUtil/IntersectVisitor>

class SUL_EXPORT CSulIntersectGraph : public osg::Referenced
{
public:
											CSulIntersectGraph();

	void									SetGraph( osg::Node* pNode );
	void									SetLine( const osg::Vec3& v0, const osg::Vec3& v1 );

	osg::LineSegment*						GetLine();

	// resets only the intersection list
	void									Reset();

	bool									Update();

	osg::Vec3								GetWorldIntersectPoint();
	osgUtil::Hit							GetHit();

private:
	osg::ref_ptr<osgUtil::IntersectVisitor>	m_iv;
	osg::ref_ptr<osg::Node>					m_rGraph;
	osg::ref_ptr<osg::LineSegment>			m_rLineSeg;
};

#endif // __SULINTERSECTGRAPH_H__