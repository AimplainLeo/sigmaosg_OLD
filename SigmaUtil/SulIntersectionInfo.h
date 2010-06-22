// SulIntersectionInfo.h

#ifndef __SULINTERSECTIONINFO_H__
#define __SULINTERSECTIONINFO_H__

class CSulIntersectionInfo : public osg::Referenced
{
public:
	CSulIntersectionInfo()
	{
	}

	void setHit( const osg::Vec3& hit )
	{
		m_posHit = hit;
	}

	const osg::Vec3& getHit() const
	{
		return m_posHit;
	}

	virtual void update() = 0;

private:
	osg::Vec3	m_posHit;
};

#endif // __SULINTERSECTIONINFO_H__