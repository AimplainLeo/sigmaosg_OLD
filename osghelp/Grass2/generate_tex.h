// generate_tex.h

#ifndef __GENERATE_TEX_H__
#define __GENERATE_TEX_H__

#include <SigmaUtil/SulRTT.h>
#include <osg/node>
#include <osg/camera>
#include <osg/texture2d>

class CGenerateTex
{
public:

	void create( 
		osg::Node* pRenderMe,			// this will be rendered
		osg::Group* pRootGroup,			// need to add generator to scene (so it will work)
		float x,						// center x location of scene (also center location for texture)
		float y,						// center x location of scene (also center location for texture)
		unsigned int	tXY,			// texture size squared
		unsigned int	sXY				// scnee size squared
	);

	osg::Camera* getCamera();

	osg::Texture2D*	getTexture();

private:
	osg::ref_ptr<CSulRTT>	m_rRTT;

	osg::ref_ptr<osg::Image>	m_rImage;
};

#endif // __GENERATE_TEX_H__