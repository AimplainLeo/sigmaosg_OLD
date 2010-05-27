// GenTrees.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ParserXml.h"
#include "SceneTerrain.h"
#include <SigmaUtil/SulRenderInstances.h>
#include <osg/ArgumentParser>
#include <osgViewer/Viewer>
#include <osgDB/FileUtils>
#include <osgDB/WriteFile>
#include <osgGA/StateSetManipulator>
#include <osgViewer/ViewerEventHandlers>
#include <iostream>

int _tmain( int argc, char** argv )
{
	// create paths from environment variable
	char* ptr = getenv( "SIGMAOSG_DATA_PATH" );
	if ( ptr )
	{
		osgDB::FilePathList filepath;
		osgDB::convertStringPathIntoFilePathList( ptr, filepath );
		osgDB::appendPlatformSpecificResourceFilePaths(filepath);
		osgDB::setDataFilePathList(filepath);
	}

	// argument setup
	osg::ArgumentParser arguments( &argc, argv );
    arguments.getApplicationUsage()->setApplicationName( arguments.getApplicationName() );
    arguments.getApplicationUsage()->setDescription( arguments.getApplicationName()+" is tree generating program." );
    arguments.getApplicationUsage()->setCommandLineUsage( arguments.getApplicationName()+" [options]");

	// commandline arguments
	arguments.getApplicationUsage()->addCommandLineOption( "-h or --help",			"Display this information");
	arguments.getApplicationUsage()->addCommandLineOption( "--xml",					"xml file (required)" );
	arguments.getApplicationUsage()->addCommandLineOption( "--outputfile",			"Override the output file specified in the xml file" );

    // report any errors if they have occurred when parsing the program arguments.
    if (arguments.errors())
    {
        arguments.writeErrorMessages( std::cout );
        return 1;
    }

	// this will print the argument info out if the user doesn't supply any arguments
    if (arguments.argc()<=1)
    {
        arguments.getApplicationUsage()->write( std::cout,osg::ApplicationUsage::COMMAND_LINE_OPTION );
    }

	// display help for commandline
	if (arguments.read("-h") || arguments.read("--help"))
	{
		arguments.getApplicationUsage()->write( std::cout,osg::ApplicationUsage::COMMAND_LINE_OPTION );
		return 0;
	}

	std::string xmlFile;
	if ( !arguments.read( "--xml", xmlFile ) )
	{
		osg::notify(osg::FATAL) << "Required option --xml is missing" << std::endl;
		return 1;
	}

	CSulString outputfile;
	arguments.read("--outputfile", outputfile);

	CSulString inputfile;
	for(int pos = 1; pos < arguments.argc(); ++pos)
	{
		if (!arguments.isOption(pos))
		{
			inputfile = arguments[pos];
		}
	}

	// load and parse xml file
	osg::ref_ptr<CParserXml> rXml = new CParserXml;
	// if an input file was supplied on the command line, use that instead of the one in the xml file.
	if(!inputfile.empty())
	{
		rXml->SetInputFileOverride(inputfile);
	}
	// if an output file was supplied on the command line, use that instead of the one in the xml file.
	if (!outputfile.empty())
	{
		rXml->SetOutputFileOverride(outputfile);
	}
	rXml->Load( xmlFile );

	osg::ref_ptr<osg::Group> rRoot = new osg::Group;

	// these are the actual trees that are generated
	Sigma::uint32 texUnit			= rXml->getTexUnit();
	bool bSuppressTexture			= rXml->isTextureSuppressed();
	float sizeMultiplier			= rXml->getSizeMultiplier();
	osg::Image* pImage				= rXml->getGen()->getImage();
	Sigma::uint32 posCount			= rXml->getGen()->getCount();
	Sigma::uint32 texSizeSquared	= rXml->getGen()->getTexSizeSquared();
	Sigma::uint32 useLights			= rXml->getUseLights();
	osg::BoundingBox bb				= rXml->getSceneTerrain()->getBoundingBoxWorld();
	CSulRenderInstances* pRenderInstances = new CSulRenderInstances( pImage, posCount, bb, sizeMultiplier, bSuppressTexture, texUnit, texSizeSquared, useLights );
	pRenderInstances->create();
	//rRoot->addChild( pRenderInstances );
	rXml->getSceneTerrain()->getPat()->addChild( pRenderInstances );

	// we should write the output file here
	if ( !rXml->getOutputFile().empty() )
	{
		osg::ref_ptr<osg::Group> pGroupSave = new osg::Group;
		pGroupSave->addChild( pRenderInstances );
		osgDB::writeNodeFile( *pGroupSave, rXml->getOutputFile() );
	}

	// if the xml doesn't have viewer suppression then we show the result
	if ( !rXml->isViewerSuppressed() )
	{
		rRoot->addChild( rXml->getSceneTerrain() );

		if ( rXml->getSceneShape()->isRenderMe() )
		{
			rRoot->addChild( rXml->getSceneShape() );
		}

		// add shapemasks to scene so we can see them
		const VEC_SHAPEMASK& vecShapeMask = rXml->getShapeMaskList();
		VEC_SHAPEMASK::const_iterator i;
		i = vecShapeMask.begin();
		while ( i!=vecShapeMask.end() )
		{
			if ( (*i)->isRenderMe() )
			{
				rRoot->addChild( *i );
			}
			++i;
		}

	    osg::ref_ptr<osgViewer::Viewer> rViewer = new osgViewer::Viewer;
		rViewer->setUpViewInWindow( 32, 32, 800, 600 );

		rViewer->setSceneData( rRoot );

		// setup an event handle for statistics
		rViewer->addEventHandler( new osgViewer::StatsHandler );

		// allow wireframe modes
		rViewer->addEventHandler( new osgGA::StateSetManipulator(rViewer->getCamera()->getOrCreateStateSet()) );

		// work with best detailed lod
		rViewer->getCamera()->setLODScale( 0.0 );
	
		return rViewer->run();
	}

	return 0;
}

