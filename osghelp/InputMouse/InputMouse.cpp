// InputMouse.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <osgViewer/Viewer>
#include <osgGA/GUIEventHandler>
#include <osg/ShapeDrawable>
#include <osgText/Text>

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

		// change background color to red when mouse pressed
        if ( ea.getEventType()==osgGA::GUIEventAdapter::PUSH )
        {
            osgViewer::ViewerBase::Cameras cams;
            pViewer->getCameras( cams );
            cams[0]->setClearColor( osg::Vec4(1, 0, 0, 1 ) );
            return true; // return true, event handled
        }

		// change background color to green when mouse released
        if ( ea.getEventType()==osgGA::GUIEventAdapter::RELEASE )
        {
            osgViewer::ViewerBase::Cameras cams;
            pViewer->getCameras( cams );
            cams[0]->setClearColor( osg::Vec4(0, 1, 0, 1 ) );
            return false; // return true, event handled
        }

        return false;
    }
};

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

/*
	osg::Geode* pGeode = new osg::Geode();
	pGeode->addDrawable( new osg::ShapeDrawable( new osg::Box(osg::Vec3(2.0f,0.0f,0.0f),2.0f) ) );
	pGroup->addChild( pGeode );
	*/

	return pGroup;
}

int _tmain(int argc, _TCHAR* argv[])
{
    // construct the viewer
    osg::ref_ptr<osgViewer::Viewer> rViewer = new osgViewer::Viewer;

    // make the viewer create a 512x512 window and position it at 32, 32
    rViewer->setUpViewInWindow( 32, 32, 512, 512 );

    // add the handler to the viewer
    osg::ref_ptr<CKeyboardHandler> rKeyboardHandler = new CKeyboardHandler;
    rViewer->addEventHandler( rKeyboardHandler.get() );

    // set the scene-graph data the viewer will render
    rViewer->setSceneData( CreateScene() );

    // execute main loop
    return rViewer->run();
}
