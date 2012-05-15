// ManySpotLights.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <SigmaUtil/SulRTT.h>
#include <SigmaUtil/SulScreenAlignedQuad.h>
#include <SigmaUtil/SulNodeCallbackCameraSync.h>
#include <SigmaUtil/SulLightManager.h>
#include <SigmaUtil/SulLightSpot.h>
#include <osgViewer/Viewer>
#include <osg/ShapeDrawable>
#include <osgDB/FileUtils>
#include <osg/matrixtransform>
#include <assert.h>


osg::Node* createSpotLight( CSulLightManager* lm, osg::Vec3& pos, osg::Vec3& dir, osg::Vec4& color )
{
	osg::MatrixTransform* mt = new osg::MatrixTransform;

	osg::Matrix mPos;
	mPos.setTrans( pos );

	osg::Matrix mRot;
	mRot.makeRotate( osg::Z_AXIS, dir );

	osg::Matrix m = mRot * mPos;

	mt->setMatrix( m );

	CSulLightSpot* spot = new CSulLightSpot( lm );
	spot->setDirection( osg::Vec3( 0, 0, 1 ) );
	spot->setDiffuseColor( color );
	spot->setConstantAtt( 0.0005f );
	spot->setLinearAtt( 0.0f );
	spot->setQuadraticAtt( 0.25f );
	mt->addChild( spot );

	osg::Geode* pGeode = new osg::Geode();
    pGeode->addDrawable( new osg::ShapeDrawable( new osg::Cylinder(osg::Vec3(0.0f,0.0f,0.0f),0.5f,3.0f) ) );
	mt->addChild( pGeode );

	return mt;
}

osg::Node* createScene( osgViewer::Viewer* viewer )
{
	// light manager sets the uniform counts on this node
	osg::Group* pRoot = new osg::Group;

	CSulLightManager* lm = new CSulLightManager;
	pRoot->addChild( lm );

	lm->addChild( 
		createSpotLight( lm, osg::Vec3(0,0,5), osg::Vec3( 0, 0, -1 ), osg::Vec4(0,1,0,1) )
	);
	
	osg::Geode* pGeode = new osg::Geode();

	// we create the simplest form of shapes in OpenSceneGraph
    pGeode->addDrawable( new osg::ShapeDrawable( new osg::Box(osg::Vec3(0.0f,0.0f,0.0f), 50.0f, 50.0f, 0.1f ) ) );
	lm->addChild( pGeode );

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
    bool bRet = fragShader2->loadShaderSourceFromFile( osgDB::findDataFile("shaders/sulFuncLightSpot.frag") );
	assert( bRet ); // asserts if file not found
	program->addShader( fragShader2 );

	lm->getOrCreateStateSet()->setAttribute( program, osg::StateAttribute::ON );

	return pRoot;
}

int _tmain(int argc, _TCHAR* argv[])
{
    // construct the viewer
    osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer;

    // make the viewer create a 512x512 window and position it at 32, 32
    viewer->setUpViewInWindow( 32, 32, 800, 600 );

    // set the scene-graph data the viewer will render
    viewer->setSceneData( createScene( viewer ) );

    // execute main loop
    return viewer->run();
}

