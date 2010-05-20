// SulShaderAmplifier.h

#ifndef __SULSHADERAMPLIFIER_H__
#define __SULSHADERAMPLIFIER_H__

#include "SulShaderBase.h"

class SUL_EXPORT CSulShaderAmplifier : public CSulShaderBase
{
public:
	CSulShaderAmplifier( osg::Node* pNode, float fAmp, bool bColor=false, osg::Program* pProg=0 );

	void						setAmp( float val );

private:
	osg::ref_ptr<osg::Uniform>	m_rUniform_amp;
	osg::ref_ptr<osg::Uniform>	m_rUniform_bcolor;
};

#endif // __SULSHADERAMPLIFIER_H__