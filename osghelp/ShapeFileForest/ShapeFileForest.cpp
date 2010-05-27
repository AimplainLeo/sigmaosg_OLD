// ShapeFileForest.cpp : Defines the entry point for the console application.
//

// we need to create a texture that represents locations of trees
// this texture needs to read the shapefile data and create points
// based on lines and polygons

#include "stdafx.h"
#include "ProcessData.h"
#include "Console.h"
#include "ParserXml.h"
#include <SigmaUtil/SulConsoleDisplay.h>
#include <SigmaUtil/SulGeomBoundingBox.h>
#include <SigmaUtil/SulCalcBoundingBox.h>
#include <SigmaUtil/SulScreenAlignedQuad.h>
#include <SigmaUtil/SulNodeCallbackCameraSync.h>
#include <SigmaUtil/SulTexCam.h>
#include <SigmaUtil/SulGenHeightTexture.h>
#include <SigmaUtil/SulCoordLatLon.h>
#include <SigmaUtil/SulCoordGeodetic.h>
#include <SigmaUtil/SulCoordUTM.h>
#include <SigmaUtil/SulGenPositionTextures.h>
#include <SigmaUtil/SulRenderInstances.h>
#include <SigmaUtil/SulGeomSphere.h>
#include <SigmaUtil/SulXmlParser.h>
#include <osg/group>
#include <osgViewer/Viewer>
#include <osgDB/FileUtils>
#include <osgDB/ReadFile>
#include <osgGA/TrackballManipulator>
#include <osgGA/StateSetManipulator>
#include <osgGA/TerrainManipulator>
#include <osgViewer/ViewerEventHandlers>
#include <osgManipulator/TranslateAxisDragger>
#include <osg/PositionAttitudeTransform>
#include <osgSim/GeographicLocation>
#include <osg/io_utils>
#include <osg/ShapeDrawable>
#include <iostream>
//#include <osg/io_utils>

//--offsetX 10240 --offsetY 10240 --sizeMultiplier 200

CParserXml		xmlParser;
bool			bShowPivots = false;
bool			bSuppressTexture = false;
Sigma::uint32	texUnit = 0;
std::string		sceneFile;
std::string		shapeFile;
std::string		outputFile;
float			sizeMultiplier = 1.0f;
float			offsetX = 0.0f;
float			offsetY = 0.0f;

bool generateCollisionGeometry = false;
osg::ref_ptr<osg::PositionAttitudeTransform> rPat_shape;
osg::ref_ptr<osg::PositionAttitudeTransform> rPat_scene;

osg::Group* pRoot = 0;
osg::Group* pRoot_work = 0;
osg::Group* pRoot_scene = 0;
CSulRenderInstances* pRenderInstances = 0;
osgSim::GeographicLocation* locScene = 0;
osgSim::GeographicLocation* locShape = 0;

osg::Node* pSceneLocal = 0;


osg::Group* createScene()
{
	pSceneLocal = osgDB::readNodeFile( sceneFile );
	if (!pSceneLocal)
	{
		osg::notify(osg::FATAL) << "Unable to load " << sceneFile << std::endl;
		return NULL;
	}
	rPat_scene = new osg::PositionAttitudeTransform;
	rPat_scene->addChild( pSceneLocal );

	locScene = dynamic_cast<osgSim::GeographicLocation*>(pSceneLocal->getUserData());

	if ( bShowPivots )
	{
		osg::Geode* pOrigo = new osg::Geode;
		osg::ShapeDrawable* pCylinder = new osg::ShapeDrawable(new osg::Cylinder(osg::Vec3(0,0,0), 10, 100));
		pCylinder->setColor(osg::Vec4(1,0,0,1));
		pOrigo->addDrawable(pCylinder);
		rPat_scene->addChild(pOrigo);
	}

	return rPat_scene;
}

osg::Group* createShape()
{
	osg::Node* pNode = osgDB::readNodeFile( shapeFile );
	if (!pNode)
	{
		osg::notify(osg::FATAL) << "Unable to load " << shapeFile << std::endl;
		return NULL;
	}
	rPat_shape = new osg::PositionAttitudeTransform;
	rPat_shape->addChild( pNode );

	pNode->getOrCreateStateSet()->setMode( GL_CULL_FACE, osg::StateAttribute::OFF | osg::StateAttribute::PROTECTED | osg::StateAttribute::OVERRIDE );

	locShape = dynamic_cast<osgSim::GeographicLocation*>(pNode->getUserData());

#ifdef SHOW_PIVOTS
	osg::Geode* pOrigo = new osg::Geode;
	osg::ShapeDrawable* pCylinder = new osg::ShapeDrawable(new osg::Cylinder(osg::Vec3(0,0,0), 10, 100));
	pCylinder->setColor(osg::Vec4(0,0,1,1));
	pOrigo->addDrawable(pCylinder);
	rPat_shape->addChild(pOrigo);
#endif // SHOW_PIVOTS

	return rPat_shape;
}

osg::Group* createShapeMasks()
{
	osg::Group* pGroup = new osg::Group;

	VEC_DATASHAPEMASK& vecShapeMask = xmlParser.getDataShapeMask();
	VEC_DATASHAPEMASK::iterator i;
	i = vecShapeMask.begin();
	while ( i!=vecShapeMask.end() )
	{
		osg::ref_ptr<osg::PositionAttitudeTransform> pat = new osg::PositionAttitudeTransform;
		osg::Node* pNode = osgDB::readNodeFile( i->m_file );
		pat->addChild( pNode );
		pNode->getOrCreateStateSet()->setMode( GL_CULL_FACE, osg::StateAttribute::OFF | osg::StateAttribute::PROTECTED | osg::StateAttribute::OVERRIDE );
		i->m_node = pat;
		i->loc = dynamic_cast<osgSim::GeographicLocation*>(pNode->getUserData());
		pGroup->addChild( pat );
		++i;
	}

	return pGroup;
}

void calcPositions()
{
	CSulCoordLatLon worldLatLon( locScene->latitude(), locScene->longitude() );
	CSulCoordLatLon sceneLatLon( locScene->latitude(), locScene->longitude() );
	CSulCoordLatLon shapeLatLon( locShape->latitude(), locShape->longitude() );
	
	osg::notify(osg::NOTICE) << "scene lat lon = " << locScene->latitude() << ", " << locScene->longitude() << std::endl;
	osg::notify(osg::NOTICE) << "shape lat lon = " << locShape->latitude() << ", " << locShape->longitude() << std::endl;

	CSulCoordUTM worldUTM( worldLatLon );

	osg::Vec3d v0 = worldUTM.getXYZ(shapeLatLon);
	osg::Vec3d v1 = worldUTM.getXYZ(sceneLatLon);

	// Computing the bounding box and using that as the offset will only work if 
	// the whole terrain (master.flt) is loaded.
	// Since we are loading one block at a time, the offset needs to be specified on the command line.
	v1 += osg::Vec3(offsetX, offsetY, 0);

	v0.z()+=40.0;
	rPat_shape->setPosition( v0 );
	rPat_scene->setPosition( v1 );

	osg::notify(osg::NOTICE) << "Shape offset = " << v0 << std::endl;
	osg::notify(osg::NOTICE) << "Scene offset = " << v1 << std::endl;

	// adjust shape masks
	VEC_DATASHAPEMASK& vecShapeMask = xmlParser.getDataShapeMask();
	VEC_DATASHAPEMASK::iterator i;
	i = vecShapeMask.begin();
	while ( i!=vecShapeMask.end() )
	{
		CSulCoordLatLon shapeMaskLatLon( i->loc->latitude(), i->loc->longitude() );
		osg::Vec3 p = worldUTM.getXYZ( shapeMaskLatLon );
		p.z() += 100.0f;
		i->m_node->setPosition( p );
		++i;
	}
}

int _tmain(int argc, char** argv)
{
    osg::ArgumentParser arguments( &argc, argv );

    arguments.getApplicationUsage()->setApplicationName( arguments.getApplicationName() );
    arguments.getApplicationUsage()->setDescription( arguments.getApplicationName()+" is tree generating program." );
    arguments.getApplicationUsage()->setCommandLineUsage( arguments.getApplicationName()+" [options]");
	arguments.getApplicationUsage()->addCommandLineOption( "-h or --help",			"Display this information");
	arguments.getApplicationUsage()->addCommandLineOption( "--sceneFile",			"scene file [FLT]" );
	arguments.getApplicationUsage()->addCommandLineOption( "--shapeFile",			"shape file [FLT]" );
	arguments.getApplicationUsage()->addCommandLineOption( "--outputFile",			"output file" );
	arguments.getApplicationUsage()->addCommandLineOption( "--noViewer",			"no viewer visible" );
	arguments.getApplicationUsage()->addCommandLineOption( "--sizeMultiplier",		"geom is 1x1 meter * multiplier" );
    arguments.getApplicationUsage()->addCommandLineOption( "--lineSpacing",			"spacing between trees");
    arguments.getApplicationUsage()->addCommandLineOption( "--lineJitterRadius",	"random placing trees in this radius (in meters)");
	arguments.getApplicationUsage()->addCommandLineOption( "--lineSnap",			"will snap tree to line (after jitter)" );
	arguments.getApplicationUsage()->addCommandLineOption( "--offsetX",				"X offset in meters" );
	arguments.getApplicationUsage()->addCommandLineOption( "--offsetY",				"Y offset in meters" );
	arguments.getApplicationUsage()->addCommandLineOption( "--suppressTexture",		"tree texture is not loaded or created." );
	arguments.getApplicationUsage()->addCommandLineOption( "--texUnit",				"texture unit to use for tree texture" );
	arguments.getApplicationUsage()->addCommandLineOption( "--xml",					"xml file" );
	arguments.getApplicationUsage()->addCommandLineOption( "--collision",			"include collision geometry in the output file" );
	arguments.getApplicationUsage()->addCommandLineOption( "--showPivots",			"show pivots" );



    // report any errors if they have occurred when parsing the program arguments.
    if (arguments.errors())
    {
        arguments.writeErrorMessages(std::cout);
        return 1;
    }
    /*
	// this will print the argument info out if the user doesn't supply any arguments
    if (arguments.argc()<=1)
    {
        arguments.getApplicationUsage()->write(std::cout,osg::ApplicationUsage::COMMAND_LINE_OPTION);
        return 1;
    }
*/
	if (arguments.read("-h") || arguments.read("--help"))
	{
		arguments.getApplicationUsage()->write(std::cout,osg::ApplicationUsage::COMMAND_LINE_OPTION);
		return 0;
	}

	std::string xmlFile;
	if ( arguments.read( "--xml", xmlFile ) )
	{
		xmlParser.Load( xmlFile );
	}	

	if ( arguments.read( "--suppressTexture"  ) )
	{
		bSuppressTexture = true;
	}

	arguments.read( "--texUnit", texUnit );

	if ( !arguments.read( "--sceneFile", sceneFile ) )
	{
		//sceneFile = "FACTerrain_noforest_flt_1024/master.flt";
		sceneFile = "\\\\Demo09\\FACTerrain\\FACTerrain_old\\FACTerrain12_6.flt";
	}

	if ( !arguments.read( "--shapeFile", shapeFile ) )
	{
		//shapeFile = "FACTerrainDebug_noforest/levendeHegn/ll2.flt";
		shapeFile = "\\\\Demo09\\FACTerrain\\FACTerrain_old\\tree_areas.flt";
	}

	if ( !arguments.read( "--outputFile", outputFile ) )
	{
		outputFile = "myass.ive";
	}

	if(arguments.read( "--collision"))
	{
		generateCollisionGeometry = true;
	}

	if ( !arguments.read( "--sizeMultiplier", sizeMultiplier ) )
	{
		sizeMultiplier = 2.0f;
	}

	if ( !arguments.read( "--offsetX", offsetX ) )
	{
		offsetX = 0.0f;
	}

	if ( !arguments.read( "--offsetY", offsetY ) )
	{
		offsetX = 0.0f;
	}

	bool bNoViwer = false;
	if ( arguments.read( "--noViewer"  ) )
	{
		bNoViwer = true;
	}

	if ( arguments.read( "--showPivots"  ) )
	{
		bShowPivots = true;
	}

	osgDB::FilePathList filepath;

	char* ptr = getenv( "SIGMAOSG_DATA_PATH" );
	if ( ptr )
	{
		osgDB::convertStringPathIntoFilePathList( ptr, filepath );
		osgDB::appendPlatformSpecificResourceFilePaths(filepath);
		osgDB::setDataFilePathList(filepath);
	}

    // construct the viewer
    osg::ref_ptr<osgViewer::Viewer> rViewer = new osgViewer::Viewer;

    rViewer->setUpViewInWindow( 32, 32, 800, 600 );

	pRoot = new osg::Group;

	pRoot_work = new osg::Group;
	pRoot_work->setDataVariance( osg::Object::DYNAMIC );	
	pRoot->addChild( pRoot_work );

	pRoot_scene = new osg::Group;
	pRoot_scene->setDataVariance( osg::Object::DYNAMIC );	
	pRoot->addChild( pRoot_scene );
	
	// scene
	osg::Group* pScene = createScene();
	osg::Group* pShape = createShape();
	if (!pScene || !pShape)
	{
		return 1;
	}
	osg::Group* pShapeMasks = createShapeMasks();
	pRoot_scene->addChild( pScene );
	pRoot_scene->addChild( pShape );
	pRoot_scene->addChild( pShapeMasks );

	// move shapefile and scene (terrain) into position based on lat, long
	calcPositions();

    // create a console
    osg::ref_ptr<CSulConsoleDisplay> rConsoleDisplay = new CSulConsoleDisplay( rViewer );
    rConsoleDisplay->Init();
	pRoot_work->addChild( rConsoleDisplay->GetNode() );
	
	CConsole* pConsole = new CConsole( 
		rViewer,
		rConsoleDisplay, 
		rPat_scene,
		rPat_shape );

	if ( bShowPivots )
	{
		osg::Geode* pOrigo = new osg::Geode;
		pOrigo->addDrawable( new osg::ShapeDrawable(new osg::Cylinder(osg::Vec3(0,0,0), 10, 100)) );
		pRoot->addChild(pOrigo);
	}

	rViewer->setSceneData( pRoot );

	// setup an event handle for statistics
    rViewer->addEventHandler( new osgViewer::StatsHandler );

	rViewer->addEventHandler( new osgGA::StateSetManipulator(rViewer->getCamera()->getOrCreateStateSet()) );

	//rViewer->getCamera()->setLODScale( 0.1 );
	rViewer->getCamera()->setLODScale( 0.0 );

	rViewer->setCameraManipulator(new osgGA::TerrainManipulator);

    // execute main loop
	pConsole->generate();
	pConsole->save();

	if ( !bNoViwer )
	{
		return rViewer->run();
	}
}
