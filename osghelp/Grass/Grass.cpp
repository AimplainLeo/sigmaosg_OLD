// Grass.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "camsync.h"
#include "terrain.h"
#include "heightRTT.h"
#include <SigmaUtil/SulGeomGrid.h>
#include <SigmaUtil/SulConsoleDisplay.h>
#include <SigmaUtil/SulStringList.h>
#include <SigmaUtil/SulRTT.h>
#include <SigmaUtil/SulTransScreenAlign.h>
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
CHeightRTT*						pHeightRTT = 0;
osg::ref_ptr< osg::Geometry >	geomGrass;
osg::Geode*						pGeodeGrass = 0;

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
	osg::ref_ptr<osg::Camera>	m_rCam;
	osg::ref_ptr<osg::Uniform>	m_rViewMatUniform;
};

class CMyConsoleClass : public osg::Referenced
{
public:
    CMyConsoleClass( CSulConsoleDisplay* pConsoleDisplay )
    {
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

			float f = sVal.GetFloat();
			ss->setAttribute( new osg::AlphaFunc(osg::AlphaFunc::GREATER, f), osg::StateAttribute::ON );
		}

		if ( sAction=="spacing" )
		{
			CSulString sVal = list.getString( 1 );

			sp = sVal.GetFloat();
			uniformSpacing->set( sp );		// grass shader (placement of grass next to each other)
			pHeightRTT->setOrtho( cc, sp );
		}

		if ( sAction=="heightadjust" )
		{
			CSulString sVal = list.getString( 1 );
			heightAdjust = sVal.GetFloat();
			uniformHeightAdjust->set( heightAdjust );
		}

		if ( sAction=="windfactor" )
		{
			CSulString sVal = list.getString( 1 );
			windFactor = sVal.GetFloat();
			uniformWindFactor->set( windFactor );
		}

		if ( sAction=="stretch" )
		{
			CSulString sVal = list.getString( 1 );
			grassStretch = sVal.GetFloat();
			uniformGrassStretch->set( grassStretch );
		}

		if ( sAction=="grid" )
		{
			CSulString sVal = list.getString( 1 );

			cc = sVal.GetFloat();

			pGeodeGrass->removeDrawable( geomGrass );
			geomGrass = createGrassGeom();
			pGeodeGrass->addDrawable( geomGrass );

			uniformCC->set( (int)cc );		// grass shader (placement of grass next to each other)
			pHeightRTT->setOrtho( cc, sp );
		}

		if ( sAction=="lock" )
		{
			CSulString sVal = list.getString( 1 );

			if ( sVal=="on" )
			{
				uniformLock->set( true );
				pHeightRTT->setLock( true );
			}

			if ( sVal=="off" )
			{
				uniformLock->set( false );
				pHeightRTT->setLock( false );
			}
		}
    }
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

    //osg::BoundingBox bb( -50., -.50, -50., 100., 100., 100. );
	osg::BoundingBox bb( -5., -.5, -5., 10., 10., 10. );
    geom->setInitialBound( bb );

	return geom;
}

osg::Node* CreateScene()
{
	osg::Group* pGroup = new osg::Group;

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

	osg::Group* pRenderMe = new osg::Group;

	osg::Geode* pGeodeShape = new osg::Geode;
	pGeodeShape->addDrawable( new osg::ShapeDrawable( new osg::Sphere(osg::Vec3(0.0f,0.0f,4.0f),1.0f) ) );
	pGroup->addChild( pGeodeShape );
	pRenderMe->addChild( rTerrain->getNode() );
	pGroup->addChild( pGeodeShape );


	pHeightRTT = new CHeightRTT;
	pHeightRTT->init( rViewer, rTextCam, rViewer->getCamera(), pGroup, pRenderMe );

	//////////////////////////
	// simple terrain part II
	//////////////////////////

	rTerrain->setCam( pHeightRTT->getCamera() );
	
	//////////////////////////
	// grass billboard stuff
	//////////////////////////

	pGeodeGrass = new osg::Geode();
	
	geomGrass = createGrassGeom();
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
	
	// camera lock
	uniformLock = new osg::Uniform( "bLock", false );
    ss->addUniform( uniformLock );

	// spacing
	uniformSpacing = new osg::Uniform( "spacing", (float)sp );
    ss->addUniform( uniformSpacing );

	// height map
	ss->setTextureAttributeAndModes( 1, pHeightRTT->getTexture(), osg::StateAttribute::ON );
	osg::Uniform* uniformHeightMap = new osg::Uniform( "texHeightMap", 1 );
    ss->addUniform( uniformHeightMap );

	// height adjust
	uniformHeightAdjust = new osg::Uniform( "heightAdjust", heightAdjust );
    ss->addUniform( uniformHeightAdjust );

	// wind factor
	uniformWindFactor = new osg::Uniform( "windFactor", windFactor );
    ss->addUniform( uniformWindFactor );

	// grass stretch
	uniformGrassStretch = new osg::Uniform( "grassStretch", grassStretch );
    ss->addUniform( uniformGrassStretch );

	// ortho camera inverse view matrix
	osg::Uniform* uniformOrthoInverseViewMatrix = new osg::Uniform( "orthoInverseViewMatrix", pHeightRTT->getCamera()->getInverseViewMatrix() );
    ss->addUniform( uniformOrthoInverseViewMatrix );
	pGeodeGrass->setUpdateCallback( new CMyUpdateCallbackGrass( pHeightRTT->getCamera(), uniformOrthoInverseViewMatrix ) );

	osg::Texture2D* pTex = new osg::Texture2D;
    osg::Image* pImage = osgDB::readImageFile( "grass2.tga" );
    pTex->setImage( pImage );
    ss->setTextureAttributeAndModes( 0, pTex, osg::StateAttribute::ON );
	ss->setMode( GL_BLEND, osg::StateAttribute::ON );
	ss->setMode( GL_LIGHTING, osg::StateAttribute::OFF );
	ss->setRenderBinDetails( 9, "DepthSortedBin" );

	ss->setMode( GL_ALPHA_TEST, osg::StateAttribute::ON );
	ss->setAttribute( new osg::AlphaFunc(osg::AlphaFunc::GREATER, 0.9f), osg::StateAttribute::ON );

	pGroup->addChild( pGeodeGrass );

	////////////////
	// a grid
	////////////////
	CSulGeomGrid* pGeomGrid = new CSulGeomGrid;
    pGeomGrid->Create( osg::Vec3(0,0,0), 10, 10, 1, 1, 5, 5 );

    osg::Geode* pGeode2 = new osg::Geode;
    pGeode2->addDrawable( pGeomGrid->getGeometry() );

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
    osg::ref_ptr<CSulConsoleDisplay> rConsoleDisplay = new CSulConsoleDisplay( rViewer.get() );
    rConsoleDisplay->Init();

	osg::Group* pGroup = new osg::Group;
    
	pGroup->addChild( CreateScene() );

	pGroup->addChild( rConsoleDisplay->GetNode() );

	// create object to receive console commands
    osg::ref_ptr<CMyConsoleClass> rTest = new CMyConsoleClass( rConsoleDisplay.get() );

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

