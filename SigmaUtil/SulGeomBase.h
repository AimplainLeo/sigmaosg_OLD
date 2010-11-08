// SulGeomBase.h

#ifndef __SULGEOMBASE_H__
#define __SULGEOMBASE_H__

#include "SulExport.h"
#include <osg/Geometry>

class SUL_EXPORT CSulGeomBase : public osg::Referenced
{
public:
	typedef std::vector< osg::ref_ptr<osg::Drawable> >	VEC_DRAWABLE;

public:
									CSulGeomBase();

	osg::Drawable*					getDrawable();
	void							setDrawable( osg::Drawable* pDrawable );

	osg::Geode*						createGeode();

protected:
	virtual void					createDrawable() = 0;

private:
	//osg::ref_ptr<osg::Drawable>		m_drawable;
	VEC_DRAWABLE					m_vecDrawable;
};

#endif // __SULGEOMBASE_H__