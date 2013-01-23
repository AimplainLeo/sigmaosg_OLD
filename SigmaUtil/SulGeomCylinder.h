// SulGeomCylinder.h

#ifndef __SULGEOMCYLINDER_H__
#define __SULGEOMCYLINDER_H__

#include "SulGeom.h"

class CSulGeomCylinder : public CSulGeom
{
public:
	CSulGeomCylinder( float radius=1.0f, float length=4.0f );

private:
	float m_radius;
	float m_length;
};

#endif // __SULGEOMCYLINDER_H__