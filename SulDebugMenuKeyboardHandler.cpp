// SulDebugMenuKeyboardHandler.cpp

#include "stdafx.h"
#include "SulDebugMenuKeyboardHandler.h"

CSulDebugMenuKeyboardHandler::CSulDebugMenuKeyboardHandler( CSulDebugMenu* pMenu )
{
	m_rMenu = pMenu;
}

bool CSulDebugMenuKeyboardHandler::handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa, osg::Object* pObject, osg::NodeVisitor* pNodeVisitor )
{
	if ( ea.getEventType()==osgGA::GUIEventAdapter::KEYDOWN )
    {
        if ( ea.getKey()==osgGA::GUIEventAdapter::KEY_Up )
        {
			m_rMenu->prev();
            return true; // return true, event handled
        }

        if ( ea.getKey()==osgGA::GUIEventAdapter::KEY_Down )
        {
			m_rMenu->next();
            return true; // return true, event handled
        }

        if ( ea.getKey()==osgGA::GUIEventAdapter::KEY_Left )
        {
			m_rMenu->dec();
            return true; // return true, event handled
        }

        if ( ea.getKey()==osgGA::GUIEventAdapter::KEY_Right )
        {
			m_rMenu->inc();
            return true; // return true, event handled
        }
    }

    return false;
}