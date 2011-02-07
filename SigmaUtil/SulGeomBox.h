// SulGeomBox.h

#ifndef __SULGEOMBOX_H__
#define __SULGEOMBOX_H__

#include "SulGeomBase.h"
#include <osg/geode>

class CSulGeomBox : public osg::Geode
{
public:
									CSulGeomBox( float extent );

private:
	void							createDrawable();

private:
	float							m_extent;
};

#endif // __SULGEOMBOX_H__