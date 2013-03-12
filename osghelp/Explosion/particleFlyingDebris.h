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

	CPropString*	m_renderBinNum;

	CPropString*	m_debrisCount;
	CPropFloat*		m_debrisPosOffsetMin;
	CPropFloat*		m_debrisPosOffsetMax;
	CPropFloat*		m_debrisSpeedMin;
	CPropFloat*		m_debrisSpeedMax;
	CPropFloat*		m_debrisThetaMin;
	CPropFloat*		m_debrisThetaMax;
	CPropFloat*		m_debrisPhiMin;
	CPropFloat*		m_debrisPhiMax;

	CPropFloat*		m_particleMass;
	CPropFloat*		m_particleLifeTime;
	CPropFloat*		m_particleSizeMin;
	CPropFloat*		m_particleSizeMax;
	CPropFloat*		m_particleAlphaMin;
	CPropFloat*		m_particleAlphaMax;
	
	CPropFloat*		m_emitterLifeTime;
	CPropFloat*		m_rateMin;
	CPropFloat*		m_rateMax;
	
};

#endif // __PARTICLEFLYINGDEBRIS_H__