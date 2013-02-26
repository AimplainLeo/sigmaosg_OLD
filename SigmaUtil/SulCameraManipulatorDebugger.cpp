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

	m_wheelZoomFactor = 2.0;
	m_panFactor = 3.0;
}

// overrides standard maniluplator
void CSulCameraManipulatorDebugger::setTransformation( const osg::Vec3d& eye, const osg::Quat& rotation )
{
//    _center = eye + rotation * osg::Vec3d( 0., 0., -_distance );
    m_rotation = rotation;
	/*
    // fix current rotation
    if( getVerticalAxisFixed() )
        fixVerticalAxis( _center, _rotation, true );
		*/
}

// overrides standard maniluplator
void CSulCameraManipulatorDebugger::setTransformation( const osg::Vec3d& eye, const osg::Vec3d& center, const osg::Vec3d& up )
{
    osg::Vec3d lv( center - eye );

    osg::Vec3d f( lv );
    f.normalize();
    osg::Vec3d s( f^up );
    s.normalize();
    osg::Vec3d u( s^f );
    u.normalize();

    osg::Matrixd rotation_matrix( s[0], u[0], -f[0], 0.0f,
                            s[1], u[1], -f[1], 0.0f,
                            s[2], u[2], -f[2], 0.0f,
                            0.0f, 0.0f,  0.0f, 1.0f );

    m_center = center;
	m_distance = lv.length();
    m_rotation = rotation_matrix.getRotate().inverse();

	/*
    // fix current rotation
    if( getVerticalAxisFixed() )
        fixVerticalAxis( _center, _rotation, true );
		*/

	m_inverseMatrix = osg::Matrixd::translate( -center ) *
           osg::Matrixd::rotate( m_rotation.inverse() ) *
           osg::Matrixd::translate( 0.0, 0.0, -m_distance );

}

void CSulCameraManipulatorDebugger::getTransformation( osg::Vec3d& eye, osg::Quat& rotation ) const
{
   // eye = _center - _rotation * osg::Vec3d( 0., 0., -_distance );
    rotation = m_rotation;
}

void CSulCameraManipulatorDebugger::getTransformation( osg::Vec3d& eye, osg::Vec3d& center, osg::Vec3d& up ) const
{
//    center = _center;
//    eye = _center + _rotation * osg::Vec3d( 0., 0., _distance );
    up = m_rotation * osg::Vec3d( 0., 1., 0. );
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
	return m_inverseMatrix;

	/*
	// calc rotation matrix around hit point
	osg::Matrixd r = osg::Matrixd::translate( -m_hit );
	r = r *  osg::Matrixd::rotate( m_rotationNew.inverse() );
	r = r * osg::Matrixd::translate( m_hit );

	m_inverseMatrix = r * m_inverseMatrix;

	return m_inverseMatrix;
	*/


	/*
	return osg::Matrixd::translate( -m_hit ) *
           osg::Matrixd::rotate( m_rotation.inverse() ) *
           osg::Matrixd::translate( 0.0, 0.0, -m_distance );
		   */

	/*
	osg::Vec3 eye( 20, 20, 5 );
	osg::Vec3d lv( m_hit - eye );
	float distance = lv.length();
	return osg::Matrixd::translate( -m_hit ) *
           osg::Matrixd::rotate( m_rotation.inverse() ) *
           osg::Matrixd::translate( 0.0, 0.0, -distance );

		   */

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
/*
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
*/

bool CSulCameraManipulatorDebugger::handleMouseRelease( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa )
{
	m_bMousePush = false;
	return osgGA::StandardManipulator::handleMouseRelease( ea, aa );
}

bool CSulCameraManipulatorDebugger::handleMouseDrag( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa )
{
	if ( !m_bMousePush && calcHitPoint( ea, aa, m_hit ) )
	{
		osgViewer::Viewer* viewer = dynamic_cast<osgViewer::Viewer*>(&aa);
		osg::Camera* cam = viewer->getCamera();

		osg::Vec3 v = m_hit - cam->getInverseViewMatrix().getTrans();
		//m_distance = v.length();

		m_bMousePush = true;
		return true;
	}

	return osgGA::StandardManipulator::handleMouseDrag( ea, aa );
}

bool CSulCameraManipulatorDebugger::performMovementLeftMouseButton( const double eventTimeDelta, const double dx, const double dy )
{
	osg::Quat rot;
	rotateYawPitch( rot, dx, dy, osg::Z_AXIS );

	//////////////////////////////////////////////
	// rotate yaw
	//////////////////////////////////////////////

	osg::Quat rotateYaw( -dx, osg::Z_AXIS );

	// create rotation around point
	osg::Matrixd r = osg::Matrixd::translate( -m_hit );
	r = r * osg::Matrixd::rotate( rotateYaw.inverse() );
	r = r * osg::Matrixd::translate( m_hit );

	m_inverseMatrix = r * m_inverseMatrix;

	//////////////////////////////////////////////
	// rotate pitch
	//////////////////////////////////////////////

	// we need viewspace x axis to control pitch
	osg::Matrixd viewMatrix = m_inverseMatrix.inverse( m_inverseMatrix );
	osg::Vec3 axis = osg::X_AXIS;
	axis = axis * osg::Matrixd( viewMatrix.getRotate() );

	osg::Quat rotatePitch( dy, axis );

	// create rotation around point
	r = osg::Matrixd::translate( -m_hit );
	r = r * osg::Matrixd::rotate( rotatePitch.inverse() );
	r = r * osg::Matrixd::translate( m_hit );
	
	m_inverseMatrix = r * m_inverseMatrix;

	return true;
}

void CSulCameraManipulatorDebugger::zoom( float factor )
{
	osg::Matrixd m;
	
	m.setTrans( 0, 0, factor );

	m_inverseMatrix = m_inverseMatrix * m;
}

bool CSulCameraManipulatorDebugger::handleMouseWheel( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us )
{
    osgGA::GUIEventAdapter::ScrollingMotion sm = ea.getScrollingMotion();

	switch( sm )
    {
        // mouse scroll up event
        case osgGA::GUIEventAdapter::SCROLL_UP:
        {
            // perform zoom
            zoom( m_wheelZoomFactor );
            us.requestRedraw();
            us.requestContinuousUpdate( isAnimating() || _thrown );
            return true;
        }

        // mouse scroll down event
        case osgGA::GUIEventAdapter::SCROLL_DOWN:
        {
            // perform zoom
            zoom( - m_wheelZoomFactor);
            us.requestRedraw();
            us.requestContinuousUpdate( false );
            return true;
        }

        // unhandled mouse scrolling motion
        default:
            return false;
   }
}

bool CSulCameraManipulatorDebugger::performMovementMiddleMouseButton( const double eventTimeDelta, const double dx, const double dy )
{
	osg::Matrixd m;
	
	m.setTrans( dx*m_panFactor, dy*m_panFactor, 0 );

	m_inverseMatrix = m_inverseMatrix * m;
	
	return true;
}