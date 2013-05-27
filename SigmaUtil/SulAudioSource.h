// SulAudioSource.h

#ifndef __SULAUDIOSOURCE_H__
#define __SULAUDIOSOURCE_H__

#include "SulAudioListener.h"
#include <al.h>

class CSulAudioSource : public osg::NodeCallback
{
public:
			CSulAudioSource( ALuint buffer, CSulAudioListener* listener );

	void	init();
	void	play();

	void	setUseSoundInAir( bool use );

	void	setLooping( bool looping );

protected:
	virtual void operator()( osg::Node* node, osg::NodeVisitor* nv );

private:
	bool			m_bFirstInit;
	osg::Timer_t	m_lastTicks;
	osg::Vec3f		m_pos;
	ALuint			m_source;
	ALuint			m_buffer;

	// some notes:
	// for looping sounds.. perhaps it's just the volume that needs to be turned on/off or perhaps just ignore looping sounds
	// for oneshot sounds.. perhaps it's enough just to start play when you can hear the sound at the correct distance.
	// sound moves around 343 meters per second at a temperature 20 degrees
	// i need a parameter that defines how far away the sound can be heard

	double			m_radiusStart;		// meter
	double			m_radiusEnd;		// meter
	double			m_maxDistance;		// meter, max distance the sound can be heard

	bool			m_bSoundStarted;
	bool			m_bPlayCalled;
	bool			m_bUseSoundInAir;	// default false

	osg::ref_ptr<CSulAudioListener>	m_listener;
};

#endif // __SULAUDIOSOURCE_H__