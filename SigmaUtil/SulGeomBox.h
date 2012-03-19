// SulGeomBox.h

#ifndef __SULGEOMBOX_H__
#define __SULGEOMBOX_H__

#include "SulGeomBase.h"
#include "SulGeomGeode.h"

class CSulGeomBox : public CSulGeomGeode
{
public:
									CSulGeomBox( float extent );
									CSulGeomBox( float extentX, float extentY, float extentZ );
									CSulGeomBox( float minX, float maxX, float minY, float maxY, float minZ, float maxZ );
									CSulGeomBox( const osg::BoundingBox& bb );

private:
	void							createDrawable();

private:
	float							m_minX;
	float							m_maxX;
	float							m_minY;
	float							m_maxY;
	float							m_minZ;
	float							m_maxZ;
};

#endif // __SULGEOMBOX_H__