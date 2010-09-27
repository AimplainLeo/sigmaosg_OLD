// SulGeomBase.h

#ifndef __SULGEOMBASE_H__
#define __SULGEOMBASE_H__

#include "SulExport.h"
#include <osg/Geometry>

class SUL_EXPORT CSulGeomBase : public osg::Referenced
{
public:
									CSulGeomBase();

	osg::Drawable*					getDrawable();
	void							setDrawable( osg::Drawable* pDrawable );

protected:
	virtual void					createDrawable() = 0;

private:
	osg::ref_ptr<osg::Drawable>		m_drawable;
};

#endif // __SULGEOMBASE_H__