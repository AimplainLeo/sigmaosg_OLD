// SulSphereIntersector.h

#ifndef __SULSPHEREINTERSECTOR_H__
#define __SULSPHEREINTERSECTOR_H__

#include "SulExport.h"
#include <osgUtil/IntersectionVisitor>

class SUL_EXPORT CSulSphereIntersector : public osgUtil::Intersector
{
public:
				CSulSphereIntersector();

	void		setRadius( float r );
	void		setPosition( const osg::Vec3& pos );

	osg::Node*	enableDebug();

public:
	virtual Intersector* clone( osgUtil::IntersectionVisitor& iv );

	virtual bool enter( const osg::Node& node );

	virtual void leave();

	virtual void intersect( osgUtil::IntersectionVisitor& iv, osg::Drawable* drawable );

	virtual void reset();

	virtual bool containsIntersections();

private:
	bool intersects( const osg::BoundingSphere& bs );

protected:
	bool m_hit;

private:
	float		m_radius;
	osg::Vec3	m_pos;

	bool		m_debug;
	osg::ref_ptr<osg::Group>	m_debugGroup;

	CSulSphereIntersector* _parent;
};

#endif // __SULSPHEREINTERSECTOR_H__