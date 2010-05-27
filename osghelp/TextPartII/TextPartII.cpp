// TextPartII.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <osgViewer/Viewer>
#include <osgText/Text>

osg::Node* CreateScene()
{
	osg::Group* pGroup = new osg::Group;

	osg::Geode* pGeodeText = new osg::Geode();
	osgText::Text* pText = new  osgText::Text;
	pText->setFont( "arial.ttf" );
	pText->setCharacterSize( 32.0f );
	pText->setAxisAlignment( osgText::Text::SCREEN );
	pText->setCharacterSizeMode( osgText::Text::SCREEN_COORDS );
	pText->setPosition( osg::Vec3( 0, 0, 0 ) );
    pText->setColor( osg::Vec4( 1.0f, 1.0f, 0.0f, 1.0f ) );
    pText->setText( "Hello World" );
    pGeodeText->addDrawable( pText );
	pGroup->addChild( pGeodeText );

	return pGroup;
}

int _tmain(int argc, _TCHAR* argv[])
{
    // construct the viewer
    osg::ref_ptr<osgViewer::Viewer> rViewer = new osgViewer::Viewer;

    // make the viewer create a 512x512 window and position it at 32, 32
    rViewer->setUpViewInWindow( 32, 32, 512, 512 );

    // set the scene-graph data the viewer will render
    rViewer->setSceneData( CreateScene() );

    // execute main loop
    return rViewer->run();
}