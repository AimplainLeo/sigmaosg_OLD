// SulGeomBarChart.h

#ifndef __SULGEOMBARCHART_H__
#define __SULGEOMBARCHART_H__

#include "SulGeomBase.h"
#include "SuLGeomQuad.h"

class CSulGeomBarChart : public CSulGeomBase
{
private:
	typedef std::vector< osg::ref_ptr<CSulGeomQuad> > VEC_GEOM_QUAD;

public:
					CSulGeomBarChart();

					sigma::uint32	createBar( const osg::Vec4& color=osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f) );

	void			setBarSize( sigma::uint32 index, float size );

protected:
	void			createDrawable();

private:
	VEC_GEOM_QUAD	m_vecBar;
	float			m_h;
};

#endif // __SULGEOMBARCHART_H__