// SulTextureBuffer.cpp

#include "stdafx.h"
#include "SulTextureBuffer.h"

CSulTextureBuffer::CSulTextureBuffer( sigma::uint32 texWidth, sigma::uint32 texHeight, sigma::uint32 numBuffers ) :
m_numBuf( numBuffers ),
m_curBuf( 0 ),
m_texFormatInternal( GL_RGBA16F_ARB ),
m_texFormatSource( GL_RGBA ),
m_texSourceType( GL_FLOAT )
{
	for ( sigma::uint32 i=0; i<m_numBuf; i++ )
	{
		osg::Texture2D* pTex = createTexture( texWidth, texHeight );
		m_vecTex.push_back( pTex );
	}

	m_texIn = m_vecTex[0];
	m_texOut = numBuffers>1?m_vecTex[1]:m_vecTex[0];
}

osg::Texture2D* CSulTextureBuffer::createTexture( unsigned short w, unsigned short h )
{
	osg::Texture2D* texture = new osg::Texture2D;
	texture->setTextureSize( w, h );
	texture->setWrap( osg::Texture::WRAP_S, osg::Texture::CLAMP_TO_EDGE );
    texture->setWrap( osg::Texture::WRAP_T, osg::Texture::CLAMP_TO_EDGE );
	texture->setSourceFormat( m_texFormatSource );
	texture->setInternalFormat( m_texFormatInternal );
	texture->setSourceType( m_texSourceType );
	texture->setFilter( osg::Texture2D::MIN_FILTER,osg::Texture2D::LINEAR );
	texture->setFilter( osg::Texture2D::MAG_FILTER,osg::Texture2D::LINEAR );

	return texture;
}

void CSulTextureBuffer::setIn( CSulTextureBuffer* texBuf )
{
	m_texIn = texBuf->getOut();
}

void CSulTextureBuffer::setIn( osg::Texture2D* texIn )
{
	m_texIn = texIn;
}

void CSulTextureBuffer::setOut( CSulTextureBuffer* texOut )
{
	m_texOut = texOut->getOut();
}

void CSulTextureBuffer::setOut( osg::Texture2D* texOut )
{
	m_texOut = texOut;
}

osg::Texture2D*	CSulTextureBuffer::getIn()
{
	return m_texIn;
}

osg::Texture2D*	CSulTextureBuffer::getOut()
{
	return m_texOut;
}

void CSulTextureBuffer::swap()
{
	if ( m_numBuf==1 )
	{
		return;
	}

	m_curBuf++;
	if ( m_curBuf==m_numBuf )
	{
		m_curBuf = 0;
	}

	m_texIn		= m_vecTex[m_curBuf];
	m_texOut	= m_vecTex[(m_curBuf+1)%m_numBuf];
}