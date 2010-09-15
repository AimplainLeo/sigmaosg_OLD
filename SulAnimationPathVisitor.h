// SulAnimationPathVisitor.h

#ifndef __SULANIMATIONPATHVISITOR_H__
#define __SULANIMATIONPATHVISITOR_H__

#include "SulExport.h"
#include <osg/NodeVisitor>
#include <osg/AnimationPath>

class SUL_EXPORT CSulAnimationPathVisitor : public osg::NodeVisitor
{
public:
										CSulAnimationPathVisitor( const osg::AnimationPath::ControlPoint& cp, const osg::Vec3d& pivotPoint );

	void								apply( osg::MatrixTransform& mt );

private:
	osg::AnimationPath::ControlPoint	m_cp;
    osg::Vec3d							m_pivotPoint;
};

#endif // __SULANIMATIONPATHVISITOR_H__