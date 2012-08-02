// SulGeomCircle.h

#ifndef __SULGEOMCIRCLE_H__
#define __SULGEOMCIRCLE_H__

#include "SulGeom.h"
#include "SulGeomLineStrip.h"

class CSulGeomCircle : public CSulGeomLineStrip
{
public:
					CSulGeomCircle( float radius, const osg::Vec4& color=osg::Vec4(0,0,0,1) );

	void			setOffset( const osg::Vec3& ofs );

private:
	void			createDrawable();

private:
	float			m_radius;
	sigma::uint32	m_segments;
	float			m_width;
	osg::Vec3		m_ofs;
	osg::Vec4		m_color;
};

#endif // __SULGEOMCIRCLE_H__