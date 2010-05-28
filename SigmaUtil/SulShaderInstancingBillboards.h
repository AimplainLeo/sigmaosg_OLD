// SulShaderInstancingBillboards.h

#ifndef __SULSHADERINSTANCINGBILLBOARDS_H__
#define __SULSHADERINSTANCINGBILLBOARDS_H__

#include "SulShaderBase.h"
#include "SulTypes.h"

class CSulShaderInstancingBillboards : public CSulShaderBase
{
public:
	CSulShaderInstancingBillboards( osg::Node* pNode, Sigma::int32 numInstances, Sigma::int32 texUnit=0, Sigma::int32 texSizeSquared=256, unsigned int useLights=0x00000001, float min=1.0f, float max=1.0f, osg::Program* pProg=0 );

private:
	osg::ref_ptr<osg::Uniform>		m_rUniform_windFactor;
	osg::ref_ptr<osg::Uniform>		m_rUniform_grassStretch;
	osg::ref_ptr<osg::Uniform>		m_rUniform_texPositions;
	osg::ref_ptr<osg::Uniform>		m_rUniform_numInstances;
	osg::ref_ptr<osg::Uniform>		m_rUniform_useLights;
	osg::ref_ptr<osg::Uniform>		m_rUniform_min;
	osg::ref_ptr<osg::Uniform>		m_rUniform_max;
};


#endif // __SULSHADERINSTANCINGBILLBOARDS_H__