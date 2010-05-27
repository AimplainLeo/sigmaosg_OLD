// SlaveCameras.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osg/ShapeDrawable>
#include <osg/Material>

osg::Node* CreateScene()
{
    osg::Geode* pGeode = new osg::Geode();

	// create 4 evenly spaced cubes
    pGeode->addDrawable( new osg::ShapeDrawable( new osg::Box(osg::Vec3(2.0f,2.0f,0.0f),2.0f) ) );
	pGeode->addDrawable( new osg::ShapeDrawable( new osg::Box(osg::Vec3(2.0f,-2.0f,0.0f),2.0f) ) );
	pGeode->addDrawable( new osg::ShapeDrawable( new osg::Box(osg::Vec3(-2.0f,2.0f,0.0f),2.0f) ) );
	pGeode->addDrawable( new osg::ShapeDrawable( new osg::Box(osg::Vec3(-2.0f,-2.0f,0.0f),2.0f) ) );

    return pGeode;
}

osg::Node* CreateScene2()
{
	// create big cube
    osg::Geode* pGeode = new osg::Geode();
    pGeode->addDrawable( new osg::ShapeDrawable( new osg::Box(osg::Vec3(0.0f,0.0f,0.0f),4.0f) ) );

	// make it red so we can easily identify the cube
	osg::Material* pMaterial = new osg::Material;
	pMaterial->setDiffuse( osg::Material::FRONT, osg::Vec4(1,0,0,1) );
	pGeode->getOrCreateStateSet()->setAttribute( pMaterial, osg::StateAttribute::OVERRIDE );

    return pGeode;
}

osg::Node* CreateScene3()
{
	// create big cube
    osg::Geode* pGeode = new osg::Geode();
    pGeode->addDrawable( new osg::ShapeDrawable( new osg::Box(osg::Vec3(0.0f,0.0f,0.0f),2.0f) ) );

	// make it red so we can easily identify the cube
	osg::Material* pMaterial = new osg::Material;
	pMaterial->setDiffuse( osg::Material::FRONT, osg::Vec4(0,1,0,1) );
	pGeode->getOrCreateStateSet()->setAttribute( pMaterial, osg::StateAttribute::OVERRIDE );

    return pGeode;
}

int _tmain(int argc, _TCHAR* argv[])
{
    // construct the viewer
    osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer;

    // make the viewer create a 512x512 window and position it at 32, 32
    viewer->setUpViewInWindow( 32, 32, 512, 512 );

    // set the scene-graph data the viewer will render
    viewer->setSceneData( CreateScene() );

	// slave camera
	osg::ref_ptr<osg::Camera> rCam1 = new osg::Camera;
	rCam1->setGraphicsContext( viewer->getCamera()->getGraphicsContext() );
	rCam1->setViewport( 0, 0, 512, 512 );
	rCam1->setInheritanceMask( rCam1->getInheritanceMask() & (~osg::CullSettings::CLEAR_COLOR) );
	rCam1->setClearMask( GL_DEPTH_BUFFER_BIT );
	rCam1->addChild( CreateScene2() );
	viewer->addSlave( rCam1.get(), false );

	// slave camera
	osg::ref_ptr<osg::Camera> rCam2 = new osg::Camera;
	rCam2->setGraphicsContext( viewer->getCamera()->getGraphicsContext() );
	rCam2->setViewport( 0, 0, 512, 512 );
	rCam2->setInheritanceMask( rCam2->getInheritanceMask() & (~osg::CullSettings::CLEAR_COLOR) );
	rCam2->setClearMask( GL_DEPTH_BUFFER_BIT );
	rCam2->addChild( CreateScene3() );
	viewer->addSlave( rCam2.get(), false );

//	viewer->removeSlave(0);
//	viewer->removeSlave(0);

//	viewer->addSlave(rCam1.get(), false);
//	viewer->addSlave(rCam2.get(), false);
/*
	viewer->setThreadingModel(osgViewer::Viewer::SingleThreaded);
	viewer->addEventHandler( new osgViewer::ThreadingHandler );
	viewer->addEventHandler( new osgViewer::StatsHandler );
*/
    // execute main loop
    return viewer->run();
}

