// SulShaderGaussH.h

#ifndef __SULSHADERGAUSSH_H__
#define __SULSHADERGAUSSH_H__

#include "SulShaderBase.h"

class SUL_EXPORT CSulShaderGaussH : public CSulShaderBase
{
public:
	CSulShaderGaussH( osg::Node* pNode, float fTexH, osg::Program* pProg=0 );
};

#endif // __SULSHADERGAUSS_H__