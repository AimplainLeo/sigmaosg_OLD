// PrecipitationEffect.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <osgViewer/Viewer>
#include <osg/ShapeDrawable>
#include <osgParticle/PrecipitationEffect>
 
osg::Node* CreateScene()
{
	osg::Group* pGroup = new osg::Group;

	osg::Geode* pGeode = new osg::Geode();
    pGeode->addDrawable( new osg::ShapeDrawable( new osg::Box(osg::Vec3(0.0f,0.0f,0.0f),100.0f,100.0f, 0.1f) ) );
	pGroup->addChild( pGeode );

	osgParticle::PrecipitationEffect* pWeather = new osgParticle::PrecipitationEffect();
	/*
	pWeather->setWind( osg::Vec3(0,0,0) );
	pWeather->setParticleSpeed( -0.001f );
	pWeather->setParticleSize( 0.03f );
	pWeather->setParticleColor( osg::Vec4(0.6, 0.6, 0.6, 1.0) );
	pWeather->setMaximumParticleDensity( 8.5f );
	pWeather->setCellSize( osg::Vec3( 5.0f, 5.0f, 5.0f ) );
	pWeather->setNearTransition( 25.0f );
	pWeather->setFarTransition( 40.0f );
	pWeather->setUseFarLineSegments( false );
	pWeather->setPosition( osg::Vec3( 0,-100,0 ) );
	*/
	pWeather->rain( 1.0f );
	pGroup->addChild( pWeather );

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

