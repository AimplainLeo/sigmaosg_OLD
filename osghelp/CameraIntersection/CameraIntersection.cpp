// CameraIntersection.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <SigmaUtil/SulIntersectionWithCamera.h>
#include <SigmaUtil/SulScreenAlignedQuad.h>
#include <SigmaUtil/SulIntersectionInfo.h>
#include <osgViewer/Viewer>
#include <osg/ShapeDrawable>
#include <osgGA/GUIEventHandler>

osg::ref_ptr<CSulIntersectionWithCamera> camInt;

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
			if ( ea.getKey()==osgGA::GUIEventAdapter::KEY_Space )
			{
				osgViewer::ViewerBase::Cameras cams;
				pViewer->getCameras( cams );

				float r = ((float)rand()/(float)RAND_MAX);
				float g = ((float)rand()/(float)RAND_MAX);
				float b = ((float)rand()/(float)RAND_MAX);
				cams[0]->setClearColor( osg::Vec4(r, g, b, 1.0f ) );

				// set intersect ray at current camera position
				osg::Vec3 pos = cams[0]->getInverseViewMatrix().getTrans();
				camInt->setPosition( pos );

				// point intersect ray in current camera direction
				osg::Vec3 dir = -osg::Matrix::transform3x3( osg::Z_AXIS, cams[0]->getInverseViewMatrix() );
				camInt->setDirection( dir );

				camInt->shoot();

 				return true; // return true, event handled
			}
		}

		return false;
	}
};

class CMyIntersect : public CSulIntersectionInfo
{
public:
	void update()
	{
	}
};

osg::Node* CreateScene()
{
	osg::Group* pGroup = new osg::Group;

	osg::Geode* pGeode = new osg::Geode();

	// we create the simplest form of shapes in OpenSceneGraph
    pGeode->addDrawable( new osg::ShapeDrawable( new osg::Sphere(osg::Vec3(0.0f,0.0f,0.0f),0.5f) ) );
    pGeode->addDrawable( new osg::ShapeDrawable( new osg::Box(osg::Vec3(2.0f,0.0f,0.0f),2.0f) ) );
    pGeode->addDrawable( new osg::ShapeDrawable( new osg::Cone(osg::Vec3(4.0f,0.0f,0.0f),0.5f,3.0f) ) );
    pGeode->addDrawable( new osg::ShapeDrawable( new osg::Cylinder(osg::Vec3(6.0f,0.0f,0.5f),0.5f,3.0f) ) );
    pGeode->addDrawable( new osg::ShapeDrawable( new osg::Capsule(osg::Vec3(8.0f,0.0f,1.0f),0.5f,3.0f) ) );
	pGroup->addChild( pGeode );

	// create camera intersection code
	CMyIntersect* tmp = new CMyIntersect;

	camInt = new CSulIntersectionWithCamera( pGeode, tmp );
	pGroup->addChild( camInt );
	
	// debugging so I can see what we are aiming at
	osg::ref_ptr<CSulScreenAlignedQuad> rQuad = new CSulScreenAlignedQuad( osg::Vec3(64,64,0), 128, 128, 800, 600 );
	rQuad->setTexture( camInt->getTexture() );
	pGroup->addChild( rQuad->getProjection() );

	return pGroup;
}

int _tmain(int argc, _TCHAR* argv[])
{
    // construct the viewer
    osg::ref_ptr<osgViewer::Viewer> rViewer = new osgViewer::Viewer;

    // make the viewer create a 512x512 window and position it at 32, 32
    rViewer->setUpViewInWindow( 32, 32, 800, 600 );

	// add the handler to the viewer
	osg::ref_ptr<CKeyboardHandler> rKeyboardHandler = new CKeyboardHandler;
	rViewer->addEventHandler( rKeyboardHandler.get() );

    // set the scene-graph data the viewer will render
    rViewer->setSceneData( CreateScene() );

    // execute main loop
    return rViewer->run();
}
