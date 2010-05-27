// fog.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <osgViewer/Viewer>
#include <osg/ShapeDrawable>
#include <osg/fog>
 
osg::Node* CreateScene()
{
    osg::Geode* pGeode = new osg::Geode();

	for ( int x=0; x<10; x++ )
	{
		for ( int y=0; y<10; y++ )
		{
			pGeode->addDrawable( new osg::ShapeDrawable( new osg::Sphere(osg::Vec3(x,y,0.0f),0.3f) ) );
		}
	}

	osg::Fog* pFog = new osg::Fog(); 
	pFog->setMode( osg::Fog::EXP2 ); 
	pFog->setColor( osg::Vec4( 1, 1, 1, 1) ); 
	pFog->setDensity( 0.05f ); 
	pGeode->getOrCreateStateSet()->setAttribute( pFog, osg::StateAttribute::ON ); 
	pGeode->getOrCreateStateSet()->setMode( GL_FOG, osg::StateAttribute::ON ); 

    return pGeode;
}

int _tmain(int argc, _TCHAR* argv[])
{
    // construct the viewer
    osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer;

    // full screen antialiasing (if supported)
    osg::DisplaySettings::instance()->setNumMultiSamples( 4 );

    // make the viewer create a 512x512 window and position it at 32, 32
    viewer->setUpViewInWindow( 32, 32, 512, 512 );

    // set the scene-graph data the viewer will render
    viewer->setSceneData( CreateScene() );

	viewer->getCamera()->setClearColor( osg::Vec4( 1,1,1,1 ) );

    // execute main loop
    return viewer->run();
}
