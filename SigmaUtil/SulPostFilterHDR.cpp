// SulPostFilterHDR.cpp

#include "stdafx.h"
#include "SulPostFilterHDR.h"
#include "SulShaderBrightPass.h"
#include "SulShaderGaussH.h"
#include "SulShaderGaussV.h"
#include "SulShaderCombine.h"
#include <osg/geode>

CSulPostFilterHDR::CSulPostFilterHDR() :
CSulPostFilter(),
m_fExp( 1.0f ),
m_fFactor( 1.0f ),
m_fMax( 1.0f ),
m_sView( "hdr" )
{
	setTexFormatInternal( GL_RGBA16F_ARB );
	setTexFormatSource( GL_RGBA );
	setTexSourceType( GL_FLOAT );
}

void CSulPostFilterHDR::setView( const CSulString& sView )
{
	m_sView = sView;
}

void CSulPostFilterHDR::in( osg::Texture2D* pTex, int iDownSampleSize )
{
	// we create a texture to be used for the final combined image
	m_rTex = createTexture( pTex->getTextureWidth(), pTex->getTextureHeight() );

	// we create 2 smaller textures used for downsampling
	osg::ref_ptr<osg::Texture2D> tex0 = createTexture( iDownSampleSize, iDownSampleSize );
	osg::ref_ptr<osg::Texture2D> tex1 = createTexture( iDownSampleSize, iDownSampleSize );

	if ( m_sView=="brightpass" )
	{
		filterBrightPass( pTex, m_rTex );
		return;
	}

	filterBrightPass( pTex, tex0 );
	filterGaussH( tex0, tex1 );
	filterGaussV( tex1, tex0 );
	filterCombine( pTex, tex0, m_rTex );
}

osg::Texture2D* CSulPostFilterHDR::out()
{
	return m_rTex;
}

void CSulPostFilterHDR::filterBrightPass( osg::Texture2D* pTexIn, osg::Texture2D* pTexOut )
{
	m_rFilterBrightPass = new CSulFilterPass;
	osg::Group* pGroup = m_rFilterBrightPass->create( pTexIn, pTexOut, "BrightPass" );
	addChild( m_rFilterBrightPass->getTexCam() );
	m_rShaderBrightPass = new CSulShaderBrightPass( pGroup, 1.2f );

/*
	osg::Geode* pGeode = createFilterPass( pTexIn, pTexOut, "BrightPass" );
	m_rShaderBrightPass = new CSulShaderBrightPass( pGeode, 1.2f );
*/
}

void CSulPostFilterHDR::filterGaussH( osg::Texture2D* pTexIn, osg::Texture2D* pTexOut )
{
	osg::Group* pGroup = createFilterPass( pTexIn, pTexOut, "GaussH" );
	osg::ref_ptr<CSulShaderGaussH> rShader = new CSulShaderGaussH( pGroup, pTexIn->getTextureHeight() );
}

void CSulPostFilterHDR::filterGaussV( osg::Texture2D* pTexIn, osg::Texture2D* pTexOut )
{
	osg::Group* pGroup = createFilterPass( pTexIn, pTexOut, "GaussV" );
	osg::ref_ptr<CSulShaderGaussV> rShader = new CSulShaderGaussV( pGroup, pTexIn->getTextureWidth() );
}

void CSulPostFilterHDR::filterCombine( osg::Texture2D* pTexOrg, osg::Texture2D* pTexIn, osg::Texture2D* pTexOut )
{
	osg::Group* pGroup = createFilterPass( pTexIn, pTexOut, "Compose" );
	m_rShaderCombine = new CSulShaderCombine( pGroup, m_fExp, m_fFactor, m_fMax, pTexOrg, pTexIn );
}

CSulShaderBrightPass* CSulPostFilterHDR::getShaderBrightPass()
{
	return m_rShaderBrightPass;
}

CSulShaderCombine* CSulPostFilterHDR::getShaderCombine()
{
	return m_rShaderCombine;
}
