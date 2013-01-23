// SulGeom.h

#ifndef __SULGEOM_H__
#define __SULGEOM_H__

#include "SulTypes.h"
#include <osg/geometry>

class CSulGeom : public osg::Geometry
{
public:
					CSulGeom( const osg::Vec4& color=osg::Vec4(1,1,1,1) );

	void			useColorArray();
	void			useNormalArray();
	void			useTextureCoordArray();

	// NOTE: remember to call createDrawable
	void			setColor( const osg::Vec4& color );
	void			setColor( float r, float g, float b, float a );

	// V : vertex
	// N : normal
	// C : color
	void			addV( const osg::Vec3& v );
	void			addVN( const osg::Vec3& v, const osg::Vec3& n );
	void			addVC( const osg::Vec3& v, const osg::Vec4& c );

	void			addVNT( const osg::Vec3& v, const osg::Vec3& n, const osg::Vec2& t );

	sigma::uint32	getNumVertices();

	virtual	void	createDrawable();

protected:
	void			removeAllPrimitiveSets();

protected:
	osg::Vec4						m_color;

	osg::ref_ptr<osg::Vec3Array>	m_verts;
	osg::ref_ptr<osg::Vec4Array>	m_colors;
	osg::ref_ptr<osg::Vec3Array>	m_normals;

	osg::ref_ptr<osg::Vec2Array>	m_texCoords;
};

#endif // __SULGEOM_H__