// SulFilterPass.cpp

#include "stdafx.h"
#include "SulFilterPass.h"
#include "SulGeomQuad.h"
#include "SulTransScreenAlign.h"
#include <osg/geode>

osg::Geode* CSulFilterPass::create( osg::Texture2D* pTexIn, osg::Texture2D* pTexOut, const std::string& sNameRTT )
{
	int texW = pTexIn->getTextureWidth();
	int texH = pTexIn->getTextureHeight();

	m_rTexCam = new CSulTexCam( pTexOut, CSulTexCam::HDR );
	m_rTexCam->setName( std::string("CIuPostFilter-Cam-")+sNameRTT );

	// quad to apply shader to
	// quad to use pTexIn
	osg::ref_ptr<CSulGeomQuad> rQuad = new CSulGeomQuad( osg::Vec3(texW/2.0f, texH/2.0f, 0), texW, texH );
	rQuad->getDrawable()->getOrCreateStateSet()->setMode( GL_LIGHTING, osg::StateAttribute::OFF );
	rQuad->getDrawable()->getOrCreateStateSet()->setMode( GL_DEPTH_TEST, osg::StateAttribute::OFF );
	rQuad->setTexture( pTexIn );

	m_rGeode	= new osg::Geode;
	m_rGeode->addDrawable( rQuad->getDrawable() );

	// the quad must be aligned to the RTT camera and cover it completly
	osg::ref_ptr<CSulTransScreenAlign> rAlign = new CSulTransScreenAlign( texW, texH );
	rAlign->AddChild( m_rGeode );

	m_rTexCam->addChild( rAlign->GetProjection() );

	return m_rGeode;
}

CSulTexCam* CSulFilterPass::getTexCam()
{
	return m_rTexCam;
}

osg::Geode* CSulFilterPass::getGeode()
{
	return m_rGeode;
}