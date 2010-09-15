// SulConsoleInputHandler.h

#ifndef __SULCONSOLEINPUTHANDLER_H__
#define __SULCONSOLEINPUTHANDLER_H__

#ifndef DELTA_HACK

#include "SulExport.h"
#include "SulConsoleDisplay.h"
#include <osgGA/GUIEventHandler>

class SUL_EXPORT CSulConsoleInputHandler : public osgGA::GUIEventHandler
{
public:
										CSulConsoleInputHandler( CSulConsoleDisplay* pConsoleDisplay );

	virtual bool						handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa, osg::Object* pObject, osg::NodeVisitor* pNodeVisitor );

private:
	osg::ref_ptr<CSulConsoleDisplay>	m_rConsoleDisplay;
};

#endif

#endif // __SULCONSOLEINPUTHANDLER_H__