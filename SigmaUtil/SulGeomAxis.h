// SulGeomAxis.h

#ifndef __SULGEOMAXIS_H__
#define __SULGEOMAXIS_H__

#include "SulGeomArrow3D.h"
#include "SulExport.h"

class SUL_EXPORT CSulGeomAxis : public osg::Group
{
public:
	CSulGeomAxis( float len=1.0f );

private:
	void createDrawable();

private:
	float								m_len;

	osg::ref_ptr<CSulGeomArrow3D>		m_arrowRight;
	osg::ref_ptr<CSulGeomArrow3D>		m_arrowForward;
	osg::ref_ptr<CSulGeomArrow3D>		m_arrowUp;
};

#endif // __SULGEOMAXIS_H__