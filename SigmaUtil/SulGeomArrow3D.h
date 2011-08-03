// SulGeomArrow3D.h

#ifndef __SULGEOMARROW3D_H__
#define __SULGEOMARROW3D_H__

#include <osg/geode>

class CSulGeomArrow3D : public osg::Geode
{
public:
				CSulGeomArrow3D( float len, const osg::Vec4& colorHead=osg::Vec4(1,0,0,1), const osg::Vec4& colorBody=osg::Vec4(1,0,0,1) );

private:
	void		createDrawable();

private:
	osg::Vec4	m_colorHead;
	osg::Vec4	m_colorBody;
	float		m_headLen;
	float		m_headRadius;
	float		m_bodyLen;	
	float		m_bodyRadius;
};


#endif // __SULGEOMARROW3D_H__