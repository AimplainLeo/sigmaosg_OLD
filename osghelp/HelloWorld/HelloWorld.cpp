// HelloWorld.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <osgViewer/Viewer>
#include <osgText/Text>

osg::Node* CreateScene()
{
	// create a geometry node and turn lighting off for it
	osg::Geode* geode = new osg::Geode();
	osg::StateSet* stateset = geode->getOrCreateStateSet();
	stateset->setMode( GL_LIGHTING,osg::StateAttribute::OFF );

	// create an ortho projection so our text is screen aligned
	osg::Camera* camera = new osg::Camera;
	camera->setProjectionMatrix( osg::Matrix::ortho2D(0,512,0,512) );
	camera->setReferenceFrame( osg::Transform::ABSOLUTE_RF );
	camera->setViewMatrix( osg::Matrix::identity() );
	camera->addChild( geode );

	// create text and add it to our geometry node
	osgText::Text* text = new  osgText::Text;
	geode->addDrawable( text );

	// set some text data
	text->setPosition( osg::Vec3( 16.f, 16.0f, 0.0f ) );
	text->setColor( osg::Vec4( 1.0f, 1.0f, 0.0f, 1.0f ) );
	text->setText( "Hello World" );

	return camera;
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

