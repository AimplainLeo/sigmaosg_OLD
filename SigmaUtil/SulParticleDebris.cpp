// SulParticleDebris.cpp

#include "stdafx.h"
#include "SulParticleDebris.h"
#include "SulMath.h"

CSulParticleDebris::CSulParticleDebris( 
	osgParticle::CenteredPlacer* placer,
	CSulParticleSystemDataOsg* data,
	const osg::Vec3& velocity,
	float debrisPosOffsetMin,
	float debrisPosOffsetMax
) :
CSulParticle( data->m_debrisLifeTime, velocity, debrisPosOffsetMin, debrisPosOffsetMax )
{
	m_data = data;
	m_placer = placer;
	m_debug = false;
}

void CSulParticleDebris::enableDebug( osg::Group* root )
{
	m_debug = true;

	// used for debugging
	osg::Vec4 color( sigma::rand0to1(),sigma::rand0to1(),sigma::rand0to1(),1 );
	m_sphere = new CSulGeodeSphere( 0.2f );
	m_sphere->setColor( color );

	m_sphereMT = new osg::MatrixTransform;
	m_sphereMT->addChild( m_sphere );
	root->addChild( m_sphereMT );
}


void CSulParticleDebris::update( double dt )
{
	CSulParticle::update( dt );

	// update placer here
	if ( m_sphereMT.valid() )
	{
		osg::Matrix m; 
		m.setTrans( getPosition() );
		m_sphereMT->setMatrix( m );
	}

	m_placer->setCenter( getPosition() );
}
