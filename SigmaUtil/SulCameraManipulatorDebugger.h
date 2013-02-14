// SulCameraManipulatorDebugger.h

#ifndef __SULCAMERAMANIPULATORDEBUGGER_H__
#define __SULCAMERAMANIPULATORDEBUGGER_H__

#include <osgGA/CameraManipulator>
#include <osgGA/OrbitManipulator>

class CSulCameraManipulatorDebugger : public osgGA::CameraManipulator
{
public:
	CSulCameraManipulatorDebugger();

protected:
	virtual bool			getHitPoint( osg::Vec3& hit );

	////////////////////////////////////////////////////////////////
	// from base class (abstract methods)
	////////////////////////////////////////////////////////////////

	virtual void			setByMatrix( const osg::Matrixd& matrix );
	virtual void			setByInverseMatrix( const osg::Matrixd& matrix );
	virtual osg::Matrixd	getMatrix() const;
	virtual osg::Matrixd	getInverseMatrix() const;

private:
	bool					handle( const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa );
	bool					handleMousePush( const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa );
};

#endif // __SULCAMERAMANIPULATORDEBUGGER_H__