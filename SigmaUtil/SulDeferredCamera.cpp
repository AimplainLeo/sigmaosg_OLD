// SulDeferredCamera.cpp

#include "stdafx.h"
#include "SulDeferredCamera.h"
#include "SulProgramShaders.h"
#include <osg/depth>

CSulDeferredCamera::CSulDeferredCamera( CSulLightManager* lightManager, sigma::uint32 w, sigma::uint32 h ) :
CSulTexCam( w, h, CSulTexCam::DEFERRED_RENDERING )
{
	m_lm = lightManager;

	// first pass (create g buffers)
//	getOrCreateStateSet()->setAttribute( new CSulProgramShaders( "shaders/sulDeferredRendering.frag,shaders/sulDeferredRendering.vert" ) );

	// second pass (composite)
	m_comp = new CSulTexCam( w, h, CSulTexCam::ORTHO );
	m_comp->setRenderOrder( osg::Camera::POST_RENDER );

	m_quad = new CSulGeomQuad( 2, 2 );
	m_quad->getGeometry()->setName( "Composite Quad" );

	m_quad->setTexture( getTexture(0), 0, "texPositions" );
	m_quad->setTexture( getTexture(1), 1, "texNormals" );
	m_quad->setTexture( getTexture(2), 2, "texColors" );
	m_quad->getOrCreateStateSet()->addUniform( m_lm->getUniformTexLightData() );
	m_quad->getOrCreateStateSet()->addUniform( m_lm->getUniformCountLightPoints() );
	m_quad->getOrCreateStateSet()->addUniform( m_lm->getUniformCountLightSpots() );
	m_comp->addChild( m_quad );

/*
		osg::Depth* depth = new osg::Depth;
		depth->setWriteMask( false );
		m_quad->getGeometry()->getOrCreateStateSet()->setAttributeAndModes( depth, osg::StateAttribute::ON );
		m_quad->getGeometry()->getOrCreateStateSet()->setMode( GL_DEPTH_TEST, osg::StateAttribute::OFF );
*/
//		m_comp->setClearMask( GL_COLOR_BUFFER_BIT );
		//setClearMask( GL_COLOR_BUFFER_BIT );

		/*
		osg::Depth* depth2 = new osg::Depth;
		depth2->setWriteMask( false );
		getOrCreateStateSet()->setAttributeAndModes( depth2, osg::StateAttribute::ON );
		getOrCreateStateSet()->setMode( GL_DEPTH_TEST, osg::StateAttribute::OFF );
		*/


	/*
	m_comp->getOrCreateStateSet()->setAttribute( new CSulProgramShaders( 
		"mycomposite.frag,"
		"mycomposite.vert,"
		"shaders/sulFuncLightPoint.frag"
	) );
	*/
}

void CSulDeferredCamera::traverse( osg::NodeVisitor& nv )
{
	// update g buffers
	CSulTexCam::traverse( nv );		

	if ( nv.getVisitorType()==osg::NodeVisitor::CULL_VISITOR )
	{
		// if we get to here.. then it is because the light is visible in the scene for this camera
		osgUtil::CullVisitor* cv = dynamic_cast<osgUtil::CullVisitor*>(&nv);

		osg::ref_ptr<osg::StateSet> ss = new osg::StateSet;
		ss->setTextureAttributeAndModes( 7, m_lm->getTextureData(), osg::StateAttribute::ON | osg::StateAttribute::PROTECTED );
		cv->pushStateSet( ss );

		m_comp->accept( nv );

		cv->popStateSet();
	}
}

osg::Texture2D* CSulDeferredCamera::getFinalTexture()
{
	return m_comp->getTexture();
}

CSulTexCam* CSulDeferredCamera::getCompositeCamera()
{
	return m_comp;
}