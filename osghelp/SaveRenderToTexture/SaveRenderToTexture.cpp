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
#include <assert.h>

#include <osgGA/StateSetManipulator>
#include <osgGA/TerrainManipulator>
#include <osgGA/TrackballManipulator>
#include <osgGA/FlightManipulator>
#include <osgGA/DriveManipulator>
#include <osgGA/KeySwitchMatrixManipulator>
#include <osgViewer/ViewerEventHandlers>
#include <osgUtil/Optimizer>

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

	/*
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
*/
	osg::Node* pLoadedModel = osgDB::readNodeFile( "C:\\Project\\ifacts\\ibin\\data\\Terrains\\Terrastan_IVE_burnedstonewalls_volumes_lit_15k\\Terrastan_IVE_burnedstonewalls_volumes_lit_15k.ive" );
	assert( pLoadedModel );

    osgUtil::Optimizer optimizer;
    optimizer.optimize(pLoadedModel);


	rGroup->addChild( pLoadedModel );

	// create a render to texture (that saves the scene in a lumiance values of floats)
	rRTT = new CSulRTT( 256, 256, GL_LUMINANCE, GL_FLOAT ); 
	rRTT->getTexture()->setInternalFormat( GL_LUMINANCE32F_ARB );
	rRTT->getTexture()->setSourceType( GL_FLOAT );
	rRTT->getTexture()->setFilter(osg::Texture2D::MIN_FILTER, osg::Texture2D::NEAREST);
	rRTT->getTexture()->setFilter(osg::Texture2D::MAG_FILTER, osg::Texture2D::NEAREST);
	rRTT->getCamera()->setClearColor( osg::Vec4(0,0,0,1) );
	//rRTT->AddChildToCamera( pGeode );
	rRTT->AddChildToCamera( pLoadedModel );
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
	/*
    osg::ref_ptr<CKeyboardHandler> rKeyboardHandler = new CKeyboardHandler;
    rViewer->addEventHandler( rKeyboardHandler.get() );
	*/

 // set up the camera manipulators.
    {
        osg::ref_ptr<osgGA::KeySwitchMatrixManipulator> keyswitchManipulator = new osgGA::KeySwitchMatrixManipulator;

        keyswitchManipulator->addMatrixManipulator( '1', "Trackball", new osgGA::TrackballManipulator() );
        keyswitchManipulator->addMatrixManipulator( '2', "Flight", new osgGA::FlightManipulator() );
        keyswitchManipulator->addMatrixManipulator( '3', "Drive", new osgGA::DriveManipulator() );
        keyswitchManipulator->addMatrixManipulator( '4', "Terrain", new osgGA::TerrainManipulator() );

        rViewer->setCameraManipulator( keyswitchManipulator.get() );
    }

    // add the LOD Scale handler
    rViewer->addEventHandler(new osgViewer::LODScaleHandler);


    // set the scene-graph data the viewer will render
    rViewer->setSceneData( CreateScene( rViewer ) );

	rViewer->realize();

    // execute main loop
    return rViewer->run();
}


#if 0
int _tmain(int argc, _TCHAR* argv[])
{
    // use an ArgumentParser object to manage the program arguments.
    osg::ArgumentParser arguments(&argc,(char**)&argv);

    arguments.getApplicationUsage()->setApplicationName(arguments.getApplicationName());
    arguments.getApplicationUsage()->setDescription(arguments.getApplicationName()+" is the standard OpenSceneGraph example which loads and visualises 3d models.");
    arguments.getApplicationUsage()->setCommandLineUsage(arguments.getApplicationName()+" [options] filename ...");
    arguments.getApplicationUsage()->addCommandLineOption("--image <filename>","Load an image and render it on a quad");
    arguments.getApplicationUsage()->addCommandLineOption("--dem <filename>","Load an image/DEM and render it on a HeightField");
    arguments.getApplicationUsage()->addCommandLineOption("--login <url> <username> <password>","Provide authentication information for http file access.");

    osgViewer::Viewer viewer(arguments);

    unsigned int helpType = 0;
    if ((helpType = arguments.readHelpType()))
    {
        arguments.getApplicationUsage()->write(std::cout, helpType);
        return 1;
    }
    
    // report any errors if they have occurred when parsing the program arguments.
    if (arguments.errors())
    {
        arguments.writeErrorMessages(std::cout);
        return 1;
    }
    /*
    if (arguments.argc()<=1)
    {
        arguments.getApplicationUsage()->write(std::cout,osg::ApplicationUsage::COMMAND_LINE_OPTION);
        return 1;
    }
	*/

    std::string url, username, password;
    while(arguments.read("--login",url, username, password))
    {
        if (!osgDB::Registry::instance()->getAuthenticationMap())
        {
            osgDB::Registry::instance()->setAuthenticationMap(new osgDB::AuthenticationMap);
            osgDB::Registry::instance()->getAuthenticationMap()->addAuthenticationDetails(
                url,
                new osgDB::AuthenticationDetails(username, password)
            );
        }
    }

    // set up the camera manipulators.
    {
        osg::ref_ptr<osgGA::KeySwitchMatrixManipulator> keyswitchManipulator = new osgGA::KeySwitchMatrixManipulator;

        keyswitchManipulator->addMatrixManipulator( '1', "Trackball", new osgGA::TrackballManipulator() );
        keyswitchManipulator->addMatrixManipulator( '2', "Flight", new osgGA::FlightManipulator() );
        keyswitchManipulator->addMatrixManipulator( '3', "Drive", new osgGA::DriveManipulator() );
        keyswitchManipulator->addMatrixManipulator( '4', "Terrain", new osgGA::TerrainManipulator() );

        std::string pathfile;
        char keyForAnimationPath = '5';
        while (arguments.read("-p",pathfile))
        {
            osgGA::AnimationPathManipulator* apm = new osgGA::AnimationPathManipulator(pathfile);
            if (apm || !apm->valid()) 
            {
                unsigned int num = keyswitchManipulator->getNumMatrixManipulators();
                keyswitchManipulator->addMatrixManipulator( keyForAnimationPath, "Path", apm );
                keyswitchManipulator->selectMatrixManipulator(num);
                ++keyForAnimationPath;
            }
        }

        viewer.setCameraManipulator( keyswitchManipulator.get() );
    }

    // add the state manipulator
    viewer.addEventHandler( new osgGA::StateSetManipulator(viewer.getCamera()->getOrCreateStateSet()) );
    
    // add the thread model handler
    viewer.addEventHandler(new osgViewer::ThreadingHandler);

    // add the window size toggle handler
    viewer.addEventHandler(new osgViewer::WindowSizeHandler);
        
    // add the stats handler
    viewer.addEventHandler(new osgViewer::StatsHandler);

    // add the help handler
    viewer.addEventHandler(new osgViewer::HelpHandler(arguments.getApplicationUsage()));

    // add the record camera path handler
    viewer.addEventHandler(new osgViewer::RecordCameraPathHandler);

    // add the LOD Scale handler
    viewer.addEventHandler(new osgViewer::LODScaleHandler);

    // add the screen capture handler
    viewer.addEventHandler(new osgViewer::ScreenCaptureHandler);

    // load the data
	/*
    osg::ref_ptr<osg::Node> loadedModel = osgDB::readNodeFiles(arguments);
    if (!loadedModel) 
    {
        std::cout << arguments.getApplicationName() <<": No data loaded" << std::endl;
        return 1;
    }
	*/
	osg::ref_ptr<osg::Node> loadedModel = osgDB::readNodeFile( "C:\\Project\\ifacts\\ibin\\data\\Terrains\\Terrastan_IVE_burnedstonewalls_volumes_lit_15k\\Terrastan_IVE_burnedstonewalls_volumes_lit_15k.ive" );

    // any option left unread are converted into errors to write out later.
    arguments.reportRemainingOptionsAsUnrecognized();

    // report any errors if they have occurred when parsing the program arguments.
    if (arguments.errors())
    {
        arguments.writeErrorMessages(std::cout);
        return 1;
    }


    // optimize the scene graph, remove redundant nodes and state etc.
    osgUtil::Optimizer optimizer;
    optimizer.optimize(loadedModel.get());

    viewer.setSceneData( loadedModel.get() );

    viewer.realize();

    return viewer.run();

}
#endif