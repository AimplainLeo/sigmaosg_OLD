// SulLightSpot.cpp

#include "stdafx.h"
#include "SulLightSpot.h"
#include "SulLightManager.h"

CSulLightSpot::CSulLightSpot( CSulLightManager* lm ) :
CSulLightBase( lm )
{
	setName( "CSulLightSpot" );
}

void CSulLightSpot::setDirection( const osg::Vec3& dir )
{
	m_dir = dir;
	m_dirty = true;
}

const osg::Vec3& CSulLightSpot::getDirection() const
{
	return m_dir;
}

void CSulLightSpot::setCutOff( float cutoff )
{
	m_cutoff = cutoff;
	m_dirty = true;
}

float CSulLightSpot::getCutOff()
{
	return m_cutoff;
}

void CSulLightSpot::setViewMatrixDirection( const osg::Vec3& dir )
{
	m_dirViewMatrix = dir;
}

const osg::Vec3& CSulLightSpot::getViewMatrixDirection()
{
	return m_dirViewMatrix;
}

void CSulLightSpot::traverse( osg::NodeVisitor& nv )
{
	CSulLightBase::traverse( nv );

	if ( nv.getVisitorType()==osg::NodeVisitor::CULL_VISITOR )
	{
		osgUtil::CullVisitor* cv = dynamic_cast<osgUtil::CullVisitor*>(&nv);

		osg::RefMatrix* pMV = cv->getModelViewMatrix();

		m_dirViewMatrix = osg::Matrix::transform3x3( m_dir, *pMV );

		m_lm->addVisible( this );
	}
}