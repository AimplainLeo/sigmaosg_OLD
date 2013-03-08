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

	CParticleDebrisSystem* p = new CParticleDebrisSystem( m_particleCount->getUint32() );
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

	propertySheet->add( m_particleCount = new CPropString( "Particle Count", 10) );
	propertySheet->add( m_renderBinNum = new CPropString( "RenderBin Num", 3000) );

	return propertySheet;
}
