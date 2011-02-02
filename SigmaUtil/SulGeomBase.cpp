// SulGeomBase.cpp

#include "stdafx.h"
#include "SulGeomBase.h"
#include <osg/geode>

CSulGeomBase::CSulGeomBase()
{
}

osg::Drawable* CSulGeomBase::getDrawable()
{
	if ( m_vecDrawable.empty() )
	{
		createDrawable();
	}

	return m_vecDrawable[0];
}

void CSulGeomBase::addDrawable( osg::Drawable* pDrawable )
{
	m_vecDrawable.push_back( pDrawable );
}

osg::Geode* CSulGeomBase::createGeode()
{
	if ( m_vecDrawable.empty() )
		createDrawable();

	osg::Geode* pGeode = new osg::Geode;

	VEC_DRAWABLE::iterator i = m_vecDrawable.begin();
	VEC_DRAWABLE::iterator ie = m_vecDrawable.end();
	while ( i!=ie )
	{
		pGeode->addDrawable( i->get() );
		++i;
	}

	return pGeode;
}

