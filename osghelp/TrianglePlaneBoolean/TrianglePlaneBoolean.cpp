// TrianglePlaneBoolean.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <SigmaUtil/SulGeomPlane.h>
#include <SigmaUtil/SulGeomLine.h>
#include <SigmaUtil/SulGeomSphere.h>
#include <SigmaUtil/SulGeomTriangle.h>
#include <SigmaUtil/SulIntPlane.h>
#include <SigmaUtil/SulClipTriangle.h>
#include <osg/group>
#include <osgViewer/Viewer>

osg::Group* createScene()
{
	osg::Group* pGroup = new osg::Group;

	pGroup->getOrCreateStateSet()->setMode( GL_LIGHTING, osg::StateAttribute::OFF );

	osg::Geode* pGeode = new osg::Geode;
	pGroup->addChild( pGeode );

	osg::Plane* pPlane = new osg::Plane( 0,0,1,0 );

	// create and object that represents the plane
	CSulGeomPlane* pGeomPlane = new CSulGeomPlane( *pPlane, 2.0f );
	pGeomPlane->setColor( 0, 0.5, 0, 1 );
	pGeode->addDrawable( pGeomPlane->getDrawable() );
	
	// create triangle
	osg::Vec3 p1( 0, 0, 2  );
	osg::Vec3 p2( 0, 0.5, -2 );
	osg::Vec3 p3( 1, 0.5, -1 );
	CSulGeomTriangle* pTri = new CSulGeomTriangle( p1, p2, p3 );
	pTri->setColor( 0,0,1,1 );
	pGeode->addDrawable( pTri->getDrawable() );

	
	CSulClipTriangle triClip( CSulDataTri( p1, p2, p3 ), *pPlane );
	for ( Sigma::uint32 i=0; i<triClip.getCount(); i++ )
	{
		CSulDataTri tri = *triClip.getTriangle( i );

		// lets move it a little in the x direction
		tri.m_p0.x()-= 0.5f;	tri.m_p0.y()-= 0.5f;
		tri.m_p1.x()-= 0.5f;	tri.m_p1.y()-= 0.5f;
		tri.m_p2.x()-= 0.5f;	tri.m_p2.y()-= 0.5f;

		CSulGeomTriangle* pTri = new CSulGeomTriangle( tri.m_p0, tri.m_p1, tri.m_p2 );
		pTri->setColor( 1,0,0,1 );
		pGeode->addDrawable( pTri->getDrawable() );
	}

	return pGroup;
}

int _tmain(int argc, _TCHAR* argv[])
{
    // construct the viewer
    osg::ref_ptr<osgViewer::Viewer> rViewer = new osgViewer::Viewer;

    rViewer->setUpViewInWindow( 32, 32, 800, 600 );

	rViewer->setSceneData( createScene() );

	return rViewer->run();
}
