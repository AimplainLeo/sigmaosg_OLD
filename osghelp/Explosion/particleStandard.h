// particleStandard.h

#ifndef __PARTICLESTANDARD_H__
#define __PARTICLESTANDARD_H__

#include "particleBase.h"
#include <SigmaUtil/SulQtPropString.h>
#include <SigmaUtil/SulQtPropFloat.h>
#include <SigmaUtil/SulQtPropVec3.h>
#include <SigmaUtil/SulQtPropBool.h>
#include <SigmaUtil/SulQtPropVec4.h>
#include <SigmaUtil/SulQtPropVec4s.h>

class CParticleStandard : public CParticleBase
{
public:
				CParticleStandard();

	osg::Node*	create( const osg::Vec3& pos );

protected:
	CSulQtPropertySheet* createPropertySheet();

private:
	CSulQtPropString*	m_renderBinNum;
	CSulQtPropFloat*		m_fluidDensity;
	CSulQtPropVec3*		m_wind;
	CSulQtPropString*	m_psTextureFile;
	CSulQtPropBool*		m_psEmissive;

	CSulQtPropVec4s*		m_particleTextureTile;
	CSulQtPropFloat*		m_particleMass;
	CSulQtPropFloat*		m_particleRadius;
	CSulQtPropFloat*		m_particleLifeTime;
	CSulQtPropFloat*		m_particleSizeMin;
	CSulQtPropFloat*		m_particleSizeMax;
	CSulQtPropFloat*		m_particleAlphaMin;
	CSulQtPropFloat*		m_particleAlphaMax;
	CSulQtPropVec4*		m_particleColorMin;
	CSulQtPropVec4*		m_particleColorMax;

	CSulQtPropFloat*		m_emitterLifeTime;
	CSulQtPropFloat*		m_emitterCompensationRatio;

	CSulQtPropFloat*		m_rateMin;
	CSulQtPropFloat*		m_rateMax;

	CSulQtPropFloat*		m_sectorRadiusMin;
	CSulQtPropFloat*		m_sectorRadiusMax;

	CSulQtPropFloat*		m_shooterThetaMin;
	CSulQtPropFloat*		m_shooterThetaMax;
	CSulQtPropFloat*		m_shooterPhiMin;
	CSulQtPropFloat*		m_shooterPhiMax;
	CSulQtPropFloat*		m_shooterInitialSpeedMin;
	CSulQtPropFloat*		m_shooterInitialSpeedMax;
};

#endif // __PARTICLESTANDARD_H__
