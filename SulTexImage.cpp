// SulTexImage.cpp

#include "stdafx.h"
#include "SulTexImage.h"

CSulTexImage::CSulTexImage( sigma::uint32 w, sigma::uint32 h )
{
	m_rData = new CSulArray2D<sigma::uint8>(w,h);

	m_rTex = new osg::Texture2D;
	
	m_w = w;
	m_h = h;
}

sigma::uint32 CSulTexImage::GetWidth()
{
	return m_w;
}

sigma::uint32 CSulTexImage::GetHeight()
{
	return m_h;
}

osg::Texture2D*	CSulTexImage::getTexture()
{
	return m_rTex.get();
}

void CSulTexImage::Set( float x, float y, float v )
{
	m_rData->Set( x, y, v );
}

/*
void CSulTexImage::Process()
{
	osg::Image* pImage = new osg::Image;
//	pImage->setPixelFormat( GL_ALPHA );
		
    pImage->setImage(
        m_w, m_h, 1,                                                // 1=r? depth perhaps?
		GL_RGB,                                                   // internal format
        GL_LUMINANCE, GL_UNSIGNED_BYTE,                                 // pixelformat, type
        reinterpret_cast<unsigned char*>(m_rData->GetDataPtr()),					// data
        osg::Image::NO_DELETE, 
        1 );                                                        // packing

	m_rTex->setImage( pImage );
 }
 */


void CSulTexImage::Process()
{
	osg::Image* pImage = new osg::Image;
	pImage->setPixelFormat( GL_ALPHA );
		
	// GL_ALPHA8 fails
	// GL_LUMINANCE8 fails
	// GL_INTENSITY fails
	// GL_INTENSITY8 fails

	/*
	unsigned char* p = new unsigned char[m_w*m_h];
	for ( int y=0; y<m_h; y++ )
		for ( int x=0; x<m_w; x++ )
		{
			unsigned char f = ((float)y/m_h)*255.0f;
			p[x+y*m_w] = f;
		}

    pImage->setImage(
        m_w, m_h, 1,                                                // 1=r? depth perhaps?
		GL_ALPHA,                                                   // internal format
        GL_ALPHA, GL_UNSIGNED_BYTE,                                 // pixelformat, type
        p,					// data
        osg::Image::NO_DELETE, 
        1 );                                                        // packing
*/


    pImage->setImage(
        m_w, m_h, 1,                                                // 1=r? depth perhaps?
		GL_ALPHA,                                                   // internal format
        GL_ALPHA, GL_UNSIGNED_BYTE,                                 // pixelformat, type
        reinterpret_cast<unsigned char*>(m_rData->GetDataPtr()),					// data
        osg::Image::NO_DELETE, 
        1 );                                                        // packing

	m_rTex->setImage( pImage );
 }
