// SulShaderGodRays.h

#ifndef __SULSHADERGODRAYS_H__
#define __SULSHADERGODRAYS_H__

#include "SulShaderBase.h"

class CSulShaderGodRays : public CSulShaderBase
{
public:
	CSulShaderGodRays( osg::Node* pNode, osg::Program* pProg=0 );

	void	setExposure( float exposure );
	void	setDecay( float decay );
	void	setDensity( float density );
	void	setWeight( float weight );

private:
	osg::ref_ptr<osg::Uniform>	m_rUniform_exposure;
	osg::ref_ptr<osg::Uniform>	m_rUniform_decay;
	osg::ref_ptr<osg::Uniform>	m_rUniform_density;
	osg::ref_ptr<osg::Uniform>	m_rUniform_weight;
};


#endif // __SULSHADERGODRAYS_H__