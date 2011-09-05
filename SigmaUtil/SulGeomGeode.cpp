// SulGeomGeode.cpp

#include "stdafx.h"
#include "SulGeomGeode.h"
#include <osg/depth>

void CSulGeomGeode::enableBlend()
{
	getOrCreateStateSet()->setMode( GL_BLEND, osg::StateAttribute::ON );
}

void CSulGeomGeode::zbuffer( bool bEnable )
{
	osg::Depth* depth = new osg::Depth;
	depth->setWriteMask( bEnable?true:false );
	getOrCreateStateSet()->setAttributeAndModes( depth, osg::StateAttribute::ON );
}