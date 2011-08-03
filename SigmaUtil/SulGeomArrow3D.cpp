// SulGeomArrow3D.cpp

#include "stdafx.h"
#include "SulGeomArrow3D.h"
#include <osg/shapedrawable>

CSulGeomArrow3D::CSulGeomArrow3D( float len, const osg::Vec4& colorHead, const osg::Vec4& colorBody ) :
osg::Geode(),
m_colorHead( colorHead ),
m_colorBody( colorBody )
{
	m_headLen		= len/3.0f; 
	m_headRadius	= len/5.0f;

	m_bodyLen		= len-m_headLen;
	m_bodyRadius	= len/10.0f;

	createDrawable();
}

void CSulGeomArrow3D::createDrawable()
{
	// note: osg uses the center value for "center of mass" that is is why the cone has a 0.25 offset!
	osg::ref_ptr<osg::ShapeDrawable> rShapeHead = new osg::ShapeDrawable( new osg::Cone(osg::Vec3(0.0f,0.0f,m_bodyLen+m_headLen*0.25f), m_headRadius, m_headLen) );
	rShapeHead->setColor( m_colorHead );

	osg::ref_ptr<osg::ShapeDrawable> rShapeBody = new osg::ShapeDrawable( new osg::Cylinder(osg::Vec3(0.0f,0.0f,m_bodyLen/2.0f),m_bodyRadius,m_bodyLen) );
	rShapeBody->setColor( m_colorBody );

	addDrawable( rShapeHead );
	addDrawable( rShapeBody );
}
