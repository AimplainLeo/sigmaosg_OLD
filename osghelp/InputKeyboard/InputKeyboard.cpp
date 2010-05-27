// InputKeyboard.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <osgViewer/Viewer>
#include <osgDB/ReadFile>
#include <osgGA/GUIEventHandler>

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

		if ( ea.getEventType()==osgGA::GUIEventAdapter::KEYDOWN )
		{
			if ( ea.getKey()==osgGA::GUIEventAdapter::KEY_Space )
			{
				osgViewer::ViewerBase::Cameras cams;
				pViewer->getCameras( cams );

				float r = ((float)rand()/(float)RAND_MAX);
				float g = ((float)rand()/(float)RAND_MAX);
				float b = ((float)rand()/(float)RAND_MAX);
				cams[0]->setClearColor( osg::Vec4(r, g, b, 1.0f ) );

 				return true; // return true, event handled
			}
		}

		return false;
	}
};

osg::Node* CreateScene()
{
	// load model
	osg::Node* pLoadedModel = osgDB::readNodeFile( "cube.obj" );
	return pLoadedModel;
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
