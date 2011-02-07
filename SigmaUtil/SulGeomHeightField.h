// SulGeomHeightField.h

#ifndef __SULGEOMHEIGHTFIELD_H__
#define __SULGEOMHEIGHTFIELD_H__

#include "SulGeomBase.h"
#include "SulTypes.h"
#include "SulArray2D.h"
#include <osg/Geode>

class CSulGeomHeightField : public osg::Geode
{
public:
					CSulGeomHeightField();

	void			Create( CSulArray2D<float>* pArray2D, float fScaleX=1.0f, float fScaleY=1.0f, float fScaleH=1.0f );

	osg::Vec3		GetNormal( sigma::int32 x, sigma::int32 y ) const;
	float			GetHeight( sigma::int32 x, sigma::int32 y ) const;

private:
	osg::ref_ptr<osg::Vec3Array>	m_rVerts;
	sigma::uint32					m_w;
	sigma::uint32					m_h;
	float							m_fScaleX;
	float							m_fScaleY;
	float							m_fScaleH;
};

#endif // __SULGEOMHEIGHTFIELD_H__
