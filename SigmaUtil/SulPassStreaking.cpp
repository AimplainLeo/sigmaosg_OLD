// SulPassStreaking.cpp

#include "stdafx.h"
#include "SulPassStreaking.h"

CSulPassStreaking::CSulPassStreaking( osg::Texture2D* pTexIn, osg::Texture2D* pTexOut, float offsetFactor, float glowFactor ) :
CSulPass( pTexIn, pTexOut, "Streaking" )
{
	m_rShader = new CSulShaderStreak( getQuad(), offsetFactor, glowFactor );
}

CSulPassStreaking::CSulPassStreaking( CSulTextureBuffer* texBuf, float offsetFactor, float glowFactor ) :
CSulPass( texBuf->getOut(), texBuf->getIn(), "Streaking" )
{
	m_rShader = new CSulShaderStreak( getQuad(), offsetFactor, glowFactor );
	texBuf->swap();
}

CSulShaderStreak* CSulPassStreaking::getShader()
{
	return m_rShader;
}

void CSulPassStreaking::setGlowFactor( float glowFactor )
{
	m_rShader->setGlowFactor( glowFactor );
}