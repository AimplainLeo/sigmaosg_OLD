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

	void									setGraph( osg::Node* pNode );
	void									setLine( const osg::Vec3& v0, const osg::Vec3& v1 );

	osg::LineSegment*						getLine();

	// resets only the intersection list
	void									reset();

	bool									update();

	osg::Vec3								getWorldIntersectPoint();
	osgUtil::Hit							getHit();

private:
	osg::ref_ptr<osgUtil::IntersectVisitor>	m_iv;
	osg::ref_ptr<osg::Node>					m_rGraph;
	osg::ref_ptr<osg::LineSegment>			m_rLineSeg;
};

#endif // __SULINTERSECTGRAPH_H__