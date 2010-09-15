// SulPassGhost.h

#ifndef __SULPASSGHOST_H__
#define __SULPASSGHOST_H__

#include "SulPass.h"
#include "SulShaderGhost.h"
#include "SulTextureBuffer.h"

class CSulPassGhost : public CSulPass
{
public:
	CSulPassGhost( osg::Texture2D* pTexIn, osg::Texture2D* pTexOut );
	CSulPassGhost( CSulTextureBuffer* texBuf );

private:
	osg::ref_ptr<CSulShaderGhost> m_rShader;
};

#endif // __SULPASSGHOST_H__