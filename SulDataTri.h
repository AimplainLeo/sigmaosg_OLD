// SulDataTri.h

#ifndef __SULDATATRI_H__
#define __SULDATATRI_H__

#include <osg/vec3>

class CSulDataTri
{
public:
	CSulDataTri()
	{
	}

	CSulDataTri( const osg::Vec3& p0, const osg::Vec3& p1, const osg::Vec3& p2 )
	{
		m_p0 = p0;
		m_p1 = p1;
		m_p2 = p2;
	}

	const float getArea() const;

	osg::Vec3 getRandomPoint() const;

public:
	osg::Vec3	m_p0;
	osg::Vec3	m_p1;
	osg::Vec3	m_p2;
};

#endif // __SULDATATRI_H__

