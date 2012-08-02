// SulGeomBoundingBox.h

#ifndef __SULGEOMBOUNDINGBOX_H__
#define __SULGEOMBOUNDINGBOX_H__

#include "SulExport.h"
#include "SulGeom.h"
#include "SulTypes.h"
#include <osg/geometry>
#include <osg/geode>

class SUL_EXPORT CSulGeomBoundingBox : public osg::Geode
{
public:
						CSulGeomBoundingBox();
						CSulGeomBoundingBox( const osg::BoundingBox& bound );

	void				update( const osg::BoundingBox& bound );

private:
	virtual void		createDrawable();

private:
	osg::PrimitiveSet*	AddCornerLines(osg::Vec3Array& verts, const osg::BoundingBox& bound, unsigned i0, unsigned i1, unsigned i2, unsigned i3);

	void				ModifyCornerLines( sigma::uint32 index, osg::Vec3Array* pVerts, const osg::BoundingBox& bound, unsigned i0, unsigned i1, unsigned i2, unsigned i3 );

private:
	osg::BoundingBox			m_bb;
	osg::ref_ptr<osg::Geometry> m_rGeo;
};

#endif // __SULGEOMBOUNDINGBOX_H__