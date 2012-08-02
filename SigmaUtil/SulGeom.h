// SulGeom.h

#ifndef __SULGEOM_H__
#define __SULGEOM_H__

#include <osg/geometry>

class CSulGeom : public osg::Geometry
{
public:
					CSulGeom( const osg::Vec4& color=osg::Vec4(1,1,1,1) );

	// NOTE: remember to call createDrawable
	void			setColor( const osg::Vec4& color );
	void			setColor( float r, float g, float b, float a );

	virtual	void	createDrawable();

protected:
	void			removeAllPrimitiveSets();

protected:
	osg::Vec4						m_color;

	osg::ref_ptr<osg::Vec3Array>	m_verts;
	osg::ref_ptr<osg::Vec4Array>	m_colors;
};

#endif // __SULGEOM_H__