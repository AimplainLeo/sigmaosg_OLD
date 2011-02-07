// SulGeomGrid.h

#ifndef __SULGEOMGRID_H__
#define __SULGEOMGRID_H__

#include "SulExport.h"
#include "SulGeomBase.h"
#include <osg/ref_ptr>
#include <osg/PrimitiveSet>
#include <osg/Geode>
#include <osg/geometry>

class SUL_EXPORT CSulGeomGrid : public osg::Geode
{
public:
			CSulGeomGrid();

	void	Create( const osg::Vec3& vPos, float fW, float fH, float fCellW=1.0f, float fCellH=1.0f, 
				float fSubDivX=1.0f, float fSubDivY=1.0f );

	void	SetGridColor( const osg::Vec4& vColor );
	void	SetGridDivColor( const osg::Vec4& vColor );

private:
	virtual void createDrawable();

private:
	osg::Vec4 m_colorGrid;
	osg::Vec4 m_colorGridDiv;
	osg::ref_ptr<osg::DrawElementsUInt>	m_rPrimitiveSet;
	osg::ref_ptr<osg::Geometry>	m_rGeo;
};

#endif // __SULGEOMGRID_H__