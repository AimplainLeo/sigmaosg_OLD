// SulPassGaussV.h

#ifndef __SULPASSGAUSSV_H__
#define __SULPASSGAUSSV_H__

#include "SulPass.h"
#include "SulShaderGaussV.h"
#include "SulTextureBuffer.h"

class CSulPassGaussV : public CSulPass
{
public:
	CSulPassGaussV( osg::Texture2D* pTexIn, osg::Texture2D* pTexOut);
	CSulPassGaussV( CSulTextureBuffer* texBuf );

private:
	osg::ref_ptr<CSulShaderGaussV> m_rShader;
};
 

#endif // __SULPASSGAUSSV_H__