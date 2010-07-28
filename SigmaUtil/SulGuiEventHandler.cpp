// SulGuiEventHandler.cpp

#include "stdafx.h"
#include "SulGuiEventHandler.h"
#include <osgViewer/Viewer>

bool CSulGuiEventHandler::handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa, osg::Object* pObject, osg::NodeVisitor* pNodeVisitor )
{
    osgViewer::Viewer* pViewer = dynamic_cast<osgViewer::Viewer*>(&aa);
    if ( !pViewer )
    {
        return false;
    }

    if ( ea.getEventType()==osgGA::GUIEventAdapter::MOVE )
    {
		float mouse_x = ea.getX();
		float mouse_y = ea.getYmax()-ea.getY();

		VEC_GUICOMP::iterator i;
		i = m_eventsMouseMove.begin();
		while ( i!=m_eventsMouseMove.end() )
		{
			CSulGuiComp* pComp = (*i);

			//mouse_x = pComp->getX();
			//mouse_y = pComp->getY();

			pComp->eventMouseMove( mouse_x, mouse_y );
			++i;
		}

        return true; // return true, event handled
    }

	// change background color to red when mouse pressed
    if ( ea.getEventType()==osgGA::GUIEventAdapter::PUSH )
    {
		float mouse_x = ea.getX();
		float mouse_y = ea.getYmax()-ea.getY();

        osgViewer::ViewerBase::Cameras cams;
        pViewer->getCameras( cams );
        cams[0]->setClearColor( osg::Vec4(1, 0, 0, 1 ) );
        return true; // return true, event handled
    }

	// change background color to green when mouse released
    if ( ea.getEventType()==osgGA::GUIEventAdapter::RELEASE )
    {
        osgViewer::ViewerBase::Cameras cams;
        pViewer->getCameras( cams );
        cams[0]->setClearColor( osg::Vec4(0, 1, 0, 1 ) );
        return false; // return true, event handled
    }

    return false;
}

void CSulGuiEventHandler::addComp( CSulGuiComp* pComp )
{
		Sigma::uint32 events = pComp->getEvents();
		if ( events & CSulGuiComp::EVENT_MOUSE_MOVE )		m_eventsMouseMove.push_back( pComp );
		if ( events & CSulGuiComp::EVENT_MOUSE_PUSHED )		m_eventsMousePushed.push_back( pComp );
		if ( events & CSulGuiComp::EVENT_MOUSE_RELEASED )	m_eventsMouseReleased.push_back( pComp );
}
