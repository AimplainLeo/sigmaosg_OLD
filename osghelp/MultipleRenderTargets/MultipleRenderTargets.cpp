// MultipleRenderTargets.cpp : Defines the entry point for the console application.
//

// NOTE: need to change texture coordinates to 0-512 instead of 0-1  .. strange, but that seems to be the way

#include "stdafx.h"
#include <SigmaUtil/SulScreenAlignedQuad.h>
#include <SigmaUtil/SulTexCam.h>
#include <SigmaUtil/SulNodeCallbackCameraSync.h>
#include <osgViewer/Viewer>
#include <osg/ShapeDrawable>
#include <osg/TextureRectangle>
#include <osg/Texture2D>

osg::TextureRectangle* textureRect[4] = {0,0,0,0};

CSulTexCam* pCam = 0;

osg::Group* createScene()
{
	osg::Group* pGroup = new osg::Group;

	osg::Geode* pGeode = new osg::Geode();
    pGeode->addDrawable( new osg::ShapeDrawable( new osg::Box(osg::Vec3(0.0f,0.0f,0.0f),2.0f) ) );
	pGroup->addChild( pGeode );

	return pGroup;
}

CSulTexCam* createMRT( osgViewer::Viewer* pViewer, osg::Group* pRenderMe )
{
    static const char *shaderSource = {
        "void main(void)\n"
        "{\n"
        "    gl_FragData[0] = vec4(1,1,0,1);\n"
        "    gl_FragData[1] = vec4(0,1,0,1);\n"
        "    gl_FragData[2] = vec4(0,0,1,1);\n"
        "    gl_FragData[3] = vec4(0,0,1,1);\n"
        "}\n"
    };
    osg::ref_ptr<osg::Shader> fshader = new osg::Shader( osg::Shader::FRAGMENT, shaderSource );
    osg::ref_ptr<osg::Program> program = new osg::Program;
    program->addShader( fshader.get() );

	osg::Group* pTmp = new osg::Group;
	pTmp->addChild( pRenderMe );
    pTmp->getOrCreateStateSet()->setAttributeAndModes( program.get(), osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE );

	pCam = new CSulTexCam( 512, 512 );
	pCam->addChild( pTmp );
	pCam->setUpdateCallback( new CSulNodeCallbackCameraSync( pViewer->getCamera() ) );
//	pCam->setRenderTargetImplementation( osg::Camera::FRAME_BUFFER_OBJECT );

    for ( int i=0; i<4; i++ ) 
	{
        textureRect[i] = new osg::TextureRectangle;
        textureRect[i]->setTextureSize( 512, 512 );
        textureRect[i]->setInternalFormat( GL_RGBA );
        textureRect[i]->setFilter( osg::Texture2D::MIN_FILTER,osg::Texture2D::LINEAR );
        textureRect[i]->setFilter( osg::Texture2D::MAG_FILTER,osg::Texture2D::LINEAR );

		pCam->attach( osg::Camera::BufferComponent(osg::Camera::COLOR_BUFFER0+i), textureRect[i] );
	}

	return pCam;
}

osg::Group* createShowMRT()
{
	osg::Group* pGroup = new osg::Group;

	osg::ref_ptr<CSulScreenAlignedQuad> rQuad = new CSulScreenAlignedQuad( osg::Vec3(128,128,0), 256, 256, 512, 512 );
	rQuad->setTexture( textureRect[0] );
	//rQuad->setTexture( pCam->getTexture() );
	pGroup->addChild( rQuad->getProjection() );

	return pGroup;
}

int _tmain(int argc, _TCHAR* argv[])
{
	osg::ref_ptr<osgViewer::Viewer> rViewer;

    // construct the viewer
    rViewer = new osgViewer::Viewer;

    // make the viewer create a 512x512 window and position it at 32, 32
    rViewer->setUpViewInWindow( 32, 32, 512, 512 );

	osg::Group* m_rootGroup = new osg::Group;
	osg::Group* scene = createScene();
	m_rootGroup->addChild( scene );

	m_rootGroup->addChild( createMRT( rViewer, scene ) );

	m_rootGroup->addChild( createShowMRT() );

    // set the scene-graph data the viewer will render
    rViewer->setSceneData( m_rootGroup );

    // execute main loop
    return rViewer->run();
}
