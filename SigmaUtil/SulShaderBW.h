// SulShaderBW.h

#ifndef __SULSHADERBW_H__
#define __SULSHADERBW_H__

#include "SulExport.h"
#include "SulShaderBase.h"

class SUL_EXPORT CSulShaderBW : public CSulShaderBase
{
public:
			CSulShaderBW( osg::Node* pNode, osg::Program* pProg=0 );

	void	setInvert( bool bInvert );
	bool	isInvert();

private:
	osg::ref_ptr<osg::Uniform>	m_rUniform_invert;
	bool						m_bInvert;
};

#endif // __SULSHADERBW_H__