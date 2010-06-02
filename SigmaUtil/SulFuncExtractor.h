// SulFuncExtractor.h

#ifndef __SULFUNCEXTRACTOR_H__
#define __SULFUNCEXTRACTOR_H__

class CSulFuncExtractor
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

		CSulDataTri tri1( v1, v3, v4);
		m_vecTri.push_back( tri1 );
	}

	void operator()(const osg::Vec3 v1, const osg::Vec3 v2, bool treatVertexDataAsTemporary)
	{
		m_vecLineSegment.push_back( new osg::LineSegment(v1,v2) );
	}

	const Sigma::VEC_LINESEGMENT& getLineSegmentList()
	{
		return m_vecLineSegment;
	}

	const Sigma::VEC_TRI& getTriList()
	{
		return m_vecTri;
	}

private:
	Sigma::VEC_LINESEGMENT	m_vecLineSegment;
	Sigma::VEC_TRI m_vecTri;
};

#endif // __SULFUNCEXTRACTOR_H__