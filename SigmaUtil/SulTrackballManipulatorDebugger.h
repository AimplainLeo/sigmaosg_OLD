// SulTrackballManipulatorDebugger.h

#ifndef __SULTRACKBALLMANIPULATORDEBUGGER_H__
#define __SULTRACKBALLMANIPULATORDEBUGGER_H__

#include "SulEntity.h"
#include "SulTypes.h"

class CSulTrackballManipulatorDebugger : public osgGA::TrackballManipulator
{
public:
					CSulTrackballManipulatorDebugger();

	void			add( sigma::uint8 key, CSulEntity* entity );

	virtual bool	handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us );

private:
	virtual bool	calcHitPoint(  const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa, osg::Vec3d& hit );
	
private:
	bool			m_bCameraControl;

	typedef std::map<sigma::uint8, osg::ref_ptr<CSulEntity> >	MAP_ENTITIES;
	MAP_ENTITIES												m_entities;
};

#endif // __SULTRACKBALLMANIPULATORDEBUGGER_H__