// SulPassGaussH.cpp

#include "stdafx.h"
#include "SulPassGaussH.h"

CSulPassGaussH::CSulPassGaussH( osg::Texture2D* pTexIn, osg::Texture2D* pTexOut ) :
CSulPass( pTexIn, pTexOut, "GaussH" )
{
	m_rShader = new CSulShaderGaussH( getGeode(), pTexIn->getTextureHeight() );
}

CSulPassGaussH::CSulPassGaussH( CSulTextureBuffer* texBuf ) :
CSulPass( texBuf->getOut(), texBuf->getIn(), "GaussH" )
{
	m_rShader = new CSulShaderGaussH( getGeode(), texBuf->getIn()->getTextureHeight() );
	texBuf->swap();
}