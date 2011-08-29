// SulIntersectGraphMouse.cpp

#include "stdafx.h"
#include "SulIntersectGraphMouse.h"
#include <osg/io_utils>

CSulIntersectGraphMouse::CSulIntersectGraphMouse( osg::Camera* pCam ) :
CSulIntersectGraph()
{
	m_rCam = pCam;
}

void CSulIntersectGraphMouse::setCamera( osg::Camera* pCam )
{
	m_rCam = pCam;
}

/*
bool CSulIntersectGraphMouse::Update( float x, float y )
{
	osg::Matrixd proj = m_rCam->getProjectionMatrix();

	osg::Viewport* viewport = m_rCam->getViewport();
	double mx = viewport->x() + (int)((double )viewport->width()*(x*0.5+0.5));
	double my = viewport->y() + (int)((double )viewport->height()*(y*0.5+0.5));

	osg::Matrix MVPW = m_rCam->getViewMatrix() * proj;
	MVPW.postMult( m_rCam->getViewport()->computeWindowMatrix() );

	osg::Matrixd inverseMVPW;
	inverseMVPW.invert(MVPW);

	osg::Vec3 nearPoint = osg::Vec3(mx,my,0.0f) * inverseMVPW;
	osg::Vec3 farPoint = osg::Vec3(mx,my,1.0f) * inverseMVPW;

	SetLine( nearPoint, farPoint );

	return CSulIntersectGraph::Update();
}
*/
/*
#include <iostream>
#include <osg/io_utils>
  */

bool CSulIntersectGraphMouse::update( float x, float y )
{
	osg::Matrixd MVP = m_rCam->getViewMatrix() * m_rCam->getProjectionMatrix();
	osg::Matrixd inverseMVP = osg::Matrix::inverse(MVP);

	osg::Vec3 nearPoint = osg::Vec3d(x, y, 0.0f) * inverseMVP;
	osg::Vec3 farPoint =  osg::Vec3d(x, y, 1.0f) * inverseMVP;

	setLine( nearPoint, farPoint );
	
	return CSulIntersectGraph::update();
}

