// SulGeomSphere.cpp

#include "stdafx.h"
#include "SulGeomSphere.h"
#include <osg/shapedrawable>

CSulGeomSphere::CSulGeomSphere( float radius, osg::Vec3 pos ) :
CSulGeomBase(),
m_radius( radius ),
m_pos( pos ),
m_color( 1,0,0,1 )
{
}

void CSulGeomSphere::createDrawable()
{
	osg::ref_ptr<osg::TessellationHints> rHints = new osg::TessellationHints;
	rHints->setDetailRatio(2.0f);

	osg::ref_ptr<osg::Sphere> rSphere = new osg::Sphere( osg::Vec3(0, 0, 0), m_radius );
	osg::ref_ptr<osg::ShapeDrawable> rShape = new osg::ShapeDrawable( rSphere, rHints );

	rSphere->setCenter( m_pos );

	rShape->setColor( m_color );

	addDrawable( rShape );
}