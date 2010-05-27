// ScreenShot.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <osgViewer/Viewer>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

class CScreenShotRequest : public osg::Referenced
{
public:
	CScreenShotRequest()
	{
		m_bTakeScreenShot = false;
	}

public:
	bool m_bTakeScreenShot;
};

class CKeyboardHandler : public osgGA::GUIEventHandler 
{
public:
	CKeyboardHandler( CScreenShotRequest* pScreenShotRequest )
	{
		m_rScreenShotRequest = pScreenShotRequest;
	}

	virtual bool handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa, 
		osg::Object* pObject, osg::NodeVisitor* pNodeVisitor )
	{
		if ( ea.getEventType()==osgGA::GUIEventAdapter::KEYDOWN )
		{
			if ( ea.getKey()==osgGA::GUIEventAdapter::KEY_Space )
			{
				// mark request for screenshot
				m_rScreenShotRequest->m_bTakeScreenShot = true;

 				return true; // return true, event handled
			}
		}

		return false;
	}

private:
	osg::ref_ptr<CScreenShotRequest> m_rScreenShotRequest;
};

class CCameraPostDrawCallback : public osg::Camera::DrawCallback
{
public:
	CCameraPostDrawCallback( CScreenShotRequest* pScreenShotRequest )
	{
		m_rScreenShotRequest = pScreenShotRequest;
	}

	void operator () ( const osg::Camera& cam ) const 
	{
		// has there been requested a screenshot?
		if ( m_rScreenShotRequest->m_bTakeScreenShot )
		{
			// save render buffer to file (your screenshot)
			osg::ref_ptr<osg::Image> rImage = new osg::Image;
			rImage->readPixels( 0, 0, 512, 512, GL_RGB, GL_UNSIGNED_BYTE );
			osgDB::writeImageFile( *rImage, "screenshot.jpg" );
	
			// done, reset request
			m_rScreenShotRequest->m_bTakeScreenShot = false;
		}
	}

private:
	osg::ref_ptr<CScreenShotRequest> m_rScreenShotRequest;
};

osg::Node* CreateScene()
{
	// load model
	osg::Node* pLoadedModel = osgDB::readNodeFile( "pyramid.obj" );
	return pLoadedModel;
}

int _tmain(int argc, _TCHAR* argv[])
{
	// construct the viewer
	osg::ref_ptr<osgViewer::Viewer> rViewer = new osgViewer::Viewer;

	// create an object to keep track of screenshot requests
	osg::ref_ptr<CScreenShotRequest> rScreenShotRequest = new CScreenShotRequest;

	rViewer->asView()->getCamera()->setPostDrawCallback( 
		new CCameraPostDrawCallback( rScreenShotRequest.get() ) );

	// make the viewer create a 512x512 window and position it at 32, 32
	rViewer->setUpViewInWindow( 32, 32, 512, 512 );

	// add the handler to the viewer
	rViewer->addEventHandler( new CKeyboardHandler( rScreenShotRequest.get() ) );

	// set the scene-graph data the viewer will render
	rViewer->setSceneData( CreateScene() );

	// execute main loop
	return rViewer->run();
}
