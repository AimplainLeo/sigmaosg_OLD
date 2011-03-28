// SulInit.cpp

#include "stdafx.h"
#include "SulInit.h"
#include "SulProfiler.h"
#include <osg/group>

osg::ref_ptr<CSulProfiler>	CSulInit::profiler = 0;

class CSulInitPostCallback : public osg::Camera::DrawCallback
{
public:
	void operator()( const osg::Camera& cam ) const
	{
		CSulInit::profiler->end( "total frame" );
		//CSulInit::profiler->dump();
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
	profiler = new CSulProfiler;

	if ( profiler.valid() )
	{
		profiler->create( "total frame" );

		pCam->setPostDrawCallback( new CSulInitPostCallback );
	}
}


