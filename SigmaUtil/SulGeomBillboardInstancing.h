// SulGeomBillboardInstancing.h

#ifndef __SULGEOMBILLBOARDINSTANCING_H__
#define __SULGEOMBILLBOARDINSTANCING_H__

#include "SulGeomBase.h"
#include "SulTypes.h"

class CSulGeomBillboardInstancing :  public CSulGeomBase
{
public:
					CSulGeomBillboardInstancing( sigma::uint32 numInstances, float sizeMultiplier=1.0f, float min=1.0f, float max=1.0f );

protected:
	void			createDrawable();

private:
	sigma::uint32	m_numInstances;
	float			m_sizeMultiplier;
};

#endif // __SULGEOMBILLBOARDINSTANCING_H__