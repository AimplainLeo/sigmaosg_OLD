// SulAudioListener.h

#ifndef __SULAUDIOLISTENER_H__
#define __SULAUDIOLISTENER_H__

class CSulAudioListener : public osg::NodeCallback
{
public:
	void init();

	virtual void operator()( osg::Node* node, osg::NodeVisitor* nv );

private:
	bool			m_bFirstInit;
	osg::Timer_t	m_lastTicks;
	osg::Vec3f		m_pos;
};

#endif // __SULAUDIOLISTENER_H__