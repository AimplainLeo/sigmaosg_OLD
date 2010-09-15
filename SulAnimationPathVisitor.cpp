// SulAnimationPathVisitor.cpp

#include "stdafx.h"
#include "SulAnimationPathVisitor.h"
#include <osg/MatrixTransform>

CSulAnimationPathVisitor::CSulAnimationPathVisitor( const osg::AnimationPath::ControlPoint& cp, const osg::Vec3d& pivotPoint ) :
m_cp(cp),
m_pivotPoint(pivotPoint)
{
}

void CSulAnimationPathVisitor::apply( osg::MatrixTransform& mt )
{
	osg::Matrix matrix;
	m_cp.getMatrix(matrix);
    mt.setMatrix( osg::Matrix::translate(-m_pivotPoint)*matrix );
}
