// TextureCubeMap.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <osgViewer/Viewer>
#include <osg/ShapeDrawable>
#include <osg/TextureCubeMap>
#include <osg/geode>
#include <osgDB/FileUtils>
#include <osgDB/ReadFile>
#include <osg/texgen>

osg::Node* createScene()
{
	osg::Geode* pGeode = new osg::Geode();
    pGeode->addDrawable( new osg::ShapeDrawable( new osg::Box(osg::Vec3(2.0f,0.0f,0.0f),2.0f) ) );

	osg::ref_ptr<osg::TextureCubeMap> texture = new osg::TextureCubeMap;
	texture->setImage( osg::TextureCubeMap::POSITIVE_X, osgDB::readImageFile( "side1.tga" ) );
	texture->setImage( osg::TextureCubeMap::NEGATIVE_X, osgDB::readImageFile( "side2.tga" ) );
	texture->setImage( osg::TextureCubeMap::POSITIVE_Y, osgDB::readImageFile( "side3.tga" ) );
	texture->setImage( osg::TextureCubeMap::NEGATIVE_Y, osgDB::readImageFile( "side4.tga" ) );
	texture->setImage( osg::TextureCubeMap::POSITIVE_Z, osgDB::readImageFile( "side5.tga" ) );
	texture->setImage( osg::TextureCubeMap::NEGATIVE_Z, osgDB::readImageFile( "side6.tga" ) );

	osg::StateSet* stateset = new osg::StateSet;
    stateset->setMode(GL_LIGHTING,  osg::StateAttribute::OFF);
	stateset->setTextureAttributeAndModes(0, texture, osg::StateAttribute::ON);
	pGeode->setStateSet(stateset);

    // texture coordinate generation
    osg::TexGen *tg = new osg::TexGen;
    tg->setMode(osg::TexGen::OBJECT_LINEAR);
    stateset->setTextureAttributeAndModes(0, tg, osg::StateAttribute::OVERRIDE | osg::StateAttribute::ON);


	return pGeode;
}

int _tmain(int argc, _TCHAR* argv[])
{
    // construct the viewer
    osg::ref_ptr<osgViewer::Viewer> rViewer = new osgViewer::Viewer;

	rViewer->setThreadingModel( osgViewer::ViewerBase::SingleThreaded );

    rViewer->setUpViewInWindow( 32, 32, 800, 600 );

	rViewer->setSceneData( createScene() );

    // execute main loop
    return rViewer->run();

}
