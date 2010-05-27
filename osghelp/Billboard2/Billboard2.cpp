// Billboard2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Laser.h"
#include <osgViewer/Viewer>
#include <osg/ShapeDrawable>

osg::Node* CreateScene()
{
	osg::Group* pGroup = new osg::Group;

	CLaser* pLaser = new CLaser;
	pLaser->Init();
	pLaser->SetPosition( osg::Vec3(5, 0, 0) );
	pLaser->SetTarget( osg::Vec3(-5, 0, 5) );
	pGroup->addChild( pLaser->getNode() );


	osg::Geode* pGeode = new osg::Geode;
	pGeode->addDrawable( new osg::ShapeDrawable( new osg::Sphere(osg::Vec3(5.0f,0.0f,0.0f),0.5f) ) );
	pGeode->addDrawable( new osg::ShapeDrawable( new osg::Sphere(osg::Vec3(-5.0f,0.0f,5.0f),0.5f) ) );
	pGroup->addChild( pGeode );


	return pGroup;
}


int _tmain(int argc, _TCHAR* argv[])
{
    // construct the viewer
    osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer;

    // make the viewer create a 512x512 window and position it at 32, 32
    viewer->setUpViewInWindow( 32, 32, 512, 512 );

    // set the scene-graph data the viewer will render
    viewer->setSceneData( CreateScene() );

    // execute main loop
    return viewer->run();
}


