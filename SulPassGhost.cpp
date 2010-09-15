// SulPassGhost.cpp

#include "stdafx.h"
#include "SulPassGhost.h"

CSulPassGhost::CSulPassGhost( osg::Texture2D* pTexIn, osg::Texture2D* pTexOut ) :
CSulPass( pTexIn, pTexOut, "Ghost" )
{
	m_rShader = new CSulShaderGhost( getGeode() );
}

CSulPassGhost::CSulPassGhost( CSulTextureBuffer* texBuf ) :
CSulPass( texBuf->getOut(), texBuf->getIn(), "Ghost" )
{
	m_rShader = new CSulShaderGhost( getGeode() );
	texBuf->swap();
}