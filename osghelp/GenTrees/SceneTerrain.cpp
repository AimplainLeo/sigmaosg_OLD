// SceneTerrain.cpp

#include "stdafx.h"
#include "SceneTerrain.h"
#include <SigmaUtil/SulGeomGrid.h>

CSceneTerrain::CSceneTerrain( const CSulString& sFile, bool bRenderMe, double offsetX, double offsetY ) :
CSceneBase( sFile, bRenderMe ),
m_offsetX( offsetX ),
m_offsetY( offsetY )
{
	drawBoundingBox();
}

CSceneTerrain::CSceneTerrain( sigma::uint32 gx, sigma::uint32 gy, float cellw, float cellh, sigma::uint32 subx, sigma::uint32 suby, bool bRenderMe, double offsetX, double offsetY ) :
CSceneBase( bRenderMe ),
m_offsetX( offsetX ),
m_offsetY( offsetY )
{
	sigma::int32 px = gx;
	sigma::int32 py = gy;

	CSulGeomGrid* pGeomGrid = new CSulGeomGrid;
	pGeomGrid->Create( osg::Vec3(-(px*cellw)/2,-(py*cellh)/2,0), gx, gy, cellw, cellh, subx, suby );

	create( pGeomGrid );
}

void CSceneTerrain::adjustPosition( CSulCoordUTM* pWorldUTM )
{
	osg::Vec3d v = pWorldUTM->getXYZ( *m_rCoordLatLon );
	v += osg::Vec3d( m_offsetX, m_offsetY, 0);
	m_rPat->setPosition( v );
}