// SulRTT.cpp

#include "stdafx.h"
#include "SulRTT.h"

CSulRTT::CSulRTT( osg::Texture2D* pTex, ESETUP eSetup )
{
	m_rTex = pTex;

	float w = pTex->getTextureWidth();
	float h = pTex->getTextureHeight();

	// cam to render to texture
	m_rCam = new osg::Camera;
	m_rCam->setRenderOrder( osg::Camera::PRE_RENDER );
	m_rCam->setRenderTargetImplementation( osg::Camera::FRAME_BUFFER_OBJECT );
	m_rCam->attach( osg::Camera::COLOR_BUFFER, m_rTex.get() );
	m_rCam->setName( "CSulRTT" );
	m_rCam->setClearColor( osg::Vec4( 0.0f, 0.0f, 0.0f, 1.0f ) );
	m_rCam->setReferenceFrame( osg::Transform::ABSOLUTE_RF );
	m_rCam->setViewport( 0, 0, w, h );
}

CSulRTT::CSulRTT( float w, float h, GLint format, GLenum source_type )
{
	m_w = w;
	m_h = h;
	m_format = format;
	m_source_type = source_type;

	setupStandard();

	m_rCam->setViewport( 0, 0, w, h );
}

CSulRTT::CSulRTT( float w, float h, ESETUP eSetup )
{
	m_w = w;
	m_h = h;

	switch ( eSetup )
	{
		case SETUP_SHADOW: setupShadow(); break;
		
		case SETUP_STANDARD: 
			{
				m_format = GL_RGBA;
				m_source_type = GL_UNSIGNED_BYTE;
				setupStandard(); 
			}
			break;

		case SETUP_HDR:
			{
				m_format = GL_RGBA32F_ARB;
				m_source_type = GL_FLOAT;
				setupStandard(); 
			}
			break;
		
		default:
			assert( 0 ); // not supported yet
	}

	m_rCam->setViewport( 0, 0, w, h );
}

void CSulRTT::setupStandard()
{
	// texture
	m_rTex = new osg::Texture2D;
	m_rTex->setTextureSize( m_w, m_h );
	m_rTex->setWrap( osg::Texture::WRAP_S, osg::Texture::CLAMP_TO_EDGE );
    m_rTex->setWrap( osg::Texture::WRAP_T, osg::Texture::CLAMP_TO_EDGE );
	m_rTex->setSourceFormat( m_format );
	m_rTex->setInternalFormat( m_format );
	m_rTex->setSourceType( m_source_type );
	m_rTex->setFilter( osg::Texture2D::MIN_FILTER,osg::Texture2D::LINEAR );
	m_rTex->setFilter( osg::Texture2D::MAG_FILTER,osg::Texture2D::LINEAR );

	// cam to render to texture
	m_rCam = new osg::Camera;
	m_rCam->setRenderOrder( osg::Camera::PRE_RENDER );
	m_rCam->setRenderTargetImplementation( osg::Camera::FRAME_BUFFER_OBJECT );	// this will render only to the texture
	m_rCam->attach( osg::Camera::COLOR_BUFFER, m_rTex );
	m_rCam->setName( "CSulRTT(standard)" );
	m_rCam->setClearColor( osg::Vec4( 0.0f, 0.0f, 0.0f, 1.0f ) );
	m_rCam->setReferenceFrame( osg::Transform::ABSOLUTE_RF );
}

void CSulRTT::setupShadow()
{
	m_rTex = new osg::Texture2D;
	m_rTex->setTextureSize( m_w, m_h );
	m_rTex->setSourceFormat( GL_DEPTH_COMPONENT );
	m_rTex->setInternalFormat( GL_DEPTH_COMPONENT );
	m_rTex->setShadowComparison( true );
	m_rTex->setShadowTextureMode( osg::Texture::LUMINANCE );
	m_rTex->setFilter( osg::Texture2D::MIN_FILTER,osg::Texture2D::LINEAR );
	m_rTex->setFilter( osg::Texture2D::MAG_FILTER,osg::Texture2D::LINEAR );

	m_rTex->setWrap( osg::Texture2D::WRAP_S,osg::Texture2D::CLAMP_TO_BORDER );
	m_rTex->setWrap( osg::Texture2D::WRAP_T,osg::Texture2D::CLAMP_TO_BORDER );
	m_rTex->setBorderColor( osg::Vec4(1.0f,1.0f,1.0f,1.0f) );

	m_rCam = new osg::Camera;
	m_rCam->setName( "CSulRTT(shadow)" );
	m_rCam->setRenderTargetImplementation( osg::Camera::FRAME_BUFFER_OBJECT );	// this will render only to the texture
	m_rCam->setRenderOrder( osg::Camera::PRE_RENDER );
	m_rCam->setClearMask( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	m_rCam->setClearColor( osg::Vec4(1.0f,1.0f,1.0f,1.0f) );
	m_rCam->setComputeNearFarMode( osg::Camera::DO_NOT_COMPUTE_NEAR_FAR );
	m_rCam->attach( osg::Camera::DEPTH_BUFFER, m_rTex );
//m_rCam->setReferenceFrame( osg::Transform::ABSOLUTE_RF );
}

osg::Texture2D* CSulRTT::getTexture()
{
	return m_rTex.get();
}

osg::Camera* CSulRTT::getCamera()
{
	return m_rCam.get();
}

bool CSulRTT::AddChildToCamera( osg::Node* pNode )
{
	return m_rCam->addChild( pNode );
}

bool CSulRTT::RemoveChildFromCamera( osg::Node* pNode )
{
	return m_rCam->removeChild( pNode );
}