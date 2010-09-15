// SulGeomBase.h

#ifndef __SULGEOMBASE_H__
#define __SULGEOMBASE_H__

#include "SulExport.h"
#include <osg/Geometry>

class SUL_EXPORT CSulGeomBase : public osg::Referenced
{
public:
								CSulGeomBase();

/*
	osg::Geometry*				getGeometry();
	void						setGeom( osg::Geometry* pGeom );
*/

	osg::Drawable*				getDrawable();
	void						setDrawable( osg::Drawable* pDrawable );

protected:
//	virtual void				createGeometry();
	virtual void				createDrawable() = 0;

private:
	//osg::ref_ptr<osg::Geometry>	m_rGeo;
	osg::ref_ptr<osg::Drawable>		m_drawable;
};

#endif // __SULGEOMBASE_H__