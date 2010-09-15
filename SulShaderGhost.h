// SulShaderGhost.h

#ifndef __SULSHADERGHOST_H__
#define __SULSHADERGHOST_H__

#include "SulShaderBase.h"

class CSulShaderGhost : public CSulShaderBase
{
public:
	CSulShaderGhost( osg::Node* pNode, osg::Program* pProg=0 );
};

#endif // __SULSHADERGHOST_H__