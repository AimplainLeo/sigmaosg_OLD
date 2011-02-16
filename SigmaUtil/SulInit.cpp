// SulInit.cpp

#include "stdafx.h"
#include "SulInit.h"
#include "SulProfiler.h"
#include <osg/group>

osg::ref_ptr<CSulProfiler>	CSulInit::profiler = 0;

class CSigmaHandler : public osgGA::GUIEventHandler 
{
public:
	virtual bool handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa, osg::Object* pObject, osg::NodeVisitor* pNodeVisitor )
	{
		if ( ea.getEventType()==osgGA::GUIEventAdapter::FRAME )
		{
			CSulInit::profiler->dump();

			CSulInit::profiler->frameUpdate();
		}		

		return false;
	}
};

class CSulInitPostCallback : public osg::Camera::DrawCallback
{
public:
	void operator()( const osg::Camera& cam ) const
	{
		CSulInit::profiler->end( "total frame" );
		CSulInit::profiler->dump();
		CSulInit::profiler->frameUpdate();
		CSulInit::profiler->start( "total frame" );
	}
};

CSulProfiler* CSulInit::Instance()
{
	return profiler;
}

void CSulInit::init( osg::Camera* pCam )
{
	//profiler = new CSulProfiler;

	if ( profiler.valid() )
	{
		profiler->create( "total frame" );

	//	pViewer->getScene()->getSceneData()->asGroup()->addChild( profiler->getChart() );

		pCam->setPostDrawCallback( new CSulInitPostCallback );

	//	pViewer->addEventHandler( new CSigmaHandler );
	}
}


