// ShadowShaders.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <osgViewer/Viewer>
#include <osg/group>
#include <osg/shapedrawable>
#include <osgShadow/ShadowTexture>
#include <osgShadow/ShadowedScene>
#include <osgShadow/ShadowVolume>
#include <osgShadow/ShadowMap>
#include <osgShadow/ParallelSplitShadowMap>
#include <osgShadow/LightSpacePerspectiveShadowMap>
#include <osgShadow/SoftShadowMap>
#include <osgDB/readFile>

static int ReceivesShadowTraversalMask = 0x1;
static int CastsShadowTraversalMask = 0x2;

#define LIGHTPOS 30.0f, 30.0f, 30.0f
//#define LIGHTPOS 1.0f, 1.0f, 1.0f

osg::Geode* createGround()
{
	osg::Geode* pGeode = new osg::Geode();

	pGeode->addDrawable( new osg::ShapeDrawable( new osg::Box(osg::Vec3(4.0f,0.0f,0.0f), 10.0f, 10.0f, 0.1f ) ) );
	pGeode->setNodeMask( ReceivesShadowTraversalMask );
/*
	osg::Texture2D* pTex = new osg::Texture2D;
    osg::Image* pImage = osgDB::readImageFile( "grass2.tga" );
    pTex->setImage( pImage );
    pGeode->getOrCreateStateSet()->setTextureAttributeAndModes( 0, pTex, osg::StateAttribute::ON );
	pGeode->getOrCreateStateSet()->setMode( GL_LIGHTING, osg::StateAttribute::ON );
	pGeode->getOrCreateStateSet()->setMode( GL_BLEND, osg::StateAttribute::ON );
	pGeode->getOrCreateStateSet()->setRenderBinDetails( 9, "DepthSortedBin" );
*/
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
	pGeode->setNodeMask( CastsShadowTraversalMask );

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

osgShadow::ShadowMap* createShadowMap( osgShadow::ShadowedScene* pShadowScene )
{
	osgShadow::ShadowMap* p = new osgShadow::ShadowMap;
	p->setTextureSize( osg::Vec2s(1024,1024) );
	pShadowScene->addChild( createLightSource() );
	//p->setLight( createLightSource() );
	return p;
}

osgShadow::ShadowTexture* createShadowTexture( osgShadow::ShadowedScene* pShadowScene )
{
	osgShadow::ShadowTexture* p = new osgShadow::ShadowTexture;
	pShadowScene->addChild( createLightSource() );
	return p;
}

osgShadow::LightSpacePerspectiveShadowMapDB* createLSPSM_DB( osgShadow::ShadowedScene* pShadowScene )
{
	pShadowScene->addChild( createLightSource() );	

	osgShadow::LightSpacePerspectiveShadowMapDB* p = new osgShadow::LightSpacePerspectiveShadowMapDB;
	p->setMinLightMargin( 10.0f );
	p->setMaxFarPlane( 0.0f );
	p->setTextureSize( osg::Vec2s(1024,1024) );
	return p;
}

osgShadow::LightSpacePerspectiveShadowMapVB* createLSPSM_VB( osgShadow::ShadowedScene* pShadowScene )
{
	osgShadow::LightSpacePerspectiveShadowMapVB* p = new osgShadow::LightSpacePerspectiveShadowMapVB;
	pShadowScene->addChild( createLightSource() );
	return p;
}

osgShadow::LightSpacePerspectiveShadowMapCB* createLSPSM_CB( osgShadow::ShadowedScene* pShadowScene )
{
	osgShadow::LightSpacePerspectiveShadowMapCB* p = new osgShadow::LightSpacePerspectiveShadowMapCB;
	pShadowScene->addChild( createLightSource() );
	return p;
}

osgShadow::ParallelSplitShadowMap* createPSSM( osgShadow::ShadowedScene* pShadowScene )
{
	pShadowScene->addChild( createLightSource() );
	osgShadow::ParallelSplitShadowMap* p = new osgShadow::ParallelSplitShadowMap( NULL, 3 );
	p->setTextureResolution( 1024 );
	return p;
}

osgShadow::SoftShadowMap* createSoftShadowMap( osgShadow::ShadowedScene* pShadowScene )
{
	pShadowScene->addChild( createLightSource() );
	osgShadow::SoftShadowMap* p = new osgShadow::SoftShadowMap;
	return p;
}

int _tmain(int argc, _TCHAR* argv[])
{
    // construct the viewer
    osg::ref_ptr<osgViewer::Viewer> rViewer = new osgViewer::Viewer;

	rViewer->getCamera()->setClearColor( osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f) );

    // make the viewer create a 800x600 window and position it at 32, 32
    rViewer->setUpViewInWindow( 32, 32, 800, 600 );

	osg::ref_ptr<osg::Group> rRoot = new osg::Group;

	// shadow scene
    osgShadow::ShadowedScene* shadowedScene = new osgShadow::ShadowedScene;
    shadowedScene->setReceivesShadowTraversalMask( ReceivesShadowTraversalMask );
    shadowedScene->setCastsShadowTraversalMask( CastsShadowTraversalMask );
	shadowedScene->addChild( createGround() );
	shadowedScene->addChild( createShapes() );
	rRoot->addChild( shadowedScene );

	//shadowedScene->setShadowTechnique( createShadowMap( shadowedScene ) );		// WORKS
	//shadowedScene->setShadowTechnique( createShadowTexture( shadowedScene ) );	// WORKS
	shadowedScene->setShadowTechnique( createLSPSM_DB( shadowedScene ) );			// WORKS
	//shadowedScene->setShadowTechnique( createLSPSM_VB( shadowedScene ) );			// WORKS
	//shadowedScene->setShadowTechnique( createLSPSM_CB( shadowedScene ) );			// WORKS
	//shadowedScene->setShadowTechnique( createPSSM( shadowedScene ) );				// not working
	//shadowedScene->setShadowTechnique( createSoftShadowMap( shadowedScene ) );	// WORKS

	// add the scene
	rViewer->setSceneData( rRoot );

    // execute main loop
    return rViewer->run();
}