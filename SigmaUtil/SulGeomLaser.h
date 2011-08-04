// SulGeomLaser.h

#ifndef __SULGEOMLASER_H__
#define __SULGEOMLASER_H__

#include "SulUpdateCallbackToggleNode.h"
#include "SulExport.h"
#include <osg/geometry>
#include <osg/positionattitudetransform>

class SUL_EXPORT CSulGeomLaser : public osg::Group
{
public:
	void			init();

	void			setPulseFrequency( double pulse );

	void			setPosition( const osg::Vec3& v );
	void			setTarget( const osg::Vec3& v );

	void			setRadius( float radius );
	void			setRadiusStart( float radius );
	void			setRadiusEnd( float radius );

	osg::Vec3		getTarget();

	osg::Group*		getLaserNode();

	void			enabled( bool bEnable );

private:
	osg::Geometry*	createQuad( float radiusStart, float radiusEnd );
	void			calcRot();
	osg::Billboard* createBillboard( float radiusStart, float radiusEnd );

private:
	osg::ref_ptr<osg::Group>						m_rLaserGroup;
	osg::ref_ptr<osg::PositionAttitudeTransform>	m_rPat;
	osg::Vec3										m_vP;		// position
	osg::Vec3										m_vT;		// target
	osg::ref_ptr<CSulUpdateCallbackToggleNode>		m_rPulse;

	float											m_radiusStart;
	float											m_radiusEnd;
};

#endif // __SULGEOMLASER_H__