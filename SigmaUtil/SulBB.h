// SulBB.h

#ifndef __SULBB_H__
#define __SULBB_H__

#include "SulGeomBoundingBox.h"
#include <osg/ComputeBoundsVisitor>

class CSulBB : public CSulGeomBoundingBox
{
public:
	CSulBB( osg::Node* pObj )
	{
		static const char fragShadeMono[] = 
			"\n"
			"void main(void) \n"
			"{ \n"
			"    gl_FragColor = vec4 (0.0, 1.0, 0.0, 1.0); \n"
			"}\n";

		osg::Shader* shaderFrag = new osg::Shader( osg::Shader::FRAGMENT, fragShadeMono );
		osg::ref_ptr<osg::Program> prg = new osg::Program;
		prg->addShader( shaderFrag );
		getOrCreateStateSet()->setAttribute( prg );

		osg::ref_ptr<osg::ComputeBoundsVisitor> tmp = new osg::ComputeBoundsVisitor;
		pObj->accept( *tmp );
		osg::BoundingBox bb = tmp->getBoundingBox();
		update( bb );

	}
};

#endif // __SULBB_H__