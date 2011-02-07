// SulGrass.cpp

#include "stdafx.h"
#include "SulGrass.h"
#include "SulGeomBoundingBox.h"
#include <osg/geometry>
#include <osg/geode>
#include <osg/positionattitudetransform>
#include <osgDB/ReadFile>
#include <osgDB/FileUtils>
#include <iostream>

// len of grid is calculated by m_spacing*m_sizeGrid
//
// so if you want to calc the grid size it is done by
// doing m_sizeGrid = len/m_spacing;

CSulGrass::CSulGrass() :
m_sizeGrid( 40 ),
m_spacing( 0.2f ),
m_heightAdjust( 0.0f ),
m_windFactor( 1.0f ),
m_grassStretch( 0.1f ),
m_vPos( 0.0f, 0.0f, 0.0f ),
m_sTextureGrass( "Textures/grass2.tga" ),
m_sizeMultiplier( 1.0f ),
m_bShowBB( false ),
m_iTexUnit_HeightMap( 1 ),
m_bReceiveShadow( false )
{
}

void CSulGrass::setReceiveShadow( bool bReceiveShadow )
{
	m_bReceiveShadow = bReceiveShadow;
}

void CSulGrass::setTextureUnit_HeightMap( sigma::int32 i )
{
	m_iTexUnit_HeightMap = i;
}

void CSulGrass::setShowBB( bool bShow )
{
	m_bShowBB = bShow;
}

void CSulGrass::setSizeMultiplier( float sMul )
{
	m_sizeMultiplier = sMul;
}

void CSulGrass::setTextureFileName( const std::string& s )
{
	m_sTextureGrass = s;
}

void CSulGrass::setPosition( const osg::Vec3& pos )
{
	m_vPos = pos;
}

void CSulGrass::createGeometry( osg::Geometry& geom, int nInstances )
{
    const float halfDimX( 0.5f*m_sizeMultiplier );
    const float halfDimZ( 0.5f*m_sizeMultiplier );

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

osg::Shader* CSulGrass::getShaderVert()
{
	return m_shaderVert;
}

osg::Shader* CSulGrass::getShaderFrag()
{
	return m_shaderFrag;
}

void CSulGrass::createShader()
{
	/*
	m_shaderVert = new osg::Shader();
    m_shaderVert->setType( osg::Shader::VERTEX );
	std::string sFileVert = osgDB::findDataFile("shaders/grass.vert");
	if ( sFileVert.empty() )
	{
		std::cout << "WARNING: CSulGrass::createShader can not find grass.vert" << std::endl;
	}
	else
	{
	    if ( !m_shaderVert->loadShaderSourceFromFile( sFileVert ) )
		{
			std::cout << "WARNING: CSulGrass::createShader can not load " << sFileVert << std::endl;
		}
	}

	m_shaderFrag = new osg::Shader();
    m_shaderFrag->setType( osg::Shader::FRAGMENT );
	std::string sFileFrag = osgDB::findDataFile("shaders/grass.frag");
	if ( sFileFrag.empty() )
	{
		std::cout << "WARNING: CSulGrass::createShader can not find grass.frag" << std::endl;
	}
	else
	{
		if ( !m_shaderFrag->loadShaderSourceFromFile( sFileFrag ) )
		{
			std::cout << "WARNING: CSulGrass::createShader can not load " << sFileFrag << std::endl;
		}
	}

	osg::ref_ptr< osg::Program > program = new osg::Program();
    program->addShader( m_shaderVert );
	program->addShader( m_shaderFrag );
*/
	m_ss = m_geodeGrass->getOrCreateStateSet();
//	m_ss->setAttribute( program.get(), osg::StateAttribute::ON );

    uniformSizeGrid = new osg::Uniform( "cc",(int)m_sizeGrid );
    m_ss->addUniform( uniformSizeGrid );
	
	// height map
//	ss->setTextureAttributeAndModes( 1, pRTT->getTexture(), osg::StateAttribute::ON );
	uniformHeightMap = new osg::Uniform( "texHeightMap", m_iTexUnit_HeightMap );
    m_ss->addUniform( uniformHeightMap );

	uniformReceiveShadow = new osg::Uniform( "bReceiveShadow", m_bReceiveShadow );
	m_ss->addUniform( uniformReceiveShadow );

	// camera lock
//	uniformLock = new osg::Uniform( "bLock", false );
//    m_ss->addUniform( uniformLock );

	// spacing
	uniformSpacing = new osg::Uniform( "spacing", (float)m_spacing );
    m_ss->addUniform( uniformSpacing );

	// height adjust
	uniformHeightAdjust = new osg::Uniform( "heightAdjust", m_heightAdjust );
    m_ss->addUniform( uniformHeightAdjust );

	// wind factor
	uniformWindFactor = new osg::Uniform( "windFactor", m_windFactor );
    m_ss->addUniform( uniformWindFactor );

	// grass stretch
	uniformGrassStretch = new osg::Uniform( "grassStretch", m_grassStretch );
    m_ss->addUniform( uniformGrassStretch );

	// grass texture
	osg::Texture2D* pTex = new osg::Texture2D;
	std::string sFileGrass = osgDB::findDataFile( m_sTextureGrass );
	if ( sFileGrass.empty() )
	{
		std::cout << "WARNING: CSulGrass::createShader can not find image " << m_sTextureGrass << std::endl;
	}
	else
	{
		osg::Image* pImage = osgDB::readImageFile( sFileGrass );
		if ( !pImage )
		{
			std::cout << "ERROR: CSulGrass::createShader can not load image " << m_sTextureGrass << std::endl;
		}
		else
		{
			pTex->setImage( pImage );
		}
	}

    m_ss->setTextureAttributeAndModes( 0, pTex, osg::StateAttribute::ON );
	m_ss->setMode( GL_BLEND, osg::StateAttribute::ON );
//	m_ss->setMode( GL_LIGHTING, osg::StateAttribute::OFF );
//	m_ss->setRenderBinDetails( 9, "DepthSortedBin" );
	m_ss->setMode( GL_ALPHA_TEST, osg::StateAttribute::ON );
	m_ss->setAttribute( new osg::AlphaFunc(osg::AlphaFunc::GREATER, 0.9f), osg::StateAttribute::ON );
}

void CSulGrass::create()
{
	m_geodeGrass = new osg::Geode;

	// create grass geometry
	osg::Geometry* geom = new osg::Geometry;
	geom->setUseDisplayList( false );
    geom->setUseVertexBufferObjects( true );
	createGeometry( *geom, m_sizeGrid*m_sizeGrid );

	// adjust boundingbox
	float bufferLen = 4.0f;
	float len = (m_sizeGrid/2.0f)*m_spacing+bufferLen;
	m_bb = new osg::BoundingBox(-len, -len, m_vPos.z()-bufferLen, len, len, 1.0f*m_sizeMultiplier+bufferLen+m_vPos.z() );
    geom->setInitialBound( *m_bb );

	// add grass geometry to geometry
	m_geodeGrass->addDrawable( geom );

	createShader();

	m_rPat = new osg::PositionAttitudeTransform;
	m_rPat->setPosition( osg::Vec3( m_vPos.x(), m_vPos.y(), 0.0f ) );
	m_rPat->addChild( m_geodeGrass );

	if ( m_bShowBB )
	{
		// for debugging (show boundingbox)
		CSulGeomBoundingBox* p = new CSulGeomBoundingBox( *m_bb );
		m_rPat->addChild( p );
	}
}

osg::BoundingBox* CSulGrass::getBoundingBox()
{
	return m_bb;
}

void CSulGrass::setHeightTexture( osg::Texture2D* pTex )
{
	if ( m_ss.valid() )
	{
		m_ss->setTextureAttributeAndModes( m_iTexUnit_HeightMap, pTex, osg::StateAttribute::ON | osg::StateAttribute::PROTECTED );
	}
	else
	{
		osg::notify( osg::WARN ) << "WARNING: CSulGrass::setHeightTexture m_ss not valid!" << std::endl;
	}
}

void CSulGrass::setSpacing( float spacing )
{
	m_spacing = spacing;
}

void CSulGrass::setGridSize( sigma::uint32 size )
{
	m_sizeGrid = size/m_spacing;	
}

sigma::uint32 CSulGrass::getGridSize()
{
	return m_spacing*m_sizeGrid;
}

osg::PositionAttitudeTransform* CSulGrass::getPat()
{
	if ( !m_geodeGrass.valid() )
	{
		create();
	}

	return m_rPat;
}

osg::Geode* CSulGrass::getGeode()
{
	if ( !m_geodeGrass.valid() )
	{
		create();
	}

	return m_geodeGrass;
}