// SulShaderStreak.h

#ifndef __SULSHADERSTREAK_H__
#define __SULSHADERSTREAK_H__

#include "SulShaderBase.h"

class SUL_EXPORT CSulShaderStreak : public CSulShaderBase
{
public:
			CSulShaderStreak( osg::Node* pNode, float offsetFactor, float glowFactor=0.2f, osg::Program* pProg=0 );

	void	setGlowFactor( float glowFactor );

private:
	osg::ref_ptr<osg::Uniform>	m_rUniform_offsetFactor;
	osg::ref_ptr<osg::Uniform>	m_rUniform_glowFactor;
};


#endif // __SULSHADERSTREAK_H__