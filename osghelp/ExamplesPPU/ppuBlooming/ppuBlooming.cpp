// ppuBlooming.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <osgViewer/Viewer>
#include <osg/ShapeDrawable>
#include <osgDB/ReadFile>
#include <osg/ClampColor>
#include <osg/Material>

#include <SigmaUtil/SulRTT.h>
#include <SigmaUtil/SulScreenAlignedQuad.h>
#include <SigmaUtil/SulNodeCallbackCameraSync.h>

#include "hdrppu.h"
#include "bloomppu.h"
#include <osgPPU/Processor.h>

osg::Node* CreateScene( osgViewer::Viewer* pViewer )
{
	osg::Group* pGroup = new osg::Group;

    // disable color clamping, because we want to work on real hdr values
    // make it protected and override, so that it is done for the whole rendering pipeline
	osg::Group* pObjectsHDR = new osg::Group;
	osg::ClampColor* clamp = new osg::ClampColor();
    clamp->setClampVertexColor(GL_FALSE);
    clamp->setClampFragmentColor(GL_FALSE);
    clamp->setClampReadColor(GL_FALSE);
	pObjectsHDR->getOrCreateStateSet()->setAttribute(clamp, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE | osg::StateAttribute::PROTECTED);
    osg::Material* mat = new osg::Material();
    mat->setColorMode(osg::Material::DIFFUSE);
    mat->setDiffuse(osg::Material::FRONT_AND_BACK, osg::Vec4(15.0, 9.0, 6.0, 1.0));
	pObjectsHDR->getOrCreateStateSet()->setAttribute(mat, osg::StateAttribute::ON);

	// background sun
	osg::Texture2D* pTex = new osg::Texture2D;
	osg::Image* pImage = osgDB::readImageFile( "sun.tga" );
    pTex->setImage( pImage );
	CSulScreenAlignedQuad* pAlignQuadSun = new CSulScreenAlignedQuad( osg::Vec3(256,256,0), 512,512,512,512 );
	pAlignQuadSun->SetTexture( pTex );
	pObjectsHDR->addChild( pAlignQuadSun->GetProjection() );
	pGroup->addChild( pAlignQuadSun->GetProjection() );

	// we create the simplest form of shapes in OpenSceneGraph
    osg::Geode* pGeode = new osg::Geode();
	pGeode->addDrawable( new osg::ShapeDrawable( new osg::Sphere(osg::Vec3(0.0f,0.0f,0.0f),0.5f) ) );
    pGeode->addDrawable( new osg::ShapeDrawable( new osg::Box(osg::Vec3(2.0f,0.0f,0.0f),2.0f) ) );
    pGeode->addDrawable( new osg::ShapeDrawable( new osg::Cone(osg::Vec3(4.0f,0.0f,0.0f),0.5f,3.0f) ) );
    pGeode->addDrawable( new osg::ShapeDrawable( new osg::Cylinder(osg::Vec3(6.0f,0.0f,0.5f),0.5f,3.0f) ) );
    pGeode->addDrawable( new osg::ShapeDrawable( new osg::Capsule(osg::Vec3(8.0f,0.0f,1.0f),0.5f,3.0f) ) );
	pObjectsHDR->addChild( pGeode );
	pGroup->addChild( pGeode );

	// create a camera that renders to a texture
	CSulRTT* pRTT = new CSulRTT( 256, 256 );
	pRTT->GetCamera()->setUpdateCallback( new CSulNodeCallbackCameraSync( pViewer->getCamera() ) );
	pRTT->GetCamera()->setClearColor(osg::Vec4(0.0f,0.0f,0.0f,0.0f));
	pRTT->GetCamera()->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	pRTT->GetTexture()->setInternalFormat( GL_RGBA16F_ARB );
	pRTT->GetTexture()->setSourceType( GL_FLOAT );
	pRTT->AddChildToCamera( pObjectsHDR );
	pGroup->addChild( pRTT->GetCamera() );

	// screen aligned quad that uses the rendered texture
	CSulScreenAlignedQuad* pAlignQuad = new CSulScreenAlignedQuad( osg::Vec3( 128, 128, 0 ), 256, 256, 512, 512 );
	pAlignQuad->GetProjection()->getOrCreateStateSet()->setRenderBinDetails( 10000, "RenderBin" );
	pAlignQuad->SetTexture( pRTT->GetTexture() );
	pGroup->addChild( pAlignQuad->GetProjection() );

	// postfilter stuff
	osgPPU::Processor* pProcessor = new osgPPU::Processor;
	pProcessor->setCamera( pRTT->GetCamera() );
	pProcessor->dirtyUnitSubgraph();
	pGroup->addChild( pProcessor );

	// we want to simulate hdr rendering, hence setup the pipeline for the hdr rendering
	HDRRendering* pHDRSetup = new  HDRRendering;
	osgPPU::Unit* firstUnit = NULL;
	osgPPU::Unit* lastUnit = NULL;
	pHDRSetup->createHDRPipeline( pProcessor, firstUnit, lastUnit );
	pProcessor->addChild( firstUnit );

	/*
	CBloomPPU* pBloomPPU = new CBloomPPU;
	pBloomPPU->Create();
	pProcessor->addChild( pBloomPPU->GetFirstUnit() );
	osgPPU::Unit* lastUnit = pBloomPPU->GetLastUnit();
	*/

	// create quad on right
	pAlignQuad = new CSulScreenAlignedQuad( osg::Vec3( 128+256, 128, 0 ), 256, 256, 512, 512 );
	pAlignQuad->GetProjection()->getOrCreateStateSet()->setRenderBinDetails( 100000, "RenderBin" );
	pAlignQuad->SetTexture( lastUnit->getOrCreateOutputTexture( 0 ) );
	pGroup->addChild( pAlignQuad->GetProjection() );

	return pGroup;
}


int _tmain(int argc, _TCHAR* argv[])
{
    // construct the viewer
    osg::ref_ptr<osgViewer::Viewer> rViewer = new osgViewer::Viewer;

	rViewer->setThreadingModel(osgViewer::Viewer::SingleThreaded);

    // make the viewer create a 512x512 window and position it at 32, 32
    rViewer->setUpViewInWindow( 32, 32, 512, 512 );

    // set the scene-graph data the viewer will render
    rViewer->setSceneData( CreateScene( rViewer.get() ) );

    // execute main loop
    return rViewer->run();
}
