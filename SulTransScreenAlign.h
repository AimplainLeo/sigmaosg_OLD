// SulTransScreenAlign.h

#ifndef __SULTRANSSCREENALIGN_H__
#define __SULTRANSSCREENALIGN_H__

#include "SulExport.h"
#include <osg/Referenced>

class SUL_EXPORT CSulTransScreenAlign : public osg::Referenced
{
public:
										CSulTransScreenAlign( float w, float h );

	void								AddChild( osg::Node* pNode );
	osg::MatrixTransform*				GetMatrixTransform();
	osg::Projection*					GetProjection();

private:
	osg::ref_ptr<osg::MatrixTransform>	m_rTrans;
	osg::ref_ptr<osg::Projection>		m_rProj;
};

#endif // __SULTRANSSCREENALIGN_H__