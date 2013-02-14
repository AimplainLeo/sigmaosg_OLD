// ParticleDebris.h

#ifndef __PARTICLEDEBRIS_H__
#define __PARTICLEDEBRIS_H__

#include <SigmaUtil/SulParticle.h>
#include <SigmaUtil/SulGeomSphere.h>
#include <osgParticle/SmokeEffect>
#include <osgParticle/SectorPlacer>

osgParticle::SmokeEffect* mycreateFireBall( const osg::Vec3& pos, int binNum )
{
	osgParticle::SmokeEffect* smoke =  new osgParticle::SmokeEffect(false);
	smoke->getOrCreateStateSet()->setRenderBinDetails( binNum, "DepthSortedBin" );
	smoke->setPosition( pos );
	smoke->setIntensity( 20.0f );
	smoke->setScale( 0.8f );
	smoke->setParticleDuration( 2.0f );
	smoke->setEmitterDuration( 2.5f );
	smoke->buildEffect();

	//smoke->setUseLocalParticleSystem( true );

	osgParticle::Particle& ptemplate = smoke->getParticleSystem()->getDefaultParticleTemplate();
	ptemplate.setAlphaRange(osgParticle::rangef(1.0f, -1.0f));

	osgParticle::ModularEmitter* me = dynamic_cast<osgParticle::ModularEmitter*>(smoke->getEmitter());

	osgParticle::RadialShooter* shooter = dynamic_cast<osgParticle::RadialShooter*>(me->getShooter());
	osgParticle::rangef r = shooter->getThetaRange();

	// I think osg has reversed the mening of theta and phi
	shooter->setThetaRange( 0.0f, osg::PI_2 );		// up/down
	shooter->setPhiRange( 0.0f, 2.0f*osg::PI );		// around
	shooter->setInitialSpeedRange( 0, 4 );
	shooter->setInitialRotationalSpeedRange(
		osg::Vec3(0, 0, -1),
		osg::Vec3(0, 0, 1)
	);

	osgParticle::SectorPlacer* placer = dynamic_cast<osgParticle::SectorPlacer*>(me->getPlacer());
	placer->setRadiusRange(0.0f,0.01f);

	osgParticle::FluidProgram* fluid = dynamic_cast<osgParticle::FluidProgram*>(smoke->getProgram());
	fluid->setFluidDensity(1.0f);	
//	fluid->setWind( wind );

	smoke->getParticleSystem()->setDefaultAttributes( "c:/myexp.png" );

	return smoke;
}


class CParticleDebris : public CSulParticle
{
public:
	CParticleDebris( const osg::Vec3& velocity ) :
	CSulParticle( velocity )
	{
		m_smoke = mycreateFireBall( getPosition(), 5000 );
		m_smoke->setDataVariance( osg::Object::DYNAMIC );
		/*
		float scaleSmoke = 0.5f;
		float intensitySmoke = 50.0f;
		m_smoke =  new osgParticle::SmokeEffect(getPosition(), scaleSmoke, intensitySmoke);
		m_smoke->setParticleDuration( 1.5f );
		m_smoke->setEmitterDuration( 2.0f );
		m_smoke->getOrCreateStateSet()->setRenderBinDetails( 5000, "DepthSortedBin" );
		*/
	
		/*
		// create sphere
		CSulGeomSphere* sphere = new CSulGeomSphere( 0.2f );
		sphere->setColor( color );

		m_mt = new osg::MatrixTransform;
		m_mt->addChild( sphere );
		m_mt->setDataVariance( osg::Object::DYNAMIC );
		*/
	}

	virtual osg::Node* getNode()
	{
		return m_smoke;
	}

	void reset()
	{
		CSulParticle::reset();

		/*
		osg::Matrix m = m_mt->getMatrix();
		m.setTrans( osg::Vec3(0,0,0) );
		m_mt->setMatrix( m );
		*/
	}

	virtual void update( double dt )
	{
		CSulParticle::update( dt );

		//m_smoke->setPosition( getPosition() );

		osgParticle::ModularEmitter* me = dynamic_cast<osgParticle::ModularEmitter*>(m_smoke->getEmitter());
		osgParticle::SectorPlacer* placer = dynamic_cast<osgParticle::SectorPlacer*>(me->getPlacer());
		placer->setCenter( getPosition() );
		/*
		osg::Matrix m = m_mt->getMatrix();
		m.setTrans( getPosition() );
		m_mt->setMatrix( m );
		*/

//		m_lp->setPosition( getPosition()+osg::Vec3(7,7,5) );
	}

private:
//	osg::ref_ptr<osg::MatrixTransform>	m_mt;
	osgParticle::SmokeEffect* m_smoke;
};

#endif // __PARTICLEDEBRIS_H__