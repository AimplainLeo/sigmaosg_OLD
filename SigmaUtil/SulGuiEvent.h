// SulGuiEvent.h

#ifndef __SULGUIEVENT_H__
#define __SULGUIEVENT_H__

#include "SulGuiCanvas.h"

// FIXME: rename to CSulGuiEventData
class CSulGuiEvent : public osg::Referenced
{
public:
	CSulGuiEvent( CSulGuiComp* pCompReceiver, CSulGuiCanvas* pCanvas )
	{
		m_rGuiCompReceiver = pCompReceiver;
		m_rGuiCanvas = pCanvas;
	}

public:
	osg::ref_ptr<CSulGuiComp>	m_rGuiCompReceiver;	// object to receive event
	osg::ref_ptr<CSulGuiCanvas>	m_rGuiCanvas;		// object to test event on
};

#endif // __SULGUIEVENT_H__