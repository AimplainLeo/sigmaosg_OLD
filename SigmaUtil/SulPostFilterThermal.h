// SulPostFilterThermal.h

#ifndef __SULPOSTFILTERTHERMAL_H__
#define __SULPOSTFILTERTHERMAL_H__

#include "SulPostFilterBase.h"
#include "SulShaderIntensity.h"
#include "SulExport.h"

class SUL_EXPORT CSulPostFilterThermal : public CSulPostFilter
{
public:
					CSulPostFilterThermal();

	void			in( osg::Texture2D* pTex, float intensityMultiplier, sigma::uint32 sizeDownSampleTexture=256 );
	osg::Texture2D* out();

	void			setWhiteHot( bool bWhiteHot );
	bool			isWhiteHot();

	void			setIntensity( float intensity );

private:
	void			filterIntensity( osg::Texture2D* pTexIn, osg::Texture2D* pTexOut );

private:
	osg::ref_ptr<osg::Texture2D>		m_rTex;
	float								m_intensity;
	osg::ref_ptr<CSulShaderIntensity>	m_rShaderIntensity;
	bool								m_bWhiteHot;
};

#endif // __SULPOSTFILTERTHERMAL_H__