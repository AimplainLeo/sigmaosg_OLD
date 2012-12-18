// SceneShape.cpp

#include "stdafx.h"
#include "SceneShape.h"
#include <SigmaUtil/SulShaderColor.h>
#include <SigmaUtil/SulGeomQuad.h>
#include <SigmaUtil/SulGeomLine.h>
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

CSceneShape::CSceneShape( float w, float h, bool bRenderMe ) :
CSceneBase( bRenderMe )
{
	CSulGeomQuad* pGeom = new CSulGeomQuad( w , h);

	pGeom->getOrCreateStateSet()->setMode( GL_CULL_FACE, osg::StateAttribute::OFF | osg::StateAttribute::PROTECTED | osg::StateAttribute::OVERRIDE );

	CSulShaderColor* p = new CSulShaderColor( pGeom, osg::Vec4( 1,1,1,0.2f) );
	pGeom->getOrCreateStateSet()->setMode( GL_BLEND, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE| osg::StateAttribute::PROTECTED );
	pGeom->getOrCreateStateSet()->setRenderingHint( osg::StateSet::TRANSPARENT_BIN );

	create( pGeom );
}

CSceneShape::CSceneShape( const osg::Vec3& p0, const osg::Vec3& p1, bool bRenderMe ) :
CSceneBase( bRenderMe )
{
	CSulGeomLine* geode = new CSulGeomLine( p0, p1 );

	// FIXME: bad way to handle line color.. specially if we have many of them
	CSulShaderColor* p = new CSulShaderColor( geode, osg::Vec4( 1,1,1,0.2f) );
	geode->getOrCreateStateSet()->setMode( GL_BLEND, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE| osg::StateAttribute::PROTECTED );
	geode->getOrCreateStateSet()->setRenderingHint( osg::StateSet::TRANSPARENT_BIN );

	create( geode );
}