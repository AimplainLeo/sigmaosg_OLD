// particleStandard.h

#ifndef __PARTICLESTANDARD_H__
#define __PARTICLESTANDARD_H__

#include "particleBase.h"
#include "propertySheet.h"

class CParticleStandard : public CParticleBase
{
public:
				CParticleStandard();

	osg::Node*	create( const osg::Vec3& pos );

protected:
	CPropertySheet* createPropertySheet();

private:
	CPropString*	m_renderBinNum;
	CPropFloat*		m_fluidDensity;
	CPropString*	m_psTextureFile;
	CPropBool*		m_psEmissive;

	CPropVec4s*		m_particleTextureTile;
	CPropFloat*		m_particleMass;
	CPropFloat*		m_particleRadius;
	CPropFloat*		m_particleLifeTime;
	CPropFloat*		m_particleSizeMin;
	CPropFloat*		m_particleSizeMax;
	CPropFloat*		m_particleAlphaMin;
	CPropFloat*		m_particleAlphaMax;
	CPropVec4*		m_particleColorMin;
	CPropVec4*		m_particleColorMax;

	CPropFloat*		m_emitterLifeTime;

	CPropFloat*		m_rateMin;
	CPropFloat*		m_rateMax;

	CPropFloat*		m_sectorRadiusMin;
	CPropFloat*		m_sectorRadiusMax;

	CPropFloat*		m_shooterThetaMin;
	CPropFloat*		m_shooterThetaMax;
	CPropFloat*		m_shooterPhiMin;
	CPropFloat*		m_shooterPhiMax;
	CPropFloat*		m_shooterInitialSpeedMin;
	CPropFloat*		m_shooterInitialSpeedMax;
};

#endif // __PARTICLESTANDARD_H__
