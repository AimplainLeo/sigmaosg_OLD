// SulParticle.h

#ifndef __SULPARTICLE_H__
#define __SULPARTICLE_H__

#include <osg/node>
#include <osg/vec3>
#include <osg/referenced>

class CSulParticle : public osg::Referenced
{
public:
	CSulParticle( const osg::Vec3& velocity )
	{
		m_vel = velocity;
		m_life = 2.0f;				// seconds
		m_pos.set( 0, 0, 0 );
	}

	virtual void reset() 
	{
		m_pos.set( 0, 0, 0 );
		m_age = m_life;
	}

	virtual osg::Node* getNode()
	{
		return 0;
	}

	bool alive()
	{
		/*
		if ( m_age<0 )
			return false;
			*/
		if ( m_pos.z()<-5.0f )
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
		m_age -= dt;
	}

private:
	osg::Vec3 m_pos;
	osg::Vec3 m_vel;

	float	m_life;
	float	m_age;
};

typedef std::vector< osg::ref_ptr<CSulParticle> >	VEC_PARTICLES;

#endif // __SULPARTICLE_H__