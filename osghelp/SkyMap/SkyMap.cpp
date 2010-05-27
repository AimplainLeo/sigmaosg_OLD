// SkyMap.cpp : Defines the entry point for the console application.
//

// http://www.daionet.gr.jp/~masa/rthdribl/
// http://www.debevec.org/Probes/

#include "stdafx.h"
#include "DebugMenu.h"
#include <SigmaUtil/SulVerticalCrossMap.h>
#include <SigmaUtil/SulGeomBox.h>
#include <SigmaUtil/SulPostFilterHDR2.h>
#include <SigmaUtil/SulNodeCallbackCameraSync.h>
#include <SigmaUtil/SulScreenAlignedQuad.h>
#include <SigmaUtil/SulDebugMenuKeyboardHandler.h>
#include <SigmaUtil/SulPostFilterStreaking.h>
#include <osgViewer/Viewer>
#include <osg/ShapeDrawable>
#include <osg/TextureCubeMap>
#include <osg/clampcolor>
#include <osg/texgen>
#include <osg/texmat>
#include <osg/texenv>
#include <osgDB/FileUtils>
#include <osgGA/TrackballManipulator>
#include <osgDB/ReadFile>

#define FS (true)

osg::ref_ptr<CSulVerticalCrossMap> texture;

// Update texture matrix for cubemaps
struct TexMatCallback : public osg::NodeCallback
{
public:

    TexMatCallback(osg::TexMat& tm) :
        _texMat(tm)
    {
    }

    virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
    {
        osgUtil::CullVisitor* cv = dynamic_cast<osgUtil::CullVisitor*>(nv);
        if (cv)
        {
            const osg::Matrix& MV = *(cv->getModelViewMatrix());
	        const osg::Matrix R = osg::Matrix::rotate( osg::DegreesToRadians(180.0f), 1.0f,0.0f,0.0f);


            osg::Quat q = MV.getRotate();
            const osg::Matrix C = osg::Matrix::rotate( q.inverse() );

            _texMat.setMatrix( C*R );
        }

        traverse(node,nv);
    }

    osg::TexMat& _texMat;
};

// this is so we can see what the downsample lum looks like
osg::Group* createLumThing( CSulPostFilterHDR2* pHDR )
{
	osg::Group* pGroup = new osg::Group;

	// have a quad to visualize texture
	CSulScreenAlignedQuad* pQuad= new CSulScreenAlignedQuad(
		osg::Vec3(600,150,0),
		400, 300,
		800, 600
	);
	pQuad->setTexture( pHDR->texLum->getOut() );
	pGroup->addChild( pQuad->getProjection() );

	return pGroup;
}


osg::Node* createHDR( osg::Node* pRenderMe, osgViewer::Viewer* pViewer )
{
	osg::Group* pGroup = new osg::Group;

	unsigned int w = FS?800:400;
	unsigned int h = FS?600:300;

	// render scene to texture
	CSulTexCam* pRTT = new CSulTexCam( w, h, CSulTexCam::HDR );
	pRTT->setUpdateCallback( new CSulNodeCallbackCameraSync( pViewer->getCamera() ) );
	pRTT->addChild(	pRenderMe );
	pGroup->addChild( pRTT );

	// hdr
	CSulPostFilterHDR2* pHDR = new CSulPostFilterHDR2;
//	pHDR->setView( "brightpass" );
	pHDR->in( pRTT->getTexture() );
	pGroup->addChild( pHDR );

	// have a quad to visualize texture
	CSulScreenAlignedQuad* pQuad= new CSulScreenAlignedQuad(
		osg::Vec3(w/2,h/2,0),
		w, h,
		800, 600
	);
	pQuad->setTexture( pHDR->out() );
	pGroup->addChild( pQuad->getProjection() );

	// some debug stuff
	osg::ref_ptr<CDebugMenu> rDebugMenu = new CDebugMenu( pHDR );
	osg::ref_ptr<CSulDebugMenuKeyboardHandler> rKeyboardHandler = new CSulDebugMenuKeyboardHandler( rDebugMenu );
    pViewer->addEventHandler( rKeyboardHandler );
	pGroup->addChild( rDebugMenu );

//	pGroup->addChild( createLumThing( pHDR )  );

	return pGroup;
}

osg::Node* createSkyBox()
{
	float extent = 1000.0f;

	CSulGeomBox* pBox = new CSulGeomBox( extent );

	osg::Geode* pGeode = new osg::Geode;
	pGeode->setCullingActive(false);
	pGeode->addDrawable( pBox->getGeometry() );

	osg::StateSet* stateset = new osg::StateSet;
    stateset->setMode(GL_LIGHTING,  osg::StateAttribute::OFF);
    stateset->setMode(GL_CULL_FACE, osg::StateAttribute::OFF);
	stateset->setTextureAttributeAndModes( 0, texture, osg::StateAttribute::ON );
	pGeode->setStateSet(stateset);

	return pGeode;
}

osg::Group* CreateScene( osgViewer::Viewer* pViewer )
{
	texture = new CSulVerticalCrossMap( "uffizi_cross.hdr" );
	//texture = new CSulVerticalCrossMap( "stpeters_cross.hdr" );
//	texture = new CSulVerticalCrossMap( "kitchen_cross.hdr" );
//	texture = new CSulVerticalCrossMap( "galileo_cross.hdr" );

	texture->setFilter( osg::Texture::MIN_FILTER, osg::Texture::LINEAR );
	texture->setFilter( osg::Texture::MAG_FILTER, osg::Texture::LINEAR );
	texture->setWrap(osg::Texture::WRAP_S, osg::Texture::CLAMP_TO_EDGE);
    texture->setWrap(osg::Texture::WRAP_T, osg::Texture::CLAMP_TO_EDGE);
    texture->setWrap(osg::Texture::WRAP_R, osg::Texture::CLAMP_TO_EDGE);

	osg::Group* pGroup = new osg::Group;

	osg::Geode* pGeode = new osg::Geode();

	// we create the simplest form of shapes in OpenSceneGraph
    pGeode->addDrawable( new osg::ShapeDrawable( new osg::Sphere(osg::Vec3(0.0f,0.0f,0.0f),0.5f) ) );
    pGeode->addDrawable( new osg::ShapeDrawable( new osg::Box(osg::Vec3(2.0f,0.0f,0.0f),2.0f) ) );
    pGeode->addDrawable( new osg::ShapeDrawable( new osg::Cone(osg::Vec3(4.0f,0.0f,0.0f),0.5f,3.0f) ) );
    pGeode->addDrawable( new osg::ShapeDrawable( new osg::Cylinder(osg::Vec3(6.0f,0.0f,0.5f),0.5f,3.0f) ) );
    pGeode->addDrawable( new osg::ShapeDrawable( new osg::Capsule(osg::Vec3(8.0f,0.0f,1.0f),0.5f,3.0f) ) );

	pGroup->addChild( pGeode );

	osg::StateSet* stateset = new osg::StateSet;
    stateset->setMode(GL_LIGHTING,  osg::StateAttribute::OFF);
	stateset->setTextureAttributeAndModes(0, texture, osg::StateAttribute::OVERRIDE |osg::StateAttribute::ON);
	pGeode->setStateSet(stateset);


    // texture coordinate generation
    osg::TexGen* tg = new osg::TexGen;
    tg->setMode( osg::TexGen::REFLECTION_MAP );
    stateset->setTextureAttributeAndModes( 0, tg, osg::StateAttribute::OVERRIDE | osg::StateAttribute::ON );

    osg::TexMat *tm = new osg::TexMat;
    stateset->setTextureAttribute(0, tm);


	pGeode->setCullCallback(new TexMatCallback(*tm));


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

    // set the scene-graph data the viewer will render
	osg::Group* pGroupRoot = new osg::Group;

	osg::Group* pScene = CreateScene( rViewer );

	pGroupRoot->addChild( pScene );
	rViewer->setSceneData( pGroupRoot );

	rViewer->setCameraManipulator( new osgGA::TrackballManipulator );

	pScene->addChild( createSkyBox() );    	
	pGroupRoot->addChild( createHDR( pScene, rViewer ) );

	double fovy = 90.0;
	double aspectRatio = 1.0;
    double zNear = 0.001;
	double zFar = 1000.0;

	rViewer->getCamera()->setProjectionMatrixAsPerspective( fovy, aspectRatio, zNear, zFar );

    // execute main loop
    return rViewer->run();

}
