// SulPostFilterNightVision.h

#ifndef __SULPOSTFILTERNIGHTVISION_H__
#define __SULPOSTFILTERNIGHTVISION_H__

#include "SulPostFilterBase.h"
#include "SulShaderIntensity.h"
#include "SulExport.h"

class SUL_EXPORT CSulPostFilterNightVision : public CSulPostFilter
{
public:
					CSulPostFilterNightVision();

	void			in( osg::Texture2D* pTex, float intensityMultiplier, sigma::uint32 sizeDownSampleTexture=256 );
	osg::Texture2D* out();

private:
	void			filterIntensity( osg::Texture2D* pTexIn, osg::Texture2D* pTexOut );

private:
	osg::ref_ptr<osg::Texture2D>		m_rTex;
	float								m_intensity;
	osg::ref_ptr<CSulShaderIntensity>	m_rShaderIntensity;
};

#endif // __SULPOSTFILTERNIGHTVISION_H__