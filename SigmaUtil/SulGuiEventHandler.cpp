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
    }

    if ( ea.getEventType() & osgGA::GUIEventAdapter::DRAG )
    {
		float mouse_x = ea.getX();
		float mouse_y = ea.getYmax()-ea.getY();

		VEC_GUICOMP::iterator i;
		i = m_eventsMouseDrag.begin();
		while ( i!=m_eventsMouseDrag.end() )
		{
			CSulGuiComp* pComp = (*i);

			osg::NodePath pathToRoot;
			osgManipulator::computeNodePathToRoot( *pComp, pathToRoot );
			osg::Matrix m = osg::computeLocalToWorld( pathToRoot );
			float mouse_local_x = mouse_x-m.getTrans().x();
			float mouse_local_y = mouse_y-m.getTrans().y();

			pComp->eventMouseDrag( mouse_local_x, mouse_local_y, mouse_x, mouse_y );
			++i;
		}
    }

    if ( ea.getEventType() & osgGA::GUIEventAdapter::PUSH )
    {
		float mouse_x = ea.getX();
		float mouse_y = ea.getYmax()-ea.getY();

		VEC_GUICOMP::iterator i;
		i = m_eventsMousePushed.begin();
		while ( i!=m_eventsMousePushed.end() )
		{
			CSulGuiComp* pComp = (*i);

			osg::NodePath pathToRoot;
			osgManipulator::computeNodePathToRoot( *pComp, pathToRoot );
			osg::Matrix m = osg::computeLocalToWorld( pathToRoot );
			float mouse_local_x = mouse_x-m.getTrans().x();
			float mouse_local_y = mouse_y-m.getTrans().y();

			pComp->eventMousePushed( mouse_local_x, mouse_local_y, mouse_x, mouse_y );
			++i;
		}
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
    }

    return false;
}

void CSulGuiEventHandler::addEvent( CSulGuiComp* pComp, sigma::uint32 eventType )
{
	switch ( eventType )
	{
		case EVENT_MOUSE_MOVE:		
			if ( std::find( m_eventsMouseMove.begin(), m_eventsMouseMove.end(), pComp )==m_eventsMouseMove.end() )
				m_eventsMouseMove.push_back( pComp );		
			break;

		case EVENT_MOUSE_DRAG:		
			if ( std::find( m_eventsMouseDrag.begin(), m_eventsMouseDrag.end(), pComp )==m_eventsMouseDrag.end() )
				m_eventsMouseDrag.push_back( pComp );		
			break;

		case EVENT_MOUSE_PUSHED:	
			if ( std::find( m_eventsMousePushed.begin(), m_eventsMousePushed.end(), pComp )==m_eventsMousePushed.end() )
				m_eventsMousePushed.push_back( pComp );		
			break;

		case EVENT_MOUSE_RELEASE:	
			if ( std::find( m_eventsMouseRelease.begin(), m_eventsMouseRelease.end(), pComp )==m_eventsMouseRelease.end() )
				m_eventsMouseRelease.push_back( pComp );	
			break;

		case EVENT_KEYDOWN:			
			if ( std::find( m_eventsKeyDown.begin(), m_eventsKeyDown.end(), pComp )==m_eventsKeyDown.end() )
				m_eventsKeyDown.push_back( pComp );			
			break;
	}
}
