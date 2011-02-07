// SulPassGodRays.cpp

#include "stdafx.h"
#include "SulPassGodRays.h"

CSulPassGodRays::CSulPassGodRays( osg::Texture2D* pTexIn, CSulTextureBuffer* pTexOut ) :
CSulPass( pTexIn, pTexOut->getOut(), "GodRays" )
{
	m_rShader = new CSulShaderGodRays( getQuad() );
}

CSulShaderGodRays* CSulPassGodRays::getShader()
{
	return m_rShader;
}