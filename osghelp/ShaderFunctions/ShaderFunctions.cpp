// ShaderFunctions.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <osgViewer/Viewer>
#include <osg/ShapeDrawable>
 
static const char* funcSource = 
"vec4 NewColor(void)"
"{"
"	vec4 v = vec4(0.0,1.0,0.0,1.0);"
"	return v;"
"}";

static const char* vertSource = 
"void main(void)"
"{"
"	gl_Position = gl_ModelViewProjectionMatrix*gl_Vertex;"
"}";

static const char* fragSource = 
"vec4 NewColor(void);"
""
"void main()"
"{"
"	gl_FragColor = NewColor();"
"}";

osg::Node* CreateScene()
{
	osg::Geode* pGeode = new osg::Geode();

	// we create the simplest form of shapes in OpenSceneGraph
    pGeode->addDrawable( new osg::ShapeDrawable( new osg::Sphere(osg::Vec3(0.0f,0.0f,0.0f),0.5f) ) );
    pGeode->addDrawable( new osg::ShapeDrawable( new osg::Box(osg::Vec3(2.0f,0.0f,0.0f),2.0f) ) );
    pGeode->addDrawable( new osg::ShapeDrawable( new osg::Cone(osg::Vec3(4.0f,0.0f,0.0f),0.5f,3.0f) ) );
    pGeode->addDrawable( new osg::ShapeDrawable( new osg::Cylinder(osg::Vec3(6.0f,0.0f,0.5f),0.5f,3.0f) ) );
    pGeode->addDrawable( new osg::ShapeDrawable( new osg::Capsule(osg::Vec3(8.0f,0.0f,1.0f),0.5f,3.0f) ) );

	// functions to use
	osg::ref_ptr<osg::Shader> func = new osg::Shader( osg::Shader::FRAGMENT );
	func->setShaderSource( funcSource );

	// fragment shader to use
	osg::ref_ptr<osg::Shader> f = new osg::Shader( osg::Shader::FRAGMENT );
	f->setShaderSource( fragSource );

	// vertex shader to use
	osg::ref_ptr<osg::Shader> v = new osg::Shader( osg::Shader::VERTEX );
	v->setShaderSource( vertSource );

	osg::ref_ptr<osg::Program> rPrg = new osg::Program;
	rPrg->setName( "myShader" );
	rPrg->addShader( func.get() );
	rPrg->addShader( f.get() );
	rPrg->addShader( v.get() );
	
	pGeode->getOrCreateStateSet()->setAttributeAndModes( rPrg.get(), osg::StateAttribute::ON );

	return pGeode;
}

int _tmain(int argc, _TCHAR* argv[])
{
    // construct the viewer
    osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer;

    // make the viewer create a 512x512 window and position it at 32, 32
    viewer->setUpViewInWindow( 32, 32, 512, 512 );

    // set the scene-graph data the viewer will render
    viewer->setSceneData( CreateScene() );

    // execute main loop
    return viewer->run();
}