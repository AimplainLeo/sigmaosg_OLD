// ParticleDebris.h

#ifndef __PARTICLEDEBRIS_H__
#define __PARTICLEDEBRIS_H__

#include <SigmaUtil/SulParticle.h>
#include <SigmaUtil/SulGeomSphere.h>
#include <osgParticle/SmokeEffect>
#include <osgParticle/SectorPlacer>
#include <osgDB/FileUtils>

osgParticle::SmokeEffect* mycreateFireBall( const osg::Vec3& pos, int binNum )
{
	osgParticle::SmokeEffect* smoke =  new osgParticle::SmokeEffect(false);
	smoke->getOrCreateStateSet()->setRenderBinDetails( binNum, "DepthSortedBin" );
	smoke->setPosition( pos );
	smoke->setIntensity( 20.0f );
	smoke->setScale( 0.8f );
	smoke->setParticleDuration( 3.0f );
	smoke->setEmitterDuration( 2.5f );
	//smoke->setUseLocalParticleSystem( false );
	smoke->buildEffect();

	osgParticle::Particle& ptemplate = smoke->getParticleSystem()->getDefaultParticleTemplate();
	ptemplate.setAlphaRange(osgParticle::rangef(1.0f, -1.0f));

	osgParticle::ModularEmitter* me = dynamic_cast<osgParticle::ModularEmitter*>(smoke->getEmitter());
	me->setReferenceFrame( osgParticle::ParticleProcessor::RELATIVE_RF );

	osgParticle::RadialShooter* shooter = dynamic_cast<osgParticle::RadialShooter*>(me->getShooter());
	osgParticle::rangef r = shooter->getThetaRange();

	// I think osg has reversed the mening of theta and phi
	shooter->setThetaRange( 0.0f, osg::PI_2 );		// up/down
	shooter->setPhiRange( 0.0f, 2.0f*osg::PI );		// around
	shooter->setInitialSpeedRange( 0, 2 );
	shooter->setInitialRotationalSpeedRange(
		osg::Vec3(0, 0, -1),
		osg::Vec3(0, 0, 1)
	);

	osgParticle::SectorPlacer* placer = dynamic_cast<osgParticle::SectorPlacer*>(me->getPlacer());
	placer->setRadiusRange(0.0f,0.01f);

	osgParticle::FluidProgram* fluid = dynamic_cast<osgParticle::FluidProgram*>(smoke->getProgram());
	fluid->setFluidDensity(1.0f);	
//	fluid->setWind( wind );

	smoke->getParticleSystem()->setDefaultAttributes( osgDB::findDataFile("images/myexp.png") );

	return smoke;
}


class CParticleDebris : public CSulParticle
{
public:
	CParticleDebris( const osg::Vec3& velocity ) :
	CSulParticle( velocity )
	{
		m_group = new osg::MatrixTransform;

		m_smoke = mycreateFireBall( osg::Vec3(0,0,0) , 5000 );
		m_smoke->setDataVariance( osg::Object::DYNAMIC );
		m_group->addChild( m_smoke );

		/*
		osg::Vec4 color( sigma::rand0to1(),sigma::rand0to1(),sigma::rand0to1(),1 );
		CSulGeomSphere* sphere = new CSulGeomSphere( 0.2f );
		sphere->setColor( color );
		m_group->addChild( sphere );
		*/
	}

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

		osgParticle::ModularEmitter* me = dynamic_cast<osgParticle::ModularEmitter*>(m_smoke->getEmitter());
		osgParticle::SectorPlacer* placer = dynamic_cast<osgParticle::SectorPlacer*>(me->getPlacer());
		placer->setCenter( getPosition() );
	}

private:
	osg::MatrixTransform*					m_group;
	osgParticle::SmokeEffect* m_smoke;
};

#endif // __PARTICLEDEBRIS_H__