// SceneShape.cpp

#include "stdafx.h"
#include "SceneShape.h"
#include <SigmaUtil/SulShaderColor.h>
#include <osg/material>
#include <osg/blendfunc>

CSceneShape::CSceneShape( const CSulString& sFile, bool bRenderMe ) :
CSceneBase( sFile, bRenderMe )
{
	m_rScene->getOrCreateStateSet()->setMode( GL_CULL_FACE, osg::StateAttribute::OFF | osg::StateAttribute::PROTECTED | osg::StateAttribute::OVERRIDE );

	CSulShaderColor* p = new CSulShaderColor( m_rScene, osg::Vec4( 1,1,1,0.2f) );
	m_rScene->getOrCreateStateSet()->setMode( GL_BLEND, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE| osg::StateAttribute::PROTECTED );
	m_rScene->getOrCreateStateSet()->setRenderingHint( osg::StateSet::TRANSPARENT_BIN );
}

