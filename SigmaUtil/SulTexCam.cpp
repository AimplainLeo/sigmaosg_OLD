// SulTexCam.cpp

#include "stdafx.h"
#include "SulTexCam.h"

CSulTexCam::CSulTexCam( Sigma::uint32 w, Sigma::uint32 h, ESETUP eSetup ) :
m_w( w ),
m_h( h ),
m_eSetup( eSetup )
{
	initTex();
	initCam();
}

CSulTexCam::CSulTexCam( osg::Texture2D* pTex, ESETUP eSetup ) :
m_w( pTex->getTextureWidth() ),
m_h( pTex->getTextureHeight()),
m_eSetup( eSetup )
{
	m_rTex = pTex;
	initCam();
}

void CSulTexCam::setTexture( osg::Texture2D* pTex )
{
	m_rTex = pTex;
	m_w = pTex->getTextureWidth();
	m_h = pTex->getTextureHeight();
}

void CSulTexCam::initTex()
{
	switch ( m_eSetup )
	{
		case STANDARD:
			{
				m_rTex = new osg::Texture2D;
				m_rTex->setTextureSize( m_w, m_h );
				m_rTex->setWrap( osg::Texture::WRAP_S, osg::Texture::CLAMP_TO_EDGE );
				m_rTex->setWrap( osg::Texture::WRAP_T, osg::Texture::CLAMP_TO_EDGE );
				m_rTex->setSourceFormat( GL_RGB );
				m_rTex->setInternalFormat( GL_RGB );
				m_rTex->setSourceType( GL_UNSIGNED_BYTE );
				m_rTex->setFilter( osg::Texture2D::MIN_FILTER,osg::Texture2D::LINEAR );
				m_rTex->setFilter( osg::Texture2D::MAG_FILTER,osg::Texture2D::LINEAR );
			}
			break;

		// NOTE: the 16F is probably wrong? 32 might be more correct (need some testing)
		case HDR:
			{
				m_rTex = new osg::Texture2D;
				m_rTex->setTextureSize( m_w, m_h );
				m_rTex->setWrap( osg::Texture::WRAP_S, osg::Texture::CLAMP_TO_EDGE );
				m_rTex->setWrap( osg::Texture::WRAP_T, osg::Texture::CLAMP_TO_EDGE );
				m_rTex->setSourceFormat( GL_RGB );
				m_rTex->setInternalFormat( GL_RGB16F_ARB );
				m_rTex->setSourceType( GL_FLOAT );
				m_rTex->setFilter( osg::Texture2D::MIN_FILTER,osg::Texture2D::LINEAR );
				m_rTex->setFilter( osg::Texture2D::MAG_FILTER,osg::Texture2D::LINEAR );
			}
			break;

		case ZVALUE_WITH_IMAGE:
			{
				m_rImage = new osg::Image;
				m_rImage->allocateImage( m_w, m_h, 1,  GL_RGB, GL_FLOAT );	
				m_rImage->setInternalTextureFormat( GL_RGB32F_ARB );

				m_rTex = new osg::Texture2D( m_rImage );
			}
			break;		

		default:
			assert( 0 ); // not supported yet	
	}
}

void CSulTexCam::initCam()
{
	switch ( m_eSetup )
	{
		case STANDARD:
		case HDR:
			{
				setRenderOrder( osg::Camera::PRE_RENDER );
				setRenderTargetImplementation( osg::Camera::FRAME_BUFFER_OBJECT );	// this will render only to the texture
				attach( osg::Camera::COLOR_BUFFER, m_rTex );
				setName( "CSulTexCam" );
				setClearColor( osg::Vec4( 0.0f, 0.0f, 0.0f, 1.0f ) );
				setReferenceFrame( osg::Transform::ABSOLUTE_RF );
				setViewport( 0, 0, m_w, m_h );
			}
			break;

		case ZVALUE_WITH_IMAGE:
			{
				setRenderOrder( osg::Camera::PRE_RENDER );
				setRenderTargetImplementation( osg::Camera::FRAME_BUFFER_OBJECT );	// this will render only to the texture
				attach( osg::Camera::COLOR_BUFFER, m_rImage );
				setName( "CSulTexCam" );
				setClearColor( osg::Vec4( 0.0f, 0.0f, 0.0f, 1.0f ) );
				setReferenceFrame( osg::Transform::ABSOLUTE_RF );
				setViewport( 0, 0, m_w, m_h );
			}
			break;

		default:
			assert( 0 ); // not supported yet
	}
}

osg::Texture2D* CSulTexCam::getTexture()
{
	return m_rTex;
}

osg::Image* CSulTexCam::getImage()
{
	return m_rImage;
}
