// SulTexImage.h

#ifndef __SULTEXIMAGE_H__
#define __SULTEXIMAGE_H__

#include "SulArray2D.h"
#include <osg/Referenced>
#include <osg/Texture2D>

class CSulTexImage : public osg::Referenced
{
public:
						CSulTexImage( sigma::uint32 w, sigma::uint32 h );

	sigma::uint32		GetWidth();
	sigma::uint32		GetHeight();

	osg::Texture2D*		getTexture();

	void				Set( float x, float y, float v );

	void				Process();

private:
	osg::ref_ptr<CSulArray2D<sigma::uint8>>	m_rData;
	osg::ref_ptr<osg::Texture2D>			m_rTex;
	sigma::uint32							m_w;
	sigma::uint32							m_h;
};

#endif // __SULTEXIMAGE_H__