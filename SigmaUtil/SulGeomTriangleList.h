// SulGeomTriangleList.h

#ifndef __SULGEOMTRIANGLELIST_H__
#define __SULGEOMTRIANGLELIST_H__

#include "SulGeom.h"
#include "SulTypes.h"
#include <osg/Geode>
#include <osg/Geometry>

class CSulGeomTriangleList : public osg::Geode
{
public: 
					CSulGeomTriangleList( const sigma::VEC_TRI& vecTri );

	void			setColor( const osg::Vec4& color );
	void			setColor( float r, float g, float b, float a );

protected:
	void			createDrawable();

private:
	sigma::VEC_TRI					m_vecTri;
	osg::ref_ptr<osg::Vec4Array>	m_rColors;
	osg::ref_ptr<osg::Geometry>		m_rGeo;
};

#endif // __SULGEOMTRIANGLELIST_H__