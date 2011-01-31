// Grass2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "camsync.h"
#include "terrain.h"
#include "heightRTT.h"
#include "placementRTT.h"
#include "grass_transformation.h"
#include "generate_tex.h"
#include <SigmaUtil/SulGeomBoundingBox.h>
#include <SigmaUtil/SulGeomGrid.h>
#include <SigmaUtil/SulConsoleDisplay.h>
#include <SigmaUtil/SulStringList.h>
#include <SigmaUtil/SulRTT.h>
#include <SigmaUtil/SulTransScreenAlign.h>
#include <SigmaUtil/SulScreenAlignedQuad.h>
#include <osgViewer/Viewer>
#include <osg/ShapeDrawable>
#include <osg/Geometry>
#include <osg/texture2d>
#include <osg/AlphaFunc>
#include <osgViewer/ViewerEventHandlers>
#include <osgDB/ReadFile>

#include <osgGA/TrackballManipulator>
#include <osgGA/FlightManipulator>
#include <osgGA/DriveManipulator>
#include <osgGA/KeySwitchMatrixManipulator>
#include <osgGA/StateSetManipulator>
#include <osgGA/AnimationPathManipulator>
#include <osgGA/TerrainManipulator>

#include <assert.h>
#include <iostream>

float cc			= 40.0f;		// grid cells cc*cc
float sp			= 0.2f;			// spacing
float heightAdjust	= 0.0f;
float windFactor	= 1.0f;
float grassStretch	= 0.1f;

// stateset for grass (FIXME: give better name for this variable)
osg::ref_ptr<osg::StateSet> ss;
osg::ref_ptr<osgViewer::Viewer> rViewer;
osg::ref_ptr<osgText::Text>		rTextCam;
osg::ref_ptr<osg::Uniform>		uniformSpacing;
osg::ref_ptr<osg::Uniform>		uniformCC;
osg::ref_ptr<osg::Uniform>		uniformLock;
osg::ref_ptr<osg::Uniform>		uniformHeightAdjust;
osg::ref_ptr<osg::Uniform>		uniformWindFactor;
osg::ref_ptr<osg::Uniform>		uniformGrassStretch;
osg::ref_ptr<osg::Uniform>		uniformHeightMap;
osg::ref_ptr< osg::Geometry >	geomGrass;
osg::Geode*						pGeodeGrass = 0;
osg::Group*						pRenderMe = 0;
osg::Group*						pRootGroup = 0;

// forward declaration prototype
osg::Geometry* createGrassGeom();

class CMyUpdateCallbackGrass : public osg::NodeCallback
{
public:
	CMyUpdateCallbackGrass( osg::Camera* pCam, osg::Uniform* uniformViewMat )
	{
		m_rCam = pCam;
		m_rViewMatUniform = uniformViewMat;
	}

    void operator()( osg::Node* node, osg::NodeVisitor* nv )
    {
		m_rViewMatUniform->set( m_rCam->getInverseViewMatrix() );

		// traverse subtree
        traverse(node,nv);
    }

private:
	osg::ref_ptr<osg::Camera>			m_rCam;
	osg::ref_ptr<osg::Uniform>			m_rViewMatUniform;
};

class CMyConsoleClass : public osg::Referenced
{
public:
    CMyConsoleClass( CSulConsoleDisplay* pConsoleDisplay, osg::Group* pGroup )
    {
		m_rGroup = pGroup;

        // use a functor to get call back for commands posted in console
        CSulConsoleFunctor<CMyConsoleClass>* pConsoleFunctor = new CSulConsoleFunctor<CMyConsoleClass>(this, &CMyConsoleClass::Command);
        pConsoleDisplay->WantCommand( pConsoleFunctor );
    }

    void Command( const CSulString& sText )
    {
		CSulStringList list(sText);

		CSulString sAction = list.getString( 0 );

		if ( sAction=="alphatest" )
		{
			CSulString sVal = list.getString( 1 );

			float f = sVal.asFloat();
			ss->setAttribute( new osg::AlphaFunc(osg::AlphaFunc::GREATER, f), osg::StateAttribute::ON );
		}

		if ( sAction=="spacing" )
		{
			CSulString sVal = list.getString( 1 );

			sp = sVal.asFloat();
			uniformSpacing->set( sp );		// grass shader (placement of grass next to each other)
		}

		if ( sAction=="heightadjust" )
		{
			CSulString sVal = list.getString( 1 );
			heightAdjust = sVal.asFloat();
			uniformHeightAdjust->set( heightAdjust );
		}

		if ( sAction=="windfactor" )
		{
			CSulString sVal = list.getString( 1 );
			windFactor = sVal.asFloat();
			uniformWindFactor->set( windFactor );
		}

		if ( sAction=="stretch" )
		{
			CSulString sVal = list.getString( 1 );
			grassStretch = sVal.asFloat();
			uniformGrassStretch->set( grassStretch );
		}

		if ( sAction=="grid" )
		{
			CSulString sVal = list.getString( 1 );

			cc = sVal.asFloat();

			pGeodeGrass->removeDrawable( geomGrass );
			geomGrass = createGrassGeom();
			pGeodeGrass->addDrawable( geomGrass );

			uniformCC->set( (int)cc );		// grass shader (placement of grass next to each other)
		}

		if ( sAction=="lock" )
		{
			CSulString sVal = list.getString( 1 );

			if ( sVal=="on" )
			{
				uniformLock->set( true );
			}

			if ( sVal=="off" )
			{
				uniformLock->set( false );
			}
		}

		if ( sAction=="generate" || sAction=="g" )
		{
			CGenerateTex* pGen = new CGenerateTex;
			pGen->create(
					pRenderMe,
					pRootGroup,
					0, 0,
					1024, 
					8
				);
			
			// lets see the result of the image
			CSulScreenAlignedQuad* quad = new CSulScreenAlignedQuad( osg::Vec3(64+256,64,0), 128, 128, 800, 600 );
			quad->setTexture( pGen->getTexture() );
			m_rGroup->addChild( quad->getProjection() );
		}

		if ( sAction=="l" )
		{
			osg::Image* pImage = osgDB::readImageFile( "myass.tif" );
			if ( !pImage )
			{
				std::cout << "readImageFile FAILED" << std::endl;
				return;
			}

			pImage->setInternalTextureFormat( GL_LUMINANCE32F_ARB );

			osg::Texture2D* pTex = new osg::Texture2D;
			pTex->setImage( pImage );

			// lets see the result of the image
			CSulScreenAlignedQuad* quad = new CSulScreenAlignedQuad( osg::Vec3(64+256+256,64,0), 128, 128, 800, 600 );
			quad->setTexture( pTex );
			m_rGroup->addChild( quad->getProjection() );
			
			// apply it
			ss->setTextureAttributeAndModes( 1, pTex, osg::StateAttribute::ON );
		}


/*
		if ( sAction=="l" )
		{
			float* pData = new float[1024*1024];
			// load raw data
			FILE* file = fopen( "myass.raw", "r+" );
			assert( file );
			fread( pData, 1024*1024*4, 1, file );
			fclose( file );

			// create image/texture
			osg::Image* pImage = new osg::Image;
			pImage->setImage( 
				1024, 1024, 1,
				GL_LUMINANCE32F_ARB,
				GL_LUMINANCE, GL_FLOAT,
				(unsigned char*)pData,
				osg::Image::USE_NEW_DELETE
			);

			pImage->setInternalTextureFormat( GL_LUMINANCE32F_ARB );

			osg::Texture2D* pTex = new osg::Texture2D;
			pTex->setImage( pImage );

			// lets see the result of the image
			CSulScreenAlignedQuad* quad = new CSulScreenAlignedQuad( osg::Vec3(64+256+256,64,0), 128, 128, 800, 600 );
			quad->setTexture( pTex );
			m_rGroup->addChild( quad->getProjection() );
			
			// apply it
			ss->setTextureAttributeAndModes( 1, pTex, osg::StateAttribute::ON );
		}
*/
    }

private:
	osg::ref_ptr<osg::Group>	m_rGroup;
};

osg::Camera* createDebugText()
{
return 0;

	//////////////////////////
	// debug text
	//////////////////////////

	// create text and add it to our geometry node
    rTextCam = new  osgText::Text;
	rTextCam->setFont("tempest.ttf");

	osg::Geode* pGeodeText = new osg::Geode;
    pGeodeText->addDrawable( rTextCam );

    // create an ortho projection so our text is screen aligned
    osg::Camera* camera = new osg::Camera;
    camera->setProjectionMatrix( osg::Matrix::ortho2D(0,800,0,600) );
	camera->setViewMatrix(osg::Matrix::identity());
    camera->setReferenceFrame( osg::Transform::ABSOLUTE_RF );
    camera->setViewMatrix( osg::Matrix::identity() );
	camera->setAllowEventFocus(false);
	camera->setClearMask(GL_DEPTH_BUFFER_BIT);
	camera->setRenderOrder(osg::Camera::POST_RENDER);
    camera->addChild( pGeodeText );

    // set some text data
    rTextCam->setPosition( osg::Vec3( 10.f, 400.0f, 0.0f ) );
    rTextCam->setColor( osg::Vec4( 1.0f, 1.0f, 0.0f, 1.0f ) );
    rTextCam->setText( "Hello World" );
	rTextCam->setDataVariance( osg::Object::DYNAMIC );

	rTextCam->getOrCreateStateSet()->setMode( GL_LIGHTING, osg::StateAttribute::OFF );

	return camera;
}

void createDAIGeometry( osg::Geometry& geom, int nInstances=1 )
{
    const float halfDimX( .5 );
    const float halfDimZ( .5 );

    osg::Vec3Array* v = new osg::Vec3Array;
    v->resize( 4 );
    geom.setVertexArray( v );

    // Geometry for a single quad.
    (*v)[ 0 ] = osg::Vec3( -halfDimX, 0., 0. );
    (*v)[ 1 ] = osg::Vec3( halfDimX, 0., 0. );
    (*v)[ 2 ] = osg::Vec3( halfDimX, 0., halfDimZ*2.0f );
    (*v)[ 3 ] = osg::Vec3( -halfDimX, 0., halfDimZ*2.0f );


	// create color array data (each corner of our triangle will have one color component)
    osg::Vec4Array* pColors = new osg::Vec4Array;
    pColors->push_back( osg::Vec4( 1.0f, 0.0f, 0.0f, 1.0f ) );
    pColors->push_back( osg::Vec4( 0.0f, 1.0f, 0.0f, 1.0f ) );
    pColors->push_back( osg::Vec4( 0.0f, 0.0f, 1.0f, 1.0f ) );
	pColors->push_back( osg::Vec4( 0.0f, 0.0f, 1.0f, 1.0f ) );
    geom.setColorArray( pColors );

	// make sure that our geometry is using one color per vertex
    geom.setColorBinding( osg::Geometry::BIND_PER_VERTEX );

    osg::Vec2Array* pTexCoords = new osg::Vec2Array( 4 );
    (*pTexCoords)[0].set( 0.0f, 0.0f );
    (*pTexCoords)[1].set( 1.0f, 0.0f );
    (*pTexCoords)[2].set( 1.0f, 1.0f );
    (*pTexCoords)[3].set( 0.0f, 1.0f );
    geom.setTexCoordArray( 0, pTexCoords );

    // Use the DrawArraysInstanced PrimitiveSet and tell it to draw 1024 instances.
    geom.addPrimitiveSet( new osg::DrawArrays( GL_QUADS, 0, 4, nInstances ) );
}

osg::Geometry* createGrassGeom()
{
	osg::Geometry* geom = new osg::Geometry;
	geom->setUseDisplayList( false );
    geom->setUseVertexBufferObjects( true );
	createDAIGeometry( *geom, cc*cc );

	return geom;
}

osg::Node* CreateScene()
{
	osg::Group* pGroup = new osg::Group;
	pRootGroup = pGroup;
	pGroup->setDataVariance( osg::Object::DYNAMIC );

	pGroup->addChild( createDebugText() );

	//////////////////////////
	// simple terrain part I
	//////////////////////////

	osg::ref_ptr<CTerrain>	rTerrain = new CTerrain;
	rTerrain->Init();
	pGroup->addChild( rTerrain->getNodeWithShader() );

	//////////////////////////
	// RTT terrain
	//////////////////////////

	pRenderMe = new osg::Group;
	pRenderMe->addChild( rTerrain->getNode() );

	CHeightRTT* pRTT = new CHeightRTT;
	pRTT->init( rViewer, 0, rViewer->getCamera(), pGroup, pRenderMe );
	
	/*
	CPlacementRTT* pPlacementRTT = new CPlacementRTT;
	pPlacementRTT->init( rViewer->getCamera(), pRenderMe );
	pGroup->addChild( pPlacementRTT->getCamera() );
	*/

	// quad 
	osg::ref_ptr<CSulGeomQuad> rQuad = new CSulGeomQuad( osg::Vec3(64,64,0), 128, 128 );
	osg::ref_ptr<CSulTransScreenAlign> rAlign = new CSulTransScreenAlign( 800, 600 );
	osg::Geode* pGeodeQuadScreen = new osg::Geode;
	pGeodeQuadScreen->addDrawable( rQuad->getDrawable() );
	rAlign->AddChild( pGeodeQuadScreen );
	pGroup->addChild( rAlign->GetProjection() );
	rQuad->setTexture( pRTT->getTexture() );
	rQuad->getDrawable()->getOrCreateStateSet()->setMode( GL_DEPTH_TEST, osg::StateAttribute::OFF | osg::StateAttribute::PROTECTED | osg::StateAttribute::OVERRIDE );

	//////////////////////////
	// simple terrain part II
	//////////////////////////

	rTerrain->setCam( pRTT->getCamera() );

	//////////////////////////
	// grass billboard stuff
	//////////////////////////

	pGeodeGrass = new osg::Geode();
	
	geomGrass = createGrassGeom();

	float len = (cc/2.0f)*sp;
	osg::BoundingBox bb( -len, -len, 0.0f, len, len, 1.0f );
    geomGrass->setInitialBound( bb );
	pGeodeGrass->addDrawable( geomGrass );

	osg::ref_ptr< osg::Shader > vertexShader = new osg::Shader();
    vertexShader->setType( osg::Shader::VERTEX );
    vertexShader->loadShaderSourceFromFile( "grass.vert" );

	osg::ref_ptr< osg::Shader > fragShader = new osg::Shader();
    fragShader->setType( osg::Shader::FRAGMENT );
    fragShader->loadShaderSourceFromFile( "grass.frag" );

	osg::ref_ptr< osg::Program > program = new osg::Program();
    program->addShader( vertexShader.get() );
	program->addShader( fragShader.get() );

	ss = pGeodeGrass->getOrCreateStateSet();
	ss->setAttribute( program.get(), osg::StateAttribute::ON | osg::StateAttribute::PROTECTED );

    uniformCC = new osg::Uniform("cc",(int)cc);
    ss->addUniform( uniformCC );
	
	// height map
//	ss->setTextureAttributeAndModes( 1, pRTT->getTexture(), osg::StateAttribute::ON );
	uniformHeightMap = new osg::Uniform( "texHeightMap", 1 );
    ss->addUniform( uniformHeightMap );

	// camera lock
	uniformLock = new osg::Uniform( "bLock", false );
    ss->addUniform( uniformLock );

	// spacing
	uniformSpacing = new osg::Uniform( "spacing", (float)sp );
    ss->addUniform( uniformSpacing );

	// height adjust
	uniformHeightAdjust = new osg::Uniform( "heightAdjust", heightAdjust );
    ss->addUniform( uniformHeightAdjust );

	// wind factor
	uniformWindFactor = new osg::Uniform( "windFactor", windFactor );
    ss->addUniform( uniformWindFactor );

	// grass stretch
	uniformGrassStretch = new osg::Uniform( "grassStretch", grassStretch );
    ss->addUniform( uniformGrassStretch );

	osg::Texture2D* pTex = new osg::Texture2D;
    osg::Image* pImage = osgDB::readImageFile( "grass2.tga" );
    pTex->setImage( pImage );
    ss->setTextureAttributeAndModes( 0, pTex, osg::StateAttribute::ON );
	ss->setMode( GL_BLEND, osg::StateAttribute::ON );
	ss->setMode( GL_LIGHTING, osg::StateAttribute::OFF );
	ss->setRenderBinDetails( 9, "DepthSortedBin" );

	ss->setMode( GL_ALPHA_TEST, osg::StateAttribute::ON );
	ss->setAttribute( new osg::AlphaFunc(osg::AlphaFunc::GREATER, 0.9f), osg::StateAttribute::ON );


	// NEW: create transform
	osg::ref_ptr<CGrassTransformation> rPat = new CGrassTransformation( rViewer );
    rPat->setPosition( osg::Vec3(0, 0, 0) );
    rPat->addChild( pGeodeGrass );

	// boundingbox 
	osg::Geode* pGeodeBB = new osg::Geode;
	CSulGeomBoundingBox* pBB = new CSulGeomBoundingBox( bb );
	pGeodeBB->addDrawable( pBB->getDrawable() );
	rPat->addChild( pGeodeBB );

	osg::Geode* pGeodeShape = new osg::Geode;
	pGeodeShape->addDrawable( new osg::ShapeDrawable( new osg::Sphere(osg::Vec3(0.0f,0.0f,4.0f),1.0f) ) );
	rPat->addChild( pGeodeShape );



	pGroup->addChild( rPat );

	////////////////
	// a grid
	////////////////
	CSulGeomGrid* pGeomGrid = new CSulGeomGrid;
    pGeomGrid->Create( osg::Vec3(0,0,0), 10, 10, 1, 1, 5, 5 );

    osg::Geode* pGeode2 = new osg::Geode;
    pGeode2->addDrawable( pGeomGrid->getDrawable() );

	pGroup->addChild( pGeode2 );

	// what does this exactly do? (you would think it would disable culling,.. but it doesn't!)
	// think it culls when an object is a certain pixel size on the screen
	pGroup->setCullingActive( false );

	return pGroup;
}

int _tmain(int argc, _TCHAR* argv[])
{
    // construct the viewer
    rViewer = new osgViewer::Viewer;

//rViewer->setThreadingModel( osgViewer::ViewerBase::SingleThreaded );

    // set up the camera manipulators.
    {
        osg::ref_ptr<osgGA::KeySwitchMatrixManipulator> keyswitchManipulator = new osgGA::KeySwitchMatrixManipulator;

        keyswitchManipulator->addMatrixManipulator( '1', "Trackball", new osgGA::TrackballManipulator() );
        keyswitchManipulator->addMatrixManipulator( '2', "Flight", new osgGA::FlightManipulator() );
        keyswitchManipulator->addMatrixManipulator( '3', "Drive", new osgGA::DriveManipulator() );
        keyswitchManipulator->addMatrixManipulator( '4', "Terrain", new osgGA::TerrainManipulator() );

        rViewer->setCameraManipulator( keyswitchManipulator.get() );
    }

    // make the viewer create a 800x600 window and position it at 32, 32
    rViewer->setUpViewInWindow( 32, 32, 800, 600 );

    // create a console
    osg::ref_ptr<CSulConsoleDisplay> rConsoleDisplay = new CSulConsoleDisplay( rViewer );
    rConsoleDisplay->Init();

	osg::Group* pGroup = new osg::Group;
    
	pGroup->addChild( CreateScene() );

	pGroup->addChild( rConsoleDisplay->GetNode() );

	// create object to receive console commands
    osg::ref_ptr<CMyConsoleClass> rTest = new CMyConsoleClass( rConsoleDisplay, pGroup );

	// FIXME: grass still gets culled
	rViewer->getCamera()->setCullingMode( osg::CullSettings::NO_CULLING );

	////////////////////////////////////////////////////////////////////////////////	
    // set the scene-graph data the viewer will render
	////////////////////////////////////////////////////////////////////////////////

    rViewer->setSceneData( pGroup );

	// setup an event handle for statistics
    rViewer->addEventHandler( new osgViewer::StatsHandler );

    // execute main loop
    return rViewer->run();
}

