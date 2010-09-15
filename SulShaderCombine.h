// SulShaderCombine.h

#ifndef __SULSHADERCOMBINE_H__
#define __SULSHADERCOMBINE_H__

#include "SulShaderBase.h"
#include "SulExport.h"

class SUL_EXPORT CSulShaderCombine : public CSulShaderBase
{
public:
	CSulShaderCombine( osg::Node* pNode, float fExposure, float fBloomFactor, float fMaxBright, osg::Texture* pSrcColor, osg::Texture* pSrcBloom, osg::Program* pProg=0 );

	void						setExposure( float f );
	void						setFactor( float f );
	void						setMax( float f );

private:
	osg::ref_ptr<osg::Uniform>	m_rUniform_exposure;
	osg::ref_ptr<osg::Uniform>	m_rUniform_bloomfactor;
	osg::ref_ptr<osg::Uniform>	m_rUniform_maxbright;
	osg::ref_ptr<osg::Uniform>	m_rUniform_bloom;
};

#endif // __SULSHADERCOMBINE_H__