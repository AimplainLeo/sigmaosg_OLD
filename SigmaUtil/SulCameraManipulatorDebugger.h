// SulCameraManipulatorDebugger.h

#ifndef __SULCAMERAMANIPULATORDEBUGGER_H__
#define __SULCAMERAMANIPULATORDEBUGGER_H__

#include <osgGA/CameraManipulator>
#include <osgGA/OrbitManipulator>
#include <osgGA/StandardManipulator>
#include <osgManipulator/Projector>

//class CSulCameraManipulatorDebugger : public osgGA::CameraManipulator
class CSulCameraManipulatorDebugger : public osgGA::StandardManipulator
{
public:
	CSulCameraManipulatorDebugger();

protected:
	virtual bool			calcHitPoint( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa, osg::Vec3d& hit );

	////////////////////////////////////////////////////////////////
	// from base class (abstract methods)
	////////////////////////////////////////////////////////////////

	virtual void setTransformation( const osg::Vec3d& eye, const osg::Quat& rotation );
	virtual void setTransformation( const osg::Vec3d& eye, const osg::Vec3d& center, const osg::Vec3d& up );
	virtual void getTransformation( osg::Vec3d& eye, osg::Quat& rotation ) const;
	virtual void getTransformation( osg::Vec3d& eye, osg::Vec3d& center, osg::Vec3d& up ) const;

	virtual void			setByMatrix( const osg::Matrixd& matrix );
	virtual void			setByInverseMatrix( const osg::Matrixd& matrix );
	virtual osg::Matrixd	getMatrix() const;
	virtual osg::Matrixd	getInverseMatrix() const;

	/*
	bool					handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa );
	bool					handleMouseDrag( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa );
	bool					handleMousePush( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa );
	bool					handleMouseRelease( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa );
	*/

	virtual bool handleMouseDrag( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa );
	virtual bool handleMouseRelease( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa );
	virtual bool performMovementLeftMouseButton( const double eventTimeDelta, const double dx, const double dy );
	virtual bool handleMouseWheel( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us );
	virtual bool performMovementMiddleMouseButton( const double eventTimeDelta, const double dx, const double dy );

	void zoom( float factor );

private:
	osg::Vec3d										m_hit;

	bool											m_bMousePush;
	float											m_mouseLastX;
	float											m_mouseLastY;

	osg::ref_ptr<osgManipulator::PlaneProjector>	m_planeProjector;

	osg::Vec3										m_center;
	osg::Quat										m_rotation;
	osg::Quat										m_rotationNew;
	osg::Matrixd									m_inverseMatrix;

	float											m_distance;

	double											m_wheelZoomFactor;
	double											m_panFactor;
};

#endif // __SULCAMERAMANIPULATORDEBUGGER_H__