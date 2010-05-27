// Intersection_LineLine.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <SigmaUtil/SulGeomPlane.h>
#include <SigmaUtil/SulGeomLine.h>
#include <SigmaUtil/SulGeomSphere.h>
#include <SigmaUtil/SulIntPlane.h>
#include <SigmaUtil/SulIntLine.h>
#include <osg/group>
#include <osgViewer/Viewer>
#include <osg/linesegment>

osg::Group* createScene()
{
	osg::Group* pGroup = new osg::Group;

	pGroup->getOrCreateStateSet()->setMode( GL_LIGHTING, osg::StateAttribute::OFF );

	osg::Geode* pGeode = new osg::Geode;
	pGroup->addChild( pGeode );

	// create line0
	osg::ref_ptr<osg::LineSegment> line0 = new osg::LineSegment( osg::Vec3(0, 0, -2), osg::Vec3(0, 0.5, 2) );
	CSulGeomLine* pLine0 = new CSulGeomLine( *line0 );
	pGeode->addDrawable( pLine0->getDrawable() );

	// create line1
	osg::ref_ptr<osg::LineSegment> line1 = new osg::LineSegment( osg::Vec3(2, 2, 0), osg::Vec3(-2, -2.5, -2) );
	CSulGeomLine* pLine1 = new CSulGeomLine( *line1 );
	pGeode->addDrawable( pLine1->getDrawable() );

	// testing
	float s, t;
	SulIntLine( *line0, *line1, &s, &t );

	osg::Vec3 p0 = line0->start() + (line0->end()-line0->start())*s;
	osg::Vec3 p1 = line1->start() + (line1->end()-line1->start())*t;
	
	// create sphere and put it at the intersection position
	CSulGeomSphere* pGeomSphere0 = new CSulGeomSphere( 0.1f, p0 );
	pGeode->addDrawable( pGeomSphere0->getDrawable() );

	CSulGeomSphere* pGeomSphere1 = new CSulGeomSphere( 0.1f, p1 );
	pGeode->addDrawable( pGeomSphere1->getDrawable() );

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