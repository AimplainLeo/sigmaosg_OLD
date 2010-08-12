// SulClipTriangle.cpp

#include "stdafx.h"
#include "SulClipTriangle.h"
#include "SulIntPlane.h"

CSulClipTriangle::CSulClipTriangle()
{
}

CSulClipTriangle::CSulClipTriangle( const CSulDataTri& tri, const osg::Plane& plane ) :
m_plane( plane )
{
	m_vecTriIn.push_back( tri );
	process();
}

CSulClipTriangle::CSulClipTriangle( const sigma::VEC_TRI& vecTri, const osg::Plane& plane ) :
m_plane( plane )
{
	m_vecTriIn = vecTri;
	process();
}

sigma::uint32 CSulClipTriangle::getCount()
{
	return m_vecTriOut.size();
}

const CSulDataTri* CSulClipTriangle::getTriangle( sigma::uint32 index )
{
	return &m_vecTriOut[index];
}

int CSulClipTriangle::testVert( const osg::Vec3& v )
{
	std::vector<osg::Vec3> vec;
	vec.push_back( v );
	return m_plane.intersect( vec );
}

void CSulClipTriangle::processClipTriPlane( const CSulDataTri& tri, const osg::Plane& plane )
{
	// how many points are on the wrong side of the plane
	sigma::uint32 count = 0;

	if ( testVert( tri.m_p0 )<0 ) count++;
	if ( testVert( tri.m_p1 )<0 ) count++;
	if ( testVert( tri.m_p2 )<0 ) count++;

	// disregard triangle because all points are on the wrong side of the plane
	if ( count==3 )
	{
		return;
	}

	// triangle is good as it is 
	if ( count==0 )
	{
		m_vecTriOut.push_back( tri );
		return;
	}

	// 2 points are on the wrong side of the plane (this clips to 1 new triangle)
	if ( count==2 )
	{
		CSulDataTri triNew;

		if ( testVert( tri.m_p0 )>0.0f ) 
		{
			triNew.m_p0 = tri.m_p0;
			SulIntPlane( tri.m_p0, tri.m_p1, plane, &triNew.m_p1 );
			SulIntPlane( tri.m_p0, tri.m_p2, plane, &triNew.m_p2 );
			m_vecTriOut.push_back( triNew );
			return;
		}
		
		if ( testVert( tri.m_p1 )>0.0f )
		{
			triNew.m_p0 = tri.m_p1;
			SulIntPlane( tri.m_p1, tri.m_p0, plane, &triNew.m_p1 );
			SulIntPlane( tri.m_p1, tri.m_p2, plane, &triNew.m_p2 );
			m_vecTriOut.push_back( triNew );
			return;
		}

		triNew.m_p0 = tri.m_p2;
		SulIntPlane( tri.m_p2, tri.m_p0, plane, &triNew.m_p1 );
		SulIntPlane( tri.m_p2, tri.m_p1, plane, &triNew.m_p2 );
		m_vecTriOut.push_back( triNew );
		return;
	}

	// if we get here it's because only 1 point is on the wrong side and we need to create two new triangles
	
	if ( testVert( tri.m_p0 )<0.0f ) 
	{
		CSulDataTri tri0;
		SulIntPlane( tri.m_p0, tri.m_p1, plane, &tri0.m_p0 ); 
		SulIntPlane( tri.m_p0, tri.m_p2, plane, &tri0.m_p1 ); 
		tri0.m_p2 = tri.m_p1;
		m_vecTriOut.push_back( tri0 );

		CSulDataTri tri1;
		tri1.m_p0 = tri.m_p2;
		tri1.m_p1 = tri0.m_p1;
		tri1.m_p2 = tri.m_p1;
		m_vecTriOut.push_back( tri1 );
	}

	if ( testVert( tri.m_p1 )<0.0f ) 
	{
		CSulDataTri tri0;
		SulIntPlane( tri.m_p1, tri.m_p0, plane, &tri0.m_p0 ); 
		SulIntPlane( tri.m_p1, tri.m_p2, plane, &tri0.m_p1 ); 
		tri0.m_p2 = tri.m_p2;
		m_vecTriOut.push_back( tri0 );

		CSulDataTri tri1;
		tri1.m_p0 = tri.m_p0;
		tri1.m_p1 = tri0.m_p0;
		tri1.m_p2 = tri.m_p2;
		m_vecTriOut.push_back( tri1 );
	}

	if ( testVert( tri.m_p2 )<0.0f ) 
	{
		CSulDataTri tri0;
		SulIntPlane( tri.m_p2, tri.m_p0, plane, &tri0.m_p0 ); 
		SulIntPlane( tri.m_p2, tri.m_p1, plane, &tri0.m_p1 ); 
		tri0.m_p2 = tri.m_p0;
		m_vecTriOut.push_back( tri0 );

		CSulDataTri tri1;
		tri1.m_p0 = tri.m_p1;
		tri1.m_p1 = tri0.m_p1;
		tri1.m_p2 = tri.m_p0;
		m_vecTriOut.push_back( tri1 );
	}


}

void CSulClipTriangle::processSingleTri( const CSulDataTri& tri )
{
	processClipTriPlane( tri, m_plane );
}

void CSulClipTriangle::process()
{
	sigma::VEC_TRI::iterator i;

	i = m_vecTriIn.begin();
	while ( i!=m_vecTriIn.end() )	
	{
		processSingleTri( *i );
		++i;
	}
}

const sigma::VEC_TRI& CSulClipTriangle::getTriangleList() const
{
	return m_vecTriOut;
}