// SulGeomBoundingBox.h

#ifndef __SULGEOMBOUNDINGBOX_H__
#define __SULGEOMBOUNDINGBOX_H__

#include "SulExport.h"
#include "SulGeomBase.h"
#include "SulTypes.h"

class SUL_EXPORT CSulGeomBoundingBox : public CSulGeomBase
{
public:
						CSulGeomBoundingBox( const osg::BoundingBox& bound );

	void				update( const osg::BoundingBox& bound );

protected:
	virtual void		createDrawable();

private:
	osg::PrimitiveSet*	AddCornerLines(osg::Vec3Array& verts, const osg::BoundingBox& bound, unsigned i0, unsigned i1, unsigned i2, unsigned i3);

	void				ModifyCornerLines( sigma::uint32 index, osg::Vec3Array* pVerts, const osg::BoundingBox& bound, unsigned i0, unsigned i1, unsigned i2, unsigned i3 );

private:
	osg::BoundingBox	m_bb;
};

#endif // __SULGEOMBOUNDINGBOX_H__