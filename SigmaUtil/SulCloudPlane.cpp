// SulCloudPlane2.cpp

#include "stdafx.h"
#include "SulCloudPlane.h"
#include <osgDB/FileUtils>
#include <iostream>

CSulCloudPlane::CSulCloudPlane() :
osg::Group()
{
	m_size = 256;
}

CSulCloudPlane::CSulCloudPlane( float size ) :
osg::Group()
{
	m_size = size;
}

osg::Program* CSulCloudPlane::createShaderProgram()
{
	osg::Program* program = new osg::Program();

	// need to add main vert and frag here!

	return program;
}

void CSulCloudPlane::create()
{
	m_rQuad = new CSulGeomQuad( m_size, m_size );
	addChild( m_rQuad );

	// create geom with has a osg::TexMat
	osg::ref_ptr<osg::TexMat> rTexMat = new osg::TexMat;
	m_rCloudScrollTexture= new CCloudScrollTexture( rTexMat, m_rQuad->getWidth() );
	m_rCloudScrollTexture->setUseDisplayList( false );
	osg::Geode* pGeodeCloudScrollTexture = new osg::Geode;
	pGeodeCloudScrollTexture->addDrawable( m_rCloudScrollTexture );
	addChild( m_rQuad );
	m_rQuad->create();
	
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
	m_rQuad->setWidth( s );
	m_rQuad->setHeight( s );

	if ( m_rCloudScrollTexture.valid() )
	{
		m_rCloudScrollTexture->setPlaneSize( s );
	}
}

void CSulCloudPlane::setWind( float x, float y )
{
	m_rCloudScrollTexture->setWind( x, y );
}

CSulGeomQuad* CSulCloudPlane::getQuad()
{
	return m_rQuad;
}
