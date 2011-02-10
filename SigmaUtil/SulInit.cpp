// SulInit.cpp

#include "stdafx.h"
#include "SulInit.h"
#include "SulProfiler.h"
#include <osg/group>

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

class CSulInitPostCallback : public osg::Camera::DrawCallback
{
public:
	void operator()( const osg::Camera& cam ) const
	{
		profiler->end( "total frame" );
		profiler->dump();
		profiler->frameUpdate();
		profiler->start( "total frame" );
	}
};

void sulInit( osgViewer::Viewer* pViewer )
{
	profiler = new CSulProfiler;

	profiler->create( "total frame" );


//	pViewer->getScene()->getSceneData()->asGroup()->addChild( profiler->getChart() );

	pViewer->getCamera()->setPostDrawCallback( new CSulInitPostCallback );

//	pViewer->addEventHandler( new CSigmaHandler );
}