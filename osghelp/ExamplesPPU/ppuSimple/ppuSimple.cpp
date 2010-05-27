// ppuSimple.cpp : Defines the entry point for the console application.
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
#include <osgPPU/Processor.h>

osg::Node* CreateScene( osgViewer::Viewer* pViewer )
{
	osg::Group* pGroup = new osg::Group;

	// sun
	osg::Texture2D* pTex = new osg::Texture2D;
	osg::Image* pImage = osgDB::readImageFile( "sun.tga" );
    pTex->setImage( pImage );
	CSulScreenAlignedQuad* pAlignQuadSun = new CSulScreenAlignedQuad( osg::Vec3(256,256,0), 512,512,512,512 );
	pAlignQuadSun->SetTexture( pTex );
	pGroup->addChild( pAlignQuadSun->GetProjection() );

	// shape
	osg::Geode* pGeode = new osg::Geode();
	pGeode->addDrawable( new osg::ShapeDrawable( new osg::Capsule(osg::Vec3(8.0f,0.0f,1.0f),0.5f,3.0f) ) );
	pGroup->addChild( pGeode );

	// clamp
	osg::ClampColor* clamp = new osg::ClampColor();
    clamp->setClampVertexColor(GL_FALSE);
    clamp->setClampFragmentColor(GL_FALSE);
    clamp->setClampReadColor(GL_FALSE);
	pGroup->getOrCreateStateSet()->setAttribute(clamp, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE | osg::StateAttribute::PROTECTED);

	// material
	osg::Material* mat = new osg::Material();
    mat->setColorMode( osg::Material::OFF );
    //mat->setDiffuse( osg::Material::FRONT_AND_BACK, osg::Vec4(15.0, 9.0, 6.0, 1.0) );
	mat->setDiffuse( osg::Material::FRONT_AND_BACK, osg::Vec4(3.0, 3.0, 3.0, 1.0) );
	//mat->setDiffuse( osg::Material::FRONT_AND_BACK, osg::Vec4(0.0, 1.0, 0.0, 1.0) );
	pGroup->getOrCreateStateSet()->setAttribute( mat, osg::StateAttribute::ON );
	

	/////////////////////////
	// modify camera
	/////////////////////////

	osg::Texture2D* m_rTex = new osg::Texture2D;
	m_rTex->setTextureSize( 512, 512 );
	m_rTex->setWrap( osg::Texture::WRAP_S, osg::Texture::CLAMP_TO_EDGE );
    m_rTex->setWrap( osg::Texture::WRAP_T, osg::Texture::CLAMP_TO_EDGE );
	m_rTex->setSourceFormat( GL_RGBA );
	m_rTex->setInternalFormat( GL_RGBA );
	m_rTex->setInternalFormat(GL_RGBA16F_ARB);
	m_rTex->setSourceType(GL_FLOAT);
	m_rTex->setFilter( osg::Texture2D::MIN_FILTER,osg::Texture2D::LINEAR );
	m_rTex->setFilter( osg::Texture2D::MAG_FILTER,osg::Texture2D::LINEAR );

	pViewer->getCamera()->setClearColor(osg::Vec4(0.0f,0.0f,0.0f,0.0f));
    pViewer->getCamera()->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	pViewer->getCamera()->setRenderTargetImplementation(osg::Camera::FRAME_BUFFER_OBJECT);
	pViewer->getCamera()->attach(osg::Camera::COLOR_BUFFER, m_rTex );

	/////////////////////////
	// postfilter stuff
	/////////////////////////
	osgPPU::Processor* pProcessor = new osgPPU::Processor;
	pProcessor->setCamera( pViewer->getCamera() );
	pProcessor->dirtyUnitSubgraph();
	pGroup->addChild( pProcessor );

	// we want to simulate hdr rendering, hence setup the pipeline for the hdr rendering
	HDRRendering* pHDRSetup = new  HDRRendering;
	osgPPU::Unit* firstUnit = NULL;
	osgPPU::Unit* lastUnit = NULL;
	pHDRSetup->createHDRPipeline( pProcessor, firstUnit, lastUnit );
	pProcessor->addChild( firstUnit );

	osgPPU::UnitOut* ppuout = new osgPPU::UnitOut();
	ppuout->setInputTextureIndexForViewportReference(-1); // need this here to get viewport from camera
	lastUnit->addChild(ppuout);

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

