// SulGeomTriangleFan.h

#ifndef __SULGEOMTRIANGLEFAN_H__
#define __SULGEOMTRIANGLEFAN_H__

#include "SulGeomBase.h"
#include "SulTypes.h"

class CSulGeomTriangleFan : public CSulGeomBase
{
public: 
					CSulGeomTriangleFan( const sigma::VEC_VEC3& vecVector );

protected:
	void			createDrawable();

private:
	sigma::VEC_VEC3					m_vecVector;
	osg::ref_ptr<osg::Vec4Array>	m_rColors;
};

#endif // __SULGEOMTRIANGLEFAN_H__