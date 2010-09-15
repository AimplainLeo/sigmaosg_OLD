// SulIntersectGraph.cpp

#include "stdafx.h"
#include "SulIntersectGraph.h"

CSulIntersectGraph::CSulIntersectGraph()
{
	m_iv = new osgUtil::IntersectVisitor;
	m_rLineSeg = new osg::LineSegment;
}

void CSulIntersectGraph::SetGraph( osg::Node* pNode )
{
	m_rGraph = pNode;
}

void CSulIntersectGraph::Reset()
{
	m_iv->reset();
}

void CSulIntersectGraph::SetLine( const osg::Vec3& v0, const osg::Vec3& v1 )
{
	m_rLineSeg->set( v0, v1 );
}

osg::LineSegment* CSulIntersectGraph::GetLine()
{
	return m_rLineSeg.get();
}

bool CSulIntersectGraph::Update()
{
	if ( !m_rGraph.valid() )
	{
		return false;
	}

	Reset();
	m_iv->addLineSegment( m_rLineSeg.get() );
	m_rGraph->accept( *m_iv );

	if ( m_iv->hits() )
	{
		return true;
	}

	return false;
}

osg::Vec3 CSulIntersectGraph::GetWorldIntersectPoint()
{
	osgUtil::IntersectVisitor::HitList hitList = m_iv->getHitList( m_rLineSeg.get() );
	return hitList[0].getWorldIntersectPoint();
}


osgUtil::Hit CSulIntersectGraph::GetHit()
{
	osgUtil::IntersectVisitor::HitList hitList = m_iv->getHitList( m_rLineSeg.get() );
	return hitList[0];
}




