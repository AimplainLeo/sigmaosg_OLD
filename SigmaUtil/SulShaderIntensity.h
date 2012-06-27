// SulShaderIntensity.h

#ifndef __SULSHADERINTENSITY_H__
#define __SULSHADERINTENSITY_H__

#include "SulShaderBase.h"

class SUL_EXPORT CSulShaderIntensity : public CSulShaderBase
{
public:
	CSulShaderIntensity( osg::Node* pNode, float fIntensity, float fMultiIntR, float fMultiIntG, float fMultiIntB, osg::Program* pProg=0 );

	void setMultiR( float f );
	void setMultiG( float f );
	void setMultiB( float f );
	void setIntensity( float f );

	void setInvert( bool bInvert );
private:
	osg::ref_ptr<osg::Uniform>	m_uniformInvert;
	osg::ref_ptr<osg::Uniform>	m_rUniform_multiR;
	osg::ref_ptr<osg::Uniform>	m_rUniform_multiG;
	osg::ref_ptr<osg::Uniform>	m_rUniform_multiB;
	osg::ref_ptr<osg::Uniform>	m_rUniform_intensity;
};

#endif // __SULSHADERINTENSITY_H__

