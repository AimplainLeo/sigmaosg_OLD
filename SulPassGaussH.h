// SulPassGaussH.h

#ifndef __SULPASSGAUSSH_H__
#define __SULPASSGAUSSH_H__

#include "SulPass.h"
#include "SulShaderGaussH.h"
#include "SulTextureBuffer.h"

class CSulPassGaussH : public CSulPass
{
public:
	CSulPassGaussH( osg::Texture2D* pTexIn, osg::Texture2D* pTexOut );
	CSulPassGaussH( CSulTextureBuffer* texBuf );

private:
	osg::ref_ptr<CSulShaderGaussH> m_rShader;
};


#endif // __SULPASSGAUSSH_H__