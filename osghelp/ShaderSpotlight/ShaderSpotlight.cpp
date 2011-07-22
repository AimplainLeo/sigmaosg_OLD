// ShaderSpotlight.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <osgViewer/Viewer>
#include <osg/ShapeDrawable>
#include <osgDB/FileUtils>
 
osg::Node* createScene()
{
	osg::Geode* pGeode = new osg::Geode();
    pGeode->addDrawable( new osg::ShapeDrawable( new osg::Box(osg::Vec3(0.0f,0.0f,-2.0f),100.0f,100.0f,2.0f) ) );

    pGeode->addDrawable( new osg::ShapeDrawable( new osg::Sphere(osg::Vec3(0.0f,0.0f,0.0f),11.0f) ) );
    pGeode->addDrawable( new osg::ShapeDrawable( new osg::Box(osg::Vec3(20.0f,0.0f,0.0f),20.0f) ) );
    pGeode->addDrawable( new osg::ShapeDrawable( new osg::Cone(osg::Vec3(40.0f,0.0f,0.0f),5.0f,30.0f) ) );
    pGeode->addDrawable( new osg::ShapeDrawable( new osg::Cylinder(osg::Vec3(60.0f,0.0f,0.5f),5.0f,30.0f) ) );
    pGeode->addDrawable( new osg::ShapeDrawable( new osg::Capsule(osg::Vec3(80.0f,0.0f,1.0f),5.0f,30.0f) ) );


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

	pGeode->getOrCreateStateSet()->setAttributeAndModes( rPrg.get(), osg::StateAttribute::ON );

	// setup up a test light
	osg::StateSet* ss = pGeode->getOrCreateStateSet();

	/*
	ss->addUniform( new osg::Uniform( "light_pos", osg::Vec3(0,25,20) ) );
	ss->addUniform( new osg::Uniform( "light_diffuse", osg::Vec4(0,1,0,1) ) );
	ss->addUniform( new osg::Uniform( "light_attConstant", 0.0f ) );
	ss->addUniform( new osg::Uniform( "light_attLinear", 0.09f ) );
	ss->addUniform( new osg::Uniform( "light_attQuadratic", 0.0f ) );
*/

	// we need to use setElement when working with arrays in GLSL

	osg::ref_ptr<osg::Uniform> rPos				= new osg::Uniform( osg::Uniform::FLOAT_VEC3,	"light_pos",			10 );
	osg::ref_ptr<osg::Uniform> rDiffuse			= new osg::Uniform( osg::Uniform::FLOAT_VEC4,	"light_diffuse",		10 );
	osg::ref_ptr<osg::Uniform> rAttConstant		= new osg::Uniform( osg::Uniform::FLOAT,		"light_attConstant",	10 );
	osg::ref_ptr<osg::Uniform> rAttLinear		= new osg::Uniform( osg::Uniform::FLOAT,		"light_attLinear",		10 );
	osg::ref_ptr<osg::Uniform> rAttQuadratic	= new osg::Uniform( osg::Uniform::FLOAT,		"light_attQuadratic",	10 );
	osg::ref_ptr<osg::Uniform> rDir				= new osg::Uniform( osg::Uniform::FLOAT_VEC3,	"light_direction",		10 );
	osg::ref_ptr<osg::Uniform> rSpotCutOff		= new osg::Uniform( osg::Uniform::FLOAT,		"light_spotCutOff",		10 );

	ss->addUniform( rPos );
	ss->addUniform( rDiffuse );
	ss->addUniform( rAttConstant );
	ss->addUniform( rAttLinear );
	ss->addUniform( rAttQuadratic );
	ss->addUniform( rDir );
	ss->addUniform( rSpotCutOff );

	rPos->setElement(			0, osg::Vec3(0,0,20) );
	rDiffuse->setElement(		0, osg::Vec4(0,1,0,1) );
	rAttConstant->setElement(	0, 0.0f );
	rAttLinear->setElement(		0, 0.2f );
	rAttQuadratic->setElement(	0, 0.0f );
	rDir->setElement(			0, osg::Vec3f(0,0,-1) );
	rSpotCutOff->setElement(	0, (float)cos( osg::DegreesToRadians(60.0) ) );


	rPos->setElement(			1, osg::Vec3(20,20,20) );
	rDiffuse->setElement(		1, osg::Vec4(1,0,0,1) );
	rAttConstant->setElement(	1, 0.0f );
	rAttLinear->setElement(		1, 0.2f );
	rAttQuadratic->setElement(	1, 0.0f );
	rDir->setElement(			1, osg::Vec3f(0,0,-1) );
	rSpotCutOff->setElement(	1, (float)cos( osg::DegreesToRadians(60.0) ) );


	/*
	ss->addUniform( new osg::Uniform( "light_pos", osg::Vec3(0,0,15) ) );
	ss->addUniform( new osg::Uniform( "light_diffuse", osg::Vec4(0,1,0,1) ) );
	ss->addUniform( new osg::Uniform( "light_attConstant", 0.0f ) );
	ss->addUniform( new osg::Uniform( "light_attLinear", 0.2f ) );
	ss->addUniform( new osg::Uniform( "light_attQuadratic", 0.0f ) );

	ss->addUniform( new osg::Uniform( "light_direction", osg::Vec3f(0,0,-1) ) );
	ss->addUniform( new osg::Uniform( "light_spotCutOff", (float)cos( osg::DegreesToRadians(60.0) ) ) );
	*/
	


	ss->addUniform( new osg::Uniform( "countLights", (int)2 ) );


	return pGeode;
}

int _tmain(int argc, _TCHAR* argv[])
{
    // construct the viewer
    osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer;

    // make the viewer create a 512x512 window and position it at 32, 32
    viewer->setUpViewInWindow( 32, 32, 800, 600 );

    // set the scene-graph data the viewer will render
    viewer->setSceneData( createScene() );

    // execute main loop
    return viewer->run();
}

