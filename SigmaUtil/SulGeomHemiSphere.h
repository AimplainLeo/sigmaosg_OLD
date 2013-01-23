// SulGeomHemiSphere.h

#ifndef __SULGEOMHEMISPHERE_H__
#define __SULGEOMHEMISPHERE_H__

#include "SulGeom.h"
#include "SulTypes.h"

class CSulGeomHemiSphere : public CSulGeom
{
public:
			CSulGeomHemiSphere( float radius=1.0f, sigma::uint32 sides=32, sigma::uint32 segments=8  );
			CSulGeomHemiSphere( const osg::Vec3& offset, float radius=1.0f, sigma::uint32 sides=32, sigma::uint32 segments=8 );

private:
	void	createDrawable();

private:
	osg::Vec3		m_offset;
	float			m_radius;
	sigma::uint32	m_sides;
	sigma::uint32	m_segments;
};

#endif // __SULGEOMHEMISPHERE_H__