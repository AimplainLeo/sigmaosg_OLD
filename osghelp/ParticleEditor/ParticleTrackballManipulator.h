// ParticleTrackballManipulator.h

#ifndef __PARTICLETRACKBALLMANIPULATOR_H__
#define __PARTICLETRACKBALLMANIPULATOR_H__

#include <SigmaUtil/SulParticleSystemOsg.h>
#include <osgGA/TrackballManipulator>

class CParticleTrackballManipulator : public osgGA::TrackballManipulator
{
public:
	CParticleTrackballManipulator( CSulParticleSystemOsg* particleSystem );

	virtual bool handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us );

private:
	bool calcHitPoint(  const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa, osg::Vec3d& hit );
	
private:
	CSulParticleSystemOsg*			m_particleSystem;
	CSulParticleSystemContainerOsg* m_particleSystemContainerLast;
	
	bool	m_bCameraControl;
};


#endif // __PARTICLETRACKBALLMANIPULATOR_H__