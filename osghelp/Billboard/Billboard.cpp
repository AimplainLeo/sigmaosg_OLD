// Billboard.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <osgViewer/Viewer>
#include <osg/Billboard>
#include <osg/geometry>
#include <osg/positionattitudetransform>

osg::Geometry* CreateTriangle()
{
	// create drawable geometry object
	osg::Geometry* pGeo = new osg::Geometry;

	// add 3 vertices creating a triangle
	osg::Vec3Array* pVerts = new osg::Vec3Array;
	pVerts->push_back( osg::Vec3( -1, 0, -1 ) );
	pVerts->push_back( osg::Vec3( 1, 0, -1 ) );
	pVerts->push_back( osg::Vec3( 0, 0, 1 ) );
	pGeo->setVertexArray( pVerts );

	// create a primitive set (add index numbers)
	osg::DrawElementsUInt* pPrimitiveSet = 
		new osg::DrawElementsUInt( osg::PrimitiveSet::TRIANGLES, 0 );
	pPrimitiveSet->push_back( 2 );
	pPrimitiveSet->push_back( 1 );
	pPrimitiveSet->push_back( 0 );
	pGeo->addPrimitiveSet( pPrimitiveSet );

	return pGeo;
}

osg::Node* CreateBillboard( float x, float y, float z )
{
	// create billboard and add a simple geometry triangle to it
	osg::Billboard* pBillboard = new osg::Billboard;
	pBillboard->setMode( osg::Billboard::POINT_ROT_EYE );
	pBillboard->addDrawable( CreateTriangle() );

	// turn off lighting so we can see the vertex colors
	osg::StateSet* pStateSet = pBillboard->getOrCreateStateSet();
	pStateSet->setMode( GL_LIGHTING, osg::StateAttribute::OFF );

	osg::PositionAttitudeTransform* pPat = new osg::PositionAttitudeTransform;
	pPat->setPosition( osg::Vec3( x, y, z ) );
	pPat->addChild( pBillboard );

	return pPat;
}

osg::Node* CreateScene()
{
	osg::Group* pGroup = new osg::Group;

	pGroup->addChild( CreateBillboard( -4, 0, 0 ) );
	pGroup->addChild( CreateBillboard( 0, 0, 0 ) );
	pGroup->addChild( CreateBillboard( 4, 0, 0 ) );

	return pGroup;
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
