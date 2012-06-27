// SulPostFilterHDR2.cpp

#include "stdafx.h"
#include "SulPostFilterHDR2.h"
#include "SulShaderBrightPass.h"
#include "SulShaderGaussH.h"
#include "SulShaderGaussV.h"
#include "SulShaderCombine.h"
#include "SulShaderCombineTex.h"
#include "SulShaderStreak.h"
#include "SulPass.h"
#include "SulPassBrightPass.h"
#include "SulPassGaussH.h"
#include "SulPassGaussV.h"
#include "SulPassCombineTex.h"
#include "SulPassGhost.h"
#include "SulPassNormal.h"
#include "SulTextureBuffer.h"
#include <osg/geode>

CSulPostFilterHDR2::CSulPostFilterHDR2() :
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

void CSulPostFilterHDR2::setView( const CSulString& sView )
{
	m_sView = sView;
}

void CSulPostFilterHDR2::setGlowFactor( float glowFactor )
{
	if(m_rStreak0.valid()) m_rStreak0->setGlowFactor( glowFactor );
	if(m_rStreak1.valid()) m_rStreak1->setGlowFactor( glowFactor );
	if(m_rStreak2.valid()) m_rStreak2->setGlowFactor( glowFactor );
	if(m_rStreak3.valid()) m_rStreak3->setGlowFactor( glowFactor );
}

#if 0
void CSulPostFilterHDR2::in( osg::Texture2D* pTex )
{
	// we create a texture to be used for the final combined image
	m_rTex = createTexture( pTex->getTextureWidth(), pTex->getTextureHeight() );

	if ( m_sView=="brightpass" )
	{
		addPass( new CSulPassBrightPass( pTex, m_rTex ) );
		return;
	}

	///////////////////
	// downsample
	///////////////////

	sigma::uint32 w = pTex->getTextureWidth();
	sigma::uint32 h = pTex->getTextureHeight();
/*
	osg::ref_ptr<osg::Texture2D> tex_ds_1 = createTexture( w/2, h/2 );
	osg::ref_ptr<osg::Texture2D> tex_ds_1_tmp = createTexture( w/2, h/2 );
	osg::ref_ptr<osg::Texture2D> tex_ds_1_tmp2 = createTexture( w/2, h/2 );
	addPass( m_rPassBrightPass = new CSulPassBrightPass(pTex, tex_ds_1) );
	addPass( m_rStreak0 = new CSulPassStreaking( tex_ds_1, tex_ds_1_tmp2,  1.0f, 0.1f ) );
	addPass( m_rStreak1 = new CSulPassStreaking( tex_ds_1_tmp2, tex_ds_1,  4.0f, 0.1f ) );
	addPass( m_rStreak2 = new CSulPassStreaking( tex_ds_1, tex_ds_1_tmp2, 16.0f, 0.1f ) );
	addPass( m_rStreak3 = new CSulPassStreaking( tex_ds_1_tmp2, tex_ds_1, 64.0f, 0.1f ) );
	addPass( new CSulPassGaussH( tex_ds_1, tex_ds_1_tmp ) );
	addPass( new CSulPassGaussV(  tex_ds_1_tmp, tex_ds_1 ) );
*/
	osg::ref_ptr<osg::Texture2D> tex_ghost1 = createTexture( w, h );
	osg::ref_ptr<osg::Texture2D> tex_ghost2 = createTexture( w, h );


	osg::ref_ptr<osg::Texture2D> tex_ds_1 = createTexture( w/2, h/2 );
	osg::ref_ptr<osg::Texture2D> tex_ds_1_tmp = createTexture( w/2, h/2 );
	osg::ref_ptr<osg::Texture2D> tex_ds_1_tmp2 = createTexture( w/2, h/2 );
	addPass( m_rPassBrightPass = new CSulPassBrightPass(pTex, tex_ds_1) );

	addPass( new CSulPassGhost( tex_ds_1, tex_ghost2) );
	//addPass( new CSulPassGhost( tex_ghost1, tex_ghost2) );

	addPass( m_rStreak0 = new CSulPassStreaking( tex_ds_1, tex_ds_1_tmp2,  1.0f, 0.1f ) );
	addPass( m_rStreak1 = new CSulPassStreaking( tex_ds_1_tmp2, tex_ds_1,  4.0f, 0.1f ) );
	addPass( m_rStreak2 = new CSulPassStreaking( tex_ds_1, tex_ds_1_tmp2, 16.0f, 0.1f ) );
	addPass( m_rStreak3 = new CSulPassStreaking( tex_ds_1_tmp2, tex_ds_1, 64.0f, 0.1f ) );
	addPass( new CSulPassGaussH( tex_ds_1, tex_ds_1_tmp ) );
	addPass( new CSulPassGaussV(  tex_ds_1_tmp, tex_ds_1 ) );

	// downsample 1/4
	osg::ref_ptr<osg::Texture2D> tex_ds_2 = createTexture( w/4, h/4 );
	osg::ref_ptr<osg::Texture2D> tex_ds_2_tmp = createTexture( w/4, h/4 );
	addPass( new CSulPassGaussH( tex_ds_1, tex_ds_2_tmp ) );
	addPass( new CSulPassGaussV( tex_ds_2_tmp, tex_ds_2 ) );

	// downsample 1/8
	osg::ref_ptr<osg::Texture2D> tex_ds_3 = createTexture( w/8, h/8 );
	osg::ref_ptr<osg::Texture2D> tex_ds_3_tmp = createTexture( w/8, h/8 );
	addPass( new CSulPassGaussH( tex_ds_2, tex_ds_3_tmp ) );
	addPass( new CSulPassGaussV( tex_ds_3_tmp, tex_ds_3 ) );

	// downsampling combining here (we reuse tex_ds_1_tmp and tex_ds_1 as output)
	addPass( new CSulPassCombineTex( tex_ds_1, tex_ds_2, tex_ds_1_tmp ) );
	addPass( new CSulPassCombineTex( tex_ds_1_tmp, tex_ds_3, tex_ds_1 ) );

	addPass( new CSulPassCombineTex( tex_ds_1, tex_ghost2, tex_ds_1_tmp ) );
	

	// composer
	addPass( m_rPassToneMap = new CSulPassToneMap( pTex, tex_ds_1_tmp, m_rTex, m_fExp, m_fFactor, m_fMax) );
}
#endif

#if 0
void CSulPostFilterHDR2::in( osg::Texture2D* pTex )
{
	// we create a texture to be used for the final combined image
	m_rTex = createTexture( pTex->getTextureWidth(), pTex->getTextureHeight() );

	if ( m_sView=="brightpass" )
	{
		addPass( new CSulPassBrightPass( pTex, m_rTex ) );
		return;
	}

	///////////////////
	// downsample
	///////////////////

	sigma::uint32 w = pTex->getTextureWidth();
	sigma::uint32 h = pTex->getTextureHeight();

	osg::ref_ptr<osg::Texture2D> tex_ds_1 = createTexture( w/2, h/2 );
	osg::ref_ptr<osg::Texture2D> tex_ds_1_tmp = createTexture( w/2, h/2 );
	osg::ref_ptr<osg::Texture2D> tex_ds_1_tmp2 = createTexture( w/2, h/2 );
	addPass( m_rPassBrightPass = new CSulPassBrightPass(pTex, tex_ds_1) );
	addPass( m_rStreak0 = new CSulPassStreaking( tex_ds_1, tex_ds_1_tmp2,  1.0f, 0.1f ) );
	addPass( m_rStreak1 = new CSulPassStreaking( tex_ds_1_tmp2, tex_ds_1,  4.0f, 0.1f ) );
	addPass( m_rStreak2 = new CSulPassStreaking( tex_ds_1, tex_ds_1_tmp2, 16.0f, 0.1f ) );
	addPass( m_rStreak3 = new CSulPassStreaking( tex_ds_1_tmp2, tex_ds_1, 64.0f, 0.1f ) );
	addPass( new CSulPassGaussH( tex_ds_1, tex_ds_1_tmp ) );
	addPass( new CSulPassGaussV(  tex_ds_1_tmp, tex_ds_1 ) );

	// downsample 1/4
	osg::ref_ptr<osg::Texture2D> tex_ds_2 = createTexture( w/4, h/4 );
	osg::ref_ptr<osg::Texture2D> tex_ds_2_tmp = createTexture( w/4, h/4 );
	addPass( new CSulPassGaussH( tex_ds_1, tex_ds_2_tmp ) );
	addPass( new CSulPassGaussV( tex_ds_2_tmp, tex_ds_2 ) );

	// downsample 1/8
	osg::ref_ptr<osg::Texture2D> tex_ds_3 = createTexture( w/8, h/8 );
	osg::ref_ptr<osg::Texture2D> tex_ds_3_tmp = createTexture( w/8, h/8 );
	addPass( new CSulPassGaussH( tex_ds_2, tex_ds_3_tmp ) );
	addPass( new CSulPassGaussV( tex_ds_3_tmp, tex_ds_3 ) );

	// downsampling combining here (we reuse tex_ds_1_tmp and tex_ds_1 as output)
	addPass( new CSulPassCombineTex( tex_ds_1, tex_ds_2, tex_ds_1_tmp ) );
	addPass( new CSulPassCombineTex( tex_ds_1_tmp, tex_ds_3, tex_ds_1 ) );

	// composer
	addPass( m_rPassToneMap = new CSulPassToneMap( pTex, tex_ds_1, m_rTex, m_fExp, m_fFactor, m_fMax) );
}
#endif

void CSulPostFilterHDR2::in( osg::Texture2D* pTex )
{
	// we create a texture to be used for the final combined image
	m_rTex = createTexture( pTex->getTextureWidth(), pTex->getTextureHeight() );

	if ( m_sView=="brightpass" )
	{
		addPass( new CSulPassBrightPass( pTex, m_rTex ) );
		return;
	}

	sigma::uint32 w = pTex->getTextureWidth();
	sigma::uint32 h = pTex->getTextureHeight();

	/**/
	// calc luminance, we do this by down sampling the orginal image until we get
	// a texture that is 1x1
	// NOTE: I would like to keep this constant so the first down sample will 
	// be 512x512 which means we will process the following
	// 512x512
	// 256x256
	// 128x128
	// 64x64
	// 32x32
	// 16x16
	// 8x8
	// 4x4
	// 2x2	this is the minimum opengl can handle
	// that is 10 downsampling steps
	osg::ref_ptr<CSulTextureBuffer> texLum512 = new CSulTextureBuffer( 512, 512, 1 );
	osg::ref_ptr<CSulTextureBuffer> texLum256 = new CSulTextureBuffer( 256, 256, 1 );
	osg::ref_ptr<CSulTextureBuffer> texLum128 = new CSulTextureBuffer( 128, 128, 1 );
	osg::ref_ptr<CSulTextureBuffer> texLum64 = new CSulTextureBuffer( 64, 64, 1 );
	osg::ref_ptr<CSulTextureBuffer> texLum32 = new CSulTextureBuffer( 32, 32, 1 );
	osg::ref_ptr<CSulTextureBuffer> texLum16 = new CSulTextureBuffer( 16, 16, 1 );
	osg::ref_ptr<CSulTextureBuffer> texLum8 = new CSulTextureBuffer( 8, 8, 1 );
	osg::ref_ptr<CSulTextureBuffer> texLum4 = new CSulTextureBuffer( 4, 4, 1 );
	osg::ref_ptr<CSulTextureBuffer> texLum2 = new CSulTextureBuffer( 2, 2, 1 );
	
	CSulPassNormal* pLumLast = 0;

	addPass( new CSulPassNormal( pTex, texLum512 ) );
	addPass( new CSulPassNormal( texLum512, texLum256 ) );
	addPass( new CSulPassNormal( texLum256, texLum128 ) );
	addPass( new CSulPassNormal( texLum128, texLum64 ) );
	addPass( new CSulPassNormal( texLum64, texLum32 ) );
	addPass( new CSulPassNormal( texLum32, texLum16 ) );
	addPass( new CSulPassNormal( texLum16, texLum8 ) );
	addPass( new CSulPassNormal( texLum8, texLum4 ) );
	addPass( pLumLast = new CSulPassNormal( texLum4, texLum2 ) );

	texLum = texLum2;

	/**/

	// downsample 1/2 + streak + brightpass
	osg::ref_ptr<CSulTextureBuffer> texBuf2 = new CSulTextureBuffer( w/2, h/2 );
	texBuf2->setOut( pTex );
	addPass( m_rPassBrightPass = new CSulPassBrightPass( texBuf2 ) );

	osg::ref_ptr<CSulTextureBuffer> texBufGhost = new CSulTextureBuffer( w/2, h/2 );
	texBufGhost->setOut( texBuf2 );
	addPass( new CSulPassGhost( texBufGhost ) );
	addPass( new CSulPassGhost( texBufGhost ) );
	addPass( new CSulPassGaussH( texBufGhost ) );
	addPass( new CSulPassGaussV( texBufGhost ) );

	//addPass( m_rStreak0 = new CSulPassStreaking( texBuf2,  1.0f, 0.1f ) );
	//addPass( m_rStreak1 = new CSulPassStreaking( texBuf2,  4.0f, 0.1f ) );
	//addPass( m_rStreak2 = new CSulPassStreaking( texBuf2, 16.0f, 0.1f ) );
	//addPass( m_rStreak3 = new CSulPassStreaking( texBuf2, 64.0f, 0.1f ) );
	//addPass( new CSulPassGaussH( texBuf2 ) );
	//addPass( new CSulPassGaussV( texBuf2 ) );

	// downsample 1/4
	osg::ref_ptr<CSulTextureBuffer> texBuf4 = new CSulTextureBuffer( w/4, h/4 );
	texBuf4->setOut( texBuf2 );
	addPass( new CSulPassGaussH( texBuf4 ) );
	addPass( new CSulPassGaussV( texBuf4 ) );

	// downsample 1/8
	osg::ref_ptr<CSulTextureBuffer> texBuf8 = new CSulTextureBuffer( w/8, h/8 );
	texBuf8->setOut( texBuf4 );
	addPass( new CSulPassGaussH( texBuf8 ) );
	addPass( new CSulPassGaussV( texBuf8 ) );

	// downsampling combining here
	addPass( new CSulPassCombineTex( texBuf2, texBuf4 ) );
	addPass( new CSulPassCombineTex( texBuf2, texBuf8 ) );

	addPass( new CSulPassCombineTex( texBuf2, texBufGhost ) );

	// composer
	addPass( m_rPassToneMap = new CSulPassToneMap( pTex, texBuf2->getOut(), m_rTex, m_fExp, m_fFactor, m_fMax, pLumLast->getTexCam() ) );
}


osg::Texture2D* CSulPostFilterHDR2::out()
{
	return m_rTex;
}

CSulShaderBrightPass* CSulPostFilterHDR2::getShaderBrightPass()
{
	return m_rPassBrightPass->getShader();
}

CSulShaderCombine* CSulPostFilterHDR2::getShaderCombine()
{
	return m_rPassToneMap->getShader();
}
