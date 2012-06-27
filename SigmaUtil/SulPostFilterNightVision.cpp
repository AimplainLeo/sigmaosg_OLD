// SulPostFilterNightVision.cpp

#include "stdafx.h"
#include "SulPostFilterNightVision.h"
#include "SulPostFilterBloom.h"

CSulPostFilterNightVision::CSulPostFilterNightVision() :
m_intensity( 1.0f )
{

}

void CSulPostFilterNightVision::in( osg::Texture2D* pTex, float intensityMultiplier, sigma::uint32 sizeDownSampleTexture )
{
	// we create a texture to be used for the final combined image
	m_rTex = createTexture( pTex->getTextureWidth(), pTex->getTextureHeight() );

	// we need to bloom the texture
	CSulPostFilterBloom* bloom = new CSulPostFilterBloom;
	bloom->in( pTex, sizeDownSampleTexture ); // texture down sample size x and y
	addChild( bloom );

	// we need to make the output green
	filterIntensity( bloom->out(), m_rTex );
}

void CSulPostFilterNightVision::filterIntensity( osg::Texture2D* pTexIn, osg::Texture2D* pTexOut )
{
	osg::Group* pGroup = createFilterPass( pTexIn, pTexOut, "Intensity" );
	m_rShaderIntensity = new CSulShaderIntensity( pGroup, m_intensity, 1.0f, 3.0f, 1.0f );
}

osg::Texture2D* CSulPostFilterNightVision::out()
{
	return m_rTex;
}



