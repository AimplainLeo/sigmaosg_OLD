// SulTexCam.cpp

#include "stdafx.h"
#include "SulTexCam.h"

CSulTexCam::CSulTexCam( ESETUP eSetup ) :
m_w( 1.0f ),
m_h( 1.0f ),
m_texW( 256 ),
m_texH( 256 ),
m_eSetup( eSetup )
{
	initTex();
	initCam();
}

CSulTexCam::CSulTexCam( osgViewer::Viewer* viewer, ESETUP eSetup ) :
m_w( viewer->getCamera()->getViewport()->width() ),
m_h( viewer->getCamera()->getViewport()->height() ),
m_texW( viewer->getCamera()->getViewport()->width() ),
m_texH( viewer->getCamera()->getViewport()->height() ),
m_eSetup( eSetup )
{
	initTex();
	initCam();
}

CSulTexCam::CSulTexCam( osg::Vec2 wh, ESETUP eSetup ) :
m_w( wh.x() ),
m_h( wh.y() ),
m_texW( wh.x() ),
m_texH( wh.y() ),
m_eSetup( eSetup )
{
	initTex();
	initCam();
}

CSulTexCam::CSulTexCam( sigma::uint32 w, sigma::uint32 h, ESETUP eSetup ) :
m_w( w ),
m_h( h ),
m_texW( w ),
m_texH( h ),
m_eSetup( eSetup )
{
	initTex();
	initCam();
}

CSulTexCam::CSulTexCam( osg::Texture2D* pTex, ESETUP eSetup ) :
m_w( pTex->getTextureWidth() ),
m_h( pTex->getTextureHeight() ),
m_texW( pTex->getTextureWidth() ),
m_texH( pTex->getTextureHeight() ),
m_eSetup( eSetup )
{
	m_vecTex.push_back( pTex );
	initCam();
}

void CSulTexCam::setWidth( sigma::uint32 w )
{
	m_w = w;
	m_texW = w;
}

void CSulTexCam::setHeight( sigma::uint32 h )
{
	m_h = h;
	m_texH = h;
}

void CSulTexCam::setTextureSize( sigma::uint32 w, sigma::uint32 h )
{
	m_texW = w;
	m_texH = h;
}

void CSulTexCam::setTexture( osg::Texture2D* pTex, sigma::uint32 index )
{
	m_vecTex[index] = pTex;
	m_w = pTex->getTextureWidth();
	m_h = pTex->getTextureHeight();
}

sigma::uint32 CSulTexCam::addTexture( osg::Texture2D* pTex )
{
	m_vecTex.push_back( pTex );
	return m_vecTex.size()-1;
}

void CSulTexCam::initTex()
{
	m_vecTex.clear();

	switch ( m_eSetup )
	{
		case STANDARD:
			{
				osg::Texture2D* tex = new osg::Texture2D;
				tex->setResizeNonPowerOfTwoHint( false );
				tex->setTextureSize( m_texW, m_texH );
				tex->setWrap( osg::Texture::WRAP_S, osg::Texture::CLAMP_TO_EDGE );
				tex->setWrap( osg::Texture::WRAP_T, osg::Texture::CLAMP_TO_EDGE );
				tex->setSourceFormat( GL_RGB );
				tex->setInternalFormat( GL_RGB );
				tex->setSourceType( GL_UNSIGNED_BYTE );
				tex->setFilter( osg::Texture2D::MIN_FILTER,osg::Texture2D::LINEAR );
				tex->setFilter( osg::Texture2D::MAG_FILTER,osg::Texture2D::LINEAR );
				m_vecTex.push_back( tex );
			}
			break;

		// NOTE: the 16F is probably wrong? 32 might be more correct (need some testing)
		case HDR:
			{
				osg::Texture2D* tex = new osg::Texture2D;
				tex->setTextureSize( m_texW, m_texH );
				tex->setWrap( osg::Texture::WRAP_S, osg::Texture::CLAMP_TO_EDGE );
				tex->setWrap( osg::Texture::WRAP_T, osg::Texture::CLAMP_TO_EDGE );
				tex->setSourceFormat( GL_RGB );
				tex->setInternalFormat( GL_RGB16F_ARB );
				tex->setSourceType( GL_FLOAT );
				tex->setFilter( osg::Texture2D::MIN_FILTER,osg::Texture2D::LINEAR );
				tex->setFilter( osg::Texture2D::MAG_FILTER,osg::Texture2D::LINEAR );
				m_vecTex.push_back( tex );
			}
			break;

		case ZVALUE_WITH_IMAGE:
			{
				m_rImage = new osg::Image;
				m_rImage->allocateImage( m_texW, m_texH, 1, GL_RGB, GL_FLOAT );	
				m_rImage->setInternalTextureFormat( GL_RGB32F_ARB );

				osg::Texture2D* tex = new osg::Texture2D( m_rImage );
				m_vecTex.push_back( tex );
			}
			break;		

		case ZVALUE_FROM_DEPTH:
			{
				m_rImage = new osg::Image;
				m_rImage->allocateImage( m_texW, m_texH, 1, GL_DEPTH_COMPONENT, GL_FLOAT );	
				m_rImage->setInternalTextureFormat( GL_DEPTH_COMPONENT );
				osg::Texture2D* tex = new osg::Texture2D( m_rImage );
				tex->setInternalFormat( GL_DEPTH_COMPONENT );
				m_vecTex.push_back( tex );
			}
			break;

		case DEFERRED_RENDERING:
			{
				// we need 3 textures for 
				// 1. diffuse color
				// 2. normals
				// 3. position

				for ( sigma::uint32 i=0; i<3; i++ )
				{
					osg::Texture2D* tex = new osg::Texture2D;
					tex->setResizeNonPowerOfTwoHint( false );
					tex->setTextureSize( m_texW, m_texH );
					tex->setWrap( osg::Texture::WRAP_S, osg::Texture::CLAMP_TO_EDGE );
					tex->setWrap( osg::Texture::WRAP_T, osg::Texture::CLAMP_TO_EDGE );
					tex->setInternalFormat( GL_RGBA32F_ARB );
					tex->setSourceFormat( GL_RGBA );
					tex->setSourceType( GL_FLOAT );
					tex->setFilter( osg::Texture2D::MIN_FILTER,osg::Texture2D::NEAREST );
					tex->setFilter( osg::Texture2D::MAG_FILTER,osg::Texture2D::NEAREST );
					m_vecTex.push_back( tex );
				}
			}
			break;

		case ORTHO:
			{
				osg::Texture2D* tex = new osg::Texture2D;
				tex->setResizeNonPowerOfTwoHint( false );
				tex->setTextureSize( m_texW, m_texH );
				tex->setWrap( osg::Texture::WRAP_S, osg::Texture::CLAMP_TO_EDGE );
				tex->setWrap( osg::Texture::WRAP_T, osg::Texture::CLAMP_TO_EDGE );
				/*
				tex->setSourceFormat( GL_RGB );
				tex->setInternalFormat( GL_RGB );
				tex->setSourceType( GL_UNSIGNED_BYTE );
				*/
					tex->setInternalFormat( GL_RGBA32F_ARB );
					tex->setSourceFormat( GL_RGBA );
					tex->setSourceType( GL_FLOAT );

				tex->setFilter( osg::Texture2D::MIN_FILTER,osg::Texture2D::NEAREST );
				tex->setFilter( osg::Texture2D::MAG_FILTER,osg::Texture2D::NEAREST );
				m_vecTex.push_back( tex );
			}
			break;

		default:
			assert( 0 ); // not supported yet	
	}
}

void CSulTexCam::initCam()
{
	setViewport( 0, 0, m_texW, m_texH );
	setProjectionMatrix(osg::Matrix::identity());
	setViewMatrix(osg::Matrix::identity());
	setReferenceFrame( osg::Transform::RELATIVE_RF );

	switch ( m_eSetup )
	{
		case STANDARD:
		case HDR:
			{
				setRenderOrder( osg::Camera::PRE_RENDER );
				setRenderTargetImplementation( osg::Camera::FRAME_BUFFER_OBJECT );	// this will render only to the texture
				attach( osg::Camera::COLOR_BUFFER, getTexture() );
				setName( "CSulTexCam" );
				setClearColor( osg::Vec4( 0.0f, 0.0f, 0.0f, 1.0f ) );
			}
			break;

		case ZVALUE_WITH_IMAGE:
			{
				setRenderOrder( osg::Camera::PRE_RENDER );
				setRenderTargetImplementation( osg::Camera::FRAME_BUFFER_OBJECT );	// this will render only to the texture
				attach( osg::Camera::COLOR_BUFFER, m_rImage );
				setName( "CSulTexCam" );
				setClearColor( osg::Vec4( 0.0f, 0.0f, 0.0f, 1.0f ) );
			}
			break;

		case ZVALUE_FROM_DEPTH:
			{
				setRenderOrder( osg::Camera::PRE_RENDER );
				setRenderTargetImplementation( osg::Camera::FRAME_BUFFER_OBJECT );
				attach( osg::Camera::DEPTH_BUFFER, m_rImage );
				setName( "CSulTexCam" );
				setClearColor( osg::Vec4( 0.0f, 0.0f, 0.0f, 1.0f ) );
			}
			break;

		case DEFERRED_RENDERING:
			{
				setRenderOrder( osg::Camera::PRE_RENDER );
				setRenderTargetImplementation( osg::Camera::FRAME_BUFFER_OBJECT );
				attach( osg::Camera::COLOR_BUFFER0, getTexture(0) );
				attach( osg::Camera::COLOR_BUFFER1, getTexture(1) );
				attach( osg::Camera::COLOR_BUFFER2, getTexture(2) );
				setName( "CSulTexCam(deferred rendering)" );
				setClearColor( osg::Vec4( 0.0f, 0.0f, 0.0f, 0.0f ) );
				setClearMask( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
			}
			break;

		case ORTHO:
			{
				//setViewport( 0, 0, m_texW*2, m_texH*2 );
				//setViewport( -(int)m_texW/2.0f, -(int)m_texH/2.0f, (int)m_texW/2.0f, (int)m_texH/2.0f );
				setProjectionMatrix( osg::Matrix::identity() );
				setViewMatrix( osg::Matrix::identity() );
				setRenderOrder( osg::Camera::PRE_RENDER );
				setReferenceFrame( osg::Transform::ABSOLUTE_RF );
				setRenderTargetImplementation( osg::Camera::FRAME_BUFFER_OBJECT );
				attach( osg::Camera::COLOR_BUFFER, getTexture() );
				setName( "CSulTexCam(ortho)" );
				setClearColor( osg::Vec4( 1.0f, 0.0f, 0.0f, 1.0f ) );
				setClearMask( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
			}
			break;

		default:
			assert( 0 ); // not supported yet
	}
}

osg::Texture2D* CSulTexCam::getTexture( sigma::uint32 index )
{
	return m_vecTex[index];
}

osg::Image* CSulTexCam::getImage()
{
	return m_rImage;
}

