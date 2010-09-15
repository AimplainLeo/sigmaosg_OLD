// SulShadow.cpp

#include "stdafx.h"
#include "SulShadow.h"

#define UNIT 1

CSulShadow::CSulShadow( osg::Group* pSceneRoot ) :
osg::TexGenNode(),
m_posLight( 100.0f, 100.0f, 100.0f ),
m_fileShaderFrag(""),
m_fileShaderVert(""),
m_size_squared( 1024 )
{
//	setStateSet( osg::StateAttribute::ON | osg::StateAttribute::PROTECTED );
	getStateSet()->setAttribute( 
		getStateSet()->getAttribute( osg::StateAttribute::TEXGEN ),
		osg::StateAttribute::ON | osg::StateAttribute::PROTECTED );
	
	

	m_rSceneRoot = pSceneRoot;

	m_rCasters = new osg::Group;

	setTextureUnit( 3 );
}

void CSulShadow::setTextureSize( sigma::uint32 size_squared )
{
	m_size_squared = size_squared;
}

void CSulShadow::useShaderFrag( const std::string& file )
{
	m_fileShaderFrag = file;
}

void CSulShadow::useShaderVert( const std::string& file )
{
	m_fileShaderVert = file;
}

void CSulShadow::setLightPosition( const osg::Vec3& pos )
{
	m_posLight = pos;

	if ( m_rST.valid() )
	{
		m_rST->setLight( m_posLight );
	}
}

void CSulShadow::addCaster( osg::Node* pNode )
{
	m_rCasters->addChild( pNode );
}

void CSulShadow::setShadowMapBounds( osg::Node* pNode )
{
	m_rShadowMapBounds = pNode;

	if ( m_rST.valid() )
	{
		m_rST->setShadowMapBounds( m_rShadowMapBounds );
	}
}

/*
static const char fragmentShaderSource_noBaseTexture[] = 
    "uniform sampler2DShadow osgShadow_shadowTexture; \n"
    "\n"
    "void main(void) \n"
    "{ \n"
    "    gl_FragColor = gl_Color * (shadow2DProj( osgShadow_shadowTexture, gl_TexCoord[1] )); \n"
    "}\n";
*/

void CSulShadow::init()
{
	// create a texture using the boundingbox of the render target object
	m_rST = new CSulShadowTexture( m_rCasters, this );
	m_rST->setShadowMapBounds( m_rShadowMapBounds );
	m_rST->setTextureSize( m_size_squared );
	m_rST->setLight( m_posLight );
	m_rST->init();
	m_rSceneRoot->addChild( m_rST->getNode() );

	osg::StateSet* ss = getOrCreateStateSet();
	ss->setTextureAttributeAndModes( UNIT, m_rST->getTexture(), osg::StateAttribute::ON | osg::StateAttribute::PROTECTED | osg::StateAttribute::OVERRIDE );
    ss->setTextureMode( UNIT, GL_TEXTURE_GEN_S, osg::StateAttribute::ON | osg::StateAttribute::PROTECTED | osg::StateAttribute::OVERRIDE );
    ss->setTextureMode( UNIT, GL_TEXTURE_GEN_T, osg::StateAttribute::ON | osg::StateAttribute::PROTECTED | osg::StateAttribute::OVERRIDE );
    ss->setTextureMode( UNIT, GL_TEXTURE_GEN_R, osg::StateAttribute::ON | osg::StateAttribute::PROTECTED | osg::StateAttribute::OVERRIDE );
    ss->setTextureMode( UNIT, GL_TEXTURE_GEN_Q, osg::StateAttribute::ON | osg::StateAttribute::PROTECTED | osg::StateAttribute::OVERRIDE );

	// some shader stuff
/*
	osg::Shader* fragment_shader = 0;
	osg::Shader* vert = 0;
	if ( m_fileShaderFrag.empty() )
	{
		fragment_shader = new osg::Shader( osg::Shader::FRAGMENT, fragmentShaderSource_noBaseTexture );
	}
	else
	{
		fragment_shader = new osg::Shader( osg::Shader::FRAGMENT );
		fragment_shader->loadShaderSourceFromFile( m_fileShaderFrag );
	}

	if ( !m_fileShaderVert.empty() )
	{
		vert = new osg::Shader( osg::Shader::VERTEX );
		vert->loadShaderSourceFromFile( m_fileShaderVert );
	}
	
    osg::Program* program = new osg::Program;
	program->addShader( fragment_shader );
	if ( vert )
	{
		program->addShader( vert );
	}
	ss->setAttribute( program );
*/
    osg::Uniform* shadowTextureSampler = new osg::Uniform("osgShadow_shadowTexture",(int)3);
    ss->addUniform( shadowTextureSampler );
}

osg::Texture2D* CSulShadow::getShadowMapTexture()
{
	return m_rST->getTexture();
}