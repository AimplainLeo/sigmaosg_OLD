// particleFlyingDebris.h

#ifndef __PARTICLEFLYINGDEBRIS_H__
#define __PARTICLEFLYINGDEBRIS_H__

#include "particleBase.h"
#include <SigmaUtil/SulQtPropString.h>
#include <SigmaUtil/SulQtPropFloat.h>
#include <osg/Vec3>
#include <osg/Node>

class CParticleFlyingDebris : public CParticleBase
{
public:
				CParticleFlyingDebris();

	osg::Node*	create( const osg::Vec3& pos );

protected:
	CSulQtPropertySheet* createPropertySheet();

	CSulQtPropString*	m_renderBinNum;

	CSulQtPropString*	m_debrisCount;
	CSulQtPropFloat*		m_debrisPosOffsetMin;
	CSulQtPropFloat*		m_debrisPosOffsetMax;
	CSulQtPropFloat*		m_debrisSpeedMin;
	CSulQtPropFloat*		m_debrisSpeedMax;
	CSulQtPropFloat*		m_debrisThetaMin;
	CSulQtPropFloat*		m_debrisThetaMax;
	CSulQtPropFloat*		m_debrisPhiMin;
	CSulQtPropFloat*		m_debrisPhiMax;

	CSulQtPropFloat*		m_particleMass;
	CSulQtPropFloat*		m_particleLifeTime;
	CSulQtPropFloat*		m_particleSizeMin;
	CSulQtPropFloat*		m_particleSizeMax;
	CSulQtPropFloat*		m_particleAlphaMin;
	CSulQtPropFloat*		m_particleAlphaMax;
	
	CSulQtPropFloat*		m_emitterLifeTime;
	CSulQtPropFloat*		m_rateMin;
	CSulQtPropFloat*		m_rateMax;
	
};

#endif // __PARTICLEFLYINGDEBRIS_H__