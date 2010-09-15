// SulShaderNormalMapTangentSpace.h

#ifndef __SULSHADERNORMALMAPTANGENTSPACE_H__
#define __SULSHADERNORMALMAPTANGENTSPACE_H__

#include "SulShaderBase.h"
#include "SulTypes.h"

class SUL_EXPORT CSulShaderNormalMapTangentSpace : public CSulShaderBase
{
public:
	CSulShaderNormalMapTangentSpace( osg::Node* pNode, sigma::int32 indexLight=0, osg::Program* pProg=0 );
};

#endif // __SULSHADERNORMALMAPTANGENTSPACE_H__