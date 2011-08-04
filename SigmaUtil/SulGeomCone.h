// SulGeomCone.h

#ifndef __SULGEOMCONE_H__
#define __SULGEOMCONE_H__

#include "SulTypes.h"
#include <osg/geode>
#include <osg/geometry>

class CSulGeomCone : public osg::Geode
{
public:
	// slices : The number of subdivisions around the Z axis
	// stacks : The number of subdivisions along the Z axis
	CSulGeomCone( float len, float radiusBottom, float radiusTop, sigma::uint32 slices=16, sigma::uint32 stacks=1 );

private:
	void createDrawable();

private:
	osg::ref_ptr<osg::Geometry>	m_rGeo;
	float			m_len;
	float			m_radiusBottom;
	float			m_radiusTop;
	sigma::uint32	m_slices;
};

#endif // __SULGEOMCONE_H__