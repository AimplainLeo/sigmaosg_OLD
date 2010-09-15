// SulConsoleInputHandler.cpp

#include "stdafx.h"
#include "SulConsoleInputHandler.h"

#ifndef DELTA_HACK

CSulConsoleInputHandler::CSulConsoleInputHandler( CSulConsoleDisplay* pConsoleDisplay )
{
	m_rConsoleDisplay = pConsoleDisplay;
}

bool CSulConsoleInputHandler::handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa, osg::Object* pObject, osg::NodeVisitor* pNodeVisitor )
{
    if ( ea.getEventType()==osgGA::GUIEventAdapter::KEYDOWN )
    {
		int key = ea.getKey();
		int mod = ea.getModKeyMask();

		if ( !m_rConsoleDisplay->IsVisible() && key!=m_rConsoleDisplay->GetActivateKey() )
		{
			return false;
		}

		if ( key!=-1 )				// -1 if inappropriate for this GUIEventAdapter
		{
			m_rConsoleDisplay->Input( key, mod );
			return true;
		}
    }

	return false;
}

#endif