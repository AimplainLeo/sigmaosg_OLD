// SulGeomLaser.h

#ifndef __SULGEOMLASER_H__
#define __SULGEOMLASER_H__

#include "SulExport.h"
#include <osg/geometry>
#include <osg/positionattitudetransform>

class SUL_EXPORT CSulGeomLaser : public osg::Referenced
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

#endif // __SULGEOMLASER_H__