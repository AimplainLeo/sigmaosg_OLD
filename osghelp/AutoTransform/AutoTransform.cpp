// AutoTransform.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <osgViewer/Viewer>
#include <osg/AutoTransform>
#include <osg/Billboard>
#include <osg/ShapeDrawable>
#include <osg/Geometry>
#include <osg/PositionAttitudeTransform>

osg::Node* CreateBox(float width, float depth, float height)
{
	osg::Geode* pGeode = new osg::Geode();
	pGeode->addDrawable( new osg::ShapeDrawable( new osg::Box(osg::Vec3(0.0f,0.0f,-height), width, depth, height) ) );
	return pGeode;
}

osg::Geometry* CreateMarker()
{
	// create drawable geometry object
	osg::Geometry* pGeo = new osg::Geometry;

	// add 4 vertices, creating a square around 0, 0, 0
	osg::Vec3Array* pVerts = new osg::Vec3Array;
	pVerts->push_back( osg::Vec3( -1, 0, 0 ) );
	pVerts->push_back( osg::Vec3( 0, 1, 0 ) );
	pVerts->push_back( osg::Vec3( 1, 0, 0 ) );
	pVerts->push_back( osg::Vec3( 0, -1, 0 ) );
	pGeo->setVertexArray( pVerts );

	// create a primitive set (add index numbers) for drawing lines
	osg::DrawElementsUInt* pPrimitiveSet = 
		new osg::DrawElementsUInt( osg::PrimitiveSet::LINE_LOOP, 0 );
	pPrimitiveSet->push_back( 3 );
	pPrimitiveSet->push_back( 2 );
	pPrimitiveSet->push_back( 1 );
	pPrimitiveSet->push_back( 0 );
	pGeo->addPrimitiveSet( pPrimitiveSet );

	return pGeo;
}

osg::Node* CreateMarkerNode( float scale = 1.0f )
{
	// create marker geode
	osg::Geode* pGeode = new osg::Geode;
	pGeode->addDrawable( CreateMarker() );

	// turn off lighting
	osg::StateSet* pStateSet = pGeode->getOrCreateStateSet();
	pStateSet->setMode( GL_LIGHTING, osg::StateAttribute::OFF );

	// scale the marker
	osg::PositionAttitudeTransform* pPat = new osg::PositionAttitudeTransform;
	pPat->setScale( osg::Vec3( scale, scale, scale ) );
	pPat->addChild( pGeode );

	return pPat;
}

osg::Node* CreateAutoTransform( float x, float y, float z, float scale )
{
	// setup an auto transform node so that it scales its child to screen size and rotates it towards the camera
	osg::AutoTransform* pAutoTransform = new osg::AutoTransform;

	pAutoTransform->setAutoScaleToScreen( true );
	pAutoTransform->setAutoRotateMode( osg::AutoTransform::ROTATE_TO_CAMERA );
	pAutoTransform->addChild( CreateMarkerNode( scale ) );
	pAutoTransform->setPosition( osg::Vec3( x, y, z ) );
	
	return pAutoTransform;
}

osg::Node* createScene()
{
	osg::Group* pGroup = new osg::Group;

	// show a box that is 10 units to each side
	pGroup->addChild( CreateBox( 10, 10, 0.1 ) );

	// put auto transform scaled markers around each corner of the box
	static const float scale = 10.0f;
	pGroup->addChild( CreateAutoTransform( -5, -5, 0, scale ) );
	pGroup->addChild( CreateAutoTransform( -5, 5, 0, scale ) );
	pGroup->addChild( CreateAutoTransform( 5, -5, 0, scale ) );
	pGroup->addChild( CreateAutoTransform( 5, 5, 0, scale ) );

	return pGroup;
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
