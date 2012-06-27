// SulDeferredCamera.h

#ifndef __SULDEFERREDCAMERA_H__
#define __SULDEFERREDCAMERA_H__

#include "SulTexCam.h"
#include "SulScreenAlignedQuad.h"
#include "SulLightManager.h"
#include "SulExport.h"
#include <osg/group>

class SUL_EXPORT CSulDeferredCamera : public CSulTexCam
{
public:
	CSulDeferredCamera( CSulLightManager* lightManager, sigma::uint32 w=200, sigma::uint32 h=150 );

	// final composite texture
	osg::Texture2D* getFinalTexture();

	CSulTexCam*		getCompositeCamera();

private:
	void traverse( osg::NodeVisitor& nv );

private:
	osg::ref_ptr<CSulLightManager>		m_lm;
	osg::ref_ptr<CSulGeomQuad>			m_quad;
	osg::ref_ptr<CSulTexCam>			m_comp;
};


#endif // __SULDEFERREDCAMERA_H__