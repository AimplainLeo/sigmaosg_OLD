// SulShaderBase.h

#ifndef __SULSHADERBASE_H__
#define __SULSHADERBASE_H__

#include "SulExport.h"
#include <osg/referenced>
#include <osg/program>

class SUL_EXPORT CSulShaderBase : public osg::Referenced
{
public:
			CSulShaderBase( osg::Node* pNode, osg::Program* pProg=0 );

	void	enable( bool bEnable=true );

	void	addShaderFrag( const std::string& file );
	void	addShaderVert( const std::string& file );

protected:
	osg::ref_ptr<osg::Program>		m_rProg;
	osg::ref_ptr<osg::Program>		m_rProgDefault;
	osg::ref_ptr<osg::StateSet>		m_rSS;
	bool							m_bEnable;
};

#endif // __SULSHADERBASE_H__