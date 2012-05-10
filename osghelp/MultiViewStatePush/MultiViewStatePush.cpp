// MultiViewStatePush.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <SigmaUtil/SulRTT.h>
#include <SigmaUtil/SulScreenAlignedQuad.h>
#include <SigmaUtil/SulNodeCallbackCameraSync.h>
#include <SigmaUtil/SulLightManager.h>
#include <SigmaUtil/SulWorkload.h>
#include <osgViewer/Viewer>
#include <osg/ShapeDrawable>
#include <osgDB/FileUtils>
#include <osgViewer/ViewerEventHandlers>
#include <assert.h>

osg::Node* createScene( osgViewer::Viewer* viewer )
{
	// no shader on root
	osg::Group* root = new osg::Group;

	// simulate workload
	new CSulWorkload( root );

	// group contains lighting shader
	CSulLightManager* lm = new CSulLightManager;
	root->addChild( lm );

	// we create the simplest form of shapes in OpenSceneGraph
	osg::Geode* pGeode = new osg::Geode();
    pGeode->addDrawable( new osg::ShapeDrawable( new osg::Sphere(osg::Vec3(0.0f,0.0f,0.0f),0.5f) ) );
    pGeode->addDrawable( new osg::ShapeDrawable( new osg::Box(osg::Vec3(2.0f,0.0f,0.0f),2.0f) ) );
    pGeode->addDrawable( new osg::ShapeDrawable( new osg::Cone(osg::Vec3(4.0f,0.0f,0.0f),0.5f,3.0f) ) );
    pGeode->addDrawable( new osg::ShapeDrawable( new osg::Cylinder(osg::Vec3(6.0f,0.0f,0.5f),0.5f,3.0f) ) );
    pGeode->addDrawable( new osg::ShapeDrawable( new osg::Capsule(osg::Vec3(8.0f,0.0f,1.0f),0.5f,3.0f) ) );
	lm->addChild( pGeode );
	
	osg::ref_ptr<CSulRTT> cam1 = new CSulRTT(400,200);
	cam1->addChild( lm );
	//cam1->setUpdateCallback( new CSulNodeCallbackCameraSync( viewer->getCamera(), CSulNodeCallbackCameraSync::MODE_SIMPLE_OFFSET ) );
	cam1->setViewMatrixAsLookAt( osg::Vec3(200, 200, 200), osg::Vec3(6.0f,0.0f,2.5f), osg::Z_AXIS );
	root->addChild( cam1 );

	osg::ref_ptr<CSulRTT> cam2 = new CSulRTT(400,200);
	cam2->addChild( lm );
	//cam2->setUpdateCallback( new CSulNodeCallbackCameraSync( viewer->getCamera(), CSulNodeCallbackCameraSync::MODE_SIMPLE_OFFSET ) );
	cam2->setViewMatrixAsLookAt( osg::Vec3(-50, 50, 50), osg::Vec3(6.0f,0.0f,2.5f), osg::Z_AXIS );
	root->addChild( cam2 );

	osg::ref_ptr<CSulScreenAlignedQuad> quad1 = new CSulScreenAlignedQuad( osg::Vec3(150, 100, 0), 300, 200, 800, 600 );
	quad1->setTexture( cam1->getTexture() );
	root->addChild( quad1->getProjection() );

	osg::ref_ptr<CSulScreenAlignedQuad> quad2 = new CSulScreenAlignedQuad( osg::Vec3(650, 100, 0), 300, 200, 800, 600 );
	quad2->setTexture( cam2->getTexture() );
	root->addChild( quad2->getProjection() );

	////////////////////////////////////////////////////////////////////////
	// create ligting stuff here
	////////////////////////////////////////////////////////////////////////

	// create a light over the cylinder
	CSulLightPoint* light = new CSulLightPoint( lm );
	light->setPosition( osg::Vec3(6.0f,0.0f,2.5f) );
	light->setDiffuseColor( osg::Vec4(1,0,0,1) );
	light->setConstantAtt( 0.0005f );
	light->setLinearAtt( 0.0f );
	light->setQuadraticAtt( 0.25f );
	lm->addChild( light );

	////////////////////////////////////////////////////////////////////////
	// add shaders so we can see the light
	////////////////////////////////////////////////////////////////////////

	osg::Program* program = new osg::Program();

	// load main.frag
	osg::ref_ptr< osg::Shader > fragShader = new osg::Shader();
    fragShader->setType( osg::Shader::FRAGMENT );
    fragShader->loadShaderSourceFromFile( osgDB::findDataFile("myshader.frag") );
	program->addShader( fragShader );

	// load main.vert
	osg::ref_ptr< osg::Shader > vertShader = new osg::Shader();
    vertShader->setType( osg::Shader::VERTEX );
    vertShader->loadShaderSourceFromFile( osgDB::findDataFile("myshader.vert") );
	program->addShader( vertShader );

	// load lighting frag
	osg::ref_ptr< osg::Shader > fragShader2 = new osg::Shader();
    fragShader2->setType( osg::Shader::FRAGMENT );
    bool bRet = fragShader2->loadShaderSourceFromFile( osgDB::findDataFile("shaders/sulFuncLightPoint.frag") );
	assert( bRet ); // asserts if file not found
	program->addShader( fragShader2 );

	lm->getOrCreateStateSet()->setAttribute( program, osg::StateAttribute::ON );
	
	return root;
}

int _tmain(int argc, _TCHAR* argv[])
{
    // construct the viewer
    osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer;

    // make the viewer create a 512x512 window and position it at 32, 32
    viewer->setUpViewInWindow( 32, 32, 800, 600 );

    // set the scene-graph data the viewer will render
    viewer->setSceneData( createScene( viewer ) );

//	viewer->setThreadingModel( osgViewer::ViewerBase::SingleThreaded );

	viewer->addEventHandler( new osgViewer::StatsHandler );

    // execute main loop
    return viewer->run();
}

