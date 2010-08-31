// SulGuiEventHandler.h

#ifndef __SULGUIEVENTHANDLER_H__
#define __SULGUIEVENTHANDLER_H__

#include "SulSigSlots.h"
#include <osgGA/guieventhandler>

class CSulGuiEventHandler : public osgGA::GUIEventHandler 
{
public:
	bool handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa, osg::Object* pObject, osg::NodeVisitor* pNodeVisitor );

	sigma::signal2<float,float>	signalMouseMove;
	sigma::signal2<float,float>	signalMouseDrag;
	sigma::signal2<int, int>	signalKeyDown;
	sigma::signal2<float,float> signalMousePush;
	sigma::signal2<float,float> signalMouseRelease;
	sigma::signal2<float,float> signalViewResize;

	sigma::signal2<float,float>	signalNativeDimensionsChanged;
};

#endif // __SULGUIEVENTHANDLER_H__