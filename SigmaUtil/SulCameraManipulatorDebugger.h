// SulCameraManipulatorDebugger.h

#ifndef __SULCAMERAMANIPULATORDEBUGGER_H__
#define __SULCAMERAMANIPULATORDEBUGGER_H__

#include <osgGA/CameraManipulator>
#include <osgGA/OrbitManipulator>
#include <osgManipulator/Projector>

class CSulCameraManipulatorDebugger : public osgGA::CameraManipulator
{
public:
	CSulCameraManipulatorDebugger();

protected:
	virtual bool			calcHitPoint( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa, osg::Vec3d& hit );

	////////////////////////////////////////////////////////////////
	// from base class (abstract methods)
	////////////////////////////////////////////////////////////////

	virtual void			setByMatrix( const osg::Matrixd& matrix );
	virtual void			setByInverseMatrix( const osg::Matrixd& matrix );
	virtual osg::Matrixd	getMatrix() const;
	virtual osg::Matrixd	getInverseMatrix() const;

private:
	bool					handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa );
	bool					handleMouseDrag( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa );
	bool					handleMousePush( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa );
	bool					handleMouseRelease( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa );

private:
	osg::Vec3d										m_hit;

	bool											m_bMousePush;
	float											m_mouseLastX;
	float											m_mouseLastY;

	osg::ref_ptr<osgManipulator::PlaneProjector>	m_planeProjector;

//	osg::Vec3										m_center;
	osg::Quat										m_rotation;
};

#endif // __SULCAMERAMANIPULATORDEBUGGER_H__