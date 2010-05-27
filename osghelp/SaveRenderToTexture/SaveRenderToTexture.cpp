// SaveRenderToTexture.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <osgViewer/Viewer>
#include <osg/ShapeDrawable>
#include <SigmaUtil/SulRTT.h>
#include <SigmaUtil/SulScreenAlignedQuad.h>
#include <SigmaUtil/SulNodeCallbackCameraSync.h>
#include <osg/Material>
#include <osgdb/writefile>
#include <osgdb/readfile>
#include <iostream>

//#define FILENAME "myass.png"
#define FILENAME "myass.tif"

osg::ref_ptr<CSulRTT> rRTT;
osg::ref_ptr<osg::Group> rGroup;
osg::ref_ptr<osg::Image> rImage;

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
			// save
            if ( ea.getKey()==osgGA::GUIEventAdapter::KEY_F1 )
            {
				bool bRet = osgDB::writeImageFile( *rImage, FILENAME );
				if ( !bRet )
				{
					std::cout << "writeImageFile FAILED" << std::endl;
					return true;
				}

				// display image
				CSulScreenAlignedQuad* quad = new CSulScreenAlignedQuad( osg::Vec3(64+256+128,64,0), 128, 128, 800, 600 );
				quad->setTexture( rImage );
				rGroup->addChild( quad->getProjection() );

				std::cout << "writeImageFile done." << std::endl;

                return true; // return true, event handled
            }

			// load
            if ( ea.getKey()==osgGA::GUIEventAdapter::KEY_F2 )
            {
				osg::Image* pImage = osgDB::readImageFile( FILENAME );
				if ( !pImage )
				{
					std::cout << "readImageFile FAILED" << std::endl;
					return true;
				}

				// display image
				CSulScreenAlignedQuad* quad = new CSulScreenAlignedQuad( osg::Vec3(64+256+256+128,64,0), 128, 128, 800, 600 );
				quad->setTexture( pImage );
				rGroup->addChild( quad->getProjection() );

				std::cout << "readImageFile done." << std::endl;
	
                return true; // return true, event handled
            }
        }

        return false;
    }
}; 

osg::Node* CreateScene( osgViewer::Viewer* pViewer )
{
	rGroup = new osg::Group;

    // we create the simplest form of shapes in OpenSceneGraph
	osg::Geode* pGeode = new osg::Geode();
    pGeode->addDrawable( new osg::ShapeDrawable( new osg::Sphere(osg::Vec3(0.0f,0.0f,0.0f),0.5f) ) );
    pGeode->addDrawable( new osg::ShapeDrawable( new osg::Box(osg::Vec3(2.0f,0.0f,0.0f),2.0f) ) );
    pGeode->addDrawable( new osg::ShapeDrawable( new osg::Cone(osg::Vec3(4.0f,0.0f,0.0f),0.5f,3.0f) ) );
    pGeode->addDrawable( new osg::ShapeDrawable( new osg::Cylinder(osg::Vec3(6.0f,0.0f,0.5f),0.5f,3.0f) ) );
    pGeode->addDrawable( new osg::ShapeDrawable( new osg::Capsule(osg::Vec3(8.0f,0.0f,1.0f),0.5f,3.0f) ) );
	rGroup->addChild( pGeode );

	// make objects red
    osg::Material* pMaterial = new osg::Material;
    pMaterial->setDiffuse( osg::Material::FRONT, osg::Vec4(1,0,0,1) );
    pGeode->getOrCreateStateSet()->setAttribute( pMaterial, osg::StateAttribute::OVERRIDE );

	// create a render to texture (that saves the scene in a lumiance values of floats)
	rRTT = new CSulRTT( 256, 256, GL_LUMINANCE, GL_FLOAT ); 
	rRTT->getTexture()->setInternalFormat( GL_LUMINANCE32F_ARB );
	rRTT->getTexture()->setSourceType( GL_FLOAT );
	rRTT->getTexture()->setFilter(osg::Texture2D::MIN_FILTER, osg::Texture2D::NEAREST);
	rRTT->getTexture()->setFilter(osg::Texture2D::MAG_FILTER, osg::Texture2D::NEAREST);
	rRTT->getCamera()->setClearColor( osg::Vec4(0,0,0,1) );
	rRTT->AddChildToCamera( pGeode );
	rRTT->getCamera()->setUpdateCallback( new CSulNodeCallbackCameraSync( pViewer->getCamera() ) );
	rGroup->addChild( rRTT->getCamera() );

	// display our texture on a quad
	osg::ref_ptr<CSulScreenAlignedQuad> rQuad = new CSulScreenAlignedQuad( osg::Vec3(128,128,0), 256, 256, 800, 600 );
	rQuad->setTexture( rRTT->getTexture() );
	rGroup->addChild( rQuad->getProjection() );

	// attach image to rtt
	rImage = new osg::Image;
	rImage->allocateImage( 256, 256, 1,  GL_LUMINANCE, GL_FLOAT );
	rImage->setInternalTextureFormat( GL_LUMINANCE32F_ARB );
	rRTT->getCamera()->attach( osg::Camera::COLOR_BUFFER, rImage );
 

    return rGroup;
}

int _tmain(int argc, _TCHAR* argv[])
{
    // construct the viewer
    osg::ref_ptr<osgViewer::Viewer> rViewer = new osgViewer::Viewer;

    // make the viewer create a window and position it at 32, 32
    rViewer->setUpViewInWindow( 32, 32, 800, 600 );

	// add the handler to the viewer
    osg::ref_ptr<CKeyboardHandler> rKeyboardHandler = new CKeyboardHandler;
    rViewer->addEventHandler( rKeyboardHandler.get() );

    // set the scene-graph data the viewer will render
    rViewer->setSceneData( CreateScene( rViewer ) );

    // execute main loop
    return rViewer->run();
}