// SulGuiEventHandler.cpp

#include "stdafx.h"
#include "SulGuiEventHandler.h"
#include "SulGuiCanvas.h"
#include "SulNodePath.h"
#include "SulProfiler.h"
#include <osgViewer/Viewer>
#include <osgManipulator/Selection>
#include <iostream>

extern osg::ref_ptr<CSulProfiler>	profiler;

struct eventDataTest
{
	CSulGuiComp* pReceiver;
	CSulGuiCanvas* pCanvas;
	float mouse_local_x;
	float mouse_local_y;
};

typedef std::vector< eventDataTest > VEC_EVENTDATA;

bool sortByLayer( const eventDataTest& r1, const eventDataTest& r2 )
{
	return r1.pCanvas->getLayer() > r2.pCanvas->getLayer();
}

CSulGuiEventHandler::CSulGuiEventHandler()
{
	m_bPause = false;

	if ( profiler )
	{
		profiler->create( "CSulGuiEventHandler event MOVE" );
		profiler->create( "CSulGuiEventHandler event DRAG" );
	}
}

void CSulGuiEventHandler::pause( bool bPause )
{
	m_bPause = bPause;
}

bool CSulGuiEventHandler::handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa, osg::Object* pObject, osg::NodeVisitor* pNodeVisitor )
{
    if ( ea.getEventType() & osgGA::GUIEventAdapter::RESIZE )
    {
		float windowWidth = ea.getWindowWidth();
		float windowHeight = ea.getWindowHeight();
		signalViewResize( windowWidth, windowHeight );
	}

	if ( m_bPause )
		return false;

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
		if ( profiler.valid() )
			profiler->start( "CSulGuiEventHandler event MOVE" );
		
		float mouse_x = ea.getX();
		float mouse_y = ea.getYmax()-ea.getY();
		signalMouseMove( mouse_x, mouse_y );
		
		if ( profiler.valid() )
			profiler->end( "CSulGuiEventHandler event MOVE" );
    }

    if ( ea.getEventType() & osgGA::GUIEventAdapter::DRAG )
    {
		if ( profiler.valid() )
			profiler->start( "CSulGuiEventHandler event DRAG" );

		float mouse_x = ea.getX();
		float mouse_y = ea.getYmax()-ea.getY();
		signalMouseDrag( mouse_x, mouse_y );

		if ( profiler.valid() )
			profiler->end( "CSulGuiEventHandler event DRAG" );
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

		signalMouseRelease( mouse_x, mouse_y );

		VEC_EVENTDATA vecEventData;

		// send new event
		VEC_EVENT::iterator i = m_vecMouseRelease.begin();
		VEC_EVENT::iterator ie = m_vecMouseRelease.end();
		while ( i!=ie )
		{
			CSulGuiEvent* e = *i;
			CSulGuiCanvas* pCanvas = e->m_rGuiCanvas;	// test event against this canvas

			// calc local mouse coords
			osg::Vec2 mouse_local = pCanvas->getLocal( mouse_x, mouse_y );
			
			// HACK: this checks that the mouse position is also inside the parents canvas (need a better way to handle this)
			bool bCheck = true;			
			//Need to check if there is a parent or we will get a zero index crash 
			if (!pCanvas->getParents().empty())
			{
				CSulGuiCanvas* pParent = dynamic_cast<CSulGuiCanvas*>(pCanvas->getParent(0));
				if ( pParent )
				{
					bCheck = pParent->isInsideWorld( mouse_x, mouse_y );
				}
			}
			else
			{
				//TKG Unsure if bCheck should be set to false in this case
				//std::cout << "CSulGuiEventHandler::handle -> line 155 \"Hello\"" << std::endl;
			}
			

			// are we inside
			if ( bCheck && pCanvas->isInside( mouse_local ) )
			{
				// save event data
				eventDataTest ed;
				ed.pReceiver = e->m_rGuiCompReceiver;
				ed.pCanvas = pCanvas;
				ed.mouse_local_x = mouse_local.x();
				ed.mouse_local_y = mouse_local.y();
				vecEventData.push_back( ed );
			}
			
			++i;
		}

		// sort eventData after layer
		std::sort( vecEventData.begin(), vecEventData.end(), sortByLayer );

		// send events and check if handled
		VEC_EVENTDATA::iterator iEDs, iEDe;
		iEDs = vecEventData.begin();
		iEDe = vecEventData.end();
		while ( iEDs!=iEDe )
		{
			bool bHandled = iEDs->pReceiver->eventMouseRelease( 
				iEDs->pCanvas,
				iEDs->mouse_local_x,
				iEDs->mouse_local_y,
				mouse_x,
				mouse_y
			);
			
			if ( bHandled )
			{
				// echo who handled it
			//	osg::notify(osg::NOTICE) << "EVENT MOUSE RELEASE: " << iEDs->pCanvas->getCompName() << " (" << iEDs->pCanvas->getLayer() << ")" << std::endl;
				break;
			}
			

			++iEDs;
		}
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
