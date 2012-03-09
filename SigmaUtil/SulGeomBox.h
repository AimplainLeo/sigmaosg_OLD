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

private:
	void							createDrawable();

private:
	float							m_extentX;
	float							m_extentY;
	float							m_extentZ;
};

#endif // __SULGEOMBOX_H__