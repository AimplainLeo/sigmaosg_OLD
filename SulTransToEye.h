// SulTransToEye.h

#ifndef __SULTRANSTOEYE_H__
#define __SULTRANSTOEYE_H__

class CSulTransToEye : public osg::Transform
{
public:
					CSulTransToEye();

	void			IgnoreZ( bool bIgnoreZ );

	///Get the transformation matrix which moves from local coords to world coords.
	virtual bool	computeLocalToWorldMatrix(osg::Matrix& matrix,osg::NodeVisitor* nv) const;

	///Get the transformation matrix which moves from world coords to local coords.
	virtual bool	computeWorldToLocalMatrix(osg::Matrix& matrix,osg::NodeVisitor* nv) const;

private:
	bool			m_bIgnoreZ;

public:
	mutable osg::Vec3		m_vEyeCur;
	mutable osg::Vec3		m_vEyeOld;
};

#endif // __SULTRANSTOEYE_H__