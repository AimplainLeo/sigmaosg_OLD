// PostFilter_HDR.cpp : Defines the entry point for the console application.
//


#include "stdafx.h"
#include <SigmaUtil/SulScreenAlignedQuad.h>
#include <SigmaUtil/SulRTT.h>
#include <SigmaUtil/SulTexCam.h>
#include <SigmaUtil/SulNodeCallbackCameraSync.h>
#include <SigmaUtil/SulShaderBW.h>
#include <SigmaUtil/SulPostFilterBloom.h>
#include <SigmaUtil/SulPostFilterHDR.h>
#include <SigmaUtil/SulPostFilterHDR2.h>
#include <SigmaUtil/SulPostFilterBrightPass.h>
#include <SigmaUtil/SulShaderAmplifier.h>
#include <SigmaUtil/SulDebugMenu.h>
#include <SigmaUtil/SulDebugValueFloat.h>
#include <SigmaUtil/SulDebugValueText.h>
#include <SigmaUtil/SulString.h>
#include <SigmaUtil/SulDebugMenuKeyboardHandler.h>
#include <osgViewer/Viewer>
#include <osg/ShapeDrawable>
#include <osgDB/FileUtils>
#include <osgGA/TrackballManipulator>

CSulTexCam* pRTT = 0;
osg::Group* pGroup = 0; 
CSulDebugMenu* pMenu = 0;
CSulShaderAmplifier* m_amp = 0;
CSulPostFilterHDR2* pHDR = 0;
CSulPostFilterBrightPass* pBrightPass = 0;
CSulScreenAlignedQuad* pQuad = 0; // quad our texture is rendered too

void createHDR( const std::string& sVal="hdr" )
{
	pHDR = new CSulPostFilterHDR2;
	pHDR->setView( sVal );
	pHDR->in( pRTT->getTexture() );
	osg::Texture2D* pTex = pHDR->out();
	pGroup->addChild( pHDR );
	pQuad->setTexture( pHDR->out() );

	// set HDR values based on menu values
	if ( pMenu )
	{
		float fBrightPass = ((CSulDebugValueFloat*)pMenu->getValueObj( "brightpass" ))->getVal();
		float fExposure = ((CSulDebugValueFloat*)pMenu->getValueObj( "exposure" ))->getVal();
		float fFactor = ((CSulDebugValueFloat*)pMenu->getValueObj( "factor" ))->getVal();
		float fMax = ((CSulDebugValueFloat*)pMenu->getValueObj( "max" ))->getVal();
		pHDR->getShaderBrightPass()->setThreshold( fBrightPass );
	
		if ( pHDR->getShaderCombine() )
		{
			pHDR->getShaderCombine()->setExposure( fExposure );
			pHDR->getShaderCombine()->setFactor( fFactor );
			pHDR->getShaderCombine()->setMax( fMax );
		}
	}
}

class CDebugMenu : public CSulDebugMenu
{
public:
	CDebugMenu()
	{
		add(
			new CSulDebugValueFloat(
				"amplifier",	// title
				-10.0f, 10.0f,	// min, max
				1.0f,			// default
				0.1f			// step
			));

		add(
			new CSulDebugValueFloat(
				"brightpass",	// title
				0.0f, 10.0f,	// min, max
				1.2f,			// default
				0.1f			// step
			));

		add(
			new CSulDebugValueFloat(
				"exposure",		// title
				0.0f, 10.0f,	// min, max
				1.0f,			// default
				0.01f			// step
			));

		add(
			new CSulDebugValueFloat(
				"factor",		// title
				0.0f, 100.0f,	// min, max
				1.0f,			// default
				0.1f			// step
			));

		add(
			new CSulDebugValueFloat(
				"max",			// title
				0.0f, 100.0f,	// min, max
				1.0f,			// default
				0.1f			// step
			));

		add(
			new CSulDebugValueText(
				"view",				// title
				"hdr,brightpass",	// values
				"hdr"				// default
			));
	}

	virtual void valueChanged( CSulDebugValueBase* pObj )
	{
		CSulString sTitle = pObj->getTitle();

		if ( sTitle=="amplifier" )
		{
			if ( m_amp )
			{
				m_amp->setAmp( ((CSulDebugValueFloat*)pObj)->getVal() );
			}
		}

		if ( sTitle=="brightpass" )
		{
			float val = ((CSulDebugValueFloat*)pObj)->getVal();
			pHDR->getShaderBrightPass()->setThreshold( val );
		}

		if ( sTitle=="exposure" )
		{
			if ( pHDR->getShaderCombine() )
			{
				float val = ((CSulDebugValueFloat*)pObj)->getVal();
				pHDR->getShaderCombine()->setExposure( val );
			}
		}

		if ( sTitle=="factor" )
		{
			float val = ((CSulDebugValueFloat*)pObj)->getVal();
			pHDR->getShaderCombine()->setFactor( val );
		}

		if ( sTitle=="max" )
		{
			float val = ((CSulDebugValueFloat*)pObj)->getVal();
			pHDR->getShaderCombine()->setMax( val );
		}

		if ( sTitle=="view" )
		{
			CSulString sVal = ((CSulDebugValueText*)pObj)->getText();

			pGroup->removeChild( pHDR );
			createHDR( sVal );
		}
	}

};

osg::Node* CreateDebugMenu()
{
	pMenu = new CDebugMenu;
	return pMenu;
}

osg::Node* CreateScene( osgViewer::Viewer* pViewer )
{
	pGroup = new osg::Group;

	//////////////////////////////////////////////////////////////////
	// create scene basics
	//////////////////////////////////////////////////////////////////

	osg::Group* pScene = new osg::Group;

	osg::Geode* pGeode = new osg::Geode();

	// we create the simplest form of shapes in OpenSceneGraph
    pGeode->addDrawable( new osg::ShapeDrawable( new osg::Sphere(osg::Vec3(0.0f,0.0f,0.0f),0.5f) ) );
    pGeode->addDrawable( new osg::ShapeDrawable( new osg::Box(osg::Vec3(2.0f,0.0f,0.0f),2.0f) ) );
    pGeode->addDrawable( new osg::ShapeDrawable( new osg::Cone(osg::Vec3(4.0f,0.0f,0.0f),0.5f,3.0f) ) );
    pGeode->addDrawable( new osg::ShapeDrawable( new osg::Cylinder(osg::Vec3(6.0f,0.0f,0.5f),0.5f,3.0f) ) );
    pGeode->addDrawable( new osg::ShapeDrawable( new osg::Capsule(osg::Vec3(8.0f,0.0f,1.0f),0.5f,3.0f) ) );
	pScene->addChild( pGeode );
	pGroup->addChild( pScene );

	//////////////////////////////////////////////////////////////////
	// create a background image
	//////////////////////////////////////////////////////////////////

	CSulScreenAlignedQuad* pBG = new CSulScreenAlignedQuad(
		osg::Vec3(400,300,0),
		800, 600,
		800, 600
	);


	pBG->getProjection()->getOrCreateStateSet()->setRenderBinDetails( -100, "RenderBin" );
//pBG->setTexture( "sunset.jpg", GL_RGBA16F_ARB );
//pBG->setTexture( "memorial.hdr", GL_RGBA16F_ARB );
pBG->setTexture( "vinesunset.hdr", GL_RGBA16F_ARB );
//pBG->setTexture( "grove.hdr", GL_RGBA16F_ARB );
//pBG->setTexture( "groveC.hdr", GL_RGBA16F_ARB );
//pBG->setTexture( "test.hdr", GL_RGBA16F_ARB );

	//pBG->getQuad()->getTexture()->setInternalFormat( GL_RGBA16F_ARB );
	pScene->addChild( pBG->getProjection() );

	// add amplifier shader
	m_amp = new CSulShaderAmplifier( pBG->getProjection(), 1.0f, false );

	//////////////////////////////////////////////////////////////////
	// we create a RTT that is going to be our HDR view 	
	//////////////////////////////////////////////////////////////////

	pRTT = new CSulTexCam( 400, 300, CSulTexCam::HDR );
	pRTT->addChild(	pScene );
	pGroup->addChild( pRTT );
	
/*
	CSulRTT* pRTT = new CSulRTT( 300, 200, CSulRTT::SETUP_HDR );
	pGroup->addChild( pRTT->getCamera() );
	pRTT->AddChildToCamera( pScene );
*/

/*
	// postfilter bloom
	CSulPostFilterBloom* pBloom = new CSulPostFilterBloom;
	pBloom->in( pRTT->getTexture(), 32 );
	osg::Texture2D* pTex = pBloom->out();
	pGroup->addChild( pBloom );
*/

	// postfilter hdr
/*
	pHDR = new CSulPostFilterHDR;
	pHDR->in( pRTT->getTexture(), 32 );
	osg::Texture2D* pTex = pHDR->out();
	pGroup->addChild( pHDR );
*/

//	osg::Texture2D* pTex = pRTT->getTexture();

	pQuad= new CSulScreenAlignedQuad(
		osg::Vec3(200,150,0),
		400, 300,
		800, 600
	);
//	pQuad->setTexture( pTex );
	pGroup->addChild( pQuad->getProjection() );

	pRTT->setUpdateCallback( new CSulNodeCallbackCameraSync( pViewer->getCamera() ) );

	createHDR();

	return pGroup;
}


int _tmain(int argc, _TCHAR* argv[])
{
	osgDB::FilePathList filepath;

	char* ptr = getenv( "SIGMAOSG_DATA_PATH" );
	if ( ptr )
	{
		osgDB::convertStringPathIntoFilePathList( ptr, filepath );
		osgDB::appendPlatformSpecificResourceFilePaths(filepath);
		osgDB::setDataFilePathList(filepath);
	}

    // construct the viewer
    osg::ref_ptr<osgViewer::Viewer> rViewer = new osgViewer::Viewer;

	rViewer->setThreadingModel( osgViewer::ViewerBase::SingleThreaded );

    rViewer->setUpViewInWindow( 32, 32, 800, 600 );

   // add the handler to the viewer
	CreateDebugMenu();
    osg::ref_ptr<CSulDebugMenuKeyboardHandler> rKeyboardHandler = new CSulDebugMenuKeyboardHandler( pMenu );
    rViewer->addEventHandler( rKeyboardHandler.get() );

    // set the scene-graph data the viewer will render
	osg::Group* pGroupRoot = new osg::Group;
	pGroupRoot->addChild( CreateScene( rViewer ) );
    rViewer->setSceneData( pGroupRoot );

	rViewer->setCameraManipulator( new osgGA::TrackballManipulator );

	pGroupRoot->addChild( pMenu );

    // execute main loop
    return rViewer->run();
}

