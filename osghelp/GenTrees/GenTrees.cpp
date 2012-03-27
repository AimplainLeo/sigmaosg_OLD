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
#include <SigmaUtil/SulGeomBoundingBox.h>
#include <SigmaUtil/SulCrossQuad.h>
#include <SigmaUtil/SulBB.h>
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
#include <osg/depth>
#include <osgDB/ReadFile>
#include <osgDB/FileUtils>
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

osg::ref_ptr<osg::Image> createTextureFromOrthoView( osgViewer::Viewer* viewer, sigma::uint32 texW, sigma::uint32 texH, osg::Node* pRenderMe, ORTHOVIEW eOrthoView )
{
	osg::ref_ptr<CSulRTT> rtt = new CSulRTT( texW, texH, CSulRTT::SETUP_ORTHO_FRONT );
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

	return img;
}

void test( osg::Node* pTrees )
{
	float w = 512;
	float h = 256;

	osg::ref_ptr<osg::Image> imgTop = createTextureFromOrthoView( rViewer, w, h, pTrees, TOP );
	osg::ref_ptr<osg::Image> imgBottom = createTextureFromOrthoView( rViewer, w, h, pTrees, BOTTOM );
	osg::ref_ptr<osg::Image> imgLeft = createTextureFromOrthoView( rViewer, w, h, pTrees, LEFT );
	osg::ref_ptr<osg::Image> imgRight = createTextureFromOrthoView( rViewer, w, h, pTrees, RIGHT );
	osg::ref_ptr<osg::Image> imgFront = createTextureFromOrthoView( rViewer, w, h, pTrees, FRONT );
	osg::ref_ptr<osg::Image> imgBack = createTextureFromOrthoView( rViewer, w, h, pTrees, BACK );

	// now we need to create a cube and add textures
	osg::TextureCubeMap* pTexCube = new osg::TextureCubeMap;
	pTexCube->setImage( osg::TextureCubeMap::POSITIVE_X, imgRight );
	pTexCube->setImage( osg::TextureCubeMap::NEGATIVE_X, imgLeft );
	pTexCube->setImage( osg::TextureCubeMap::POSITIVE_Y, imgFront );
	pTexCube->setImage( osg::TextureCubeMap::NEGATIVE_Y, imgBack );
	pTexCube->setImage( osg::TextureCubeMap::POSITIVE_Z, imgBottom );
	pTexCube->setImage( osg::TextureCubeMap::NEGATIVE_Z, imgTop );

	osg::ComputeBoundsVisitor* tmp = new osg::ComputeBoundsVisitor;
	pTrees->accept( *tmp );
	osg::BoundingBox bb = tmp->getBoundingBox();

	CSulGeomBoundingBox* pBB = new CSulGeomBoundingBox( bb );
	rRoot->addChild( pBB );

	CSulGeomBox* pBox = new CSulGeomBox( bb );
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


osg::ref_ptr<osg::Node> createBB( osg::Node* pObj )
{
	osg::ComputeBoundsVisitor* tmp = new osg::ComputeBoundsVisitor;
	pObj->accept( *tmp );
	osg::BoundingBox bb = tmp->getBoundingBox();
	osg::ref_ptr<CSulGeomBoundingBox> pBB = new CSulGeomBoundingBox( bb );
	return pBB;
}

void test2( osg::Node* pRenderMe )
{
	/*
	CSulCrossQuad* p = new CSulCrossQuad( rViewer, pRenderMe, 3, 256 );
	rRoot->addChild( p );

	{
	osg::ComputeBoundsVisitor* tmp = new osg::ComputeBoundsVisitor;
	pRenderMe->accept( *tmp );
	osg::BoundingBox bb = tmp->getBoundingBox();
	rRoot->addChild( createBB(pRenderMe) );
	}
	*/
}

osg::ref_ptr<osg::Group> generateTrees( CParserXml* xml )
{
	osg::Image* pImagePositions		= xml->getGen()->getImage();						// this is the texture that contains the positions for the trees
	sigma::uint32 posCount			= xml->getGen()->getCount();						// number of positions in pImage
	osg::BoundingBox bb				= xml->getSceneTerrain()->getBoundingBoxWorld();
	float minTree					= xml->getMinTree();
	float maxTree					= xml->getMaxTree();
	bool bSuppressTexture			= xml->isTextureSuppressed();
	bool bSuppressShaders			= xml->getSuppressShaders();
	sigma::uint32 texUnit			= xml->getTexUnit();
	sigma::uint32 texSizeSquared	= xml->getGen()->getTexSizeSquared();
	sigma::uint32 useLights			= xml->getUseLights();

	osg::ref_ptr<osg::Group> group = new osg::Group;

	// create a temporary group with the tree shader (we use this to render the trees to texture)
	osg::ref_ptr<osg::Group> tmp = new osg::Group;
	osg::Program* program = xml->getProgram();
	if ( program )
	{
		tmp->getOrCreateStateSet()->setAttribute( program );
	}

/////////////
	if ( !bSuppressTexture )
	{
		// texture tree
		//std::string sFileTree = osgDB::findDataFile( "images/tree_aligned_256.png" );
		std::string sFileTree = osgDB::findDataFile( "images/trees.png" );
		if ( sFileTree.empty() )
		{
			std::cout << "WARNING: CSulRenderCellInstances::create can not find image " << sFileTree << std::endl;
		}
		else
		{
			osg::Image* pImageTree = osgDB::readImageFile( sFileTree );
			osg::Texture2D* pTex = new osg::Texture2D( pImageTree );
			pTex->setFilter(osg::Texture2D::MIN_FILTER, osg::Texture2D::LINEAR);
			pTex->setFilter(osg::Texture2D::MAG_FILTER, osg::Texture2D::LINEAR);
			pTex->setResizeNonPowerOfTwoHint(false);
			group->getOrCreateStateSet()->setTextureAttributeAndModes( texUnit, pTex, osg::StateAttribute::ON );
		}
	}

	osg::Uniform* u = new osg::Uniform( osg::Uniform::SAMPLER_2D, "tex" );
	u->set( (int)texUnit );
	group->getOrCreateStateSet()->addUniform( u );

////////////

	// texture of trees
	std::string sFileTree = osgDB::findDataFile( "images/trees.png" );
	if ( sFileTree.empty() )
	{
		std::cout << "WARNING: generateTrees: can not find image " << sFileTree << std::endl;
	}
	else
	{
		osg::Image* pImageTree = osgDB::readImageFile( sFileTree );
		osg::Texture2D* pTex = new osg::Texture2D( pImageTree );
		pTex->setFilter(osg::Texture2D::MIN_FILTER, osg::Texture2D::LINEAR);
		pTex->setFilter(osg::Texture2D::MAG_FILTER, osg::Texture2D::LINEAR);
		pTex->setResizeNonPowerOfTwoHint(false);
		group->getOrCreateStateSet()->setTextureAttributeAndModes( 0, pTex, osg::StateAttribute::ON );
		tmp->getOrCreateStateSet()->setTextureAttributeAndModes( 0, pTex, osg::StateAttribute::ON );
	}

	bb.zMax() += maxTree;

	bSuppressTexture = true;
	bool bUseZDirectionNormal = true;

	if ( xml->cells() )
	{
		// how many cells
		osg::Vec2 cellXY = xml->getCellXY();

		// create object to create cell instances
		osg::ref_ptr<CSulRenderCellInstances> cellInst = new CSulRenderCellInstances( cellXY, pImagePositions, posCount, bb, minTree, maxTree, bSuppressTexture, texUnit, bUseZDirectionNormal, bSuppressShaders );

		for ( sigma::uint32 y=0; y<cellXY.y(); y++ )
			for ( sigma::uint32 x=0; x<cellXY.x(); x++ )
			{
				// create a cell that render tree instances (1 drawable)
				osg::ref_ptr<osg::Geode> p = cellInst->createCrossQuadCell( x, y );

				osg::BoundingBox bbb = p->getDrawable(0)->getInitialBound();

				// create tmp shade (we need to make sure that the trees use the correct render)
				tmp->addChild( p );

				osg::ref_ptr<CSulCrossQuad> lod = new CSulCrossQuad( rViewer, tmp, &bbb, xml->getCellJson(), xml->getUseCellDebug(), texUnit );
				group->addChild( lod );
				
				lod->getOrCreateStateSet()->addUniform( new osg::Uniform( "use_tree_shader", 3 ) );
				lod->getOrCreateStateSet()->setMode( GL_BLEND, osg::StateAttribute::ON );

				osg::Depth* depth = new osg::Depth;
				depth->setWriteMask( false );
				lod->getOrCreateStateSet()->setAttributeAndModes( depth, osg::StateAttribute::ON );


				osg::ref_ptr<osg::LOD> lod_org = new osg::LOD;
				lod_org->addChild( p );
				lod_org->setRange( 0, xml->getLodDistMin(), xml->getLodDistMax() );
				group->addChild( lod_org );

				tmp->removeChild( p );
				
			}
	}
	else
	{
		osg::ref_ptr<CSulRenderInstances> inst = new CSulRenderInstances( pImagePositions, posCount, bb, minTree, maxTree, bSuppressTexture, texUnit, texSizeSquared, useLights, true, bSuppressShaders );
		group->addChild( inst );
		//pTrees->createBillboard();
		inst->createCrossQuad();
	}

	std::cout << "Number of trees: " << posCount << std::endl;

	return group;
}

int _tmain( int argc, char** argv )
{
	rRoot = new osg::Group;

	osg::ref_ptr<osg::Group> rTreeContainer = new osg::Group;

	rViewer = new osgViewer::Viewer;
	rViewer->init();
	rViewer->setThreadingModel( osgViewer::ViewerBase::SingleThreaded );
	rViewer->setUpViewInWindow( 32, 32, 800, 600 );
	rViewer->setSceneData( rRoot );


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

	rXml->getSceneTerrain()->getPat()->addChild( rTreeContainer );

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

	///////////////////////////////////////////////////////////////
	// add uniforms here
	///////////////////////////////////////////////////////////////

	const CParserXml::VEC_UNIFORMDATA& vecUniformDataList = rXml->getUniformDataList();
	osg::StateSet* ss = rTreeContainer->getOrCreateStateSet();
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
		rTreeContainer->getOrCreateStateSet()->setAttribute( program );
	}

	///////////////////////////////////////////////////////////////
	// generate trees
	///////////////////////////////////////////////////////////////

	osg::ref_ptr<osg::Node> pTrees = generateTrees( rXml );
	rTreeContainer->addChild( pTrees );

	///////////////////////////////////////////////////////////////
	// lod?
	//
	//	test by creating a cube of the boundingbox
	//	
	///////////////////////////////////////////////////////////////

/*
	osg::Geode* pGeode = new osg::Geode();

	// we create the simplest form of shapes in OpenSceneGraph
    pGeode->addDrawable( new osg::ShapeDrawable( new osg::Sphere(osg::Vec3(-10.0f,-10.0f,0.0f),0.5f) ) );
    pGeode->addDrawable( new osg::ShapeDrawable( new osg::Box(osg::Vec3(-5.0f,-5.0f,0.0f),2.0f) ) );
    pGeode->addDrawable( new osg::ShapeDrawable( new osg::Cone(osg::Vec3(0.0f,0.0f,0.0f),0.5f,3.0f) ) );
    pGeode->addDrawable( new osg::ShapeDrawable( new osg::Cylinder(osg::Vec3(2.0f,2.0f,0.0f),0.5f,3.0f) ) );
    pGeode->addDrawable( new osg::ShapeDrawable( new osg::Capsule(osg::Vec3(5.0f,5.0f,0.0f),0.5f,3.0f) ) );
	rRoot->addChild( pGeode );
*/
	//test( pGeode );

//	test2( pGeode );
	
	//test( pTrees );


	////////////////////////////////////////////
	// we should write the output file here
	////////////////////////////////////////////

	if ( !rXml->getOutputFile().empty() )
	{
		writeFile( pTrees, rGeomCol, rXml->getOutputFile(), rXml );

		/*
		osg::ref_ptr<osg::Group> pGroupSave = new osg::Group;
		pGroupSave->addChild( pTrees );
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

		if ( rXml->getUseSun() )
		{
			osg::Light *light = new osg::Light();

			// each light must have a unique number
			light->setLightNum( 3 );

			light->setDirection( osg::Vec3(1,0,0) );
			light->setDiffuse( osg::Vec4(1,0,0,1) );
			light->setSpecular( osg::Vec4(1.0, 1.0, 1.0, 1.0) );
			light->setAmbient( osg::Vec4(0.0, 0.0, 0.0, 1.0) );

			osg::LightSource * lightsource = new osg::LightSource();
			lightsource->setLight(light);
			rRoot->addChild( lightsource );
		}

		// setup an event handle for statistics
		rViewer->addEventHandler( new osgViewer::StatsHandler );

		// allow wireframe modes
		rViewer->addEventHandler( new osgGA::StateSetManipulator(rViewer->getCamera()->getOrCreateStateSet()) );

		rViewer->setCameraManipulator(new osgGA::TerrainManipulator);

		// work with best detailed lod
	//	rViewer->getCamera()->setLODScale( 0.0 );
	
		return rViewer->run();
	}

	return 0;
}

