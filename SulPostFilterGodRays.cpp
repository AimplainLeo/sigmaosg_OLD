// SulPostFilterGodRays.cpp

#include "stdafx.h"
#include "SulPostFilterGodRays.h"
#include "SulPassNormal.h"
#include "SulPassCombineTex.h"

void CSulPostFilterGodRays::in( osg::Texture2D* pTex_normal, osg::Texture2D* pTex_occluding )
{
	// we create a texture to be used for the final combined image
	m_rTex = createTexture( pTex_normal->getTextureWidth(), pTex_normal->getTextureHeight() );

	//addPass( new CSulPassNormal( pTex_normal, m_rTex ) );

	osg::ref_ptr<CSulTextureBuffer> tex512 = new CSulTextureBuffer( 512, 512, 1 );
	addPass( m_rPassGodRays = new CSulPassGodRays( pTex_occluding, tex512 ) );

	addPass( new CSulPassCombineTex( pTex_normal, tex512, m_rTex ) );

//	m_rTex = tex512->getOut();
}

osg::Texture2D* CSulPostFilterGodRays::out()
{
	return m_rTex;
}

CSulPassGodRays* CSulPostFilterGodRays::getPassGodRays()
{
	return m_rPassGodRays;
}