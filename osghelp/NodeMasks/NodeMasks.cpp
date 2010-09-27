// NodeMasks.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <osgViewer/Viewer>
#include <osg/ShapeDrawable>
#include <osg/MatrixTransform>

osg::Group* createSphere( const osg::Vec3& pos )
{
	osg::Geode* pGeode = new osg::Geode();
    pGeode->addDrawable( new osg::ShapeDrawable( new osg::Sphere(osg::Vec3(0.0f,0.0f,0.0f),0.5f) ) );
	
	osg::MatrixTransform* mt = new osg::MatrixTransform;
	osg::Matrix m;
	m.setTrans( pos );
	mt->setMatrix( m );
	mt->addChild( pGeode );

	return mt;
}
 
osg::Group* createScene()
{
	osg::Group* pGroup0 = createSphere( osg::Vec3(0,0,0) );
	osg::Group* pGroup1 = createSphere( osg::Vec3(2,0,0) );
	osg::Group* pGroup2 = createSphere( osg::Vec3(2,0,0) );
	
	osg::Group* pGroup = new osg::Group;
	pGroup->addChild( pGroup0 );
	pGroup0->addChild( pGroup1 );
	pGroup1->addChild( pGroup2 );

	// node masks (top)
	osg::Group* pGroup3 = createSphere( osg::Vec3(0,0,2) );
	osg::Group* pGroup4 = createSphere( osg::Vec3(2,0,0) );
	pGroup4->setNodeMask( 0 );
	osg::Group* pGroup5 = createSphere( osg::Vec3(2,0,0) );
	
	pGroup->addChild( pGroup3 );
	pGroup3->addChild( pGroup4 );
	pGroup4->addChild( pGroup5 );

	pGroup4->setNodeMask( 0xFFFFFFFF );

	return pGroup;
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

