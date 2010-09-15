// SulShaderColor.h

#ifndef __SULSHADERCOLOR_H__
#define __SULSHADERCOLOR_H__

#include "SulShaderBase.h"

class CSulShaderColor : public CSulShaderBase
{
public:
			CSulShaderColor( osg::Node* pNode, const osg::Vec4& color, osg::Program* pProg=0 );

private:
	osg::ref_ptr<osg::Uniform>	m_rUniform_color;
};

#endif // __SULSHADERCOLOR_H__