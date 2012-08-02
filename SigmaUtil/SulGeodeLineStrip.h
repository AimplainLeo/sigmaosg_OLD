// SulGeodeLineStrip.h

#ifndef __SULGEODELINESTRIP_H__
#define __SULGEODELINESTRIP_H__

#include "SulGeode.h"
#include "SulGeomLineStrip.h"
#include "SulTypes.h"
#include "SulExport.h"
#include <osg/linewidth>

class SUL_EXPORT CSulGeodeLineStrip : public CSulGeode
{
public:
					CSulGeodeLineStrip();
					CSulGeodeLineStrip( const sigma::VEC_VEC3& vecVector );

	void			setLines( const sigma::VEC_VEC3& vecVector );

	void			setWidth( float width );
	
private:
	void			initConstructor();
	void			createDrawable();

private:
	osg::ref_ptr<CSulGeomLineStrip>	m_geomLineStrip;

	sigma::VEC_VEC3					m_vecVector;
	osg::ref_ptr<osg::Vec4Array>	m_rColors;
	osg::ref_ptr<osg::LineWidth>	m_rLineWidth;
	float							m_width;
};

#endif // __SULGEODELINESTRIP_H__