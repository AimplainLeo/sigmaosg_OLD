// SulShaderNormalMapObjectSpace.h

#ifndef __SULSHADERNORMALMAPOBJECTSPACE_H__
#define __SULSHADERNORMALMAPOBJECTSPACE_H__

#include "SulShaderBase.h"
#include "SulTypes.h"

class SUL_EXPORT CSulShaderNormalMapObjectSpace : public CSulShaderBase
{
public:
	CSulShaderNormalMapObjectSpace( osg::Node* pNode, sigma::int32 indexLight=0, osg::Program* pProg=0 );
};

#endif // __SULSHADERNORMALMAPOBJECTSPACE_H__