// SulTexCam.h

#ifndef __SULTEXCAM_H__
#define __SULTEXCAM_H__

#include "SulTypes.h"
#include "SulExport.h"
#include <osg/camera>
#include <osg/texture2d>
#include <osgViewer/Viewer>

class SUL_EXPORT CSulTexCam : public osg::Camera
{
public:
	enum ESETUP
	{
		STANDARD,
		SHADOW,
		HDR,
		ZVALUE_WITH_IMAGE,			// draws the scene using the vertex z values
		ZVALUE_FROM_DEPTH,
		DEFERRED_RENDERING,
		ORTHO
	};

public:
						CSulTexCam( ESETUP eSetup=ORTHO );

						CSulTexCam( osgViewer::Viewer* viewer, ESETUP eSetup=STANDARD );

						CSulTexCam( osg::Vec2 wh, ESETUP eSetup=STANDARD );
						CSulTexCam( sigma::uint32 w, sigma::uint32 h, ESETUP eSetup=STANDARD );
						CSulTexCam( osg::Texture2D* pTex, ESETUP eSetup=STANDARD );

	void				setWidth( sigma::uint32 w );
	void				setHeight( sigma::uint32 h );

	void				setTexture( osg::Texture2D* pTex, sigma::uint32 index=0 );
	sigma::uint32		addTexture( osg::Texture2D* pTex );
	osg::Texture2D*		getTexture( sigma::uint32 index=0 );

	osg::Image*			getImage();

	void				setTextureSize( sigma::uint32 w, sigma::uint32 h );

	void				initTex();
	void				initCam();

private:
	typedef std::vector< osg::ref_ptr< osg::Texture2D > >	VEC_TEX;

	VEC_TEX							m_vecTex;

	osg::ref_ptr<osg::Image>		m_rImage;
	sigma::uint32					m_w;
	sigma::uint32					m_h;
	ESETUP							m_eSetup;

	sigma::uint32					m_texW;
	sigma::uint32					m_texH;
};

#endif // __SULTEXCAM_H__