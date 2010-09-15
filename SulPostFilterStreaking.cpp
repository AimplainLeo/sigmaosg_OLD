// SulPostFilterStreaking.cpp

#include "stdafx.h"
#include "SulPostFilterStreaking.h"
#include "SulShaderStreak.h"
#include "SulShaderCombineTex.h"
#include "SulPassStreaking.h"
#include <osg/geode>

void CSulPostFilterStreaking::in( osg::Texture2D* pTex )
{
	// we create a texture to be used for the final combined image
	m_rTex = createTexture( pTex->getTextureWidth(), pTex->getTextureHeight() );

	osg::ref_ptr<osg::Texture2D> tex0 = createTexture( pTex->getTextureWidth(), pTex->getTextureHeight() );
	osg::ref_ptr<osg::Texture2D> tex1 = createTexture( pTex->getTextureWidth(), pTex->getTextureHeight() );

	addPass( new CSulPassStreaking( pTex, tex0,	1.0f,  0.3f  ) );
	addPass( new CSulPassStreaking( tex0, tex1,	4.0f,  0.3f ) );
	addPass( new CSulPassStreaking( tex1, tex0,	16.0f, 0.3f ) );
	addPass( new CSulPassStreaking( tex0, m_rTex,  64.0f, 0.3f ) );
}

osg::Texture2D* CSulPostFilterStreaking::out()
{
	return m_rTex;
}
