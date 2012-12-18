// AntialiasedLines.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <osgViewer/Viewer>
#include <osgText/Text>
#include <osg/Geometry>

osg::Node* createScene()
{
    // create drawable geometry object
    osg::Geometry* geo = new osg::Geometry;

    // add vertices 
    osg::Vec3Array* verts = new osg::Vec3Array;
    verts->push_back( osg::Vec3( -1, 0, 0 ) );
    verts->push_back( osg::Vec3( 0, 0, -1 ) );
    verts->push_back( osg::Vec3( 1, 0, 0 ) );
	verts->push_back( osg::Vec3( 0, 0, 1 ) );
    geo->setVertexArray( verts );

    // create a primitive set (add index numbers)
    osg::DrawElementsUInt* ps = 
        new osg::DrawElementsUInt( osg::PrimitiveSet::LINES, 0 );
	ps->push_back( 0 );
    ps->push_back( 3 );
    ps->push_back( 3 );
    ps->push_back( 2 );
	ps->push_back( 2 );
    ps->push_back( 1 );
    ps->push_back( 1 );
    ps->push_back( 0 );
    geo->addPrimitiveSet( ps );

    // create geometry node that will contain all our drawables
    osg::Geode* geode = new osg::Geode;
    osg::StateSet* pStateSet = geode->getOrCreateStateSet();
    pStateSet->setMode( GL_LIGHTING, osg::StateAttribute::OFF );
	pStateSet->setMode( GL_LINE_SMOOTH, osg::StateAttribute::ON );
	pStateSet->setMode( GL_BLEND, osg::StateAttribute::ON );
    geode->addDrawable( geo );

    return geode;
}

int _tmain(int argc, _TCHAR* argv[])
{
    // construct the viewer
    osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer;

    // make the viewer create a window and position it
    viewer->setUpViewInWindow( 32, 32, 512, 512 );

    // set the scene-graph data the viewer will render
    viewer->setSceneData( createScene() );

    // execute main loop
    return viewer->run();
}
