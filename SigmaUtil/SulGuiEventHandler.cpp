// SulGuiEventHandler.cpp

#include "stdafx.h"
#include "SulGuiEventHandler.h"
#include "SulGuiCanvas.h"
#include "SulNodePath.h"
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

		// deprecated, not well suited for gui
		signalMousePush( mouse_x, mouse_y );

		// send new event
		VEC_EVENT::iterator i = m_vecMousePush.begin();
		VEC_EVENT::iterator ie = m_vecMousePush.end();
		while ( i!=ie )
		{
			CSulGuiEvent* e = *i;
			CSulGuiCanvas* pCanvas = e->m_rGuiCanvas;	// test event against this canvas

			// calc local mouse coords
			osg::NodePath path;
			sulNodePath( *pCanvas, path, 0, true );
			osg::Matrix m = osg::computeLocalToWorld( path );
			float mouse_local_x = mouse_x-m.getTrans().x();
			float mouse_local_y = mouse_y-m.getTrans().y();
			
			// are we inside
			if ( pCanvas->isInside( mouse_local_x, mouse_local_y ) )
			{
				e->m_rGuiCompReceiver->eventMousePush( pCanvas, mouse_local_x, mouse_local_y, mouse_x, mouse_y );
			}
			
			++i;
		}

    }

    if ( ea.getEventType() & osgGA::GUIEventAdapter::RELEASE )
    {
		float mouse_x = ea.getX();
		float mouse_y = ea.getYmax()-ea.getY();

		// deprecated, not well suited for gui
		signalMouseRelease( mouse_x, mouse_y );

		// send new event
		VEC_EVENT::iterator i = m_vecMouseRelease.begin();
		VEC_EVENT::iterator ie = m_vecMouseRelease.end();
		while ( i!=ie )
		{
			CSulGuiEvent* e = *i;
			CSulGuiCanvas* pCanvas = e->m_rGuiCanvas;	// test event against this canvas

			// calc local mouse coords
			osg::NodePath path;
			sulNodePath( *pCanvas, path, 0, true );
			osg::Matrix m = osg::computeLocalToWorld( path );
			float mouse_local_x = mouse_x-m.getTrans().x();
			float mouse_local_y = mouse_y-m.getTrans().y();
			
			// are we inside
			if ( pCanvas->isInside( mouse_local_x, mouse_local_y ) )
			{
				e->m_rGuiCompReceiver->eventMouseRelease( pCanvas, mouse_local_x, mouse_local_y, mouse_x, mouse_y );
			}
			
			++i;
		}
    }

    if ( ea.getEventType() & osgGA::GUIEventAdapter::RESIZE )
    {
		float windowWidth = ea.getWindowWidth();
		float windowHeight = ea.getWindowHeight();
		signalViewResize( windowWidth, windowHeight );
	}

    return false;
}


void CSulGuiEventHandler::wantEvent( CSulGuiComp* pCompReceiver, CSulGuiCanvas* pCanvas, EEVENT e )
{
	switch ( e )
	{
		case EVENT_MOUSEPUSH:
			m_vecMousePush.push_back( new CSulGuiEvent(pCompReceiver,pCanvas) );
			break;

		case EVENT_MOUSERELEASE:
			m_vecMouseRelease.push_back( new CSulGuiEvent(pCompReceiver,pCanvas) );
			break;
	}
}
