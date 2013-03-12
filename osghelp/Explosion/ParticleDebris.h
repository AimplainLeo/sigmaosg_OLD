// ParticleDebris.h

#ifndef __PARTICLEDEBRIS_H__
#define __PARTICLEDEBRIS_H__

#include <SigmaUtil/SulParticle.h>
#include <SigmaUtil/SulGeomSphere.h>
#include <osg/MatrixTransform>
#include <osgParticle/SmokeEffect>
#include <osgParticle/SectorPlacer>
#include <osgDB/FileUtils>

class CParticleDebris : public CSulParticle
{
public:
	CParticleDebris( 
		const osg::Vec3& velocity,
		float debrisPosOffsetMin = 0.0f,
		float debrisPosOffsetMax = 0.0f,
		float particleMass		= 0.7f,
		float particleLifeTime	= 3.0f,
		float particleSizeMin	= 0.75f,
		float particleSizeMax	= 3.0f,
		float particleAlphaMin	= 0.5f,
		float particleAlphaMax	= 1.0f,
		float emitterLifeTime	= 2.5f,
		float rateMin			= 10.0,
		float rateMax			= 10.0
	);

	virtual osg::Node* getNode()
	{
		return m_group;
	}

	void reset()
	{
		//CSulParticle::reset();

		/*
		osg::Matrix m = m_mt->getMatrix();
		m.setTrans( osg::Vec3(0,0,0) );
		m_mt->setMatrix( m );
		*/
	}

	// note: getPosition is the local particle system
	virtual void update( double dt )
	{
		CSulParticle::update( dt );
		/*
		osgParticle::ModularEmitter* me = dynamic_cast<osgParticle::ModularEmitter*>(m_smoke->getEmitter());
		osgParticle::SectorPlacer* placer = dynamic_cast<osgParticle::SectorPlacer*>(me->getPlacer());
		placer->setCenter( getPosition() );
		*/

		m_placer->setCenter( getPosition() );
	}

private:
	osgParticle::SmokeEffect*	mycreateFireBall( const osg::Vec3& pos, int binNum );
	osg::Node*					myAnimatedBurningSmoke( const osg::Vec3& pos, int binNum );

private:
	osg::MatrixTransform*		m_group;
//	osgParticle::SmokeEffect*	m_smoke;

	osgParticle::SectorPlacer*	m_placer;
	float						m_particleMass;
	float						m_particleLifeTime;
	float						m_particleSizeMin;
	float						m_particleSizeMax;
	float						m_particleAlphaMin;
	float						m_particleAlphaMax;
	float						m_emitterLifeTime;
	float						m_rateMin;
	float						m_rateMax;
};

#endif // __PARTICLEDEBRIS_H__