// SulGeomQuadTiles.h

#ifndef __SULGEOMQUADTILES_H__
#define __SULGEOMQUADTILES_H__

#include "SulGeomBase.h"
#include "SulTypes.h"

class CSulGeomQuadTiles : public CSulGeomBase
{
public:
	enum EPLANE
	{
		PLANE_XY,
		PLANE_XZ
	};

public:
	CSulGeomQuadTiles( 
		const osg::Vec3& vCenter, 
		float w=1.0f, float h=1.0f, 
		sigma::uint32 tileX=3, sigma::uint32 tileY=3, 
		bool bFadeEdges=true,
		EPLANE ePlane=PLANE_XY );

	sigma::uint32 getTileX();
	sigma::uint32 getTileY();

	float getWidth();
	float getHeight();

protected:
	virtual void createDrawable();

	void createTiles();

private:
	osg::Vec2Array* m_texCoords;
	osg::Vec4Array* m_colors;

	osg::Vec3 m_vCenter;
	float	m_w;
	float	m_h;						// total width and height with all quads
	sigma::uint32	m_tileX;
	sigma::uint32	m_tileY;					// number of tiles x and y with in w and h
	bool	m_bFadeEdges;
	EPLANE	m_ePlane;
};

#endif // __SULGEOMQUADTILES_H__