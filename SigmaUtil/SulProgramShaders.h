// SulProgramShaders.h

#ifndef __SULPROGRAMSHADERS_H__
#define __SULPROGRAMSHADERS_H__

#include "SulExport.h"
#include "SulString.h"

class SUL_EXPORT CSulProgramShaders : public osg::Program
{
public:
	CSulProgramShaders( CSulString sShaderFiles );
};

#endif // __SULPROGRAMSHADERS_H__