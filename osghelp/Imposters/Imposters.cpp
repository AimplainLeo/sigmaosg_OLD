// Imposters.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <osgViewer/Viewer>
#include <osg/ShapeDrawable>
#include <osgSim/Impostor>
#include <osgGA/StateSetManipulator>
#include <osgDB/ReadFile>
#include <osgDB/FileUtils>
#include <osg/positionattitudetransform>


float Threshold = 100.0f;            // distance where impostor are shown

osg::Node* createScene()
{
	osg::Geode* pGeode = new osg::Geode();

	// we create the simplest form of shapes in OpenSceneGraph
    pGeode->addDrawable( new osg::ShapeDrawable( new osg::Sphere(osg::Vec3(0.0f,0.0f,0.0f),0.5f) ) );
    pGeode->addDrawable( new osg::ShapeDrawable( new osg::Box(osg::Vec3(2.0f,0.0f,0.0f),2.0f) ) );
    pGeode->addDrawable( new osg::ShapeDrawable( new osg::Cone(osg::Vec3(4.0f,0.0f,0.0f),0.5f,3.0f) ) );
    pGeode->addDrawable( new osg::ShapeDrawable( new osg::Cylinder(osg::Vec3(6.0f,0.0f,0.5f),0.5f,3.0f) ) );
    pGeode->addDrawable( new osg::ShapeDrawable( new osg::Capsule(osg::Vec3(8.0f,0.0f,1.0f),0.5f,3.0f) ) );

	return pGeode;
}

osg::Group* createForest()
{
	osg::Group* pGroup = new osg::Group;

	//osg::Node* tree = osgDB::readNodeFile( "C:/deleteme/obj/Broadleaf_RT.obj" );
	osg::Node* tree = osgDB::readNodeFile( "osg_pinet1_cooked.osg" );
	

	int gx = 5;
	int gy = 5;
	float dist = 20.0f;

	for ( int y=0; y<gy; y++ )
		for ( int x=0; x<gx; x++ )
		{
			osg::ref_ptr<osg::PositionAttitudeTransform> rPat = new osg::PositionAttitudeTransform;
			//rPat->setPosition( osg::Vec3(x*dist, 0, y*dist) );
			rPat->setPosition( osg::Vec3(x*dist, y*dist, 0) );
			rPat->addChild( tree );
			pGroup->addChild( rPat );
		}

	return pGroup;
}

int _tmain(int argc, _TCHAR* argv[])
{
    // construct the viewer
    osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer;

    // make the viewer create a 512x512 window and position it at 32, 32
    viewer->setUpViewInWindow( 32, 32, 512, 512 );

	osg::Group* group = new osg::Group;

	//osg::Node* pScene = createScene();
	osg::Node* pScene = createForest();
	

    osgSim::Impostor * impostor = new osgSim::Impostor();
    impostor->setImpostorThreshold(static_cast<float> (Threshold));
    impostor->addChild( pScene );
    impostor->setRange(0, 0.0f, 1e7f);
    impostor->setCenter(pScene->getBound().center());
    group->addChild( impostor );
	
	// set the scene-graph data the viewer will render
    viewer->setSceneData( group );

	viewer->addEventHandler( new osgGA::StateSetManipulator(viewer->getCamera()->getOrCreateStateSet()) );

    // execute main loop
    return viewer->run();
}
