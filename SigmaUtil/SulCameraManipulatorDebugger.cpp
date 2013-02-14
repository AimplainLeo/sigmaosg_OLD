// SulCameraManipulatorDebugger.cpp

#include "stdafx.h"
#include "SulCameraManipulatorDebugger.h"

CSulCameraManipulatorDebugger::CSulCameraManipulatorDebugger() 
{
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
	// FIXME:
	return osg::Matrixd();
}

bool CSulCameraManipulatorDebugger::getHitPoint( osg::Vec3& hit )
{
	return true;
}

bool CSulCameraManipulatorDebugger::handleMousePush( const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa )
{
	// we need to find point that the mouse hit. This can be anything so we call a virtual method so the super class has
	// a chance to decide the rotation point

	osg::Vec3 hit;
	if ( getHitPoint( hit ) )
	{
		return true;
	}

	return true;
}

bool CSulCameraManipulatorDebugger::handle( const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa )
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

		case osgGA::GUIEventAdapter::PUSH:
			return handleMousePush( ea, aa );

    }
	
    return false;
}
