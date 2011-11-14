// Intersection_SphereGraph.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <SigmaUtil/SulSphereIntersector.h>
#include <SigmaUtil/SulGeomSphere.h>
#include <osgViewer/Viewer>
#include <osg/ShapeDrawable>
#include <osg/matrixtransform>
#include <osgUtil/IntersectionVisitor>
#include <iostream> 

float radius = 1.0f;
osg::Vec3 pos( 3.5f,0,0 );
osg::ref_ptr<osg::MatrixTransform> mt;

osg::Group* pGroup = 0;
osg::Group* pGG = 0;
CSulSphereIntersector* g_pSI = 0;
osgUtil::IntersectionVisitor* g_iv = 0;

void testCol();
void updateTransform();

class CKeyboardHandler : public osgGA::GUIEventHandler 
{
public:
	virtual bool handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa, osg::Object* pObject, osg::NodeVisitor* pNodeVisitor )
	{
		osgViewer::Viewer* pViewer = dynamic_cast<osgViewer::Viewer*>(&aa);
        if ( !pViewer )
		{
			return false;
		}

		if ( ea.getEventType()==osgGA::GUIEventAdapter::KEYDOWN )
		{
			if ( ea.getKey()==osgGA::GUIEventAdapter::KEY_Up )
			{
				pos.z() += 1.0f;
				updateTransform();
			}

			if ( ea.getKey()==osgGA::GUIEventAdapter::KEY_Down )
			{
				pos.z() -= 1.0f;
				updateTransform();
			}

			if ( ea.getKey()==osgGA::GUIEventAdapter::KEY_Left )
			{
				pos.x() -= 1.0f;
				updateTransform();
			}

			if ( ea.getKey()==osgGA::GUIEventAdapter::KEY_Right )
			{
				pos.x() += 1.0f;
				updateTransform();
			}

			g_pSI->reset();
			g_pSI->setPosition( pos );
			testCol();
		}

		return false;
	}
};

void updateTransform()
{
	osg::Matrix m = mt->getMatrix();
	m.setTrans( pos );
	mt->setMatrix( m );
}

void testCol()
{
	pGroup->accept( *g_iv );

	if ( g_iv->getIntersector()->containsIntersections() )
	{
		std::cout << "BAM!!!!" << std::endl;
	}
}

osg::Node* CreateScene()
{
	pGroup = new osg::Group;
	
	osg::Geode* pGeode1 = new osg::Geode();
	pGroup->addChild( pGeode1 );	
	osg::Geode* pGeode2 = new osg::Geode();
	pGroup->addChild( pGeode2 );	
	osg::Geode* pGeode3 = new osg::Geode();
	pGroup->addChild( pGeode3 );	
	osg::Geode* pGeode4 = new osg::Geode();
	pGroup->addChild( pGeode4 );	
	osg::Geode* pGeode5 = new osg::Geode();
	pGroup->addChild( pGeode5 );	

	// we create the simplest form of shapes in OpenSceneGraph
    pGeode1->addDrawable( new osg::ShapeDrawable( new osg::Sphere(osg::Vec3(0.0f,0.0f,0.0f),0.5f) ) );
    pGeode2->addDrawable( new osg::ShapeDrawable( new osg::Box(osg::Vec3(2.0f,0.0f,0.0f),2.0f) ) );
    pGeode3->addDrawable( new osg::ShapeDrawable( new osg::Cone(osg::Vec3(4.0f,0.0f,0.0f),0.5f,3.0f) ) );
    pGeode4->addDrawable( new osg::ShapeDrawable( new osg::Cylinder(osg::Vec3(6.0f,0.0f,0.5f),0.5f,3.0f) ) );
    pGeode5->addDrawable( new osg::ShapeDrawable( new osg::Capsule(osg::Vec3(8.0f,0.0f,1.0f),0.5f,3.0f) ) );

	// create dummy sphere, so we can see where the collision should take place
	CSulGeomSphere* pSphere = new CSulGeomSphere( radius, osg::Vec3( 0, 0, 0 ) );

	mt = new osg::MatrixTransform;
	mt->addChild( pSphere );
	pGG->addChild( mt );
	updateTransform();

	// test collision
	g_pSI = new CSulSphereIntersector;
	g_iv = new osgUtil::IntersectionVisitor;
	g_iv->setIntersector( g_pSI );

	g_pSI->setRadius( radius );
	g_pSI->setPosition( pos );

	testCol();

	return pGroup;
}

int _tmain(int argc, _TCHAR* argv[])
{
    // construct the viewer
    osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer;

    // make the viewer create a 512x512 window and position it at 32, 32
    viewer->setUpViewInWindow( 32, 32, 512, 512 );

	// add the handler to the viewer
	osg::ref_ptr<CKeyboardHandler> rKeyboardHandler = new CKeyboardHandler;
	viewer->addEventHandler( rKeyboardHandler.get() );

	osg::Group* pRoot = new osg::Group;
	pGG = new osg::Group;
	pRoot->addChild( pGG );
	pRoot->addChild( CreateScene() );

    // set the scene-graph data the viewer will render
    viewer->setSceneData( pRoot );

    // execute main loop
    return viewer->run();
}

