// GenTrees.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ParserXml.h"
#include "SceneTerrain.h"
#include <SigmaUtil/SulRenderInstances.h>
#include <SigmaUtil/SulGeomAxis.h>
#include <SigmaUtil/SulRenderCellInstances.h>
#include <osg/ArgumentParser>
#include <osgViewer/Viewer>
#include <osgDB/FileUtils>
#include <osgDB/WriteFile>
#include <osgGA/StateSetManipulator>
#include <osgGA/TerrainManipulator>
#include <osgViewer/ViewerEventHandlers>
#include <iostream>

#include "AddGeometryFromPrototypeFunctor.h"
#include "GeometryInstancingBuilder.h"

osg::ref_ptr<osg::Geometry> createSingleTreeCollisionGeometry( float s )
{
	osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
	vertices->push_back( osg::Vec3(-0.5f, 0.0f, 0.0f) * s );
	vertices->push_back( osg::Vec3( 0.5f, 0.0f, 0.0f) * s );
	vertices->push_back( osg::Vec3( 0.5f, 0.0f, 1.0f) * s );
	vertices->push_back( osg::Vec3(-0.5f, 0.0f, 1.0f) * s );

	vertices->push_back( osg::Vec3(0.0f, -0.5f, 0.0f) * s );
	vertices->push_back( osg::Vec3(0.0f,  0.5f, 0.0f) * s );
	vertices->push_back( osg::Vec3(0.0f,  0.5f, 1.0f) * s );
	vertices->push_back( osg::Vec3(0.0f, -0.5f, 1.0f) * s );

	osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry;
	geometry->setVertexArray(vertices);
    geometry->addPrimitiveSet( new osg::DrawArrays( GL_QUADS, 0, 8) );

	return geometry;
}

osg::ref_ptr<osg::Node> createTreeCollisionGeometry( const std::vector<osg::Vec3>& positionList, float maxTreeSize )
{
	GeometryInstancingBuilder builder;
	builder.setPositionList(positionList);
	builder.setInstanceGeometry(createSingleTreeCollisionGeometry(maxTreeSize));
	return builder.build();
}

void writeFile( osg::Node* pInstances, osg::Node* pGeomCol, const CSulString& sFile, CParserXml* xml )
{
	osg::ref_ptr<osg::Group> pGroupSave = new osg::Group;
	pGroupSave->addChild( pInstances );

	if ( pGeomCol )
	{
		pGroupSave->addChild( pGeomCol );
	}

	// save file
	osgDB::writeNodeFile( *pGroupSave, sFile );

// for debugging
	//osgDB::writeNodeFile( *pGroupSave, "debug.osg" );
}

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
	rXml->load( xmlFile );

	osg::ref_ptr<osg::Group> rRoot = new osg::Group;
 
	// these are the actual trees that are generated
	sigma::uint32 texUnit			= rXml->getTexUnit();
	bool bSuppressTexture			= rXml->isTextureSuppressed();
	osg::Image* pImage				= rXml->getGen()->getImage();			// this is the texture that contains the positions for the trees
	sigma::uint32 posCount			= rXml->getGen()->getCount();
	sigma::uint32 texSizeSquared	= rXml->getGen()->getTexSizeSquared();
	sigma::uint32 useLights			= rXml->getUseLights();
	osg::BoundingBox bb				= rXml->getSceneTerrain()->getBoundingBoxWorld();
	float minTree					= rXml->getMinTree();
	float maxTree					= rXml->getMaxTree();
	bool bSuppressShaders			= rXml->getSuppressShaders();
	
//	CSulRenderInstances* pRenderInstances = 0;
	osg::Group* pRenderInstances = 0;

	bool bUseZDirectionNormal = true;

	if ( rXml->cells() )
	{
		osg::Vec2 cellXY = rXml->getCellXY();

		pRenderInstances = new CSulRenderCellInstances( cellXY, pImage, posCount, bb, minTree, maxTree, bSuppressTexture, texUnit, bUseZDirectionNormal, bSuppressShaders );
		//((CSulRenderCellInstances*)pRenderInstances)->createCrossQuad();
		((CSulRenderCellInstances*)pRenderInstances)->process();
	}
	else
	{
		pRenderInstances = new CSulRenderInstances( pImage, posCount, bb, minTree, maxTree, bSuppressTexture, texUnit, texSizeSquared, useLights, true, bSuppressShaders );
		//pRenderInstances->createBillboard();
		((CSulRenderInstances*)pRenderInstances)->createCrossQuad();
	}

	std::cout << "Number of trees: " << posCount << std::endl;

	rXml->getSceneTerrain()->getPat()->addChild( pRenderInstances );

	osg::ref_ptr<osg::Node> rGeomCol; 
	if ( rXml->hasColGeom() )
	{
		rGeomCol = createTreeCollisionGeometry( rXml->getGen()->getPositions(), rXml->getMaxTree() );
		rGeomCol->setNodeMask(0x1);
		rGeomCol->getOrCreateStateSet()->setMode(GL_CULL_FACE, osg::StateAttribute::OFF);
	}

	// add uniforms here
	const CParserXml::VEC_UNIFORMDATA& vecUniformDataList = rXml->getUniformDataList();
	osg::StateSet* ss = pRenderInstances->getOrCreateStateSet();
	CParserXml::VEC_UNIFORMDATA::const_iterator i = vecUniformDataList.begin();
	CParserXml::VEC_UNIFORMDATA::const_iterator iE = vecUniformDataList.end();
	while ( i!=iE )
	{
		CUniformData* p = (*i);
		osg::Uniform* pUniform = p->createUniform();
		ss->addUniform( pUniform );

		++i;
	}

	////////////////////////////////////////////
	// add shader if requested here
	////////////////////////////////////////////

	osg::Program* program = rXml->getProgram();
	if ( program )
	{
		pRenderInstances->getOrCreateStateSet()->setAttribute( program );
	}

	////////////////////////////////////////////
	// we should write the output file here
	////////////////////////////////////////////

	if ( !rXml->getOutputFile().empty() )
	{
		writeFile( pRenderInstances, rGeomCol, rXml->getOutputFile(), rXml );

		/*
		osg::ref_ptr<osg::Group> pGroupSave = new osg::Group;
		pGroupSave->addChild( pRenderInstances );
		if ( rGeomCol.valid() )
		{
			pGroupSave->addChild( rGeomCol );
		}

		// save file
		osgDB::writeNodeFile( *pGroupSave, rXml->getOutputFile() );
		*/
	}

	// if the xml doesn't have viewer suppression then we show the result
	if ( !rXml->isViewerSuppressed() )
	{
		if ( rXml->pivotVisible() )
		{
			CSulGeomAxis* p = new CSulGeomAxis( 1.0f );
			rRoot->addChild( p );
		}

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

		rViewer->setCameraManipulator(new osgGA::TerrainManipulator);

		// work with best detailed lod
		rViewer->getCamera()->setLODScale( 0.0 );
	
		return rViewer->run();
	}

	return 0;
}

