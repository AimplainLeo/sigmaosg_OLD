// SulGeomLines.h

#ifndef __SULGEOMLINES_H__
#define __SULGEOMLINES_H__

#include "SulGeom.h"
#include "SulTypes.h"
#include "SulExport.h"

class SUL_EXPORT CSulGeomLines : public CSulGeom
{
public: 
					CSulGeomLines( const osg::Vec4& color=osg::Vec4(0,0,0,1) );
					CSulGeomLines( const sigma::VEC_LINESEGMENT& vecLines );

	// NOTE: remember to call createDrawable
	void			addLine( const osg::Vec3& pos );
	void			addLine( const osg::Vec3& pos0, const osg::Vec3& pos1 );
	void			setLines( const sigma::VEC_LINESEGMENT& vecLines );

	// only call if needed (constructors call this automatically)
	void			createDrawable();

private:
	void			initConstructor();

private:
	sigma::VEC_LINESEGMENT			m_vecLines;
};

#endif // __SULGEOMLINES_H__