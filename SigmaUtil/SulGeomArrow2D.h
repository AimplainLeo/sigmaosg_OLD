// SulGeomArrow2D.h

#ifndef __SULGEOMARROW2D_H__
#define __SULGEOMARROW2D_H__

#include "SulGeomBase.h"

class CSulGeomArrow2D : public CSulGeomBase
{
public:
			CSulGeomArrow2D( float len, float width, float headSize );

	void	createDrawable();

private:
	float	m_len;
	float	m_width;
	float	m_headSize;
};

#endif // __SULGEOMARROW2D_H__