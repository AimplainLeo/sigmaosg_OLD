// SavingSceneToDisk.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <osg/ShapeDrawable>
#include <osgDB/WriteFile>

osg::Node* CreateScene()
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

int _tmain(int argc, _TCHAR* argv[])
{
	osg::ref_ptr<osg::Node>  rNode = CreateScene();
	osgDB::writeNodeFile( *rNode, "myscene.osg" );
	osgDB::writeNodeFile( *rNode, "myscene.ive" );
	
	return 0;
}
