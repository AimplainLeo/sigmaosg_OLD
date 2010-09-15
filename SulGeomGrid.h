// SulGeomGrid.h

#ifndef __SULGEOMGRID_H__
#define __SULGEOMGRID_H__

#include "SulExport.h"
#include "SulGeomBase.h"
#include <osg/ref_ptr>
#include <osg/PrimitiveSet>

class SUL_EXPORT CSulGeomGrid : public CSulGeomBase
{
public:
			CSulGeomGrid();

	void	Create( const osg::Vec3& vPos, float fW, float fH, float fCellW=1.0f, float fCellH=1.0f, 
				float fSubDivX=1.0f, float fSubDivY=1.0f );

	void	SetGridColor( const osg::Vec4& vColor );
	void	SetGridDivColor( const osg::Vec4& vColor );

protected:
	virtual void createDrawable();

private:
	osg::Vec4 m_colorGrid;
	osg::Vec4 m_colorGridDiv;
	osg::ref_ptr<osg::DrawElementsUInt>	m_rPrimitiveSet;
};

#endif // __SULGEOMGRID_H__