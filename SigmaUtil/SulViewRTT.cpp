// SulViewRTT.cpp

#include "stdafx.h"
#include "SulViewRTT.h"
#include "SulNodeCallbackCameraSync.h"
#include "SulScreenAlignedQuad.h"

CSulViewRTT::CSulViewRTT( osgViewer::Viewer* viewer )
{
}

CSulViewRTT::CSulViewRTT( osgViewer::Viewer* viewer, osg::Node* render, CSulTexCam::ESETUP eSetup, sigma::int32 x, sigma::int32 y, sigma::uint32 w, sigma::uint32 h )
{
	m_rtt = new CSulTexCam( w, h, eSetup );
	m_rtt->addChild( render );

	addChild( m_rtt );

	float view_w = viewer->getCamera()->getViewport()->width();
	float view_h = viewer->getCamera()->getViewport()->height();

	osg::ref_ptr<CSulScreenAlignedQuad> quad = new CSulScreenAlignedQuad( osg::Vec3( x + w/2.0f, view_h - (y + h/2.0f), 0), w, h, view_w, view_h );
	quad->setTexture( m_rtt->getTexture() );
	
	addChild( quad );
}

void CSulViewRTT::setReferenceFrame( osg::Transform::ReferenceFrame rf )
{
	m_rtt->setReferenceFrame( rf );
}

osg::Texture2D* CSulViewRTT::getTexture( sigma::uint32 index )
{
	return m_rtt->getTexture( index );
}

sigma::uint32 CSulViewRTT::getWidth()
{
	return m_rtt->getViewport()->width();
}

sigma::uint32 CSulViewRTT::getHeight()
{
	return m_rtt->getViewport()->height();
}

CSulTexCam* CSulViewRTT::getTexCam()
{
	return m_rtt;
}