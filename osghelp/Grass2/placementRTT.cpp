// placementRTT.cpp

#include "stdafx.h"
#include "placementRTT.h"
#include <SigmaUtil/SulNodeCallbackCameraSync.h>

CPlacementRTT::CPlacementRTT()
{
}

void CPlacementRTT::init( osg::Camera* pCam, osg::Node* pRenderMe )
{
	m_rRTT = new CSulRTT( 1024, 1024, GL_LUMINANCE, GL_FLOAT ); 
	m_rRTT->getTexture()->setInternalFormat( GL_LUMINANCE32F_ARB );
	m_rRTT->getTexture()->setSourceType( GL_FLOAT );
	m_rRTT->getTexture()->setFilter(osg::Texture2D::MIN_FILTER, osg::Texture2D::NEAREST);
	m_rRTT->getTexture()->setFilter(osg::Texture2D::MAG_FILTER, osg::Texture2D::NEAREST);

	m_rRTT->setClearColor( osg::Vec4(0,0,0,1) );
	m_rRTT->setUpdateCallback( new CSulNodeCallbackCameraSync( pCam ) );


	osg::Group* pGroupRenderMe = new osg::Group;
	m_rRTT->addChild( pGroupRenderMe );

	pGroupRenderMe->addChild( pRenderMe );
	getCamera()->addChild( pGroupRenderMe );
	osg::StateSet* ss = pGroupRenderMe->getOrCreateStateSet();

	osg::ref_ptr< osg::Shader > vertexShader = new osg::Shader();
    vertexShader->setType( osg::Shader::VERTEX );
    vertexShader->loadShaderSourceFromFile( "placement_render.vert" );

	osg::ref_ptr< osg::Shader > fragShader = new osg::Shader();
    fragShader->setType( osg::Shader::FRAGMENT );
    fragShader->loadShaderSourceFromFile( "placement_render.frag" );

	osg::ref_ptr< osg::Program > program = new osg::Program();
    program->addShader( vertexShader.get() );
	program->addShader( fragShader.get() );

	osg::Uniform* uniformInverseViewMatrix = new osg::Uniform( "viewInverseMatrixCam", m_rRTT->getInverseViewMatrix() );
    ss->addUniform( uniformInverseViewMatrix );

	ss->setAttribute( program, osg::StateAttribute::ON | osg::StateAttribute::PROTECTED );

}

osg::Camera* CPlacementRTT::getCamera()
{
	return m_rRTT;
}

osg::Texture2D*	CPlacementRTT::getTexture()
{
	return m_rRTT->getTexture();
}