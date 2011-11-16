// SulSphereIntersector.cpp

#include "stdafx.h"
#include "SulSphereIntersector.h"
#include "SulSphereIntersectorTriangle.h"
#include <osg/KdTree>
#include <osg/TriangleFunctor>
#include <osg/shapedrawable>
#include <assert.h>

CSulSphereIntersector::CSulSphereIntersector() :
_parent(0),
m_debug(false)
{
}

osgUtil::Intersector* CSulSphereIntersector::clone( osgUtil::IntersectionVisitor& iv )
{
	osg::ref_ptr<CSulSphereIntersector> p = new CSulSphereIntersector;
	p->setPosition( m_pos );
	p->setRadius( m_radius );
	p->m_hit = m_hit;
	p->m_debug = m_debug;
	p->m_debugGroup = m_debugGroup;
	p->_parent = this;

	return p.release();
}

void CSulSphereIntersector::setRadius( float r )
{
	m_radius = r;
}

void CSulSphereIntersector::setPosition( const osg::Vec3& pos )
{
	m_pos = pos;
}

bool CSulSphereIntersector::enter( const osg::Node& node )
{
	// test to see if we are interested in the node
	return !node.isCullingActive() || intersects( node.getBound() );
}

void CSulSphereIntersector::leave()
{
	// we do nothing
}

bool CSulSphereIntersector::intersects( const osg::BoundingSphere& bs )
{
	return true;
    // if bs not valid then return true based on the assumption that an invalid sphere is yet to be defined.
    if (!bs.valid()) return true; 

	// calc distance from sphere to sphere (centers)
	osg::Vec3 vLen = m_pos - bs._center;
	float len = vLen.length();

	float r = bs.radius();
	if ( (r+m_radius)<len )
		return false;

    // passed all the rejection tests so sphere must intersect bounding sphere, return true.
    return true;
}

osg::Node* CSulSphereIntersector::enableDebug()
{
	m_debug = true;
	m_debugGroup = new osg::Group;
	return m_debugGroup;
}

void CSulSphereIntersector::intersect( osgUtil::IntersectionVisitor& iv, osg::Drawable* drawable )
{
    osg::KdTree* kdTree = iv.getUseKdTreeWhenAvailable() ? dynamic_cast<osg::KdTree*>(drawable->getShape()) : 0;
    if ( kdTree )
    {
		assert( 0 ); // not supported yet
		return;
	}

	if (iv.getDoDummyTraversal()) return;

	osg::TriangleFunctor<CSulSphereIntersectorTriangle> ti;
	if ( m_debug ) 
	{
		m_debugGroup->addChild( ti.enableDebug() );
	}

/**/
		osg::Matrix matrix;
		if (iv.getViewMatrix()) matrix.preMult( *iv.getViewMatrix() );
		if (iv.getModelMatrix()) matrix.preMult( *iv.getModelMatrix() );
		osg::Matrix inverse;
		inverse.invert(matrix);
/**/

    ti.set( m_pos * inverse, m_radius );
    drawable->accept( ti );

//	osg::ShapeDrawable* p = dynamic_cast<osg::ShapeDrawable*>(drawable);
	
	if ( ti.hit() )
	{
//		p->setColor( osg::Vec4(0,1,0,1) );

		if ( _parent ) _parent->m_hit = true; else m_hit = true;
	}
	else
	{
	//	p->setColor( osg::Vec4(1,1,1,1) );
	}

}

void CSulSphereIntersector::reset()
{
	m_hit = false;
}

bool CSulSphereIntersector::containsIntersections()
{
	return m_hit;
}