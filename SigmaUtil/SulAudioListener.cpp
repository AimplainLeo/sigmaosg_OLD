// SulAudioListener.cpp

#include "stdafx.h"
#include "SulAudioListener.h"

#include "al.h"
#include "alc.h"
#include "alut.h"

void CSulAudioListener::init()
{
	m_bFirstInit = true;
}

void CSulAudioListener::operator()( osg::Node* node, osg::NodeVisitor* nv )
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

	// 1. we need to update position, velocity and orientation
	// 2. should update at a certain frequency

	osg::Matrix w = osg::computeLocalToWorld( nv->getNodePath() );

	// velocity (we base the velocity time and distance moved from last time)
	osg::Vec3f vel = (w.getTrans()-m_pos)/dt;
	alListenerfv( AL_ORIENTATION, (float*)&vel );

	// position
	m_pos = w.getTrans();
	alListenerfv( AL_POSITION, (float*)&m_pos );

	// orientation
	osg::Quat q = w.getRotate();
	osg::Vec3f dir(0,1,0);
	dir = q * dir;	// now rotate this dir
	float listenerOri[] = {dir.x(),dir.y(),dir.z(), 0.0,1.0,0.0};	// orientation (it seems that the ori is a direction and an up vector)
	alListenerfv( AL_ORIENTATION, listenerOri );




}


