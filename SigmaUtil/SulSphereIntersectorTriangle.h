// SulSphereIntersectorTriangle.h

#ifndef __SULSPHEREINTERSECTORTRIANGLE_H__
#define __SULSPHEREINTERSECTORTRIANGLE_H__

#include "SulIntTriangle.h"
#include "SulGeomTriangle.h"
#include <osg/vec3>
#include <osg/plane>

extern osg::Group* pGG;

class CSulSphereIntersectorTriangle
{
public:
	CSulSphereIntersectorTriangle()
	{
		m_hit = false;
	}

	void set( const osg::Vec3& pos, float r )
	{
		m_hit = false;
		m_pos =  pos;
		m_radius = r;
	}

	bool hit()
	{
		return m_hit;
	}

	inline void operator () ( const osg::Vec3& v1, const osg::Vec3& v2, const osg::Vec3& v3, bool treatVertexDataAsTemporary )
	{
		// if triangle is degenerated
		if (v1==v2 || v2==v3 || v1==v3) return;

		// if any one vertex is inside the sphere, then there is hit
		if ( (v1-m_pos).length()<m_radius || (v1-m_pos).length()<m_radius || (v1-m_pos).length()<m_radius )
		{
			m_hit = true;
			return;
		}

		// test if sphere intersects triangle plane, if not leave (no intersection)
		osg::Plane plane = osg::Plane( v1, v2, v3 );
		float d = plane.distance( m_pos );
		if ( fabs(d)>m_radius )
			return;

		// create a ray from m_pos with planes normal and test that the ray interestects the triangle
		osg::ref_ptr<osg::LineSegment> ls = new osg::LineSegment( m_pos-plane.getNormal()*1000.0f, m_pos+plane.getNormal()*1000.0f );
		CSulDataTri tri( v1, v2, v3 );
		osg::Vec3 vHit;
		if ( sulIntTriangle( *ls, tri, vHit, false ) )
		{
			CSulGeomTriangle* p = new CSulGeomTriangle( v1, v2, v3 );
			p->setColor( 0, 0, 1, 1 );
			pGG->addChild( p );

			
			m_hit = true;
			return;
		}
	}

private:
	float m_radius;
	osg::Vec3 m_pos;
	bool m_hit;
};

#endif // __SULSPHEREINTERSECTORTRIANGLE_H__
