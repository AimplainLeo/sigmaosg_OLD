// SulShaderZ.h

#ifndef __SULSHADERZ_H__
#define __SULSHADERZ_H__

#include "SulShaderBase.h"

class SUL_EXPORT CSulShaderZ : public CSulShaderBase
{
public:
			CSulShaderZ( osg::Node* pNode, osg::Program* pProg=0 );
};

#endif // __SULSHADERZ_H__