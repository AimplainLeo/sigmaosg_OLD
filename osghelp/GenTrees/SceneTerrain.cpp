// SceneTerrain.cpp

#include "stdafx.h"
#include "SceneTerrain.h"

CSceneTerrain::CSceneTerrain( const CSulString& sFile, bool bRenderMe, double offsetX, double offsetY ) :
CSceneBase( sFile, bRenderMe ),
m_offsetX( offsetX ),
m_offsetY( offsetY )
{
	drawBoundingBox();
}

void CSceneTerrain::adjustPosition( CSulCoordUTM* pWorldUTM )
{
	osg::Vec3d v = pWorldUTM->getXYZ( *m_rCoordLatLon );
	v += osg::Vec3d( m_offsetX, m_offsetY, 0);
	m_rPat->setPosition( v );
}