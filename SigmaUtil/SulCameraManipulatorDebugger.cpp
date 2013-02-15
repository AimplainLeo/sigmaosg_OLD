// SulCameraManipulatorDebugger.cpp

#include "stdafx.h"
#include "SulCameraManipulatorDebugger.h"
#include <osgViewer/Viewer>

CSulCameraManipulatorDebugger::CSulCameraManipulatorDebugger() 
{
	// create default intersection plane for mouse 
	osg::Plane plane( osg::Vec3(0,0,1), osg::Vec3(0,0,0) );
	m_planeProjector = new osgManipulator::PlaneProjector;
	m_planeProjector->setPlane( plane );

	m_bMousePush = false;
	m_mouseLastX = 0;
	m_mouseLastY = 0;
}

void CSulCameraManipulatorDebugger::setByMatrix( const osg::Matrixd& matrix )
{
	// FIXME:
}

void CSulCameraManipulatorDebugger::setByInverseMatrix( const osg::Matrixd& matrix )
{
    setByMatrix( osg::Matrixd::inverse( matrix ) );
}

osg::Matrixd CSulCameraManipulatorDebugger::getMatrix() const
{
	// FIXME:
	return osg::Matrixd();
}

osg::Matrixd CSulCameraManipulatorDebugger::getInverseMatrix() const
{
	osg::Vec3 eye( 20, 20, 5 );
	osg::Vec3d lv( m_hit - eye );
	float distance = lv.length();
	return osg::Matrixd::translate( -m_hit ) *
           osg::Matrixd::rotate( m_rotation.inverse() ) *
           osg::Matrixd::translate( 0.0, 0.0, -distance );

	 /*

	osg::Matrix m;

	m.makeLookAt( 
		osg::Vec3(20,20,5),
		osg::Vec3(0,0,0),
		osg::Vec3(0,0,1)
	);

	return m;
	*/
}

bool CSulCameraManipulatorDebugger::calcHitPoint(  const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa, osg::Vec3d& hit )
{
	// if not overrided then we will create a zero plane and intersect with that
	osgViewer::Viewer* viewer = dynamic_cast<osgViewer::Viewer*>(&aa);
	if ( viewer )
	{
		osgManipulator::PointerInfo pi;
		pi.reset();
		pi.setCamera( viewer->getCamera() );
		pi.setMousePosition( ea.getX(), ea.getY() );
		m_planeProjector->project( pi, hit );
		return true;
	}

	return false;
}

bool CSulCameraManipulatorDebugger::handleMouseDrag( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa )
{
    unsigned int buttonMask = ea.getButtonMask();

	// calc delta values for mouse drag
	float dx = ea.getXnormalized() - m_mouseLastX;
	float dy = ea.getYnormalized() - m_mouseLastY;

	// remember last values
	m_mouseLastX = ea.getXnormalized();
	m_mouseLastY = ea.getYnormalized();

	// rotate
	if ( buttonMask==osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON )
    {
		// rotate around point
		osg::Quat deltaRotation;
		osg::Vec3 up = m_rotation * osg::Z_AXIS;
		float angle = dx;
		deltaRotation.makeRotate( angle, up );
		
        m_rotation = m_rotation * deltaRotation;
    }

	// translate
	if ( buttonMask==osgGA::GUIEventAdapter::MIDDLE_MOUSE_BUTTON )
    {
        
    }

	return true;
}

bool CSulCameraManipulatorDebugger::handleMousePush( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa )
{
	// initialize last values to current values
	m_mouseLastX = ea.getXnormalized();
	m_mouseLastY = ea.getYnormalized();

	// we need to find point that the mouse hit. This can be anything so we call a virtual method so the super class has
	// a chance to decide the rotation point

	if ( calcHitPoint( ea, aa, m_hit ) )
	{
		m_bMousePush = true;
		return true;
	}

	return false;
}

bool CSulCameraManipulatorDebugger::handleMouseRelease( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa )
{
	m_bMousePush = false;
	return true;
}

bool CSulCameraManipulatorDebugger::handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa )
{
    switch ( ea.getEventType() )
    {
        case osgGA::GUIEventAdapter::KEYDOWN:
            {
				return true;
            }
            

        case osgGA::GUIEventAdapter::KEYUP:
            {
				return true;
            }

        case osgGA::GUIEventAdapter::FRAME:
            {
                double t = ea.getTime();

				return true;
            }

		case osgGA::GUIEventAdapter::DRAG:
			return handleMouseDrag( ea, aa );

		case osgGA::GUIEventAdapter::PUSH:
			return handleMousePush( ea, aa );

		case osgGA::GUIEventAdapter::RELEASE:
			return handleMouseRelease( ea, aa );

    }
	
    return false;
}
