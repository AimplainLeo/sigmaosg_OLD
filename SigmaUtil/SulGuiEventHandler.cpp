// SulGuiEventHandler.cpp

#include "stdafx.h"
#include "SulGuiEventHandler.h"
#include <osgViewer/Viewer>
#include <osgManipulator/Selection>

bool CSulGuiEventHandler::handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa, osg::Object* pObject, osg::NodeVisitor* pNodeVisitor )
{
    osgViewer::Viewer* pViewer = dynamic_cast<osgViewer::Viewer*>(&aa);
    if ( !pViewer )
    {
        return false;
    }

    if ( ea.getEventType() & osgGA::GUIEventAdapter::KEYDOWN )
    {
		int key = ea.getKey();
		int mod = ea.getModKeyMask();

		if ( key!=-1 )				// -1 if inappropriate for this GUIEventAdapter
		{
			VEC_GUICOMP::iterator i;
			i = m_eventsKeyDown.begin();
			while ( i!=m_eventsKeyDown.end() )
			{
				CSulGuiComp* pComp = (*i);
				pComp->eventKeyDown( key, mod );
				++i;
			}

			return true;
		}
    }

    if ( ea.getEventType() & osgGA::GUIEventAdapter::MOVE )
    {
		float mouse_x = ea.getX();
		float mouse_y = ea.getYmax()-ea.getY();

		VEC_GUICOMP::iterator i;
		i = m_eventsMouseMove.begin();
		while ( i!=m_eventsMouseMove.end() )
		{
			CSulGuiComp* pComp = (*i);

			osg::NodePath pathToRoot;
			osgManipulator::computeNodePathToRoot( *pComp, pathToRoot );
			osg::Matrix m = osg::computeLocalToWorld( pathToRoot );
			float mouse_local_x = mouse_x-m.getTrans().x();
			float mouse_local_y = mouse_y-m.getTrans().y();

			pComp->eventMouseMove( mouse_local_x, mouse_local_y, mouse_x, mouse_y );
			++i;
		}

//        return true; // return true, event handled
    }

    if ( ea.getEventType() & osgGA::GUIEventAdapter::PUSH )
    {
/*
		float mouse_x = ea.getX();
		float mouse_y = ea.getYmax()-ea.getY();

        osgViewer::ViewerBase::Cameras cams;
        pViewer->getCameras( cams );
        cams[0]->setClearColor( osg::Vec4(1, 0, 0, 1 ) );
*/
   //     return true; // return true, event handled
    }

    if ( ea.getEventType() & osgGA::GUIEventAdapter::RELEASE )
    {
		float mouse_x = ea.getX();
		float mouse_y = ea.getYmax()-ea.getY();

		VEC_GUICOMP::iterator i;
		i = m_eventsMouseRelease.begin();
		while ( i!=m_eventsMouseRelease.end() )
		{
			CSulGuiComp* pComp = (*i);

			osg::NodePath pathToRoot;
			osgManipulator::computeNodePathToRoot( *pComp, pathToRoot );
			osg::Matrix m = osg::computeLocalToWorld( pathToRoot );
			float mouse_local_x = mouse_x-m.getTrans().x();
			float mouse_local_y = mouse_y-m.getTrans().y();

			pComp->eventMouseRelease( mouse_local_x, mouse_local_y, mouse_x, mouse_y );
			++i;
		}

//        return false; // return true, event handled
    }

    return false;
}

void CSulGuiEventHandler::addComp( CSulGuiComp* pComp )
{
		Sigma::uint32 events = pComp->getEvents();
		if ( events & CSulGuiComp::EVENT_MOUSE_MOVE )		m_eventsMouseMove.push_back( pComp );
		if ( events & CSulGuiComp::EVENT_MOUSE_PUSHED )		m_eventsMousePushed.push_back( pComp );
		if ( events & CSulGuiComp::EVENT_MOUSE_RELEASE )	m_eventsMouseRelease.push_back( pComp );
		if ( events & CSulGuiComp::EVENT_KEYDOWN )			m_eventsKeyDown.push_back( pComp );
}
