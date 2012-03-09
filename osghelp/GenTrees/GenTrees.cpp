// GenTrees.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ParserXml.h"
#include "SceneTerrain.h"
#include <SigmaUtil/SulRenderInstances.h>
#include <SigmaUtil/SulGeomAxis.h>
#include <SigmaUtil/SulRenderCellInstances.h>
#include <SigmaUtil/SulRTT.h>
#include <SigmaUtil/SulScreenAlignedQuad.h>
#include <SigmaUtil/SulNodeCallbackCameraSync.h>
#include <SigmaUtil/SulGeomBox.h>
#include <osg/ArgumentParser>
#include <osgViewer/Viewer>
#include <osgDB/FileUtils>
#include <osgDB/WriteFile>
#include <osgGA/StateSetManipulator>
#include <osgGA/TerrainManipulator>
#include <osgViewer/ViewerEventHandlers>
#include <osg/ShapeDrawable>
#include <osg/TextureCubeMap>
#include <osg/lod>
#include <iostream>

#include "AddGeometryFromPrototypeFunctor.h"
#include "GeometryInstancingBuilder.h"

osg::ref_ptr<osgViewer::Viewer> rViewer;
osg::ref_ptr<osg::Group>		rRoot;

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
	osg::ref_ptr<osg::Group> pGroupSave;

	if ( xml->getLodDistMin()>0 )
	{
		osg::LOD* p = new osg::LOD;
		pGroupSave = p;

		float min = xml->getLodDistMin();
		float max = xml->getLodDistMax();
		p->setRange( 0, min, max );
	}
	else
	{
		pGroupSave = new osg::Group;
	}

	
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

enum ORTHOVIEW
{
	FRONT,
	BACK,
	LEFT,
	RIGHT,
	TOP,
	BOTTOM
};

//osg::Texture2D* createTextureFromOrthoView( osgViewer::Viewer* viewer, sigma::uint32 texW, sigma::uint32 texH, osg::Node* pRenderMe, ORTHOVIEW eOrthoView )
osg::ref_ptr<osg::Image> createTextureFromOrthoView( osgViewer::Viewer* viewer, sigma::uint32 texW, sigma::uint32 texH, osg::Node* pRenderMe, ORTHOVIEW eOrthoView )
{
	//osg::ref_ptr<CSulRTT> rtt = new CSulRTT( texW, texH, CSulRTT::SETUP_ORTHO_FRONT );
	CSulRTT* rtt = new CSulRTT( texW, texH, CSulRTT::SETUP_ORTHO_FRONT );
	rtt->setClearColor( osg::Vec4(0,0,0,0) );
	rtt->addChild( pRenderMe );

	// bounding box
	osg::ComputeBoundsVisitor* tmp = new osg::ComputeBoundsVisitor;
	pRenderMe->accept( *tmp );
	osg::BoundingBox bb = tmp->getBoundingBox();


	// force camera into ortha and look at trees
	float l;
	float r;
	float t = bb.zMax();
	float b = bb.zMin();
	
	osg::Matrix m;
	switch ( eOrthoView )
	{
		case FRONT:
			{
				float mm[16] =
				{
					 1, 0,  0, 0,		// x,y,z
					 0, 0, -1, 0,
					 0, 1,  0, 0,
					 0, 0,  bb.yMin(), 1
				};
				
				m.set( mm );

				l = bb.xMin();
				r = bb.xMax();
				
				t = bb.zMin();
				b = bb.zMax();
			}
			break;

		case LEFT:
			{
				float mm[16] =
				{
					  0, 0, -1, 0,		// x,y,z
					  0, 1,  0, 0,
					  1, 0,  0, 0,
					  0, 0,  bb.xMin(), 1
				};

				m.set( mm );
				t = bb.yMax();
				b = bb.yMin();

				r = bb.zMin();
				l = bb.zMax();
			}
			break;

		case RIGHT:
			{
				float mm[16] =
				{
					  0, 0, 1, 0,		// x,y,z
					  0, 1,  0, 0,
					  1, 0,  0, 0,
					  0, 0,  -bb.xMax(), 1
				};
				m.set( mm );

				r = bb.zMax();
				l = bb.zMin();

				b = bb.yMin();
				t = bb.yMax();
			}
			break;

		case BACK:
			{
				float mm[16] =
				{
					 -1, 0,  0, 0,		// x,y,z
					  0, 0,  1, 0,
					  0, 1,  0, 0,
					  0, 0,  -bb.yMax(), 1
				};
				m.set( mm );
				r = -bb.xMax();
				l = -bb.xMin();
			}
			break;

		case TOP:
			{
				float mm[16] =
				{
					  1, 0,  0, 0,		// x,y,z
					  0, 1,  0, 0,
					  0, 0,  1, 0,
					  0, 0,  -bb.zMax(), 1
				};
				m.set( mm );
				r = bb.xMin();
				l = bb.xMax();
				t = bb.yMax();
				b = bb.yMin();
			}
			break;

		case BOTTOM:
			{
				float mm[16] =
				{
					  1, 0,  0, 0,		// x,y,z
					  0, -1,  0, 0,
					  0, 0,  -1, 0,
					  0, 0,  bb.zMin(), 1
				};
				m.set( mm );
				l = bb.xMin();
				r = bb.xMax();
				b = -bb.yMin();
				t = -bb.yMax();
			}
			break;

	}
	rtt->setViewMatrix( m );

	rtt->setProjectionMatrixAsOrtho( l, r, b, t, -100000.1f, 1000000.0f  );


	osg::ref_ptr<osg::Image> img = new osg::Image;
	rtt->attach( osg::Camera:: COLOR_BUFFER, img );

	rRoot->addChild( rtt );
	viewer->frame();
	rRoot->removeChild( rtt );	

//	return rtt->getTexture();
	return img;
}

void test( osg::Node* pRenderInstances )
{
	float w = 512;
	float h = 256;

	/*
	osg::Texture2D* texTop = createTextureFromOrthoView( rViewer, w, h, pRenderInstances, TOP );
	osg::Texture2D* texBottom = createTextureFromOrthoView( rViewer, w, h, pRenderInstances, BOTTOM );
	osg::Texture2D* texLeft = createTextureFromOrthoView( rViewer, w, h, pRenderInstances, LEFT );
	osg::Texture2D* texRight = createTextureFromOrthoView( rViewer, w, h, pRenderInstances, RIGHT );
	osg::Texture2D* texFront = createTextureFromOrthoView( rViewer, w, h, pRenderInstances, FRONT );
	osg::Texture2D* texBack = createTextureFromOrthoView( rViewer, w, h, pRenderInstances, BACK );
	*/

	osg::ref_ptr<osg::Image> imgTop = createTextureFromOrthoView( rViewer, w, h, pRenderInstances, TOP );
	osg::ref_ptr<osg::Image> imgBottom = createTextureFromOrthoView( rViewer, w, h, pRenderInstances, BOTTOM );
	osg::ref_ptr<osg::Image> imgLeft = createTextureFromOrthoView( rViewer, w, h, pRenderInstances, LEFT );
	osg::ref_ptr<osg::Image> imgRight = createTextureFromOrthoView( rViewer, w, h, pRenderInstances, RIGHT );
	osg::ref_ptr<osg::Image> imgFront = createTextureFromOrthoView( rViewer, w, h, pRenderInstances, FRONT );
	osg::ref_ptr<osg::Image> imgBack = createTextureFromOrthoView( rViewer, w, h, pRenderInstances, BACK );

	// now we need to create a cube and add textures
	osg::TextureCubeMap* pTexCube = new osg::TextureCubeMap;
	pTexCube->setImage( osg::TextureCubeMap::POSITIVE_X, imgRight );
	pTexCube->setImage( osg::TextureCubeMap::NEGATIVE_X, imgLeft );
	pTexCube->setImage( osg::TextureCubeMap::POSITIVE_Y, imgFront );
	pTexCube->setImage( osg::TextureCubeMap::NEGATIVE_Y, imgBack );
	pTexCube->setImage( osg::TextureCubeMap::POSITIVE_Z, imgBottom );
	pTexCube->setImage( osg::TextureCubeMap::NEGATIVE_Z, imgTop );

	osg::ComputeBoundsVisitor* tmp = new osg::ComputeBoundsVisitor;
	pRenderInstances->accept( *tmp );
	osg::BoundingBox bb = tmp->getBoundingBox();

	CSulGeomBox* pBox = new CSulGeomBox( (bb.xMax()-bb.xMin())/2.0f, (bb.yMax()-bb.yMin())/2.0f, (bb.zMax()-bb.zMin())/2.0f );
	pBox->zbuffer( false );
	pBox->getOrCreateStateSet()->setMode( GL_BLEND, osg::StateAttribute::ON );
	pBox->getOrCreateStateSet()->setTextureAttributeAndModes( 0, pTexCube, osg::StateAttribute::ON );
	pBox->getOrCreateStateSet()->setMode( GL_LIGHTING, osg::StateAttribute::OFF );
	rRoot->addChild( pBox );


	
	/*
	osg::ref_ptr<CSulScreenAlignedQuad> test = new CSulScreenAlignedQuad( osg::Vec3(w/2, h/2, 0), w, h, 800, 600 );
	test->setTexture( tex );
	rRoot->addChild( test->getProjection() );
	*/
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

	rRoot = new osg::Group;
 
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
	
	osg::Group* pRenderInstances = 0;

	bool bUseZDirectionNormal = true;

	if ( rXml->cells() )
	{
		osg::Vec2 cellXY = rXml->getCellXY();

		pRenderInstances = new CSulRenderCellInstances( cellXY, pImage, posCount, bb, minTree, maxTree, bSuppressTexture, texUnit, bUseZDirectionNormal, bSuppressShaders );
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

	///////////////////////////////////////////////////////////////
	// collision geometry
	///////////////////////////////////////////////////////////////

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

	///////////////////////////////////////////////////////////////
	// lod?
	//
	//	test by creating a cube of the boundingbox
	//	
	///////////////////////////////////////////////////////////////

	rViewer = new osgViewer::Viewer;
	rViewer->init();
	rViewer->setThreadingModel( osgViewer::ViewerBase::SingleThreaded );
	rViewer->setUpViewInWindow( 32, 32, 800, 600 );
	rViewer->setSceneData( rRoot );

	/*
	osg::Geode* pGeode = new osg::Geode();

	// we create the simplest form of shapes in OpenSceneGraph
    pGeode->addDrawable( new osg::ShapeDrawable( new osg::Sphere(osg::Vec3(-10.0f,-10.0f,0.0f),0.5f) ) );
    pGeode->addDrawable( new osg::ShapeDrawable( new osg::Box(osg::Vec3(-5.0f,-5.0f,0.0f),2.0f) ) );
    pGeode->addDrawable( new osg::ShapeDrawable( new osg::Cone(osg::Vec3(0.0f,0.0f,0.0f),0.5f,3.0f) ) );
    pGeode->addDrawable( new osg::ShapeDrawable( new osg::Cylinder(osg::Vec3(2.0f,2.0f,0.5f),0.5f,3.0f) ) );
    pGeode->addDrawable( new osg::ShapeDrawable( new osg::Capsule(osg::Vec3(5.0f,5.0f,1.0f),0.5f,3.0f) ) );
	rRoot->addChild( pGeode );

	test( pGeode );
	*/
	//test( pRenderInstances );


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

