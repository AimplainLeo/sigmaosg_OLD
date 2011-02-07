// SulGrid.cpp

///////////////////////////////////////////////////////////////////////////////////////
// experimental code
//	I'm trying to get the grid to resize compared to the distance to the camera
//	so later I can sub divide the grid,.. but apparently I've stopped this and
//	have hereby marked this class as DEPRECATED!
///////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SulGrid.h"
#include <osgManipulator/Projector>

CSulGrid::CSulGrid() 
{
	m_rGeomGrid = new CSulGeomGrid;
	m_rGeomGrid->Create( osg::Vec3(-2,-2,0), 4, 4, 1, 1, 5, 5 );

	m_rPat = new osg::PositionAttitudeTransform;
	m_rPat->addChild( m_rGeomGrid );
}

osg::PositionAttitudeTransform* CSulGrid::GetPAT()
{
	return m_rPat.get();
}

void CSulGrid::CalcCorners( osg::Node* pNode, osg::Camera* pCam )
{
	osg:: PositionAttitudeTransform* pPat = dynamic_cast<osg::PositionAttitudeTransform*>(pNode);

	osg::Plane plane( osg::Vec3(0,0,1), osg::Vec3(0,0,0) );

	osg::ref_ptr<osgManipulator::PlaneProjector> rPlaneProj = new osgManipulator::PlaneProjector;
	rPlaneProj->setPlane( plane );

	osg::Vec3d v0, v1, v2, v3;
	osgManipulator::PointerInfo pi;
	
	pi.reset();
	pi.setCamera( pCam );
	pi.setMousePosition( 0, 0 ); 
	rPlaneProj->project( pi, v0 );

	pi.reset();
	pi.setCamera( pCam );
	pi.setMousePosition( 0, 512 ); 
	rPlaneProj->project( pi, v1 );

	pi.reset();
	pi.setCamera( pCam );
	pi.setMousePosition( 512, 512 ); 
	rPlaneProj->project( pi, v2 );

	pi.reset();
	pi.setCamera( pCam );
	pi.setMousePosition( 512, 0 ); 
	rPlaneProj->project( pi, v0 );

	float f0 = v0.length();
	float f1 = v1.length();
	float f2 = v2.length();
	float f3 = v3.length();

	float f = f0;
	if ( f1>f ) f = f1;
	if ( f2>f ) f = f2;
	if ( f3>f ) f = f3;

	f = 1.0f+f*.1f;
	pPat->setScale( osg::Vec3( f, f, f ) );
}

void CSulGrid::testme( osg::Node* node )
{
	osg::Camera* pCam = 0;
	osg::Node* p = node;
	while ( p )
	{
		pCam = dynamic_cast<osg::Camera*>(p);
		if ( pCam )
		{
			CalcCorners( node, pCam );

			/*
			osg::Vec3 v;
			osgManipulator::PointerInfo pi;
			
			pi.reset();
			pi.setCamera( pCam );
			pi.setMousePosition( 256, 256 ); 
			rPlaneProj->project( pi, v );
		//	pPat->setPosition( v );

			float f = 1.0f+v.length()*10.0f;
			pPat->setScale( osg::Vec3( f, f, f ) );
			*/

			break;
		}

		p = p->getParent( 0 );
	}
}

void CSulGrid::Update()
{
	testme( m_rPat.get() );
}

