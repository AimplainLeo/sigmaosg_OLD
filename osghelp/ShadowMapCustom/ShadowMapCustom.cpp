// ShadowMapCustom.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <SigmaUtil/SulShadow.h>
#include <SigmaUtil/SulScreenAlignedQuad.h>
#include <osgViewer/Viewer>
#include <osg/group>
#include <osg/shapedrawable>

//#define LIGHTPOS 1.0f, 1.0f, 1.0f
#define LIGHTPOS 100.0f, 100.0f, 100.0f

osg::Geode* createGround()
{
	osg::Geode* pGeode = new osg::Geode();

	pGeode->addDrawable( new osg::ShapeDrawable( new osg::Box(osg::Vec3(4.0f,0.0f,0.0f), 10.0f, 10.0f, 0.1f ) ) );

	return pGeode;
}

osg::Geode* createShapes()
{
	osg::Geode* pGeode = new osg::Geode();

	// we create the simplest form of shapes in OpenSceneGraph
	pGeode->addDrawable( new osg::ShapeDrawable( new osg::Sphere(osg::Vec3(0.0f,0.0f,0.5f),0.5f) ) );
	pGeode->addDrawable( new osg::ShapeDrawable( new osg::Box(osg::Vec3(2.0f,0.0f,1.0f),2.0f) ) );
	pGeode->addDrawable( new osg::ShapeDrawable( new osg::Cone(osg::Vec3(4.0f,0.0f,1.0f),0.5f,3.0f) ) );
	pGeode->addDrawable( new osg::ShapeDrawable( new osg::Cylinder(osg::Vec3(6.0f,0.0f,1.5f),0.5f,3.0f) ) );
	pGeode->addDrawable( new osg::ShapeDrawable( new osg::Capsule(osg::Vec3(8.0f,0.0f,2.0f),0.5f,3.0f) ) );

	return pGeode;
}

osg::LightSource* createLightSource()
{
	// light
	osg::Vec4 lightpos;
	lightpos.set( LIGHTPOS, 1.0f );		// 0.0 = directional light , 1.0 = positioned light
    osg::LightSource* ls = new osg::LightSource;
    ls->getLight()->setPosition(lightpos);	
	return ls;
}

int _tmain(int argc, _TCHAR* argv[])
{
    // construct the viewer
    osg::ref_ptr<osgViewer::Viewer> rViewer = new osgViewer::Viewer;

	rViewer->getCamera()->setClearColor( osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f) );

    // make the viewer create a 800x600 window and position it at 32, 32
    rViewer->setUpViewInWindow( 32, 32, 800, 600 );

	osg::ref_ptr<osg::Group> rRoot = new osg::Group;

	osg::Node* pGround = createGround();
	osg::Node* pShapes = createShapes();
	rRoot->addChild( pShapes );

	////////////////////////////////////////////////
	// shadow
	////////////////////////////////////////////////

	CSulShadow* pShadow = new CSulShadow( rRoot );

	pShadow->addCaster( pShapes );
//	pShadow->addReceiver( pGround );	
//	pShadow->addReceiver( pShapes );		// self shadowing has problems

	pShadow->init();

	rRoot->addChild( pShadow );

	////////////////////////////////////////////////
	// something else
	////////////////////////////////////////////////

	// add the scene
	rViewer->setSceneData( rRoot );

    // execute main loop
    return rViewer->run();
}