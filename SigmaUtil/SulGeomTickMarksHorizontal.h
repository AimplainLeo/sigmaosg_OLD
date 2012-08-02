// SulGeomTickMarksHorizontal.h

#ifndef __SULGEOMTICKMARKSHORIZONTAL_H__
#define __SULGEOMTICKMARKSHORIZONTAL_H__

#include "SulGeomLines.h"
#include "SulExport.h"

class SUL_EXPORT CSulGeomTickMarksHorizontal : public CSulGeomLines
{
public:
	// numTicks : number of ticks on one side of the horizontal line
	// tickSpacing : spacing between ticks
	// tickHeight : height of tickmarks
	// color : color of tickmarks
	CSulGeomTickMarksHorizontal( sigma::uint32 numTicks, float tickSpacing, float tickHeight, float ofsY, const osg::Vec4& color=osg::Vec4(0,0,0,1) );

private:
	void createDrawable();

private:
	sigma::uint32	m_numTicks;
	float			m_tickSpacing;
	float			m_tickHeight;
	float			m_ofsY;
};

#endif // __SULGEOMTICKMARKSHORIZONTAL_H__