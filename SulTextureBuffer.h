// SulTextureBuffer.h

#ifndef __SULTEXTUREBUFFER_H__
#define __SULTEXTUREBUFFER_H__

#include "SulTypes.h"
#include <osg/referenced>
#include <osg/texture2d>

class CSulTextureBuffer : public osg::Referenced
{
private:
	typedef std::vector< osg::ref_ptr<osg::Texture2D> >	VEC_TEXTURE;

public:
					CSulTextureBuffer( sigma::uint32 texWidth, sigma::uint32 texHeight, sigma::uint32 numBuffers=2 );

	void			setIn( CSulTextureBuffer* texBuf );
	void			setIn( osg::Texture2D* texIn );
	void			setOut( CSulTextureBuffer* texOut );
	void			setOut( osg::Texture2D* texOut );

	osg::Texture2D*	getIn();
	osg::Texture2D*	getOut();

	void			swap();

private:
	osg::Texture2D* createTexture( unsigned short w, unsigned short h );

private:
	VEC_TEXTURE						m_vecTex;
	osg::ref_ptr<osg::Texture2D>	m_texIn;
	osg::ref_ptr<osg::Texture2D>	m_texOut;
	sigma::uint32					m_numBuf;
	sigma::uint32					m_curBuf;
	GLint							m_texFormatInternal;
	GLint							m_texFormatSource;
	GLenum							m_texSourceType;
};

#endif // __SULTEXTUREBUFFER_H__