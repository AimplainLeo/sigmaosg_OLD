// SulGeomCrosshairSimple.h

#ifndef __SULGEOMCROSSHAIRSIMPLE_H__
#define __SULGEOMCROSSHAIRSIMPLE_H__

#include "SulGeomLines.h"

class CSulGeomCrosshairSimple : public CSulGeomLines
{
public:
	CSulGeomCrosshairSimple( float centerOffset, float len, const osg::Vec4& color=osg::Vec4(0,0,0,1) );

protected:
	void createDrawable();

private:
	float		m_centerOffset;
	float		m_lenTop;
	float		m_lenBottom;
	float		m_lenLeft;
	float		m_lenRight;
};

#endif // __SULGEOMCROSSHAIRSIMPLE_H__