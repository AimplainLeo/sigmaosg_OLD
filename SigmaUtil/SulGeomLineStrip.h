// SulGeomLineStrip.h

#ifndef __SULGEOMLINESTRIP_H__
#define __SULGEOMLINESTRIP_H__

#include "SulGeomBase.h"
#include "SulTypes.h"
#include <osg/linewidth>
#include <osg/Geode>
 
class CSulGeomLineStrip : public osg::Geode
{
public: 
					CSulGeomLineStrip();
					CSulGeomLineStrip( const sigma::VEC_VEC3& vecVector );

	void			setLines( const sigma::VEC_VEC3& vecVector );

	void			setWidth( float width );
	
private:
	void			initConstructor();
	void			createDrawable();

private:
	sigma::VEC_VEC3					m_vecVector;
	osg::ref_ptr<osg::Vec4Array>	m_rColors;
	osg::ref_ptr<osg::LineWidth>	m_rLineWidth;
	float							m_width;
};


#endif // __SULGEOMLINESTRIP_H__