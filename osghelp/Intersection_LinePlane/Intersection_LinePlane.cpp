// Intersection_LinePlane.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <SigmaUtil/SulGeomPlane.h>
#include <SigmaUtil/SulGeomLine.h>
#include <SigmaUtil/SulGeomSphere.h>
#include <SigmaUtil/SulIntPlane.h>
#include <osg/group>
#include <osgViewer/Viewer>

osg::Group* createScene()
{
	osg::Group* pGroup = new osg::Group;

	pGroup->getOrCreateStateSet()->setMode( GL_LIGHTING, osg::StateAttribute::OFF );

	osg::Plane* pPlane = new osg::Plane( 0,0,1,0 );

	// create and object that represents the plane
	CSulGeomPlane* pGeomPlane = new CSulGeomPlane( *pPlane, 2.0f );
	pGroup->addChild( pGeomPlane );
	
	// create line
	osg::Vec3 p1( 0, 0, -2  );
	osg::Vec3 p2( 0, 0.5, 2 );
	CSulGeomLine* pLine = new CSulGeomLine( p1, p2 );
	pGroup->addChild( pLine );

	// calc intersection
	osg::Vec3 pos;
	SulIntPlane( p1, p2, *pPlane, &pos );

	// create sphere and put it at the intersection position
	CSulGeomSphere* pGeomSphere = new CSulGeomSphere( 0.1f, pos );
	pGroup->addChild( pGeomSphere );

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
