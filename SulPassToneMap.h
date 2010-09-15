// SulPassToneMap.h

#ifndef __SULPASSTONEMAP_H__
#define __SULPASSTONEMAP_H__

#include "SulPass.h"
#include "SulShaderCombine.h"
#include "SulTextureBuffer.h"

class CSulPassToneMap : public CSulPass
{
public:
	CSulPassToneMap( osg::Texture2D* pTexOrg, osg::Texture2D* pTexIn, osg::Texture2D* pTexOut, float exp, float factor, float max, CSulTexCam* pTexCamLum=0 );

	CSulShaderCombine* getShader();

private:
	osg::ref_ptr<CSulShaderCombine> m_rShader;
};

#endif // __SULPASSTONEMAP_H__