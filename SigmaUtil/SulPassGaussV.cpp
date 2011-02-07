// SulPassGaussV.cpp

#include "stdafx.h"
#include "SulPassGaussV.h"

CSulPassGaussV::CSulPassGaussV( osg::Texture2D* pTexIn, osg::Texture2D* pTexOut ) :
CSulPass( pTexIn, pTexOut, "GaussV" )
{
	m_rShader = new CSulShaderGaussV( getQuad(), pTexIn->getTextureHeight() );
}

CSulPassGaussV::CSulPassGaussV( CSulTextureBuffer* texBuf ) :
CSulPass( texBuf->getOut(), texBuf->getIn(), "GuassV" )
{
	m_rShader = new CSulShaderGaussV( getQuad(), texBuf->getIn()->getTextureHeight() );
	texBuf->swap();
}