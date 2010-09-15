// SulShaderGaussV.h

#ifndef __SULSHADERGAUSSV_H__
#define __SULSHADERGAUSSV_H__

#include "SulShaderBase.h"

class SUL_EXPORT CSulShaderGaussV : public CSulShaderBase
{
public:
	CSulShaderGaussV( osg::Node* pNode, float fTexV, osg::Program* pProg=0 );
};

#endif // __SULSHADERGAUSSV_H__