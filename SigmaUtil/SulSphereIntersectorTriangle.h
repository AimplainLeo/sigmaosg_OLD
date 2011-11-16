// SulSphereIntersectorTriangle.h

#ifndef __SULSPHEREINTERSECTORTRIANGLE_H__
#define __SULSPHEREINTERSECTORTRIANGLE_H__

#include "SulIntTriangle.h"
#include "SulGeomTriangle.h"
#include <osg/vec3>
#include <osg/plane>

class CSulSphereIntersectorTriangle
{
public:
	CSulSphereIntersectorTriangle()
	{
		m_hit = false;
	}

	osg::Node* enableDebug()
	{
		// note: the triangle is drawn in model space (you are probably viewing it in view space!)
		osg::Vec3 v( 0, 0, 0 );
		m_debugTri = new CSulGeomTriangle( v, v, v );
		m_debugTri->setColor( 0, 0, 1, 1 );
		return m_debugTri;
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
		// FIXME: currently a 1000.0f+1000.0f line segment,..needs to be a ray!
		osg::Vec3 s = (m_pos-plane.getNormal()*1000.0f);
		osg::Vec3 e = (m_pos+plane.getNormal()*1000.0f);

		osg::ref_ptr<osg::LineSegment> ls = new osg::LineSegment( s, e );
		CSulDataTri tri( v1, v2, v3 );
		osg::Vec3 vHit;
		if ( sulIntTriangle( *ls, tri, vHit, false ) )
		{
			// debugging
			if ( m_debugTri.valid() )
			{
				m_debugTri->setVertices( v1, v2, v3 );
			}
			
			m_hit = true;
			return;
		}
	}

private:
	float m_radius;
	osg::Vec3 m_pos;
	bool m_hit;
	osg::ref_ptr<CSulGeomTriangle>	m_debugTri;
};

#endif // __SULSPHEREINTERSECTORTRIANGLE_H__
