// SulGeomLaser.h

#ifndef __SULGEOMLASER_H__
#define __SULGEOMLASER_H__

#include "SulExport.h"
#include <osg/geometry>
#include <osg/positionattitudetransform>

class SUL_EXPORT CSulGeomLaser : public osg::Referenced
{
public:
	void			init();

	void			setPosition( const osg::Vec3& v );
	void			setTarget( const osg::Vec3& v );
	void			setRadius(float fRadius);

	osg::Vec3		getTarget();

	osg::Node*		getNode();

	void			enabled( bool bEnable );

private:
	osg::Geometry*	createQuad(float fThickness);
	void			calcRot();
	osg::Billboard* createBillboard(float radius);

private:
	osg::ref_ptr<osg::PositionAttitudeTransform>	m_rPat;
	osg::Vec3										m_vP;		// position
	osg::Vec3										m_vT;		// target
};

#endif // __SULGEOMLASER_H__