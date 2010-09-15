// SulShaderBrightPass.h

#ifndef __SULSHADERBRIGHTPASS_H__
#define __SULSHADERBRIGHTPASS_H__

#include "SulShaderBase.h"
#include "SulExport.h"

class SUL_EXPORT CSulShaderBrightPass : public CSulShaderBase
{
public:
	CSulShaderBrightPass( osg::Node* pNode, float fThreshold, osg::Program* pProg=0 );

	void						setThreshold( float val );
	void						setFalloff( float val );

private:
	osg::ref_ptr<osg::Uniform>	m_rUniform_threshold;
	osg::ref_ptr<osg::Uniform>	m_rUniform_falloff;
};

#endif // __SULSHADERBRIGHTPASS_H__