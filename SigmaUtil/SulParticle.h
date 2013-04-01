// SulParticle.h

#ifndef __SULPARTICLE_H__
#define __SULPARTICLE_H__

#include <osg/node>
#include <osg/vec3>
#include <osg/referenced>
#include <osgParticle/range>

class CSulParticle : public osg::Referenced
{
public:
	CSulParticle( float lifetime, const osg::Vec3& velocity, float posOffsetMin=0.0f, float posOffsetMax=0.0f )
	{
		m_vel = velocity;
		m_lifeTime = lifetime;				// seconds
		m_age = 0.0f;

		
		m_rangeOffsetRadius.set( posOffsetMin, posOffsetMax );

		m_pos = m_vel;
		m_pos.z() = 0.0f;
		m_pos.normalize();
		m_pos = m_pos *	m_rangeOffsetRadius.get_random();
	}

	virtual void reset() 
	{
		m_pos.set( 0, 0, 0 );
		m_age = 0.0f;
	}

	virtual osg::Node* getNode()
	{
		return 0;
	}

	bool alive()
	{
		if ( m_age>m_lifeTime )
			return false;

		return true;
	}

	const osg::Vec3& getPosition() const
	{
		return m_pos;
	}

	void setVelocity( const osg::Vec3& v )
	{
		m_vel = v;
	}

	virtual void update( double dt )
	{
		osg::Vec3 a( 0.0f, 0.0f, -9.8f );
		m_vel = m_vel + a*dt;
		m_pos = m_pos + m_vel*dt;
		m_age += dt;
	}

private:
	osg::Vec3 m_pos;
	osg::Vec3 m_vel;

	float	m_lifeTime;
	float m_age;

	osgParticle::rangef m_rangeOffsetRadius;
};

typedef std::vector< osg::ref_ptr<CSulParticle> >	VEC_PARTICLES;

#endif // __SULPARTICLE_H__