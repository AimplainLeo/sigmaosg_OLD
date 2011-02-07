// SulGeomLineList.h

#ifndef __SULGEOMLINELIST_H__
#define __SULGEOMLINELIST_H__

#include "SulTypes.h"
#include <osg/geode>

class CSulGeomLineList : public osg::Geode
{
public: 
					CSulGeomLineList( const sigma::VEC_LINESEGMENT& vecLines );

	void			setColor( const osg::Vec4& color );
	void			setColor( float r, float g, float b, float a );

private:
	void			createDrawable();

private:
	sigma::VEC_LINESEGMENT			m_vecLines;
	osg::ref_ptr<osg::Vec4Array>	m_rColors;
	osg::ref_ptr<osg::Geometry>		m_rGeo;
};

#endif // __SULGEOMLINELIST_H__