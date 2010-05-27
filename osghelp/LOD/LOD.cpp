// LOD.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <osgViewer/Viewer>
#include <osg/ShapeDrawable>
#include <osg/animationpath>
#include <osg/matrixtransform>
#include <osg/PolygonMode>
 
osg::Node* CreateScene()
{
	// create group
	osg::Group* pGroup = new osg::Group;

	// create a box to be used as a visual reference plane 
	osg::Geode* pGeode = new osg::Geode();
	pGeode->addDrawable( new osg::ShapeDrawable( new osg::Box(osg::Vec3(0.0f,0.0f,-1.0f),10.0f,10.0f,1.0f) ) );
	pGroup->addChild( pGeode );

	// create LOD and set mode to PIXEL_SIZE_ON_SCREEN
	osg::LOD* pLod = new osg::LOD;
	pLod->setRangeMode( osg::LOD::PIXEL_SIZE_ON_SCREEN );
	
	// create high detail sphere and add to our lod
    osg::TessellationHints* hints = new osg::TessellationHints;
    hints->setDetailRatio(0.5f);
	pGeode = new osg::Geode();
	pGeode->addDrawable( new osg::ShapeDrawable( new osg::Sphere(osg::Vec3(0.0f,4.0f,0.0f),0.5f), hints ) );
	osg::StateSet* pStateSet = pGeode->getOrCreateStateSet();
	pStateSet->setAttribute( new osg::PolygonMode(osg::PolygonMode::FRONT_AND_BACK,osg::PolygonMode::LINE) ); 
	pLod->addChild( pGeode, 75, 1000 );

	// create low detail sphere and add to our lod
    hints = new osg::TessellationHints;
    hints->setDetailRatio(0.1f);
	pGeode = new osg::Geode();
	pGeode->addDrawable( new osg::ShapeDrawable( new osg::Sphere(osg::Vec3(0.0f,4.0f,0.0f),0.5f), hints ) );
	pStateSet = pGeode->getOrCreateStateSet();
	pStateSet->setAttribute( new osg::PolygonMode(osg::PolygonMode::FRONT_AND_BACK,osg::PolygonMode::LINE) ); 
	pLod->addChild( pGeode, 0, 75 );

	// use AnimationPathCallback to move LOD group
    osg::MatrixTransform* pMatTrans = new osg::MatrixTransform;
	pMatTrans->addChild( pLod );
    pMatTrans->setUpdateCallback( new osg::AnimationPathCallback(osg::Vec3(0, 0, 0), osg::Z_AXIS, osg::inDegrees(45.0f)) );
	pGroup->addChild( pMatTrans );

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

