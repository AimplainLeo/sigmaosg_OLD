// SulFilterPass.cpp

#include "stdafx.h"
#include "SulFilterPass.h"
#include "SulGeomQuad.h"
#include "SulTransScreenAlign.h"
#include <osg/geode>

osg::Group* CSulFilterPass::create( osg::Texture2D* pTexIn, osg::Texture2D* pTexOut, const std::string& sNameRTT )
{
	int texW = pTexIn->getTextureWidth();
	int texH = pTexIn->getTextureHeight();

	m_rTexCam = new CSulTexCam( pTexOut, CSulTexCam::HDR );
	m_rTexCam->setName( std::string("CSulFilterPass-Cam-")+sNameRTT );

	// quad to apply shader to
	// quad to use pTexIn
	osg::ref_ptr<CSulGeomQuad> rQuad = new CSulGeomQuad( osg::Vec3(texW/2.0f, texH/2.0f, 0), texW, texH );
	rQuad->getOrCreateStateSet()->setMode( GL_LIGHTING, osg::StateAttribute::OFF );
	rQuad->getOrCreateStateSet()->setMode( GL_DEPTH_TEST, osg::StateAttribute::OFF );
	rQuad->setTexture( pTexIn );

	m_rGroup	= new osg::Group;
	m_rGroup->addChild( rQuad );

	// the quad must be aligned to the RTT camera and cover it completely
	osg::ref_ptr<CSulTransScreenAlign> rAlign = new CSulTransScreenAlign( texW, texH );
	rAlign->addChild( m_rGroup );

	m_rTexCam->addChild( rAlign->getProjection() );

	return m_rGroup;
}

CSulTexCam* CSulFilterPass::getTexCam()
{
	return m_rTexCam;
}

osg::Group* CSulFilterPass::getGroup()
{
	return m_rGroup;
}