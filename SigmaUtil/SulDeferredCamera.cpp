// SulDeferredCamera.cpp

#include "stdafx.h"
#include "SulDeferredCamera.h"
#include "SulProgramShaders.h"
#include <osg/depth>

CSulDeferredCamera::CSulDeferredCamera( CSulLightManager* lightManager, sigma::uint32 w, sigma::uint32 h ) :
CSulTexCam( w, h, CSulTexCam::DEFERRED_RENDERING )
{
	m_lm = lightManager;

//	m_gamma = 1.0f;

	// first pass (create g buffers)
//	getOrCreateStateSet()->setAttribute( new CSulProgramShaders( "shaders/sulDeferredRendering.frag,shaders/sulDeferredRendering.vert" ) );

	////////////////////////////////////
	// second pass (composite)
	////////////////////////////////////

	m_comp = new CSulTexCam( w, h );
	m_comp->setRenderOrder( osg::Camera::POST_RENDER );

	/*
	// gamma
	m_uniformGamma = new osg::Uniform( osg::Uniform::FLOAT, "gamma" );
	m_uniformGamma->set( (float)m_gamma );
	m_comp->getOrCreateStateSet()->addUniform( m_uniformGamma );
	*/

	m_quad = new CSulScreenAlignedQuad( osg::Vec3(w/2,h/2,0), w, h, w, h );
	m_quad->getGeode()->lighting( false );
	m_quad->getGeode()->zbufferWrite( false );
	m_quad->setName( "Composite Quad" );


	/*
	m_comp = new CSulTexCam( w, h, CSulTexCam::ORTHO );
	m_comp->setRenderOrder( osg::Camera::POST_RENDER );

	m_quad = new CSulGeomQuad( 2, 2 );
	m_quad->lighting( false );
	m_quad->zbufferWrite( false );
	m_quad->getGeometry()->setName( "Composite Quad" );
	*/

	m_quad->setTexture( getTexture(0), 0, "texPositions" );
	m_quad->setTexture( getTexture(1), 1, "texNormals" );
	m_quad->setTexture( getTexture(2), 2, "texColors" );
	m_quad->getOrCreateStateSet()->addUniform( m_lm->getUniformTexLightData() );
	//m_quad->getOrCreateStateSet()->addUniform( m_lm->getUniformCountLightPoints() );
	//m_quad->getOrCreateStateSet()->addUniform( m_lm->getUniformCountLightSpots() );
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

/*
void CSulDeferredCamera::setGamma( float gamma )
{
	m_gamma = gamma;
	m_uniformGamma->set( (float)m_gamma );
}
*/

void CSulDeferredCamera::traverse( osg::NodeVisitor& nv )
{
	// update g buffers
	CSulTexCam::traverse( nv );		

	if ( nv.getVisitorType()==osg::NodeVisitor::CULL_VISITOR )
	{
		// if we get to here.. then it is because the light is visible in the scene for this camera
		osgUtil::CullVisitor* cv = dynamic_cast<osgUtil::CullVisitor*>(&nv);

		osg::ref_ptr<osg::StateSet> ss = new osg::StateSet;
		ss->setTextureAttributeAndModes( 7, m_lm->getTextureData(), osg::StateAttribute::ON /*| osg::StateAttribute::PROTECTED*/ );

	//	ss->addUniform( static_cast<osg::Uniform*>(m_lm->getUniformCountLightPoints()->clone( osg::CopyOp::DEEP_COPY_ALL )) );
		
		//osg::Uniform* u0 = new osg::Uniform( osg::Uniform::SAMPLER_2D, "texLightPoint" );
		//u0->set( (int)7 );
		//ss->addUniform( u0 );

		osg::Uniform* u1 = new osg::Uniform( osg::Uniform::INT, "countLightPoints" );
		u1->set( (int)m_lm->getLightPointList().size() );
		//u1->set( 1 );
		ss->addUniform( u1 );

		osg::Uniform* u2 = new osg::Uniform( osg::Uniform::INT, "countLightSpots" );
		u2->set( (int)m_lm->getLightSpotList().size() );
		ss->addUniform( u2 );
		
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