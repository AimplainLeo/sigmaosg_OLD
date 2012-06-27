// SulViewQuad.cpp

#include "stdafx.h"
#include "SulViewQuad.h"

CSulViewQuad::CSulViewQuad()
{
	m_saq = new CSulScreenAlignedQuad;
	addChild( m_saq );
}

CSulViewQuad::CSulViewQuad( osgViewer::Viewer* viewer, osg::Texture2D* tex, sigma::int32 x, sigma::int32 y, sigma::uint32 w, sigma::uint32 h )
{
	float view_w = viewer->getCamera()->getViewport()->width();
	float view_h = viewer->getCamera()->getViewport()->height();

	m_saq = new CSulScreenAlignedQuad( osg::Vec3(x + w/2.0f, view_h - ((h/2.0f)+y), 0), w, h, view_w, view_h );
	m_saq->setTexture( tex );

	addChild( m_saq );
}

CSulViewQuad::CSulViewQuad( osgViewer::Viewer* viewer )
{
	float view_w = viewer->getCamera()->getViewport()->width();
	float view_h = viewer->getCamera()->getViewport()->height();

	// quad covers the whole view
	m_saq = new CSulScreenAlignedQuad( osg::Vec3(view_w/2.0f, view_h - ((view_h/2.0f)), 0), view_w, view_h, view_w, view_h );

	addChild( m_saq );
}

CSulGeomQuad* CSulViewQuad::getQuad()
{
	return m_saq->getQuad();
}