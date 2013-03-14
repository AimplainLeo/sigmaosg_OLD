// particleFlyingDebris.cpp

#include "stdafx.h"
#include "particleFlyingDebris.h"
#include "ParticleDebrisSystem.h"
#include <osg/group>

CParticleFlyingDebris::CParticleFlyingDebris() :
CParticleBase()
{
}

osg::Node* CParticleFlyingDebris::create( const osg::Vec3& pos )
{
	CParticleBase::create( pos );

	osg::Group* group = new osg::Group;
	/*
	group->getOrCreateStateSet()->setRenderBinDetails( m_renderBinNum->getUint32(), "DepthSortedBin" );

	CParticleDebrisSystem* p = new CParticleDebrisSystem( 
		m_debrisCount->getUint32(),
		m_debrisPosOffsetMin->getValue(),
		m_debrisPosOffsetMax->getValue(),
		m_debrisSpeedMin->getValue(),
		m_debrisSpeedMax->getValue(),
		m_debrisThetaMin->getValue(),
		m_debrisThetaMax->getValue(),
		m_debrisPhiMin->getValue(),
		m_debrisPhiMax->getValue(),
		m_particleMass->getValue(),
		m_particleLifeTime->getValue(),
		m_particleSizeMin->getValue(),
		m_particleSizeMax->getValue(),
		m_particleAlphaMin->getValue(),
		m_particleAlphaMax->getValue(),
		m_emitterLifeTime->getValue(),
		m_rateMin->getValue(),
		m_rateMax->getValue()
	);
	p->init();

	osg::Matrix m;
	m.makeTranslate( pos );
	p->setMatrix( m );

	group->addChild( p );
	*/
	return group;
}

CSulQtPropertySheet* CParticleFlyingDebris::createPropertySheet()
{
	CSulQtPropertySheet* propertySheet = new CSulQtPropertySheet;
	/*
	// general
	propertySheet->add( m_renderBinNum = new CSulQtPropString( "RenderBin Num", 3000) );

	// debris
	propertySheet->add( m_debrisCount = new CSulQtPropString( "Debris Count", 10) );
	propertySheet->add( m_debrisPosOffsetMin = new CSulQtPropFloat( "Debris Pos Offset Min", 0.0f ) );
	propertySheet->add( m_debrisPosOffsetMax = new CSulQtPropFloat( "Debris Pos Offset Max", 0.0f ) );
	propertySheet->add( m_debrisSpeedMin = new CSulQtPropFloat( "Debris Speed Min", 10.0f ) );
	propertySheet->add( m_debrisSpeedMax = new CSulQtPropFloat( "Debris Speed Max", 10.0f ) );
	propertySheet->add( m_debrisThetaMin = new CSulQtPropFloat( "Debris Theta Min", 0.0f ) );
	propertySheet->add( m_debrisThetaMax = new CSulQtPropFloat( "Debris Theta Max", 2*osg::PI ) );
	propertySheet->add( m_debrisPhiMin = new CSulQtPropFloat( "Debris Phi Min", 0.0f ) );
	propertySheet->add( m_debrisPhiMax = new CSulQtPropFloat( "Debris Phi Max", osg::PI_2) );

	// particles
	propertySheet->add( m_particleMass = new CSulQtPropFloat( "Particle Mass", 0.7f ) );
	propertySheet->add( m_particleLifeTime = new CSulQtPropFloat( "Particle Life Time (sec)", 3.0f ) );
	propertySheet->add( m_particleSizeMin = new CSulQtPropFloat( "Particle Size Min", 0.75f) );
	propertySheet->add( m_particleSizeMax = new CSulQtPropFloat( "Particle Size Max", 3.0f) );
	propertySheet->add( m_particleAlphaMin = new CSulQtPropFloat( "Particle Alpha Min", 0.5f) );
	propertySheet->add( m_particleAlphaMax = new CSulQtPropFloat( "Particle Alpha Max", 1.0f) );

	// emitter
	propertySheet->add( m_emitterLifeTime = new CSulQtPropFloat( "Emitter Life Time", 2.5f) );

	// rate
	propertySheet->add( m_rateMin = new CSulQtPropFloat( "Rate Min", 10.0f) );
	propertySheet->add( m_rateMax = new CSulQtPropFloat( "Rate Max", 10.0f) );
	*/
	return propertySheet;
}
