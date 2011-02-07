// SulGeomLine.h

#ifndef __SULGEOMLINE_H__
#define __SULGEOMLINE_H__

#include "SulGeomBase.h"
#include <osg/linesegment>
#include <osg/linewidth>
#include <osg/geode>

class CSulGeomLine : public osg::Geode
{
public:
				CSulGeomLine( const osg::LineSegment& line );

				CSulGeomLine( const osg::Vec3& v0, const osg::Vec3& v1 );

	void		setWidth( float width );

private:
	void		createDrawable();

private:
	osg::Vec3	m_v0;
	osg::Vec3	m_v1;
	osg::ref_ptr<osg::LineWidth>	m_rLineWidth;
};

#endif // __SULGEOMLINE_H__