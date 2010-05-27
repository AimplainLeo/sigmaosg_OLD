// ShaderPerPixelLighting.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <osgViewer/Viewer>
#include <osg/ShapeDrawable>
 
osg::Node* CreateScene()
{
	osg::TessellationHints* hints = new osg::TessellationHints;
    hints->setDetailRatio(0.01f);

	// we create a low detailed sphere to test per pixel lighting
	osg::Geode* pGeodePerPixel = new osg::Geode();
    pGeodePerPixel->addDrawable( new osg::ShapeDrawable( new osg::Sphere(osg::Vec3(0.0f,0.0f,0.0f),0.5f), hints ) );

	// we create a low detailed sphere to test normal lighting 
	osg::Geode* pGeode = new osg::Geode();
	pGeode->addDrawable( new osg::ShapeDrawable( new osg::Sphere(osg::Vec3(2.0f,0.0f,0.0f),0.5f), hints ) );

	// fragment shader to use
	osg::ref_ptr<osg::Shader> f = new osg::Shader( osg::Shader::FRAGMENT );
	f->loadShaderSourceFromFile( "perpixellighting.frag" );

	// vertex shader to use
	osg::ref_ptr<osg::Shader> v = new osg::Shader( osg::Shader::VERTEX );
	v->loadShaderSourceFromFile( "perpixellighting.vert" );

	osg::ref_ptr<osg::Program> rPrg = new osg::Program;
	rPrg->setName( "myShader" );
	rPrg->addShader( f.get() );
	rPrg->addShader( v.get() );
	
	pGeode->getOrCreateStateSet()->setAttributeAndModes( rPrg.get(), osg::StateAttribute::ON );

	// add geodes to a common group
	osg::Group* pGroup = new osg::Group;
	pGroup->addChild( pGeodePerPixel );
	pGroup->addChild( pGeode );

	return pGroup;
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


