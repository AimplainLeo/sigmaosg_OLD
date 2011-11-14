// SulSphereIntersector.h

#ifndef __SULSPHEREINTERSECTOR_H__
#define __SULSPHEREINTERSECTOR_H__

#include <osgUtil/IntersectionVisitor>

class CSulSphereIntersector : public osgUtil::Intersector
{
public:
	void		setRadius( float r );
	void		setPosition( const osg::Vec3& pos );

public:
	virtual Intersector* clone( osgUtil::IntersectionVisitor& iv );

	virtual bool enter( const osg::Node& node );

	virtual void leave();

	virtual void intersect( osgUtil::IntersectionVisitor& iv, osg::Drawable* drawable );

	virtual void reset();

	virtual bool containsIntersections();

private:
	bool intersects( const osg::BoundingSphere& bs );

private:
	float m_radius;
	osg::Vec3 m_pos;
	bool m_hit;

};

#endif // __SULSPHEREINTERSECTOR_H__