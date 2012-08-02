// SulGeomSphere.h

#ifndef __SULGEOMSPHERE_H__
#define __SULGEOMSPHERE_H__

#include "SulGeode.h"
#include "SulExport.h"
#include <osg/shapedrawable>

class SUL_EXPORT CSulGeomSphere : public CSulGeode
{

public:
				CSulGeomSphere( float radius=1.0f, osg::Vec3 pos=osg::Vec3(0,0,0)  );

	void		setColor( const osg::Vec4& color );

private:
	void		createDrawable();

private:
	float		m_radius;
	osg::Vec3	m_pos;
	osg::Vec4	m_color;

	osg::ref_ptr<osg::ShapeDrawable> m_rShape;
};

#endif // __SULGEOMSPHERE_H__