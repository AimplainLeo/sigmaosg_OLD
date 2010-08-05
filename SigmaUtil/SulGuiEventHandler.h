// SulGuiEventHandler.h

#ifndef __SULGUIEVENTHANDLER_H__
#define __SULGUIEVENTHANDLER_H__

#include "SulGuiComp.h"

class CSulGuiEventHandler : public osgGA::GUIEventHandler 
{
public:
	enum EVENT
	{
		EVENT_MOUSE_MOVE,
		EVENT_MOUSE_PUSHED,
		EVENT_MOUSE_RELEASE,
		EVENT_KEYDOWN
	};

public:
	bool handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa, osg::Object* pObject, osg::NodeVisitor* pNodeVisitor );

	void addEvent( CSulGuiComp* pComp, Sigma::uint32 eventType );

private:
	VEC_GUICOMP		m_eventsMouseMove;
	VEC_GUICOMP		m_eventsMousePushed;
	VEC_GUICOMP		m_eventsMouseRelease;
	VEC_GUICOMP		m_eventsKeyDown;
};

#endif // __SULGUIEVENTHANDLER_H__