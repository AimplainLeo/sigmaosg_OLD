// SulGeomBase.h

#ifndef __SULGEOMBASE_H__
#define __SULGEOMBASE_H__

#if 0

#include "SulExport.h"
#include <osg/Group>

class SUL_EXPORT CSulGeomBase : public osg::Group
{
public:
//	typedef std::vector< osg::ref_ptr<osg::Drawable> >	VEC_DRAWABLE;

public:
									CSulGeomBase();

									/*
	osg::Drawable*					getDrawable();

	// returns only the first drawable (if any)
	void							addDrawable( osg::Drawable* pDrawable );
	*/

//	osg::Geode*						createGeode();

protected:
//	virtual void					createDrawable() = 0;

	/*
private:
	VEC_DRAWABLE					m_vecDrawable;
	*/
};

#endif 

#endif // __SULGEOMBASE_H__