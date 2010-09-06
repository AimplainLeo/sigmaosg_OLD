// SulGuiEventHandler.cpp

#include "stdafx.h"
#include "SulGuiEventHandler.h"
#include <osgViewer/Viewer>
#include <osgManipulator/Selection>

bool CSulGuiEventHandler::handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa, osg::Object* pObject, osg::NodeVisitor* pNodeVisitor )
{
	/*
    osgViewer::Viewer* pViewer = dynamic_cast<osgViewer::Viewer*>(&aa);
    if ( !pViewer )
    {
        return false;
    }
	*/

    if ( ea.getEventType() & osgGA::GUIEventAdapter::KEYDOWN )
    {
/*
		float w = pViewer->getCamera()->getViewport()->width();
		float h = pViewer->getCamera()->getViewport()->height();
		osg::notify(osg::NOTICE) << "w = " << w << "   h = " << h << std::endl;
*/

		int key = ea.getKey();
		int mod = ea.getModKeyMask();

		if ( key!=-1 )				// -1 if inappropriate for this GUIEventAdapter
		{
			signalKeyDown( key, mod );
			return true;
		}
    }

    if ( ea.getEventType() & osgGA::GUIEventAdapter::MOVE )
    {
		float mouse_x = ea.getX();
		float mouse_y = ea.getYmax()-ea.getY();
		signalMouseMove( mouse_x, mouse_y );
    }

    if ( ea.getEventType() & osgGA::GUIEventAdapter::DRAG )
    {
		float mouse_x = ea.getX();
		float mouse_y = ea.getYmax()-ea.getY();
		signalMouseDrag( mouse_x, mouse_y );
    }

    if ( ea.getEventType() & osgGA::GUIEventAdapter::PUSH )
    {
		float mouse_x = ea.getX();
		float mouse_y = ea.getYmax()-ea.getY();
		signalMousePush( mouse_x, mouse_y );
    }

    if ( ea.getEventType() & osgGA::GUIEventAdapter::RELEASE )
    {
		float mouse_x = ea.getX();
		float mouse_y = ea.getYmax()-ea.getY();
		signalMouseRelease( mouse_x, mouse_y );
    }

    if ( ea.getEventType() & osgGA::GUIEventAdapter::RESIZE )
    {
		float windowWidth = ea.getWindowWidth();
		float windowHeight = ea.getWindowHeight();
		signalViewResize( windowWidth, windowHeight );
	}

    return false;
}

