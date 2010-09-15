// SulPostFilterGhost.cpp

#include "stdafx.h"
#include "SulPostFilterGhost.h"
#include "SulPassGhost.h"

void CSulPostFilterGhost::in( osg::Texture2D* pTex )
{
	// we create a texture to be used for the final combined image
	m_rTex = createTexture( pTex->getTextureWidth(), pTex->getTextureHeight() );
	osg::Texture2D* tex0 = createTexture( pTex->getTextureWidth(), pTex->getTextureHeight() );

	addPass( new CSulPassGhost( pTex, tex0 ) );
	addPass( new CSulPassGhost( tex0, m_rTex ) );
}

osg::Texture2D* CSulPostFilterGhost::out()
{
	return m_rTex;
}
