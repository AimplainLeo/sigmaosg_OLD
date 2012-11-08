// SulGeomLineStrip.h

#ifndef __SULGEOMLINESTRIP_H__
#define __SULGEOMLINESTRIP_H__

#include "SulGeom.h"
#include "SulTypes.h"
#include "SulExport.h"
#include <osg/linewidth>
#include <osg/Geode>
 
class SUL_EXPORT CSulGeomLineStrip : public CSulGeom
{
public: 
					CSulGeomLineStrip( const osg::Vec4& color=osg::Vec4(1,1,1,1) );
					CSulGeomLineStrip( const sigma::VEC_VEC3& vecVector );
					CSulGeomLineStrip( const sigma::VEC_VEC3& vecVector, const osg::Vec4& color=osg::Vec4(1,1,1,1) );

	void			setLines( const sigma::VEC_VEC3& vecVector );
	void			setWidth( float width );

protected:
	virtual void	createDrawable();

private:
	void			initConstructor();

private:
	sigma::VEC_VEC3					m_vecVector;
	osg::ref_ptr<osg::Vec4Array>	m_rColors;
	osg::ref_ptr<osg::LineWidth>	m_rLineWidth;
	float							m_width;
	osg::Vec4						m_color;
};



#endif // __SULGEOMLINESTRIP_H__