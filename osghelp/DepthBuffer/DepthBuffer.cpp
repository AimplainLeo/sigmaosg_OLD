// DepthBuffer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <osgViewer/Viewer>
#include <osg/ShapeDrawable>
#include <osgGA/GUIEventHandler>

class CTestDrawCallback : public osg::Camera::DrawCallback
{
public:
	void operator()( const osg::Camera& cam ) const
	{
		osg::Image* pTest = new osg::Image;
		pTest->readPixels( 256,256,1,1, GL_DEPTH_COMPONENT, GL_FLOAT ); 
		float f = (*(float*)pTest->data());

		osg::notify(osg::ALWAYS) << 
			"f = " << f <<
			std::endl;
	}
};

osg::Node* CreateScene()
{
	osg::Group* pGroup = new osg::Group;

	osg::Geode* pGeode = new osg::Geode();
    pGeode->addDrawable( new osg::ShapeDrawable( new osg::Box(osg::Vec3(0.0f,0.0f,0.0f),2.0f) ) );
	pGroup->addChild( pGeode );

	return pGroup;
}

int _tmain(int argc, _TCHAR* argv[])
{
	osg::ref_ptr<osgViewer::Viewer> rViewer;

    // construct the viewer
    rViewer = new osgViewer::Viewer;

    // make the viewer create a 512x512 window and position it at 32, 32
    rViewer->setUpViewInWindow( 32, 32, 512, 512 );

    // set the scene-graph data the viewer will render
    rViewer->setSceneData( CreateScene() );

	rViewer->getCamera()->setPostDrawCallback( new CTestDrawCallback );

    // execute main loop
    return rViewer->run();
}
