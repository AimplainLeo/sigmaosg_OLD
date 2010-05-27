// Console.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <osgDB/ReadFile>
#include <osgViewer/Viewer>
#include <SigmaUtil/SulConsoleDisplay.h>

class CMyTestClass : public osg::Referenced
{
public:
	CMyTestClass( CSulConsoleDisplay* pConsoleDisplay, osg::Group* pGroupScene, osgViewer::Viewer* pViewer )
	{
		m_rGroupScene = pGroupScene;
		m_rViewer = pViewer;

		// use a functor to get call back for commands posted in console
		CSulConsoleFunctor<CMyTestClass>* pConsoleFunctor = new CSulConsoleFunctor<CMyTestClass>(this, &CMyTestClass::Command);
		pConsoleDisplay->WantCommand( pConsoleFunctor );
	}

	void Command( const CSulString& sText )
	{
		osg::Node* pLoadedModel = osgDB::readNodeFile( sText );
		m_rGroupScene->addChild( pLoadedModel );
		m_rViewer->getCameraManipulator()->computeHomePosition();
		m_rViewer->getCameraManipulator()->home( 1.0f );
	}

private:
	osg::ref_ptr<osg::Group> m_rGroupScene;
	osg::ref_ptr<osgViewer::Viewer> m_rViewer;
};

int _tmain(int argc, _TCHAR* argv[])
{
    // construct the viewer
    osg::ref_ptr<osgViewer::Viewer> rViewer = new osgViewer::Viewer;

    // make the viewer create a 512x512 window and position it at 32, 32
    rViewer->setUpViewInWindow( 32, 32, 512, 512 );

	// create a console
	osg::ref_ptr<CSulConsoleDisplay> rConsoleDisplay = new CSulConsoleDisplay( rViewer.get() );
	rConsoleDisplay->Init();
	
	osg::Group* pGroup = new osg::Group;
	pGroup->addChild( rConsoleDisplay->GetNode() );

	// create object to receive console commands
	osg::ref_ptr<CMyTestClass> rTest = new CMyTestClass( rConsoleDisplay.get(), pGroup, rViewer.get() );

    // set the scene-graph data the viewer will render
    rViewer->setSceneData( pGroup );

    // execute main loop
    return rViewer->run();
}
