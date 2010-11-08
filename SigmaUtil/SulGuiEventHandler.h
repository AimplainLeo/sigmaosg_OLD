// SulGuiEventHandler.h

#ifndef __SULGUIEVENTHANDLER_H__
#define __SULGUIEVENTHANDLER_H__

#include "SulGuiComp.h"
#include "SulGuiEvent.h"
#include "SulSigSlots.h"
#include <osgGA/guieventhandler>

class CSulGuiEventHandler : public osgGA::GUIEventHandler 
{
public:
	enum EEVENT
	{
		EVENT_MOUSEPUSH,
		EVENT_MOUSERELEASE
	};

	typedef std::vector< osg::ref_ptr<CSulGuiEvent> > VEC_EVENT;	

public:
	bool handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa, osg::Object* pObject, osg::NodeVisitor* pNodeVisitor );

	void wantEvent( CSulGuiComp* pCompReceiver, CSulGuiCanvas* pCanvas, EEVENT e );

	sigma::signal2<float,float>	signalMouseMove;
	sigma::signal2<float,float>	signalMouseDrag;
	sigma::signal2<int, int>	signalKeyDown;
	sigma::signal2<float,float> signalMousePush;
	sigma::signal2<float,float> signalMouseRelease;
	sigma::signal2<float,float> signalViewResize;

	sigma::signal2<float,float>	signalNativeDimensionsChanged;

private:
	VEC_EVENT	m_vecMousePush;
	VEC_EVENT	m_vecMouseRelease;
};

#endif // __SULGUIEVENTHANDLER_H__