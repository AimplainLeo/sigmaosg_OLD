// SulTestGeoms.h

#ifndef __SULTESTGEOMS_H__
#define __SULTESTGEOMS_H__

#include <osg/ShapeDrawable>
#include <osg/geode>

class CSulTestGeoms : public osg::Geode 
{
public:
	CSulTestGeoms()
	{
		addDrawable( m_rSphere = new osg::ShapeDrawable( new osg::Sphere(osg::Vec3(0.0f,0.0f,0.0f),0.5f) ) );
		addDrawable( new osg::ShapeDrawable( new osg::Box(osg::Vec3(2.0f,0.0f,0.0f),2.0f) ) );
		addDrawable( new osg::ShapeDrawable( new osg::Cone(osg::Vec3(4.0f,0.0f,0.0f),0.5f,3.0f) ) );
		addDrawable( new osg::ShapeDrawable( new osg::Cylinder(osg::Vec3(6.0f,0.0f,0.5f),0.5f,3.0f) ) );
		addDrawable( new osg::ShapeDrawable( new osg::Capsule(osg::Vec3(8.0f,0.0f,1.0f),0.5f,3.0f) ) );

		m_rSphere->setName( "CSulTestGeoms-> Sphere drawable" );
	}

public:
	osg::ref_ptr<osg::ShapeDrawable>	m_rSphere;
};

#endif // __SULTESTGEOMS_H__