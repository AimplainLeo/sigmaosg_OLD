// Laser.h

#ifndef __LASER_H__
#define __LASER_H__

#include <osg/geometry>
#include <osg/positionattitudetransform>

class CLaser
{
public:
	void			Init();

	void			SetPosition( const osg::Vec3& v );
	void			SetTarget( const osg::Vec3& v );

	osg::Node*		getNode();

private:
	osg::Geometry*	CreateQuad();
	void			calcRot();

private:
	osg::ref_ptr<osg::PositionAttitudeTransform>	m_rPat;
	osg::Vec3										m_vP;		// position
	osg::Vec3										m_vT;		// target
};

#endif // __LASER_H__