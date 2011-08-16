// ShaderSpotlight.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <SigmaUtil/SulUniformArrayHack.h>
#include <osgViewer/Viewer>
#include <osg/ShapeDrawable>
#include <osgDB/FileUtils>
#include <osg/MatrixTransform>

#include <SigmaUtil/SulGeomCone.h>
#include <SigmaUtil/SulGeomAxis.h>

osg::Vec3 pos( 0, 0, 20 );
osg::Vec3 target( 20, 20, -20 );
float angle = 60.0f;
float coneLen = 30.0f;

osg::Group* createScene()
{
	osg::Group* pScene = new osg::Group;

	osg::Geode* pGeode = new osg::Geode();
	pScene->addChild( pGeode );
    pGeode->addDrawable( new osg::ShapeDrawable( new osg::Box(osg::Vec3(0.0f,0.0f,-2.0f),100.0f,100.0f,2.0f) ) );

/*
    pGeode->addDrawable( new osg::ShapeDrawable( new osg::Sphere(osg::Vec3(0.0f,0.0f,0.0f),11.0f) ) );
    pGeode->addDrawable( new osg::ShapeDrawable( new osg::Box(osg::Vec3(20.0f,0.0f,0.0f),20.0f) ) );
    pGeode->addDrawable( new osg::ShapeDrawable( new osg::Cone(osg::Vec3(40.0f,0.0f,0.0f),5.0f,30.0f) ) );
    pGeode->addDrawable( new osg::ShapeDrawable( new osg::Cylinder(osg::Vec3(60.0f,0.0f,0.5f),5.0f,30.0f) ) );
    pGeode->addDrawable( new osg::ShapeDrawable( new osg::Capsule(osg::Vec3(80.0f,0.0f,1.0f),5.0f,30.0f) ) );
*/

	// fragment shader to use
	osg::ref_ptr<osg::Shader> f = new osg::Shader( osg::Shader::FRAGMENT );
	f->loadShaderSourceFromFile( osgDB::findDataFile("shaders/spotlight.frag") );

	// vertex shader to use
	osg::ref_ptr<osg::Shader> v = new osg::Shader( osg::Shader::VERTEX );
	v->loadShaderSourceFromFile( osgDB::findDataFile("shaders/spotlight.vert") );

	osg::ref_ptr<osg::Program> rPrg = new osg::Program;
	rPrg->setName( "myShader" );
	rPrg->addShader( f.get() );
	rPrg->addShader( v.get() );

	osg::StateSet* ss = pScene->getOrCreateStateSet();

	ss->setAttributeAndModes( rPrg.get(), osg::StateAttribute::ON );

	// setup up a test light
	CSulUniformArrayHack* rPos			= new CSulUniformArrayHack( "light_pos",			2, osg::Uniform::FLOAT_VEC3, ss );
	CSulUniformArrayHack* rDiffuse		= new CSulUniformArrayHack( "light_diffuse",		2, osg::Uniform::FLOAT_VEC4, ss );
	CSulUniformArrayHack* rAttConstant	= new CSulUniformArrayHack( "light_attConstant",	2, osg::Uniform::FLOAT, ss );
	CSulUniformArrayHack* rAttLinear	= new CSulUniformArrayHack( "light_attLinear",		2, osg::Uniform::FLOAT, ss );
	CSulUniformArrayHack* rAttQuadratic	= new CSulUniformArrayHack( "light_attQuadratic",	2, osg::Uniform::FLOAT, ss );
	CSulUniformArrayHack* rDir			= new CSulUniformArrayHack( "light_direction",		2, osg::Uniform::FLOAT_VEC3, ss );
	CSulUniformArrayHack* rSpotCutOff	= new CSulUniformArrayHack( "light_spotCutOff",		2, osg::Uniform::FLOAT, ss );
	CSulUniformArrayHack* rType			= new CSulUniformArrayHack( "light_type",			2, osg::Uniform::INT, ss );

	osg::Vec3 dirTest = target-pos;
	dirTest.normalize();

	rPos->setElement(			0, osg::Vec3(0,0,20) );
	rDiffuse->setElement(		0, osg::Vec4(2,2,2,1) );
	rAttConstant->setElement(	0, 1.0f );
	rAttLinear->setElement(		0, 0.2f );
	rAttQuadratic->setElement(	0, 0.0f );
	rDir->setElement(			0, /*osg::Vec3f(0,0,-1)*/ dirTest );
	rSpotCutOff->setElement(	0, (float)cos( osg::DegreesToRadians(angle) ) );
	rType->setElement(			0, 1 );

	/*
	rPos->setElement(			1, osg::Vec3(20,20,20) );
	rDiffuse->setElement(		1, osg::Vec4(2,0,0,1) );
	rAttConstant->setElement(	1, 1.0f );
	rAttLinear->setElement(		1, 0.2f );
	rAttQuadratic->setElement(	1, 0.0f );
	rDir->setElement(			1, osg::Vec3f(0,0,-1) );
	rSpotCutOff->setElement(	1, (float)cos( osg::DegreesToRadians(60.0) ) );
	rType->setElement(			1, 1 );
*/

	ss->addUniform( new osg::Uniform( "countLights", (int)1 ) );

	return pScene;
}

int _tmain(int argc, _TCHAR* argv[])
{
    // construct the viewer
    osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer;

    // make the viewer create a 512x512 window and position it at 32, 32
    viewer->setUpViewInWindow( 32, 32, 800, 600 );

    // set the scene-graph data the viewer will render
	osg::Group* pGroup = new osg::Group;
	pGroup->addChild( createScene() );
	


	// create a cone
	
	// calc angle to radius 

	float radius = tan( osg::DegreesToRadians(angle) ) * coneLen;

	osg::ref_ptr<CSulGeomCone> rCone = new CSulGeomCone( coneLen, 1.0f, radius );
	rCone->enableBlend();
	rCone->setColorBottom( osg::Vec4( 1, 1, 1, 0 ) );
	rCone->setColorTop( osg::Vec4( 1, 1, 1, 1 ) );
	

	osg::ref_ptr<osg::MatrixTransform> rMT = new osg::MatrixTransform;
	osg::Matrix m;
	m.makeLookAt( pos, pos - osg::Vec3(target-pos), osg::Z_AXIS );
	m = osg::Matrix::inverse(m);
	rMT->setMatrix( m  );
	rMT->addChild( rCone );
	rMT->addChild( new CSulGeomAxis(10.0f) );
	pGroup->addChild( rMT );


    viewer->setSceneData( pGroup );

    // execute main loop
    return viewer->run();
}

