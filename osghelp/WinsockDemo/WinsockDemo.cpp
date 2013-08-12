// WinsockDemo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "networkserver.h"
#include "networkclient.h"
#include <osgViewer/Viewer>
#include <osgDB/ReadFile>
#include <osgDB/FileUtils>
#include <osg/matrixtransform>
#include <WinSock2.h>

osg::Node* createScene()
{
	osg::Group* group = new osg::Group;

	osg::MatrixTransform* mt = new osg::MatrixTransform;
	group->addChild( mt );
	mt->addChild( osgDB::readNodeFile( osgDB::findDataFile( "p21umr91km-Abrams_Tank/3ds file.3Ds" ) ) );
	osg::Matrix m;
	m.makeScale( osg::Vec3(0.01f, 0.01f, 0.01f ) );
	m.setTrans( osg::Vec3(0,0,4.0f) );
	mt->setMatrix( m );

	// terrain
	group->addChild( osgDB::readNodeFile( osgDB::findDataFile( "terrain_simple.ive" ) ) );

	// sun
	osg::Light* light = new osg::Light;
	light->setLightNum( 0 );						
	light->setDiffuse( osg::Vec4( 0.8f, 1.0f, 0.8f, 1.0f) );
	light->setPosition( osg::Vec4(0,0,1,0) );		// last param	w = 0.0 directional light (direction), 1.0 point light (position)
	osg::LightSource* lightSource = new osg::LightSource;    
	lightSource->setLight( light );
	group->addChild( lightSource );
	group->getOrCreateStateSet()->setMode( GL_LIGHT0, osg::StateAttribute::ON );

	return group;
}

int _tmain(int argc, _TCHAR* argv[])
{
	WSAData wsaData;
	int error;
	error = WSAStartup(MAKEWORD(2, 2), &wsaData); // Make a call to start up winsock, MAKEWORD simply tells the version we are going to us. Version 2.2 for us
    if (error == SOCKET_ERROR)  // Almost EVERY Winsock function will reutrn a number on error, so we should always check for errors
    {
        std::cout << "Server: We Errored Starting Winsock!!!\n";
        return 0;
    }

	CNetworkServer* ns = new CNetworkServer();
	ns->debug();
	ns->startThread();

	CNetworkClient* nc = new CNetworkClient();
	nc->startThread();

	// construct the viewer
	osg::ref_ptr<osgViewer::Viewer> rViewer = new osgViewer::Viewer;

	// make the viewer create a 512x512 window and position it at 32, 32
	rViewer->setUpViewInWindow( 32, 32, 512, 512 );

	// set the scene-graph data the viewer will render
	rViewer->setSceneData( createScene() );

	// execute main loop
	return rViewer->run();
}



