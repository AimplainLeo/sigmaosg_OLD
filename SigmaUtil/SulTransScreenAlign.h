// SulTransScreenAlign.h

#ifndef __SULTRANSSCREENALIGN_H__
#define __SULTRANSSCREENALIGN_H__

#include "SulExport.h"
#include <osg/Referenced>

class SUL_EXPORT CSulTransScreenAlign : public osg::Referenced
{
public:
										CSulTransScreenAlign( float left, float right, float bottom, float top );
										CSulTransScreenAlign( float w, float h );

	void								addChild( osg::Node* pNode );
	osg::MatrixTransform*				getMatrixTransform();
	osg::Projection*					getProjection();

private:
	osg::ref_ptr<osg::MatrixTransform>	m_rTrans;
	osg::ref_ptr<osg::Projection>		m_rProj;
};

#endif // __SULTRANSSCREENALIGN_H__