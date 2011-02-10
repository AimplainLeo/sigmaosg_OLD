// SulGuiProfiler.cpp

#include "stdafx.h"
#include "SulGuiProfiler.h"

CSulGuiProfiler::CSulGuiProfiler() :
CSulGuiCanvas( "PROFILER" )
{
}

CSulGuiProfiler::CSulGuiProfiler( CSulProfiler* pProfiler ) :
CSulGuiCanvas( "PROFILER" )
{
	setProfiler( pProfiler );
}

void CSulGuiProfiler::setProfiler( CSulProfiler* pProfiler )
{
	m_rProfiler = pProfiler;


	osg::MatrixTransform* mt = new osg::MatrixTransform;
	osg::Matrix m;
	m.setTrans( osg::Vec3(16,16,0) );
	mt->setMatrix( m );

	mt->addChild( m_rProfiler->getChart() );

	addChild( mt );
}

void CSulGuiProfiler::setupAttr( CSulXmlAttr* pAttr )
{
	CSulGuiCanvas::setupAttr( pAttr );
}

void CSulGuiProfiler::init()
{
	CSulGuiCanvas::init();
}
