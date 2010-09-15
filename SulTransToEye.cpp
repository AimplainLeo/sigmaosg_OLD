// SulTransToEye.cpp

#include "stdafx.h"
#include "SulTransToEye.h"

CSulTransToEye::CSulTransToEye() :
osg::Transform()
{
	m_bIgnoreZ = false;
	m_vEyeOld.set( 0,0,0 );
	m_vEyeCur.set( 0,0,0 );
}

void CSulTransToEye::IgnoreZ( bool bIgnoreZ )
{
	m_bIgnoreZ = bIgnoreZ;
}

///Get the transformation matrix which moves from local coords to world coords.
bool CSulTransToEye::computeLocalToWorldMatrix(osg::Matrix& matrix,osg::NodeVisitor* nv) const
{
	if ( osgUtil::CullVisitor* cv = dynamic_cast<osgUtil::CullVisitor*>(nv) )
	{
		osg::Vec3 eyePointLocal = cv->getEyeLocal();

		if ( m_bIgnoreZ )
		{
			eyePointLocal.z() = 0;
		}

		if ( m_vEyeCur.x()<0.1f && m_vEyeCur.y()<0.1f && m_vEyeCur.z()<0.1f )
		{
			m_vEyeOld = m_vEyeCur = eyePointLocal;
		}
		else
		{
			m_vEyeOld = m_vEyeCur;
			m_vEyeCur = eyePointLocal;
		}

		matrix.preMult(osg::Matrix::translate(eyePointLocal.x(),eyePointLocal.y(),eyePointLocal.z()));
	}
	return true;
}

///Get the transformation matrix which moves from world coords to local coords.
bool CSulTransToEye::computeWorldToLocalMatrix(osg::Matrix& matrix,osg::NodeVisitor* nv) const
{
	if ( osgUtil::CullVisitor* cv = dynamic_cast<osgUtil::CullVisitor*>(nv) )
	{
		osg::Vec3 eyePointLocal = cv->getEyeLocal();

		if ( m_bIgnoreZ )
		{
			eyePointLocal.z() = 0;
		}

		if ( m_vEyeCur.x()<0.1f && m_vEyeCur.y()<0.1f && m_vEyeCur.z()<0.1f )
		{
			m_vEyeOld = m_vEyeCur = eyePointLocal;
		}
		else
		{
			m_vEyeOld = m_vEyeCur;
			m_vEyeCur = eyePointLocal;
		}

		matrix.postMult(osg::Matrix::translate(-eyePointLocal.x(),-eyePointLocal.y(),-eyePointLocal.z()));
	}
	return true;
}