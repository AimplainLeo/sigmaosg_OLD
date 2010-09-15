// SulShaderGuiFrame.h

#ifndef __SULSHADERGUIFRAME_H__
#define __SULSHADERGUIFRAME_H__

#include "SulShaderBase.h"

class SUL_EXPORT CSulShaderGuiFrame : public CSulShaderBase
{
public:
	CSulShaderGuiFrame( osg::Node* pNode, osg::Program* pProg=0 );

private:
	osg::ref_ptr<osg::Uniform>	m_rUniform_w;
	osg::ref_ptr<osg::Uniform>	m_rUniform_h;
	osg::ref_ptr<osg::Uniform>	m_rUniform_border;
};


#endif // __SULSHADERGUIFRAME_H__