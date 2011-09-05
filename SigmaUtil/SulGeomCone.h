// SulGeomCone.h

#ifndef __SULGEOMCONE_H__
#define __SULGEOMCONE_H__

#include "SulTypes.h"
#include "SulGeomGeode.h"
#include "SulExport.h"
#include <osg/geometry>

class SUL_EXPORT CSulGeomCone : public CSulGeomGeode
{
public:
	// slices : The number of subdivisions around the Z axis
	// stacks : The number of subdivisions along the Z axis
	CSulGeomCone( float len, float radiusBottom, float radiusTop, sigma::uint32 slices=16, sigma::uint32 stacks=1 );

	void setColorBottom( const osg::Vec4& color );
	void setColorTop( const osg::Vec4& color );

	void setLength( float len );
	void setRadiusBottom( float radius );
	void setRadiusTop( float radius );

private:
	void createDrawable();

private:
	osg::ref_ptr<osg::Geometry>	m_rGeo;
	float			m_len;
	float			m_radiusBottom;
	float			m_radiusTop;
	sigma::uint32	m_slices;
	osg::Vec4		m_colorTop;
	osg::Vec4		m_colorBottom;
	osg::ref_ptr<osg::Vec4Array> m_colors;
};

#endif // __SULGEOMCONE_H__