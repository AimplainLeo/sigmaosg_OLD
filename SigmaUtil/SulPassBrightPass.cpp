// SulPassBrightPass.cpp

#include "stdafx.h"
#include "SulPassBrightPass.h"

CSulPassBrightPass::CSulPassBrightPass( osg::Texture2D* pTexIn, osg::Texture2D* pTexOut ) :
CSulPass( pTexIn, pTexOut, "BrightPass" )
{
	m_rShaderBrightPass = new CSulShaderBrightPass( getQuad(), 1.2f );
}

CSulPassBrightPass::CSulPassBrightPass( CSulTextureBuffer* texBuf ) :
CSulPass( texBuf->getOut(), texBuf->getIn(), "BrightPass" )
{
	m_rShaderBrightPass = new CSulShaderBrightPass( getQuad(), 1.2f );
	texBuf->swap();
}

CSulShaderBrightPass* CSulPassBrightPass::getShader()
{
	return m_rShaderBrightPass;
}