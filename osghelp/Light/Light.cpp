// Light.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <osgViewer/Viewer>
#include <osg/ShapeDrawable>

osg::Node* CreateSpheres( float z )
{
	osg::Geode* pGeode = new osg::Geode();

    pGeode->addDrawable( new osg::ShapeDrawable( new osg::Sphere(osg::Vec3(0.0f,0.0f,z),0.5f) ) );
	pGeode->addDrawable( new osg::ShapeDrawable( new osg::Sphere(osg::Vec3(2.0f,0.0f,z),0.5f) ) );
	pGeode->addDrawable( new osg::ShapeDrawable( new osg::Sphere(osg::Vec3(4.0f,0.0f,z),0.5f) ) );
	pGeode->addDrawable( new osg::ShapeDrawable( new osg::Sphere(osg::Vec3(6.0f,0.0f,z),0.5f) ) );
	pGeode->addDrawable( new osg::ShapeDrawable( new osg::Sphere(osg::Vec3(8.0f,0.0f,z),0.5f) ) );

	return pGeode;
}

osg::Node* CreateScene()
{
	osg::Group* pGroup = new osg::Group;

	// osg:Light allows us to set default parameter values to be used by osg::LightSource
	// note: we do not use light number 0, because we do not want to override the osg default headlights
	osg::Light* pLight = new osg::Light;
	pLight->setLightNum( 4 );						
	pLight->setDiffuse( osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f) );
	pLight->setPosition( osg::Vec4(1,0,0,0) );		// last param	w = 0.0 directional light (direction)
													//				w = 1.0 point light (position)
	// light source
	osg::LightSource* pLightSource = new osg::LightSource;    
	pLightSource->setLight( pLight );
	pGroup->addChild( pLightSource );

	pGroup->addChild( CreateSpheres( 0 ) );
	pGroup->addChild( CreateSpheres( 2 ) );
	pGroup->addChild( CreateSpheres( 4 ) );
	pGroup->addChild( CreateSpheres( 6 ) );

	pGroup->getOrCreateStateSet()->setMode( GL_LIGHT4, osg::StateAttribute::ON );

	return pGroup;
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

