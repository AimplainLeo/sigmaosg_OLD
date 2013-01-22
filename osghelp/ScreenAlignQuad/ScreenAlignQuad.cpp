// ScreenAlignQuad.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <osgViewer/Viewer>
#include <osg/Geometry>
#include <SigmaUtil/SulGeomQuad.h>
#include <osg/ShapeDrawable>
#include <osg/MatrixTransform>
/*
osg::Node* CreateScene()
{
	// create geometry quad
	osg::ref_ptr<CSulGeomQuad> pGeomQuad = new CSulGeomQuad( osg::Vec3(110,110, 0), 100, 100, CSulGeomQuad::PLANE_XY );

    // create geometry node that will contain all our drawables
    osg::Geode* pGeode = new osg::Geode;
    osg::StateSet* pStateSet = pGeode->getOrCreateStateSet();
    pStateSet->setMode( GL_LIGHTING, osg::StateAttribute::OFF );
    pGeode->addDrawable( pGeomQuad->GetGeometry() );
       
	osg::Camera* pCamera = new osg::Camera;
	pCamera->setClearMask( GL_DEPTH_BUFFER_BIT );
    pCamera->setProjectionMatrix( osg::Matrix::ortho2D(0,512,0,512) );
    pCamera->setReferenceFrame( osg::Transform::ABSOLUTE_RF );
    pCamera->setViewMatrix( osg::Matrix::identity() );
	pCamera->setRenderOrder( osg::Camera::POST_RENDER );
	pCamera->addChild( pGeode );

	osg::Group* pGroup = new osg::Group;

	osg::Geode* pGeodeShape = new osg::Geode();
	pGeodeShape->addDrawable( new osg::ShapeDrawable( new osg::Sphere(osg::Vec3(0.0f,0.0f,0.0f),0.5f) ) );
	pGroup->addChild( pGeodeShape );
	pGroup->addChild( pCamera );

    return pGroup;
}
*/

osg::Node* createScene()
{
	// create geometry quad
	osg::ref_ptr<CSulGeomQuad> pGeomQuad = new CSulGeomQuad( osg::Vec3(110,110, 0), 100, 100, CSulGeomQuad::PLANE_XY );
	pGeomQuad->getOrCreateStateSet()->setMode( GL_DEPTH_TEST, osg::StateAttribute::OFF );
	
    osg::StateSet* pStateSet = pGeomQuad->getOrCreateStateSet();
    pStateSet->setMode( GL_LIGHTING, osg::StateAttribute::OFF );
	
	osg::MatrixTransform* mtrans = new osg::MatrixTransform;
	mtrans->setReferenceFrame( osg::Transform::ABSOLUTE_RF );
	mtrans->setMatrix( osg::Matrix::identity() );
	mtrans->addChild( new CSulGeode(pGeomQuad) );
	
	osg::Projection* pProj = new osg::Projection();
	osg::Matrixd m_mOrtho = osg::Matrix::ortho2D( 0, 512, 0, 512 );
	pProj->addChild( mtrans );
	pProj->setMatrix( m_mOrtho );

	osg::Group* pGroup = new osg::Group;
	osg::Geode* pGeodeShape = new osg::Geode();
	pGeodeShape->addDrawable( new osg::ShapeDrawable( new osg::Sphere(osg::Vec3(0.0f,0.0f,0.0f),0.5f) ) );
	pGroup->addChild( pGeodeShape );
	pGroup->addChild( pProj );
	
    return pGroup;
}

int _tmain(int argc, _TCHAR* argv[])
{
    // construct the viewer
    osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer;

    // make the viewer create a 512x512 window and position it at 32, 32
    viewer->setUpViewInWindow( 32, 32, 512, 512 );

    // set the scene-graph data the viewer will render
    viewer->setSceneData( createScene() );

    // execute main loop
    return viewer->run();
}
