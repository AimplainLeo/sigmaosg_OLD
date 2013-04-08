// SulGeomCylinder.h

#ifndef __SULGEOMCYLINDER_H__
#define __SULGEOMCYLINDER_H__

#include "SulGeom.h"
#include "SulExport.h"

class SUL_EXPORT CSulGeomCylinder : public CSulGeom
{
public:
			CSulGeomCylinder( float radius=1.0f, float length=4.0f, sigma::uint32 segments=32 );

private:
	void	createDrawable();

private:
	float			m_radius;
	float			m_length;
	osg::Vec4		m_color;
	sigma::uint32	m_segments;
};

#endif // __SULGEOMCYLINDER_H__