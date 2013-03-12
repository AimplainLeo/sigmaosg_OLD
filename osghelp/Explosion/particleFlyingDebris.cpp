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
	group->getOrCreateStateSet()->setRenderBinDetails( m_renderBinNum->getUint32(), "DepthSortedBin" );

	CParticleDebrisSystem* p = new CParticleDebrisSystem( 
		m_debrisCount->getUint32(),
		m_debrisPosOffsetMin->getFloat(),
		m_debrisPosOffsetMax->getFloat(),
		m_debrisSpeedMin->getFloat(),
		m_debrisSpeedMax->getFloat(),
		m_debrisThetaMin->getFloat(),
		m_debrisThetaMax->getFloat(),
		m_debrisPhiMin->getFloat(),
		m_debrisPhiMax->getFloat(),
		m_particleMass->getFloat(),
		m_particleLifeTime->getFloat(),
		m_particleSizeMin->getFloat(),
		m_particleSizeMax->getFloat(),
		m_particleAlphaMin->getFloat(),
		m_particleAlphaMax->getFloat(),
		m_emitterLifeTime->getFloat(),
		m_rateMin->getFloat(),
		m_rateMax->getFloat()
	);
	p->init();

	osg::Matrix m;
	m.makeTranslate( pos );
	p->setMatrix( m );

	group->addChild( p );

	return group;
}

CPropertySheet* CParticleFlyingDebris::createPropertySheet()
{
	CPropertySheet* propertySheet = new CPropertySheet;

	// general
	propertySheet->add( m_renderBinNum = new CPropString( "RenderBin Num", 3000) );

	// debris
	propertySheet->add( m_debrisCount = new CPropString( "Debris Count", 10) );
	propertySheet->add( m_debrisPosOffsetMin = new CPropFloat( "Debris Pos Offset Min", 0.0f ) );
	propertySheet->add( m_debrisPosOffsetMax = new CPropFloat( "Debris Pos Offset Max", 0.0f ) );
	propertySheet->add( m_debrisSpeedMin = new CPropFloat( "Debris Speed Min", 10.0f ) );
	propertySheet->add( m_debrisSpeedMax = new CPropFloat( "Debris Speed Max", 10.0f ) );
	propertySheet->add( m_debrisThetaMin = new CPropFloat( "Debris Theta Min", 0.0f ) );
	propertySheet->add( m_debrisThetaMax = new CPropFloat( "Debris Theta Max", 2*osg::PI ) );
	propertySheet->add( m_debrisPhiMin = new CPropFloat( "Debris Phi Min", 0.0f ) );
	propertySheet->add( m_debrisPhiMax = new CPropFloat( "Debris Phi Max", osg::PI_2) );

	// particles
	propertySheet->add( m_particleMass = new CPropFloat( "Particle Mass", 0.7f ) );
	propertySheet->add( m_particleLifeTime = new CPropFloat( "Particle Life Time (sec)", 3.0f ) );
	propertySheet->add( m_particleSizeMin = new CPropFloat( "Particle Size Min", 0.75f) );
	propertySheet->add( m_particleSizeMax = new CPropFloat( "Particle Size Max", 3.0f) );
	propertySheet->add( m_particleAlphaMin = new CPropFloat( "Particle Alpha Min", 0.5f) );
	propertySheet->add( m_particleAlphaMax = new CPropFloat( "Particle Alpha Max", 1.0f) );

	// emitter
	propertySheet->add( m_emitterLifeTime = new CPropFloat( "Emitter Life Time", 2.5f) );

	// rate
	propertySheet->add( m_rateMin = new CPropFloat( "Rate Min", 10.0f) );
	propertySheet->add( m_rateMax = new CPropFloat( "Rate Max", 10.0f) );

	return propertySheet;
}
