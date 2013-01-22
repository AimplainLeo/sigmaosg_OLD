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

osg::Program* CSulCloudPlane::getShaderProgram()
{
	return m_prgShaders;
}

void CSulCloudPlane::create()
{
	// create geom with has a osg::TexMat
	osg::ref_ptr<osg::TexMat> rTexMat = new osg::TexMat;

	m_rQuad = new CSulGeomQuad( m_size, m_size );
	m_rQuad->create();
	m_rQuad->setUseDisplayList( false );
	m_rQuad->setDrawCallback( m_mytestass = new mytest( rTexMat, m_rQuad->getWidth() ) );
	addChild( new CSulGeode(m_rQuad) );

	m_prgShaders = createShaderProgram();
	m_prgShaders->setName( "CSulCloudPlane Shader Program" );

	bool bRet;

	osg::ref_ptr< osg::Shader > vertexShader = new osg::Shader();
    vertexShader->setType( osg::Shader::VERTEX );
    bRet = vertexShader->loadShaderSourceFromFile( osgDB::findDataFile("shaders/clouds.vert") );
	assert( bRet );

	osg::ref_ptr< osg::Shader > fragShader = new osg::Shader();
    fragShader->setType( osg::Shader::FRAGMENT );
    bRet = fragShader->loadShaderSourceFromFile( osgDB::findDataFile("shaders/clouds.frag") );
	assert( bRet );

    m_prgShaders->addShader( vertexShader.get() );
	m_prgShaders->addShader( fragShader.get() );

	osg::ref_ptr<osg::StateSet> ss = getOrCreateStateSet();
	ss->setAttribute( m_prgShaders, osg::StateAttribute::ON );
}

void CSulCloudPlane::setSize( float s )
{
	m_rQuad->setWidth( s );
	m_rQuad->setHeight( s );

	/*
	if ( m_rCloudScrollTexture.valid() )
	{
		m_rCloudScrollTexture->setPlaneSize( s );
	}
	*/

	if ( m_mytestass.valid() )
		m_mytestass->setPlaneSize( s );
}

void CSulCloudPlane::setWind( float x, float y )
{
	//m_rCloudScrollTexture->setWind( x, y );

	if ( m_mytestass.valid() )
		m_mytestass->setWind( x, y );
}

CSulGeomQuad* CSulCloudPlane::getQuad()
{
	return m_rQuad;
}
