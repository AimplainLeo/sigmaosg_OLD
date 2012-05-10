// ManyLights.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "particles.h"
#include <SigmaUtil/SulRTT.h>
#include <SigmaUtil/SulScreenAlignedQuad.h>
#include <SigmaUtil/SulNodeCallbackCameraSync.h>
#include <SigmaUtil/SulLightManager.h>
#include <osgViewer/Viewer>
#include <osg/ShapeDrawable>
#include <osgDB/FileUtils>
#include <assert.h>

osg::Node* createScene( osgViewer::Viewer* viewer )
{
	// light manager sets the uniform counts on this node
	osg::Group* pRoot = new osg::Group;

	CSulLightManager* ml = new CSulLightManager;
	pRoot->addChild( ml );
	
	CMyParticleSystem* p = new CMyParticleSystem( ml );
	p->init();
	osg::MatrixTransform* mtrans = new osg::MatrixTransform;
	mtrans->addChild( p );
	osg::Matrix m;
	m.makeTranslate( 0, 0, 1 );
	mtrans->setMatrix( m ); 
	ml->addChild( mtrans );

	osg::Geode* pGeode = new osg::Geode();

	// we create the simplest form of shapes in OpenSceneGraph
    pGeode->addDrawable( new osg::ShapeDrawable( new osg::Box(osg::Vec3(0.0f,0.0f,0.0f), 50.0f, 50.0f, 0.1f ) ) );
    pGeode->addDrawable( new osg::ShapeDrawable( new osg::Cylinder(osg::Vec3(0.0f,0.0f,0.0f),0.5f,3.0f) ) );
	ml->addChild( pGeode );

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

	ml->getOrCreateStateSet()->setAttribute( program, osg::StateAttribute::ON );

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

