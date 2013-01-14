// SulAudioSource.h

#ifndef __SULAUDIOSOURCE_H__
#define __SULAUDIOSOURCE_H__

#include <al.h>

class CSulAudioSource : public osg::NodeCallback
{
public:
			CSulAudioSource( ALuint buffer );

	void	init();

	void	play();

protected:
	virtual void operator()( osg::Node* node, osg::NodeVisitor* nv );

private:
	bool			m_bFirstInit;
	osg::Timer_t	m_lastTicks;
	osg::Vec3f		m_pos;
	ALuint			m_source;
	ALuint			m_buffer;
};

#endif // __SULAUDIOSOURCE_H__