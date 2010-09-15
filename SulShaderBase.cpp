// SulShaderBase.cpp

#include "stdafx.h"
#include "SulShaderBase.h"
#include <osgDB/FileUtils>

CSulShaderBase::CSulShaderBase( osg::Node* pNode, osg::Program* pProg ) :
m_bEnable( false )
{
	// create shader program
	m_rProg = pProg ? pProg : new osg::Program;
	m_rProgDefault	= new osg::Program;

	m_rSS = pNode->getOrCreateStateSet();

	enable();
}

void CSulShaderBase::enable( bool bEnable )
{
	// check for same state
	if ( bEnable==m_bEnable )
	{
		return;
	}

	m_bEnable = bEnable;

	if ( m_rSS.valid() )
	{
		if ( m_bEnable )
		{
			m_rSS->setAttributeAndModes( m_rProgDefault.get(), osg::StateAttribute::OFF );
			m_rSS->setAttributeAndModes( m_rProg.get(), osg::StateAttribute::ON );    
		}
		else
		{
			m_rSS->setAttributeAndModes( m_rProg.get(), osg::StateAttribute::OFF );
			m_rSS->setAttributeAndModes( m_rProgDefault.get(), osg::StateAttribute::ON );
		}
	}
}

void CSulShaderBase::addShaderFrag( const std::string& file )
{
	// vertex shader to use
	osg::ref_ptr<osg::Shader> f = new osg::Shader( osg::Shader::FRAGMENT );
	std::string fileFrag = osgDB::findDataFile( file );
	if ( fileFrag.empty() )
	{
		osg::notify(osg::WARN) << "ERROR: CSulShaderBase -> " << file << " not found." << std::endl;
	}
	else
	{
		f->loadShaderSourceFromFile( fileFrag );
	}

	m_rProg->addShader( f );
}

void CSulShaderBase::addShaderVert( const std::string& file )
{
	// vertex shader to use
	osg::ref_ptr<osg::Shader> v = new osg::Shader( osg::Shader::VERTEX );
	std::string fileVert = osgDB::findDataFile( file );
	if ( fileVert.empty() )
	{
		osg::notify(osg::WARN) << "ERROR: CSulShaderBase -> " << file << " not found." << std::endl;
	}
	else
	{
		v->loadShaderSourceFromFile( fileVert );
	}

	m_rProg->addShader( v );
}