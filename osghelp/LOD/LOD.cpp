// LOD.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <osgViewer/Viewer>
#include <osg/ShapeDrawable>
#include <osg/PolygonMode>

osg::Node* createScene()
{
	osg::Geode* geode = new osg::Geode();

	// create LOD
	osg::LOD* lod = new osg::LOD;
	lod->setRangeMode( osg::LOD::DISTANCE_FROM_EYE_POINT );

	// create high detail sphere and add to our lod
    osg::TessellationHints* hints = new osg::TessellationHints;
    hints->setDetailRatio(0.5f);
	geode = new osg::Geode();
	geode->addDrawable( new osg::ShapeDrawable( new osg::Sphere(osg::Vec3(0.0f,4.0f,0.0f),0.5f), hints ) );
	lod->addChild( geode, 0, 10 );

	// create low detail sphere and add to our lod
    hints = new osg::TessellationHints;
    hints->setDetailRatio(0.1f);
	geode = new osg::Geode();
	geode->addDrawable( new osg::ShapeDrawable( new osg::Sphere(osg::Vec3(0.0f,4.0f,0.0f),0.5f), hints ) );
	lod->addChild( geode, 10, 10000 );

	// let's view it in wireframe for better understanding
	lod->getOrCreateStateSet()->setAttribute( new osg::PolygonMode(osg::PolygonMode::FRONT_AND_BACK,osg::PolygonMode::LINE) ); 

	return lod;

}

int _tmain(int argc, _TCHAR* argv[])
{
    // construct the viewer
    osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer;

    // make the viewer create a 512x512 window and position it at 32, 32
    viewer->setUpViewInWindow( 32, 32, 512, 512 );

    // set the scene-graph data the viewer will render
    viewer->setSceneData( createScene() );

    // execute main loop
    return viewer->run();
}

