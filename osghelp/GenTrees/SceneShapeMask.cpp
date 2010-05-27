// SceneShapeMask.cpp

#include "stdafx.h"
#include "SceneShapeMask.h"
#include <SigmaUtil/SulShaderColor.h>

CSceneShapeMask::CSceneShapeMask( const CSulString& sFile, bool bRenderMe, float lineDist ) :
CSceneBase( sFile, bRenderMe, osg::Vec4(1,1,0,0.5f) ),
m_lineDist( lineDist )
{
	if ( m_rScene.valid() )
	{
		m_rScene->getOrCreateStateSet()->setMode( GL_CULL_FACE, osg::StateAttribute::OFF | osg::StateAttribute::PROTECTED | osg::StateAttribute::OVERRIDE );

		CSulShaderColor* p = new CSulShaderColor( m_rScene, osg::Vec4( 0,1,0,0.3f) );

		m_rScene->getOrCreateStateSet()->setMode( GL_BLEND, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE| osg::StateAttribute::PROTECTED );
		m_rScene->getOrCreateStateSet()->setRenderingHint( osg::StateSet::TRANSPARENT_BIN );
	}
}

float CSceneShapeMask::getLineDist()
{
	return m_lineDist;
}