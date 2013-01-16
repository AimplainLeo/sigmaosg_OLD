// SulAudioListener.cpp

#include "stdafx.h"
#include "SulAudioListener.h"

#include "al.h"
#include "alc.h"
#include "alut.h"

void CSulAudioListener::init()
{
	m_bFirstInit = true;

	// set some defaults
	float listenerOri[] = {0,0,-1, 0.0,0.0,1.0};	// orientation (it seems that the ori is a direction and an up vector)
	alListenerfv( AL_ORIENTATION, listenerOri );
	alListener3f( AL_POSITION, 0,0,0 );
	alListener3f( AL_VELOCITY, 0,0,0 );
}

void CSulAudioListener::operator()( osg::Node* node, osg::NodeVisitor* nv )
{
	traverse( node, nv );

	osg::Camera* cam = dynamic_cast<osg::Camera*>(node);
	osg::Matrix w = cam->getInverseViewMatrix();

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

	// 1. we need to update position, velocity and orientation
	// 2. should update at a certain frequency

	//osg::Matrix w = osg::computeLocalToWorld( nv->getNodePath() );

	// velocity (we base the velocity time and distance moved from last time)
	osg::Vec3f vel = (w.getTrans()-m_pos)/dt;
	alListenerfv( AL_VELOCITY, (float*)&vel );

	// position
	m_pos = w.getTrans();
	alListenerfv( AL_POSITION, (float*)&m_pos );

	// orientation
	osg::Quat q = w.getRotate();
	osg::Vec3f dir(0,0,-1);
	dir = q * dir;	// now rotate this dir
	float listenerOri[] = {dir.x(),dir.y(),dir.z(), 0.0,0.0,1.0};	// orientation (it seems that the ori is a direction and an up vector)
	alListenerfv( AL_ORIENTATION, listenerOri );
}


