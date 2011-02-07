// SulGeomTriangleFan.h

#ifndef __SULGEOMTRIANGLEFAN_H__
#define __SULGEOMTRIANGLEFAN_H__

#include "SulTypes.h"
#include <osg/geode>

class CSulGeomTriangleFan : public osg::Geode
{
public: 
					CSulGeomTriangleFan( const sigma::VEC_VEC3& vecVector );

private:
	void			createDrawable();

private:
	sigma::VEC_VEC3					m_vecVector;
	osg::ref_ptr<osg::Vec4Array>	m_rColors;
};

#endif // __SULGEOMTRIANGLEFAN_H__