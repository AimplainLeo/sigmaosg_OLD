// Gui.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <osgViewer/Viewer>
#include <osg/ShapeDrawable>
#include <osgGA/TrackballManipulator>
#include <SigmaUtil/SulGuiManager.h>
#include <SigmaUtil/SulShaderColor.h>
#include <SigmaUtil/SulGuiListBox.h>
#include <SigmaUtil/SulProfiler.h>
#include <SigmaUtil/SulInit.h>
#include <iostream>

extern osg::ref_ptr<CSulProfiler>	profiler;

osg::ref_ptr<CSulGuiManager> pGuiManager;

class CKeyboardHandler : public osgGA::GUIEventHandler 
{
public:
	virtual bool handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa, osg::Object* pObject, osg::NodeVisitor* pNodeVisitor )
	{
		osgViewer::Viewer* pViewer = dynamic_cast<osgViewer::Viewer*>(&aa);
        if ( !pViewer )
		{
			return false;
		}

		if ( ea.getEventType()==osgGA::GUIEventAdapter::FRAME )
		{
			profiler->frameUpdate();
		}		

		if ( ea.getEventType()==osgGA::GUIEventAdapter::KEYDOWN )
		{
			if ( ea.getKey()==osgGA::GUIEventAdapter::KEY_Space )
			{
				pGuiManager->show( !pGuiManager->isVisible() );
 				return true; // return true, event handled
			}

			if ( ea.getKey()==osgGA::GUIEventAdapter::KEY_Return )
			{
				CSulGuiCanvas* p = pGuiManager->getCanvas( "myCanvas" );
				float w = p->getW();
				float h = p->getH();
				std::cout << "canvas: w = " << w << ", h = " << h << std::endl;

				float viewW = pGuiManager->getViewW();
				float viewH = pGuiManager->getViewH();
				std::cout << "view: w = " << viewW << ", h = " << viewH << std::endl;
				
			}
		}

		return false;
	}
};

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
	pGuiManager = new CSulGuiManager( pViewer );
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

	sulInit( rViewer );

	pRoot->addChild( createGui(rViewer) );

	// add the handler to the viewer
	osg::ref_ptr<CKeyboardHandler> rKeyboardHandler = new CKeyboardHandler;
	rViewer->addEventHandler( rKeyboardHandler.get() );

    // execute main loop
    return rViewer->run();
}



