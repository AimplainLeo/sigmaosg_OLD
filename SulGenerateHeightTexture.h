// SulGenerateHeightTexture.h

// DEPRECATED!!! use CSulGenHeightTexture instead
// this class was created while researching techniques to
// accomplish heightmap textures, meaning this class isn't
// that well written.

#ifndef __SULGENERATEHEIGHTTEXTURE_H__
#define __SULGENERATEHEIGHTTEXTURE_H__

#include "SulExport.h"
#include "SulRTT.h"
#include <osg/group>
#include <osg/referenced>

class SUL_EXPORT CSulGenerateHeightTexture : public osg::Referenced
{
public:
	~CSulGenerateHeightTexture();

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
	osg::ref_ptr<osg::Group>					m_rRoot;
	osg::ref_ptr<CSulRTT>						m_rRTT;
	osg::ref_ptr<osg::Image>					m_rImage;
};

#endif // __SULGENERATEHEIGHTTEXTURE_H__