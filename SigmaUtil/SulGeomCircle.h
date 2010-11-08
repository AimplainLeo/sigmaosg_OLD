// SulGeomCircle.h

#ifndef __SULGEOMCIRCLE_H__
#define __SULGEOMCIRCLE_H__

#include "SulGeomLineStrip.h"

class CSulGeomCircle : public CSulGeomLineStrip
{
public:
					CSulGeomCircle( float radius );

	void			setOffset( const osg::Vec3& ofs );

	void			createDrawable();

private:
	float			m_radius;
	sigma::uint32	m_segments;
	float			m_width;
	osg::Vec3		m_ofs;
};

#endif // __SULGEOMCIRCLE_H__