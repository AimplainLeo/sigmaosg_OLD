// SulGeomLineList.h

#ifndef __SULGEOMLINELIST_H__
#define __SULGEOMLINELIST_H__

#include "SulGeomBase.h"
#include "SulTypes.h"

class CSulGeomLineList : public CSulGeomBase
{
public: 
					CSulGeomLineList( const sigma::VEC_LINESEGMENT& vecLines );

	void			setColor( const osg::Vec4& color );
	void			setColor( float r, float g, float b, float a );

protected:
	void			createDrawable();

private:
	sigma::VEC_LINESEGMENT			m_vecLines;
	osg::ref_ptr<osg::Vec4Array>	m_rColors;
};

#endif // __SULGEOMLINELIST_H__