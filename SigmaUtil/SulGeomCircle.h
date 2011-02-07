// SulGeomCircle.h

#ifndef __SULGEOMCIRCLE_H__
#define __SULGEOMCIRCLE_H__

#include "SulGeomLineStrip.h"

class CSulGeomCircle : public osg::Group
{
public:
					CSulGeomCircle( float radius );

	void			setOffset( const osg::Vec3& ofs );

private:
	void			createDrawable();

private:
	osg::ref_ptr<CSulGeomLineStrip>	m_rLine;
	float			m_radius;
	sigma::uint32	m_segments;
	float			m_width;
	osg::Vec3		m_ofs;
};

#endif // __SULGEOMCIRCLE_H__