// SulGeomBox.h

#ifndef __SULGEOMBOX_H__
#define __SULGEOMBOX_H__

#include "SulGeomBase.h"

class CSulGeomBox : public CSulGeomBase
{
public:
									CSulGeomBox( float extent );

protected:
	void							createDrawable();

private:
	float							m_extent;
};

#endif // __SULGEOMBOX_H__