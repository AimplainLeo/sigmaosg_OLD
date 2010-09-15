// SulClipLines.cpp

#include "stdafx.h"
#include "SulClipLines.h"
#include "SulIntPlane.h"

CSulClipLines::CSulClipLines( const sigma::VEC_LINESEGMENT& lines, const osg::Plane& plane ) :
m_vecLinesIn( lines ),
m_plane( plane )
{
	process();
}

const sigma::VEC_LINESEGMENT& CSulClipLines::getLinesList()
{
	return m_vecLines;
}

sigma::uint32 CSulClipLines::getLineCount()
{
	return m_vecLines.size();
}

int CSulClipLines::testVert( const osg::Vec3& v )
{
	std::vector<osg::Vec3> vec;
	vec.push_back( v );
	return m_plane.intersect( vec );
}

void CSulClipLines::process()
{
	sigma::VEC_LINESEGMENT::const_iterator i;

	i = m_vecLinesIn.begin();
	while ( i!=m_vecLinesIn.end() )
	{
		// how many points are on the wrong side of the plane
		sigma::uint32 count = 0;

		if ( testVert( (*i)->start() )<0 ) count++;
		if ( testVert( (*i)->end() )<0 ) count++;

		// disregard line because all points are on the wrong side of the plane
		if ( count==2 )
		{
			++i;
			continue;
		}

		// line is good as it is 
		if ( count==0 )
		{
			m_vecLines.push_back( (*i) );
			++i;
			continue;
		}

		// we have a point on each side of the plane.. we need to clip
		osg::Vec3 p;
		SulIntPlane( *(*i), m_plane, &p );

		if ( testVert( (*i)->start() )>0 ) 
		{
			m_vecLines.push_back( new osg::LineSegment((*i)->start(), p) );
		}
		else
		{
			m_vecLines.push_back( new osg::LineSegment((*i)->end(), p) );
		}

		++i;
	}
}