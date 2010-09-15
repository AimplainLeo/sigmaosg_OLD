// SulPassBrightPass.h

#ifndef __SULPASSBRIGHTPASS_H__
#define __SULPASSBRIGHTPASS_H__

#include "SulPass.h"
#include "SulShaderBrightPass.h"
#include "SulTextureBuffer.h"

class CSulPassBrightPass : public CSulPass
{
public:
	CSulPassBrightPass( osg::Texture2D* pTexIn, osg::Texture2D* pTexOut );
	CSulPassBrightPass( CSulTextureBuffer* texBuf );

	CSulShaderBrightPass* getShader();

private:
	osg::ref_ptr<CSulShaderBrightPass>	m_rShaderBrightPass;
};

#endif // __SULPASSBRIGHTPASS_H__