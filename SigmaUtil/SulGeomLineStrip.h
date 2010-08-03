// SulGeomLineStrip.h

#ifndef __SULGEOMLINESTRIP_H__
#define __SULGEOMLINESTRIP_H__

#include "SulGeomBase.h"
#include "SulTypes.h"
#include <osg/linewidth>

class CSulGeomLineStrip : public CSulGeomBase
{
public: 
					CSulGeomLineStrip( const Sigma::VEC_VEC3& vecVector );

	void			setWidth( float width );
	
protected:
	void			createDrawable();

private:
	Sigma::VEC_VEC3 m_vecVector;
	osg::ref_ptr<osg::Vec4Array>	m_rColors;
	osg::ref_ptr<osg::LineWidth>	m_rLineWidth;
};


#endif // __SULGEOMLINESTRIP_H__