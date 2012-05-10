// SulGeomSphere.cpp

#include "stdafx.h"
#include "SulGeomSphere.h"

CSulGeomSphere::CSulGeomSphere( float radius, osg::Vec3 pos ) :
CSulGeomGeode(),
m_radius( radius ),
m_pos( pos ),
m_color( 1,0,0,1 )
{
	createDrawable();
}

void CSulGeomSphere::createDrawable()
{
	osg::ref_ptr<osg::TessellationHints> rHints = new osg::TessellationHints;
	rHints->setDetailRatio(2.0f);

	osg::ref_ptr<osg::Sphere> rSphere = new osg::Sphere( osg::Vec3(0, 0, 0), m_radius );
	m_rShape = new osg::ShapeDrawable( rSphere, rHints );

	rSphere->setCenter( m_pos );

	m_rShape->setColor( m_color );

	addDrawable( m_rShape );
}

void CSulGeomSphere::setColor( const osg::Vec4& color )
{
	m_rShape->setColor( color );
}