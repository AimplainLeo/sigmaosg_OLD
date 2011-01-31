// SulExtractGeometry.h

#ifndef __SULEXTRACTGEOMETRY_H__
#define __SULEXTRACTGEOMETRY_H__

#include "SulTypes.h"
#include "SulFuncExtractor.h"
#include <osg/geode>
#include <osg/TemplatePrimitiveFunctor>
#include <iterator>

class CSulExtractGeometry : public osg::NodeVisitor
{
public:
 	CSulExtractGeometry() : 
		NodeVisitor( NodeVisitor::TRAVERSE_ALL_CHILDREN ) 
	{
	}

    virtual void apply( osg::Geode &geode ) 
	{
		int drawCount = geode.getNumDrawables();

		for ( int i=0; i<drawCount; i++ )
		{	
			osg::Drawable* p = geode.getDrawable(i);
			osg::TemplatePrimitiveFunctor<CSulFuncExtractor> functor;
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

	const sigma::VEC_LINESEGMENT& getLineSegmentList() const
	{
		return m_vecLineSegment;
	}

	const sigma::VEC_TRI& getTriList() const
	{
		return m_vecTri;
	}

private:
	sigma::VEC_LINESEGMENT	m_vecLineSegment;
	sigma::VEC_TRI m_vecTri;
};

#endif // __SULEXTRACTGEOMETRY_H__