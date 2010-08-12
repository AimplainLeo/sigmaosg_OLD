// SulGenPositionMask.h

#ifndef __SULGENPOSITIONMASK_H__
#define __SULGENPOSITIONMASK_H__

#include "SulTypes.h"

class CSulGenPositionMask : public osg::Referenced
{
public:
		sigma::VEC_TRI			m_vecTriMask;
		sigma::VEC_LINESEGMENT	m_vecLineMask;
		float					m_lineDist;
};

typedef std::vector< osg::ref_ptr<CSulGenPositionMask> > VEC_GENPOSITIONMASK;

#endif // __SULGENPOSITIONMASK_H__