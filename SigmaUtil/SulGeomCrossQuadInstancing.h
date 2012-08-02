// SulGeomCrossQuadInstancing.h

#ifndef __SULGEOMCROSSQUADINSTANCING_H__
#define __SULGEOMCROSSQUADINSTANCING_H__

#include "SulGeom.h"
#include "SulTypes.h"
#include <osg/geode>

class CSulGeomCrossQuadInstancing :  public osg::Geode
{
public:
					CSulGeomCrossQuadInstancing( sigma::uint32 numInstances, bool bUseZDirectionNormal=false, float sizeMultiplier=1.0f );

private:
	void			createDrawable();

private:
	sigma::uint32	m_numInstances;
	float			m_sizeMultiplier;
	bool			m_bUseZDirectionNormal;
};

#endif // __SULGEOMCROSSQUADINSTANCING_H__