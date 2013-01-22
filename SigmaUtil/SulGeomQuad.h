// SulGeomQuad.h

#ifndef __SULGEOMQUAD_H__
#define __SULGEOMQUAD_H__

#include "SulExport.h"
#include "SulGeom.h"
#include "SulTypes.h"
#include "SulString.h"
#include "SulGeode.h"
#include <osg/ref_ptr>
#include <osg/Vec3>
#include <osg/geode>
#include <osg/geometry>

// FIXME: should be able to choose between vertice colors or all_over_colors

class SUL_EXPORT CSulGeomQuad : public CSulGeom
{
public:
	enum EPLANE
	{
		PLANE_XY,
		PLANE_XZ,
		PLANE_YZ
	};

public:
									CSulGeomQuad( float w=1.0f, float h=1.0f, EPLANE ePlane=PLANE_XY );
									CSulGeomQuad( const osg::Vec3& vCenter, float w=1.0f, float h=1.0f, EPLANE ePlane=PLANE_XY );

	// these 2 methods should be private/protected										
	void							create();
	void							Create( const osg::Vec3& vCenter, float w=1.0f, float h=1.0f, EPLANE ePlane=PLANE_XY );

	void							createUV();

	void							setColor( const osg::Vec4& c );
	void							setColor( float r, float g, float b, float a );
	const osg::Vec4&				getColor( sigma::uint32 index );

	// FIXME: these texture methods should be moved to the base class (they are very generic)
	void							setTexture( osg::Image* pImage, GLint internalFormat=GL_RGB, sigma::uint32 unit=0 );
	void							setTexture( osg::Texture* pTex, sigma::uint32 unit=0, const CSulString& uniformName="" );
	osg::Texture2D*					setTexture( const CSulString& file, sigma::uint32 unit=0 );

	osg::Texture*					getTexture( sigma::uint32 unit=0 );

	osg::Image*						getImage();

	void							setUV( float uv );
	void							setUV( float u, float v );
	void							setUV( float u0, float u1, float v0, float v1 );
	float							getU();

	void							setWidth( float w );
	void							setHeight( float h );

	float							getWidth();
	float							getHeight();

	void							setCenter( const osg::Vec3& vCenter );

private:
	void							calcVertPositions();

private:
	sigma::MAP_TEXTURE				m_mapTex;
	osg::ref_ptr<osg::Vec4Array>	m_rColors;
	osg::ref_ptr<osg::Vec3Array>	m_rVerts;
	osg::ref_ptr<osg::Vec2Array>	m_rUV;
	osg::Vec3						m_vCenter;
	float							m_w;
	float							m_h;
	EPLANE							m_ePlane;

	osg::ref_ptr<osg::Image>		m_rImage;	// only valid if used
};

#endif // __SULGEOMQUAD_H__