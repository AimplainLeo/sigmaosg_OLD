// SulInit.cpp

#include "stdafx.h"
#include "SulInit.h"
#include "SulProfiler.h"

osg::ref_ptr<CSulProfiler>	profiler;

class CSigmaHandler : public osgGA::GUIEventHandler 
{
public:
	virtual bool handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa, osg::Object* pObject, osg::NodeVisitor* pNodeVisitor )
	{
		if ( ea.getEventType()==osgGA::GUIEventAdapter::FRAME )
		{
			profiler->dump();

			profiler->frameUpdate();
		}		

		return false;
	}
};



void sulInit( osgViewer::Viewer* pViewer )
{
	profiler = new CSulProfiler;

	pViewer->addEventHandler( new CSigmaHandler );
}