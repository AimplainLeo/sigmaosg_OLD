// AntialiasedLines.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <osgViewer/Viewer>
#include <osgText/Text>
#include <osg/Geometry>

osg::Node* CreateScene()
{
    // create drawable geometry object
    osg::Geometry* pGeo = new osg::Geometry;

    // add vertices 
    osg::Vec3Array* pVerts = new osg::Vec3Array;
    pVerts->push_back( osg::Vec3( -1, 0, 0 ) );
    pVerts->push_back( osg::Vec3( 0, 0, -1 ) );
    pVerts->push_back( osg::Vec3( 1, 0, 0 ) );
	pVerts->push_back( osg::Vec3( 0, 0, 1 ) );
    pGeo->setVertexArray( pVerts );

    // create a primitive set (add index numbers)
    osg::DrawElementsUInt* pPrimitiveSet = 
        new osg::DrawElementsUInt( osg::PrimitiveSet::LINES, 0 );
	pPrimitiveSet->push_back( 0 );
    pPrimitiveSet->push_back( 3 );
    pPrimitiveSet->push_back( 3 );
    pPrimitiveSet->push_back( 2 );
	pPrimitiveSet->push_back( 2 );
    pPrimitiveSet->push_back( 1 );
    pPrimitiveSet->push_back( 1 );
    pPrimitiveSet->push_back( 0 );
    pGeo->addPrimitiveSet( pPrimitiveSet );

    // create geometry node that will contain all our drawables
    osg::Geode* pGeode = new osg::Geode;
    osg::StateSet* pStateSet = pGeode->getOrCreateStateSet();
    pStateSet->setMode( GL_LIGHTING, osg::StateAttribute::OFF );
	pStateSet->setMode( GL_LINE_SMOOTH, osg::StateAttribute::ON );
	pStateSet->setMode( GL_BLEND, osg::StateAttribute::ON );
    pGeode->addDrawable( pGeo );

    return pGeode;
}

int _tmain(int argc, _TCHAR* argv[])
{
    // construct the viewer
    osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer;

    // make the viewer create a 512x512 window and position it at 32, 32
    viewer->setUpViewInWindow( 32, 32, 512, 512 );

    // set the scene-graph data the viewer will render
    viewer->setSceneData( CreateScene() );

    // execute main loop
    return viewer->run();
}
