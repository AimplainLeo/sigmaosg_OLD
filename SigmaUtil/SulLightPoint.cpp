// SulLightPoint.cpp

#include "stdafx.h"
#include "SulLightPoint.h"
#include "SulLightManager.h"

CSulLightPoint::CSulLightPoint( CSulLightManager* lm ) :
CSulLightBase( lm )
{
}

void CSulLightPoint::traverse( osg::NodeVisitor& nv )
{
	CSulLightBase::traverse( nv );

	if( nv.getVisitorType()==osg::NodeVisitor::CULL_VISITOR )
	{
		m_lm->addVisible( this );
	}
}
