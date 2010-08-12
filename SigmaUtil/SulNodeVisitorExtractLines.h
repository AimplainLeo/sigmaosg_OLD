// SulNodeVisitorExtractLines.h

// DEPRECATED: use CSulFuncExtractor

#ifndef __SULNODEVISITOREXTRACTLINES_H__
#define __SULNODEVISITOREXTRACTLINES_H__

#include "SulDataTri.h"
#include "SulTypes.h"
#include <osg/geode>
#include <osg/geometry>
#include <osg/TemplatePrimitiveFunctor>
#include <osg/linesegment>

class Extractor
{
public:
	void operator()(const osg::Vec3 v1, bool treatVertexDataAsTemporary){}
	void operator()(const osg::Vec3 v1, const osg::Vec3 v2, const osg::Vec3 v3, bool treatVertexDataAsTemporary)
	{
		CSulDataTri tri( v1, v2, v3);
		m_vecTri.push_back( tri );
	}

	void operator()(const osg::Vec3 v1, const osg::Vec3 v2, const osg::Vec3 v3, const osg::Vec3 v4, bool treatVertexDataAsTemporary)
	{
		CSulDataTri tri0( v1, v2, v3);
		m_vecTri.push_back( tri0 );

		CSulDataTri tri1( v2, v3, v4);
		m_vecTri.push_back( tri1 );
	}

	void operator()(const osg::Vec3 v1, const osg::Vec3 v2, bool treatVertexDataAsTemporary)
	{
		m_vecLineSegment.push_back( new osg::LineSegment(v1,v2) );
	}

	const sigma::VEC_LINESEGMENT& getLineSegmentList()
	{
		return m_vecLineSegment;
	}

	const sigma::VEC_TRI& getTriList()
	{
		return m_vecTri;
	}

private:
	sigma::VEC_LINESEGMENT	m_vecLineSegment;
	sigma::VEC_TRI m_vecTri;
};


class CSulNodeVisitorExtractLines : public osg::NodeVisitor
{
public:
 	CSulNodeVisitorExtractLines() : 
		NodeVisitor( NodeVisitor::TRAVERSE_ALL_CHILDREN ) 
	{
	}

    virtual void apply( osg::Geode &geode ) 
	{
		int drawCount = geode.getNumDrawables();

		for ( int i=0; i<drawCount; i++ )
		{	
			osg::Drawable* p = geode.getDrawable(i);
			osg::TemplatePrimitiveFunctor<Extractor> functor;
			p->accept(functor);

			const sigma::VEC_LINESEGMENT& src = functor.getLineSegmentList();

			sigma::VEC_LINESEGMENT::const_iterator iS;
			sigma::VEC_LINESEGMENT::const_iterator iE;
			iS = src.begin();
			iE = src.end();
			std::copy( iS, iE, std::back_inserter(m_vecLineSegment) );

			const sigma::VEC_TRI& srcTri = functor.getTriList();
			sigma::VEC_TRI::const_iterator iTriS;
			sigma::VEC_TRI::const_iterator iTriE;
			iTriS = srcTri.begin();
			iTriE = srcTri.end();
			std::copy( iTriS, iTriE, std::back_inserter(m_vecTri) );
		}

		// continue traversing through the graph
        traverse( geode );
	}

	const sigma::VEC_LINESEGMENT& getLineSegmentList()
	{
		return m_vecLineSegment;
	}

	const sigma::VEC_TRI& getTriList()
	{
		return m_vecTri;
	}

private:
	sigma::VEC_LINESEGMENT	m_vecLineSegment;
	sigma::VEC_TRI m_vecTri;
};

#endif // __SULNODEVISITOREXTRACTLINES_H__