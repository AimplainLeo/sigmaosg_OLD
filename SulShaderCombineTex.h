// SulShaderCombineTex.h


#ifndef __SULSHADERCOMBINETEX_H__
#define __SULSHADERCOMBINETEX_H__

#include "SulShaderBase.h"

class CSulShaderCombineTex : public CSulShaderBase
{
public:
	CSulShaderCombineTex( osg::Node* pNode, osg::Texture* pTex0, osg::Texture* pTex1, osg::Program* pProg=0 );
};

#endif // __SULSHADERCOMBINE_H__