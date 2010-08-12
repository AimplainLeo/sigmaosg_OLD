// SulShadowTexture.cpp

#include "stdafx.h"
#include "SulShadowTexture.h"
#include "SulShadowTextureNodeCallback.h"

CSulShadowTexture::CSulShadowTexture( osg::Node* pRenderMe, osg::TexGenNode* pTexGenNode ) :
m_size_squared( 1024 )
{
	m_rTexGenNode = pTexGenNode;
	m_rRenderMe = pRenderMe;

	m_rGroup = new osg::Group;	
}

void CSulShadowTexture::init()
{
	m_rRTT = new CSulRTT( m_size_squared, m_size_squared, CSulRTT::SETUP_SHADOW );
	m_rRTT->AddChildToCamera( m_rRenderMe );
	m_rGroup->addChild( m_rRTT->getCamera() );

/**/
//		osg::Uniform* pTemp = m_rRTT->getCamera()->getOrCreateStateSet()->getOrCreateUniform(  "osg_ViewMatrixInverse", osg::Uniform::FLOAT_MAT4 );
//		pTemp->set(osg::Matrix::inverse(m_rRTT->getCamera()->getViewMatrix()));		
/**/

	// this adjusts the camera to cover the object area to be shadowed
	m_rShadowTextureNodeCallback = new CSulShadowTextureNodeCallback( m_rShadowMapBounds, m_rRenderMe, m_rRTT->getCamera(), m_rTexGenNode, m_pos );
	m_rGroup->setUpdateCallback( m_rShadowTextureNodeCallback );
}

void CSulShadowTexture::setShadowMapBounds( osg::Node* pNode )
{
	m_rShadowMapBounds = pNode;
}

void CSulShadowTexture::setTextureSize( sigma::uint32 size_squared )
{
	m_size_squared = size_squared;
}

osg::Node* CSulShadowTexture::getNode()
{
	return m_rGroup;
}

osg::Camera* CSulShadowTexture::getCamera()
{
	return m_rRTT->getCamera();
}

osg::Texture2D*	CSulShadowTexture::getTexture()
{
	return m_rRTT->getTexture();
}

void CSulShadowTexture::setLight( const osg::Vec3& pos )
{
	m_pos	= pos;

	if ( m_rShadowTextureNodeCallback.valid()  )
	{
		m_rShadowTextureNodeCallback->setLight( pos );
	}
}

