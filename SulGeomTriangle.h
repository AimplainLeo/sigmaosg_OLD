// SulGeomTriangle.h

#ifndef __SULGEOMTRIANGLE_H__
#define __SULGEOMTRIANGLE_H__

#include "SulGeomBase.h"
#include "SulDataTri.h"

class CSulGeomTriangle : public CSulGeomBase
{
public: 
	enum TYPE
	{
		EQUILATERAL,
		ARBITRARY 
	};

public:
				// will create an equilateral triangle with distance d from each corner
				// triangle is created in plane X, Y with z=0
				CSulGeomTriangle( float d=1.0f );

				// will create an arbitrary triangle based on the 3 given positions
				CSulGeomTriangle( const osg::Vec3& p0, const osg::Vec3& p1, const osg::Vec3& p2 );

	void		setColor( float r, float g, float b, float a );

	const CSulDataTri	getTriangle() const;

protected:
	void		createDrawable();

private:
	TYPE		m_type;
	float		m_distance;
	osg::Vec3	m_p0;
	osg::Vec3	m_p1;
	osg::Vec3	m_p2;
	osg::ref_ptr<osg::Vec4Array>	m_rColors;
};

#endif // __SULGEOMTRIANGLE_H__