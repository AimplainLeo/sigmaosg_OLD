// SulEntity.h

#ifndef __SULENTITY_H__
#define __SULENTITY_H__

#include <osg/referenced>

class CSulEntity : public osg::Referenced
{
public:
	virtual void create( const osg::Vec3& pos ) {}
};

#endif // __SULENTITY_H__