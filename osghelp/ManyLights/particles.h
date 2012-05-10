// particles.h

#ifndef __PARTICLES_H__
#define __PARTICLES_H__

#include <SigmaUtil/SulParticle.h>
#include <SigmaUtil/SulLightManager.h>
#include <SigmaUtil/SulMath.h>
#include <SigmaUtil/SulGeomSphere.h>
#include <SigmaUtil/SulParticleSystem.h>
#include <osg/MatrixTransform>

class CMyParticle : public CSulParticle
{
public:
	CMyParticle( CSulLightManager* lm, const osg::Vec3& velocity ) :
	CSulParticle( velocity )
	{
		m_ml = lm;

		osg::Vec4 color( sigma::rand0to1(),sigma::rand0to1(),sigma::rand0to1(),1 );

		m_lp = new CSulLightPoint( lm );
		m_lp->setPosition( getPosition() );
		m_lp->setDiffuseColor( color );

		m_lp->setConstantAtt( 0.0005f );
		m_lp->setLinearAtt( 0.0f );
		m_lp->setQuadraticAtt( 0.25f );

		// create sphere
		CSulGeomSphere* sphere = new CSulGeomSphere( 0.2f );
		sphere->setColor( color );

		m_mt = new osg::MatrixTransform;
		m_mt->addChild( sphere );
		m_mt->addChild( m_lp );

		m_mt->setDataVariance( osg::Object::DYNAMIC );
	}

	virtual osg::Node* getNode()
	{
		return m_mt;
	}

	void reset()
	{
		CSulParticle::reset();

		osg::Matrix m = m_mt->getMatrix();
		m.setTrans( osg::Vec3(0,0,0) );
		m_mt->setMatrix( m );
	}

	virtual void update( double dt )
	{
		CSulParticle::update( dt );

		osg::Matrix m = m_mt->getMatrix();
		m.setTrans( getPosition() );
		m_mt->setMatrix( m );

//		m_lp->setPosition( getPosition()+osg::Vec3(7,7,5) );
	}

private:
	osg::ref_ptr<osg::MatrixTransform>	m_mt;
	osg::ref_ptr<osg::Geode>			m_geode;
	osg::ref_ptr<CSulLightPoint>		m_lp;
	osg::ref_ptr<CSulLightManager>		m_ml;
};

class CMyParticleSystem : public CSulParticleSystem
{
public:
	CMyParticleSystem( CSulLightManager* ml ) :
	  CSulParticleSystem( 100 )
	{
		m_ml = ml;
	}

	virtual void createParticle( const osg::Vec3& velocity ) 
	{
		add( new CMyParticle( m_ml, velocity ) );
	}

	virtual void remitParticle( CSulParticle* p ) 
	{
		/*
		remove( p );
		add( new CSulMyParticle( m_ml, createVelocity() ) );
		*/
		
		p->reset();
		p->setVelocity( createVelocity() );	
	}

private:
	osg::ref_ptr<CSulLightManager>	m_ml;
};

#endif // __PARTICLES_H__