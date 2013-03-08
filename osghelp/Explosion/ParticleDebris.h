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
	CParticleDebris( const osg::Vec3& velocity ) :
	CSulParticle( velocity )
	{
		m_group = new osg::MatrixTransform;

		m_group->addChild( myAnimatedBurningSmoke( osg::Vec3(0,0,0) , 5000 ) );
		//m_group->addChild( mycreateFireBall( osg::Vec3(0,0,0) , 5000 ) );
		
		m_group->setDataVariance( osg::Object::DYNAMIC );
		

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
};

#endif // __PARTICLEDEBRIS_H__