// particleFlyingDebris.h

#ifndef __PARTICLEFLYINGDEBRIS_H__
#define __PARTICLEFLYINGDEBRIS_H__

#include "particleBase.h"
#include <osg/Vec3>
#include <osg/Node>

class CParticleFlyingDebris : public CParticleBase
{
public:
				CParticleFlyingDebris();

	osg::Node*	create( const osg::Vec3& pos );

protected:
	CPropertySheet* createPropertySheet();

	CPropString*	m_particleCount;
	CPropString*	m_renderBinNum;
};

#endif // __PARTICLEFLYINGDEBRIS_H__