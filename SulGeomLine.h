// SulGeomLine.h

#ifndef __SULGEOMLINE_H__
#define __SULGEOMLINE_H__

#include "SulGeomBase.h"
#include <osg/linesegment>
#include <osg/linewidth>

class CSulGeomLine : public CSulGeomBase
{
public:
				CSulGeomLine( const osg::LineSegment& line );

				CSulGeomLine( const osg::Vec3& v0, const osg::Vec3& v1 );

	void		setWidth( float width );

protected:
	void		createDrawable();

private:
	osg::Vec3	m_v0;
	osg::Vec3	m_v1;
	osg::ref_ptr<osg::LineWidth>	m_rLineWidth;
};

#endif // __SULGEOMLINE_H__