// SulIntersectGraph.cpp

#include "stdafx.h"
#include "SulIntersectGraph.h"

CSulIntersectGraph::CSulIntersectGraph()
{
	m_rLineSeg = new osgUtil::LineSegmentIntersector( osg::Vec3(0,0,0), osg::Vec3(1,1,1) );
	m_iv = new osgUtil::IntersectionVisitor;
	m_iv->setIntersector( m_rLineSeg );
}

void CSulIntersectGraph::setGraph( osg::Node* pNode )
{
	m_rGraph = pNode;
}

void CSulIntersectGraph::reset()
{
	m_iv->reset();
}

void CSulIntersectGraph::setLine( const osg::Vec3& v0, const osg::Vec3& v1 )
{
	m_rLineSeg->setStart( v0 );
	m_rLineSeg->setEnd( v1 );
}

/*
osg::LineSegment* CSulIntersectGraph::getLine()
{
	return m_rLineSeg.get();
}
*/

bool CSulIntersectGraph::update()
{
	if ( !m_rGraph.valid() )
	{
		return false;
	}

	reset();
//	m_iv->addLineSegment( m_rLineSeg.get() );
	m_rGraph->accept( *m_iv );

	if ( m_iv->getIntersector()->containsIntersections() )
	{
		return true;
	}

	return false;
}

osg::Vec3 CSulIntersectGraph::getWorldIntersectPoint()
{
	return m_rLineSeg->getFirstIntersection().getWorldIntersectPoint();

		/*
	osgUtil::IntersectVisitor::HitList hitList = m_iv->getHitList( m_rLineSeg.get() );
	return hitList[0].getWorldIntersectPoint();
	*/
}

/*
osgUtil::Hit CSulIntersectGraph::getHit()
{
	osgUtil::IntersectVisitor::HitList hitList = m_iv->getHitList( m_rLineSeg.get() );
	return hitList[0];
}
*/




