// SulAudioSource.cpp

#include "stdafx.h"
#include "SulAudioSource.h"

CSulAudioSource::CSulAudioSource( ALuint buffer )
{
	m_buffer = buffer;
}

void CSulAudioSource::init()
{
	m_bFirstInit = true;
	
	alGenSources( 1, &m_source );				// create source object
	alSourcei( m_source, AL_BUFFER, m_buffer );	// bind source and buffer

	alSourcef( m_source, AL_PITCH, 1 );
	alSourcef( m_source, AL_GAIN, 1 );
	alSourcei(m_source, AL_LOOPING, AL_TRUE);

	alSourcei( m_source, AL_SOURCE_RELATIVE , AL_FALSE );
	
	alSourcef( m_source, AL_ROLLOFF_FACTOR, 0.5f );
	//alSourcef( m_source, AL_REFERENCE_DISTANCE, 0.00001f );

}

void CSulAudioSource::operator()( osg::Node* node, osg::NodeVisitor* nv )
{
	traverse( node, nv );

	// handle timing
	if ( m_bFirstInit )
	{
		m_bFirstInit = false;
		m_lastTicks = osg::Timer::instance()->tick();
		return;
	}

	double ticks = osg::Timer::instance()->tick();
	double dt = osg::Timer::instance()->delta_s( m_lastTicks, ticks );
	m_lastTicks = ticks;

	alSource3f( m_source, AL_POSITION, 0, 0, 0 );
	alSource3f( m_source, AL_VELOCITY, 0, 0, 0 );

	
	
	
}

void CSulAudioSource::play()
{
	alSourcePlay( m_source );
}