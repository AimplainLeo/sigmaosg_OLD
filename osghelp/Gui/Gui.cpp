// Gui.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <osgViewer/Viewer>
#include <osg/ShapeDrawable>
#include <osgGA/TrackballManipulator>
#include <SigmaUtil/SulGuiManager.h>
#include <SigmaUtil/SulShaderColor.h>
#include <SigmaUtil/SulGuiListBox.h>

osg::Node* createScene()
{
	osg::Group* pGroup = new osg::Group;

	osg::Geode* pGeode = new osg::Geode();

	// we create the simplest form of shapes in OpenSceneGraph
    pGeode->addDrawable( new osg::ShapeDrawable( new osg::Sphere(osg::Vec3(0.0f,0.0f,0.0f),0.5f) ) );
    pGeode->addDrawable( new osg::ShapeDrawable( new osg::Box(osg::Vec3(2.0f,0.0f,0.0f),2.0f) ) );
    pGeode->addDrawable( new osg::ShapeDrawable( new osg::Cone(osg::Vec3(4.0f,0.0f,0.0f),0.5f,3.0f) ) );
    pGeode->addDrawable( new osg::ShapeDrawable( new osg::Cylinder(osg::Vec3(6.0f,0.0f,0.5f),0.5f,3.0f) ) );
    pGeode->addDrawable( new osg::ShapeDrawable( new osg::Capsule(osg::Vec3(8.0f,0.0f,1.0f),0.5f,3.0f) ) );
	pGroup->addChild( pGeode );

	return pGroup;
}

osg::Group* createGui( osgViewer::Viewer* pViewer )
{
	osg::Group* pGroup = new osg::Group;

	// create gui
	CSulGuiManager* pGuiManager = new CSulGuiManager( pViewer );
	pGuiManager->load( "test.xml", 0, "default_theme.xml" );
	pGroup->addChild( pGuiManager );

	//pGuiManager->setEditMode( true );

	CSulGuiListBox* pListBox = pGuiManager->getListBox( "testme" );
	if ( pListBox )
	{
		pListBox->addTextItem( "one" );
		pListBox->addTextItem( "two" );
		pListBox->addTextItem( "three" );
		pListBox->addTextItem( "four" );
	}

	return pGroup;
}

int _tmain(int argc, _TCHAR* argv[])
{
    // construct the viewer
    osg::ref_ptr<osgViewer::Viewer> rViewer = new osgViewer::Viewer;

    // make the viewer create a 512x512 window and position it at 32, 32
    rViewer->setUpViewInWindow( 32, 32, 1024, 512 );

	osg::Group* pRoot = new osg::Group;

	pRoot->addChild( createScene() );

    // set the scene-graph data the viewer will render
    rViewer->setSceneData( pRoot );

	rViewer->setCameraManipulator( new osgGA::TrackballManipulator );

	pRoot->addChild( createGui(rViewer) );

    // execute main loop
    return rViewer->run();
}


