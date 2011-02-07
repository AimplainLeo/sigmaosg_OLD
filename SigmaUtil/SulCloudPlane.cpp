// SulCloudPlane2.cpp

#include "stdafx.h"
#include "SulCloudPlane.h"
#include <osgDB/FileUtils>
#include <iostream>

CSulCloudPlane::CSulCloudPlane() :
CSulGeomQuad( 256, 256 )
{
}

CSulCloudPlane::CSulCloudPlane( float size ) :
CSulGeomQuad( size, size )
{
}

osg::Program* CSulCloudPlane::createShaderProgram()
{
	osg::Program* program = new osg::Program();

	// need to add main vert and frag here!

	return program;
}

void CSulCloudPlane::createDrawable()
{
	// create geom with has a osg::TexMat
	osg::ref_ptr<osg::TexMat> rTexMat = new osg::TexMat;
	m_rCloudScrollTexture= new CCloudScrollTexture( rTexMat, getWidth() );
	m_rCloudScrollTexture->setUseDisplayList( false );
	addDrawable( m_rCloudScrollTexture );
	create();
	
	osg::Program* pShaderProgram = createShaderProgram();

	bool bRet;

	osg::ref_ptr< osg::Shader > vertexShader = new osg::Shader();
    vertexShader->setType( osg::Shader::VERTEX );
    bRet = vertexShader->loadShaderSourceFromFile( osgDB::findDataFile("shaders/clouds.vert") );
	assert( bRet );

	osg::ref_ptr< osg::Shader > fragShader = new osg::Shader();
    fragShader->setType( osg::Shader::FRAGMENT );
    bRet = fragShader->loadShaderSourceFromFile( osgDB::findDataFile("shaders/clouds.frag") );
	assert( bRet );

    pShaderProgram->addShader( vertexShader.get() );
	pShaderProgram->addShader( fragShader.get() );

	osg::ref_ptr<osg::StateSet> ss = getOrCreateStateSet();
	ss->setAttribute( pShaderProgram, osg::StateAttribute::ON );
}

void CSulCloudPlane::setSize( float s )
{
	setWidth( s );
	setHeight( s );

	if ( m_rCloudScrollTexture.valid() )
	{
		m_rCloudScrollTexture->setPlaneSize( s );
	}
}

void CSulCloudPlane::setWind( float x, float y )
{
	m_rCloudScrollTexture->setWind( x, y );
}