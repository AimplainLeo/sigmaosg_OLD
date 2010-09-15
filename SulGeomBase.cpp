// SulGeomBase.cpp

#include "stdafx.h"
#include "SulGeomBase.h"

CSulGeomBase::CSulGeomBase()
{
}

/*
osg::Geometry* CSulGeomBase::getGeometry()
{
	if ( !m_rGeo.valid() )
	{
		createGeometry();
	}

	return m_rGeo.get();
}

void CSulGeomBase::setGeom( osg::Geometry* pGeom )
{
	m_rGeo = pGeom;
}

void CSulGeomBase::createGeometry()
{
	// create drawable geometry object
    m_rGeo = new osg::Geometry;
}
*/

osg::Drawable* CSulGeomBase::getDrawable()
{
	if ( !m_drawable.valid() )
	{
		createDrawable();
	}

	return m_drawable;
}

void CSulGeomBase::setDrawable( osg::Drawable* pDrawable )
{
	m_drawable = pDrawable;
}
