// SulPostFilterBrightPass.cpp

#include "stdafx.h"
#include "SulPostFilterBrightPass.h"
#include <osg/geode>

CSulPostFilterBrightPass::CSulPostFilterBrightPass() :
CSulPostFilterBase()
{
	setTexFormatInternal( GL_RGBA16F_ARB );
	setTexFormatSource( GL_RGBA );
	setTexSourceType( GL_FLOAT );
}

void CSulPostFilterBrightPass::in( osg::Texture2D* pTex )
{
	// we create a texture to be used for the final combined image
	m_rTex = createTexture( pTex->getTextureWidth(), pTex->getTextureHeight() );

	filterBrightPass( pTex, m_rTex );
}

osg::Texture2D* CSulPostFilterBrightPass::out()
{
	return m_rTex;
}

void CSulPostFilterBrightPass::filterBrightPass( osg::Texture2D* pTexIn, osg::Texture2D* pTexOut )
{
	osg::Geode* pGeode = createFilterPass( pTexIn, pTexOut, "BrightPass" );
	m_rShaderBrightPass = new CSulShaderBrightPass( pGeode, 1.2f );
}
