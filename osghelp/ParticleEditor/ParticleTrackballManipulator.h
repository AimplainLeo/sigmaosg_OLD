// ParticleTrackballManipulator.h

#ifndef __PARTICLETRACKBALLMANIPULATOR_H__
#define __PARTICLETRACKBALLMANIPULATOR_H__

#include <SigmaUtil/SulParticleSystemOsg.h>
#include <osgGA/TrackballManipulator>

class CParticleTrackballManipulator : public osgGA::TrackballManipulator
{
public:
	CParticleTrackballManipulator( class CViewerWidget* viewerWidget );

	virtual bool handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us );

private:
	bool calcHitPoint(  const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa, osg::Vec3d& hit );
	
private:
	class CViewerWidget* m_viewerWidget;
	bool	m_bCameraControl;
};


#endif // __PARTICLETRACKBALLMANIPULATOR_H__