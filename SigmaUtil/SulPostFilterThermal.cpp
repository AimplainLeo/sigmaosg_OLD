// SulPostFilterThermal.cpp

#include "stdafx.h"
#include "SulPostFilterThermal.h"
#include "SulPostFilterBloom.h"

// TODO:
// create intensity for object radiating heat
// light should not effect output

CSulPostFilterThermal::CSulPostFilterThermal() :
m_intensity( 1.0f ),
m_bWhiteHot( true )
{

}

void CSulPostFilterThermal::in( osg::Texture2D* pTex, float intensityMultiplier, sigma::uint32 sizeDownSampleTexture )
{
	// we create a texture to be used for the final combined image
	m_rTex = createTexture( pTex->getTextureWidth(), pTex->getTextureHeight() );

	// we need to bloom the texture
	CSulPostFilterBloom* bloom = new CSulPostFilterBloom;
	bloom->in( pTex, sizeDownSampleTexture ); // texture down sample size x and y
	addChild( bloom );

	// adjust intensity
	filterIntensity( bloom->out(), m_rTex );
}

void CSulPostFilterThermal::filterIntensity( osg::Texture2D* pTexIn, osg::Texture2D* pTexOut )
{
	osg::Group* pGroup = createFilterPass( pTexIn, pTexOut, "Intensity" );
	m_rShaderIntensity = new CSulShaderIntensity( pGroup, m_intensity, 1.0f, 1.0f, 1.0f );
}

osg::Texture2D* CSulPostFilterThermal::out()
{
	return m_rTex;
}

void CSulPostFilterThermal::setWhiteHot( bool bWhiteHot )
{
	m_bWhiteHot = bWhiteHot;
	m_rShaderIntensity->setInvert( !bWhiteHot );
}

bool CSulPostFilterThermal::isWhiteHot()
{
	return m_bWhiteHot;
}

void CSulPostFilterThermal::setIntensity( float intensity )
{
	m_rShaderIntensity->setIntensity( intensity );
}