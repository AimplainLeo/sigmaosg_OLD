// particleBase.cpp

#include "stdafx.h"
#include "particleBase.h"

CParticleBase::CParticleBase()
{
	m_propertySheet = 0;
}

osg::Node* CParticleBase::create( const osg::Vec3& pos )
{
	if ( !m_propertySheet )
		m_propertySheet = createPropertySheet();

	return 0;
}

CPropertySheet* CParticleBase::getPropertySheet()
{
	if ( !m_propertySheet )
		m_propertySheet = createPropertySheet();

	return m_propertySheet;
}