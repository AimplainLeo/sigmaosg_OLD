// SulGenerateHeightTexture.cpp

#include "stdafx.h"
#include "SulGenerateHeightTexture.h"
#include <osgDB/FileUtils>
#include <iostream>

CSulGenerateHeightTexture::~CSulGenerateHeightTexture()
{
	if ( m_rRoot.valid() )
	{
		m_rRoot->removeChild( m_rRTT->getCamera() );
	}
}

void CSulGenerateHeightTexture::create( 
	osg::Node* pRenderMe,			// this will be rendered
	osg::Group* pRootGroup,			// need to add generator to scene (so it will work)
	float x,						// center x location of scene (also center location for texture)
	float y,						// center x location of scene (also center location for texture)
	unsigned int	tXY,			// texture size squared
	unsigned int	sXY			// screen size squared
	)
{
	m_rRoot = pRootGroup;

	m_rRTT = new CSulRTT( tXY, tXY, GL_RGBA, GL_FLOAT );
	m_rRTT->getTexture()->setInternalFormat( GL_RGBA32F_ARB );

	/*
	m_rRTT = new CSulRTT( tXY, tXY, GL_LUMINANCE, GL_FLOAT ); 
	m_rRTT->getTexture()->setInternalFormat( GL_LUMINANCE32F_ARB );
	m_rRTT->getTexture()->setSourceType( GL_FLOAT );
	*/
	m_rRTT->getTexture()->setFilter(osg::Texture2D::MIN_FILTER, osg::Texture2D::NEAREST);
	m_rRTT->getTexture()->setFilter(osg::Texture2D::MAG_FILTER, osg::Texture2D::NEAREST);
	m_rRTT->getCamera()->setClearColor( osg::Vec4(0,0,0,1) );
	pRootGroup->addChild( m_rRTT->getCamera() );

	// need to ortho camera
	float l = sXY/2.0f;
	float r = -l;
	float b = l;
	float t = -l;
	m_rRTT->getCamera()->setProjectionMatrixAsOrtho( l, r, b, t, 0.1f, 1000000.0f  );

	// RTT camera should follow main camera
	osg::Matrix mViewMatrix;
	float m[16] =
	{
		 0, 1, 0, 0,
		-1, 0, 0, 0,
		 0, 0, 1, 0,
		 0, 0,-10000, 1
	};
	mViewMatrix.set( m );
	mViewMatrix.setTrans( y, -x, -100 );
	m_rRTT->getCamera()->setViewMatrix( mViewMatrix );

	////////////////////////////////////////////////////////////////////
	// shader to make the render height instead of texture
	////////////////////////////////////////////////////////////////////

	osg::Group* pGroupRenderMe = new osg::Group;
	if ( pRenderMe ) pGroupRenderMe->addChild( pRenderMe );
	getCamera()->addChild( pGroupRenderMe );

	osg::StateSet* ss = pGroupRenderMe->getOrCreateStateSet();

	osg::ref_ptr< osg::Shader > vertexShader = new osg::Shader();
    vertexShader->setType( osg::Shader::VERTEX );
	std::string sVert = osgDB::findDataFile("shaders/height_render.vert");
	if ( sVert.empty() )
	{
		std::cout << "WARNING: CSulGenerateHeightTexture::create -> shader vert not found: " << sVert << std::cout;
	}
	else
	{
		vertexShader->loadShaderSourceFromFile( osgDB::findDataFile("shaders/height_render.vert") );
	}

	osg::ref_ptr< osg::Shader > fragShader = new osg::Shader();
    fragShader->setType( osg::Shader::FRAGMENT );
	std::string sFrag = osgDB::findDataFile("shaders/height_render.frag");
	if ( sFrag.empty() )
	{
		std::cout << "WARNING: CSulGenerateHeightTexture::create -> shader frag not found: " << sFrag << std::cout;
	}
	else
	{
		fragShader->loadShaderSourceFromFile( osgDB::findDataFile("shaders/height_render.frag") );
	}

	osg::ref_ptr< osg::Program > program = new osg::Program();
    program->addShader( vertexShader.get() );
	program->addShader( fragShader.get() );

	osg::Uniform* uniformInverseViewMatrix = new osg::Uniform( "viewInverseMatrixCam", m_rRTT->getCamera()->getInverseViewMatrix() );
    ss->addUniform( uniformInverseViewMatrix );

	ss->setAttribute( program.get(), osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE );


	m_rImage = new osg::Image;

	m_rImage->allocateImage( tXY, tXY, 1,  GL_LUMINANCE, GL_FLOAT );	

	m_rImage->setInternalTextureFormat( GL_LUMINANCE32F_ARB );

	m_rRTT->getCamera()->attach( osg::Camera::COLOR_BUFFER, m_rImage );
}

osg::Camera* CSulGenerateHeightTexture::getCamera()
{
	return m_rRTT->getCamera();
}

osg::Texture2D*	CSulGenerateHeightTexture::getTexture()
{
	return m_rRTT->getTexture();
}
