// Shadow.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <osgViewer/Viewer>
#include <osg/ShapeDrawable>
#include <osgShadow/ShadowedScene>
#include <osgShadow/ShadowVolume>
#include <osgShadow/ShadowMap>
#include <osgShadow/ParallelSplitShadowMap>
#include <osgShadow/LightSpacePerspectiveShadowMap>
 
static int ReceivesShadowTraversalMask = 0x1;
static int CastsShadowTraversalMask = 0x2;

osg::Node* CreateScene()
{
	osg::Geode* pGeode = new osg::Geode();

	// we create the simplest form of shapes in OpenSceneGraph
	pGeode->addDrawable( new osg::ShapeDrawable( new osg::Sphere(osg::Vec3(0.0f,0.0f,0.0f),0.5f) ) );
    pGeode->addDrawable( new osg::ShapeDrawable( new osg::Box(osg::Vec3(2.0f,0.0f,0.0f),2.0f) ) );
    pGeode->addDrawable( new osg::ShapeDrawable( new osg::Cone(osg::Vec3(4.0f,0.0f,0.0f),0.5f,3.0f) ) );
    pGeode->addDrawable( new osg::ShapeDrawable( new osg::Cylinder(osg::Vec3(6.0f,0.0f,0.5f),0.5f,3.0f) ) );
    pGeode->addDrawable( new osg::ShapeDrawable( new osg::Capsule(osg::Vec3(8.0f,0.0f,1.0f),0.5f,3.0f) ) );
	
//	pGeode->setNodeMask( ReceivesShadowTraversalMask );
//	pGeode->setNodeMask( CastsShadowTraversalMask );
	pGeode->setNodeMask( 0x3 );
	

    osgShadow::ShadowedScene* shadowedScene = new osgShadow::ShadowedScene;
    shadowedScene->setReceivesShadowTraversalMask( ReceivesShadowTraversalMask );
    shadowedScene->setCastsShadowTraversalMask( CastsShadowTraversalMask );

	osg::Vec4 lightpos;
	lightpos.set( -1.0f, 0.0f, 0.0f, 0.0f);
    osg::LightSource* ls = new osg::LightSource;
    ls->getLight()->setPosition(lightpos);

/*
	// shadowmap
	osg::ref_ptr<osgShadow::ShadowMap> sm = new osgShadow::ShadowMap;
	sm->setLight( ls );
	shadowedScene->setShadowTechnique( sm );
*/

/*
	// shadow volume
	osgShadow::ShadowVolume* sv = new osgShadow::ShadowVolume;
	sv->setDynamicShadowVolumes( false );
	sv->setDrawMode(osgShadow::ShadowVolumeGeometry::GEOMETRY);
	//sv->setDrawMode(osgShadow::ShadowVolumeGeometry::STENCIL_TWO_SIDED);
	//sv->setDrawMode(osgShadow::ShadowVolumeGeometry::STENCIL_TWO_PASS);
	shadowedScene->setShadowTechnique( sv );
*/

/*
	// parallel split shadowmap
	int mapcount = 3;
	osg::ref_ptr<osgShadow::ParallelSplitShadowMap> pssm = new osgShadow::ParallelSplitShadowMap(NULL,mapcount);
	shadowedScene->setShadowTechnique( pssm );
*/

	osg::ref_ptr<osgShadow::MinimalShadowMap> sm = NULL;
	//sm = new osgShadow::LightSpacePerspectiveShadowMapDB;
	//sm = new osgShadow::LightSpacePerspectiveShadowMapVB;
	sm = new osgShadow::LightSpacePerspectiveShadowMapCB;
	shadowedScene->setShadowTechnique( sm );

	shadowedScene->addChild( pGeode );
	shadowedScene->addChild( ls );

	return shadowedScene;
}

int _tmain(int argc, _TCHAR* argv[])
{
	// hint to tell viewer to request stencil buffer when setting up windows
	osg::DisplaySettings::instance()->setMinimumNumStencilBits( 8 );

    // construct the viewer
    osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer;

    // make the viewer create a 512x512 window and position it at 32, 32
    viewer->setUpViewInWindow( 32, 32, 512, 512 );

    // set the scene-graph data the viewer will render
    viewer->setSceneData( CreateScene() );

    // execute main loop
    return viewer->run();
}
