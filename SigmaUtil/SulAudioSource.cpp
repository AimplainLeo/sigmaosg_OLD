// SulAudioSource.cpp

#include "stdafx.h"
#include "SulAudioSource.h"

CSulAudioSource::CSulAudioSource( ALuint buffer, CSulAudioListener* listener )
{
	m_listener = listener;
	m_buffer = buffer;
	m_radiusStart = 0.0;
	m_maxDistance = 1000.0;
	m_bPlayCalled = false;
	m_bUseSoundInAir = false;
}

void CSulAudioSource::init()
{
	m_bFirstInit = true;
	
	alGenSources( 1, &m_source );				// create source object
	alSourcei( m_source, AL_BUFFER, m_buffer );	// bind source and buffer

	alSourcef( m_source, AL_PITCH, 1 );
	alSourcef( m_source, AL_GAIN, 1 );
	alSourcei( m_source, AL_LOOPING, AL_TRUE );

	alSourcei( m_source, AL_SOURCE_RELATIVE , AL_FALSE );
	
	alSourcef( m_source, AL_ROLLOFF_FACTOR, 0.5f );
	
	alSource3f( m_source, AL_POSITION, 0, 0, 0 );
	alSource3f( m_source, AL_VELOCITY, 0, 0, 0 );
}

void CSulAudioSource::setUseSoundInAir( bool use )
{
	m_bUseSoundInAir = use;
}

void CSulAudioSource::setLooping( bool looping )
{
	alSourcei( m_source, AL_LOOPING, looping?AL_TRUE:AL_FALSE );
}

void CSulAudioSource::operator()( osg::Node* node, osg::NodeVisitor* nv )
{
	traverse( node, nv );

	// calc world position
	osg::Matrix w = osg::computeLocalToWorld( nv->getNodePath(), false );
	w = w.inverse(w);
	m_pos = w.getTrans();

/*
Possible values of state
AL_INITIAL			??????? 
AL_STOPPED			is set when the sound has finished (and I assume if stopped)
AL_PLAYING			is set when the sound is playing
AL_PAUSED1
*/

	ALint state;
	alGetSourcei( m_source, AL_SOURCE_STATE, &state );

	if ( m_bPlayCalled && state!=AL_PLAYING )
	{
		// calc listener position relative to sound origin and only start if the sound is inside
		osg::Vec3 listenerPos = m_listener->getPosition();

		osg::Vec3 v = listenerPos-m_pos;
		double len = v.length();

		// not inside radius
		if ( len>m_maxDistance )
			return;

		m_bPlayCalled = false; // reset it
		alSourcePlay( m_source );
	}

	if ( state==AL_STOPPED )
	{
		int a = 0;
	}

	if ( state!=AL_PLAYING )
		return;

	// handle timing
	if ( m_bFirstInit )
	{
		m_bFirstInit = false;
		m_lastTicks = osg::Timer::instance()->tick();
		osg::Vec3f m_pos = w.getTrans();
		return;
	}

	double ticks = osg::Timer::instance()->tick();
	double dt = osg::Timer::instance()->delta_s( m_lastTicks, ticks );
	m_lastTicks = ticks;

	// velocity (we base the velocity time and distance moved from last time)
	osg::Vec3f vel = (w.getTrans()-m_pos)/dt;
	alSourcefv( m_source, AL_VELOCITY, (float*)&vel );


	// adjust coordinate system to osg with z as up and down
	osg::Vec3 test( m_pos.x(), m_pos.z(), m_pos.y() );

	alSourcefv( m_source, AL_POSITION, (float*)&test );

	// calc sound movement through the air
	m_radiusStart += 343.0*dt;
}

void CSulAudioSource::play()
{
	m_bPlayCalled = true;
}