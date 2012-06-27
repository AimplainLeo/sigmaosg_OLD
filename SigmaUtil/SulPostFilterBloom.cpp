// SulPostFilterBloom.cpp

#include "stdafx.h"
#include "SulPostFilterBloom.h"
#include "SulShaderBrightPass.h"
#include "SulShaderGaussH.h"
#include "SulShaderGaussV.h"
#include "SulShaderCombine.h"
#include <osg/geode>

CSulPostFilterBloom::CSulPostFilterBloom() :
CSulPostFilter(),
m_fExp( 2.0f ),
m_fFactor( 1.0f ),
m_fMax( 1.0f )
{
}

void CSulPostFilterBloom::in( osg::Texture2D* pTex, sigma::int32 iDownSampleSize )
{
	// we create a texture to be used for the final combined image
	m_rTex = createTexture( pTex->getTextureWidth(), pTex->getTextureHeight() );

	// we create 2 smaller textures used for downsampling
	osg::ref_ptr<osg::Texture2D> tex0 = createTexture( iDownSampleSize, iDownSampleSize );
	osg::ref_ptr<osg::Texture2D> tex1 = createTexture( iDownSampleSize, iDownSampleSize );

	filterBrightPass( pTex, tex0 );
	filterGaussH( tex0, tex1 );
	filterGaussV( tex1, tex0 );
	filterCombine( pTex, tex0, m_rTex );
}

osg::Texture2D* CSulPostFilterBloom::out()
{
	return m_rTex;
}

void CSulPostFilterBloom::filterBrightPass( osg::Texture2D* pTexIn, osg::Texture2D* pTexOut )
{
	osg::Group* pGroup = createFilterPass( pTexIn, pTexOut, "BrightPass" );
	osg::ref_ptr<CSulShaderBrightPass> rShader = new CSulShaderBrightPass( pGroup, 0.4f );
}

void CSulPostFilterBloom::filterGaussH( osg::Texture2D* pTexIn, osg::Texture2D* pTexOut )
{
	osg::Group* pGroup = createFilterPass( pTexIn, pTexOut, "GaussH" );
	osg::ref_ptr<CSulShaderGaussH> rShader = new CSulShaderGaussH( pGroup, pTexIn->getTextureHeight() );
}

void CSulPostFilterBloom::filterGaussV( osg::Texture2D* pTexIn, osg::Texture2D* pTexOut )
{
	osg::Group* pGroup = createFilterPass( pTexIn, pTexOut, "GaussV" );
	osg::ref_ptr<CSulShaderGaussV> rShader = new CSulShaderGaussV( pGroup, pTexIn->getTextureWidth() );
}

void CSulPostFilterBloom::filterCombine( osg::Texture2D* pTexOrg, osg::Texture2D* pTexIn, osg::Texture2D* pTexOut )
{
	osg::Group* pGroup = createFilterPass( pTexIn, pTexOut, "Compose" );
	osg::ref_ptr<CSulShaderCombine> rShaderCombine = new CSulShaderCombine( pGroup, m_fExp, m_fFactor, m_fMax, pTexOrg, pTexIn );
}
