// SulDebugMenuKeyboardHandler.h

#ifndef __SULDEBUGMENUKEYBOARDHANDLER_H__
#define __SULDEBUGMENUKEYBOARDHANDLER_H__

#include "SulDebugMenu.h"
#include "SulExport.h"
#include <osgGA/GUIEventHandler>

class SUL_EXPORT CSulDebugMenuKeyboardHandler : public osgGA::GUIEventHandler 
{
public:
					CSulDebugMenuKeyboardHandler( CSulDebugMenu* pMenu );

    virtual bool	handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa, osg::Object* pObject, osg::NodeVisitor* pNodeVisitor );

private:
	osg::ref_ptr<CSulDebugMenu>	m_rMenu;
};

#endif // __SULDEBUGMENUKEYBOARDHANDLER_H__