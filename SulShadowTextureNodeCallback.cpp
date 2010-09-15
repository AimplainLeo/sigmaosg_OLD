// SulShadowTextureNodeCallback.cpp

#include "stdafx.h"
#include "SulShadowTextureNodeCallback.h"
#include <osg/PositionAttitudeTransform>
#include <iostream>

// NOTE: pRenderMe is the target area the camera should cover... it is NOT the actual object being rendered!!!

CSulShadowTextureNodeCallback::CSulShadowTextureNodeCallback( osg::Node* pBounds, osg::Node* pRenderMe, osg::Camera* pCam, osg::TexGenNode* pTexGenNode, const osg::Vec3& posLight )
{
	m_rBounds = pBounds;
	m_rRenderMe = pRenderMe;
	m_rCam = pCam;
	m_rTexGenNode = pTexGenNode;
	m_rPosLight = posLight;
}

void CSulShadowTextureNodeCallback::setLight( const osg::Vec3& pos )
{
	m_rPosLight = pos;
}

void CSulShadowTextureNodeCallback::operator()(osg::Node* node, osg::NodeVisitor* nv)
{
    // first update subgraph to make sure objects are all moved into postion
    traverse(node,nv);

	/*
	int i = nv->getFrameStamp()->getFrameNumber();
	std::cout << "CSulShadowTextureNodeCallback " << i << std::endl;
*/

	/*
	osg::Uniform* pTemp = m_rCam->getOrCreateStateSet()->getOrCreateUniform(  "osg_ViewMatrixInverse", osg::Uniform::FLOAT_MAT4 );
	pTemp->set(osg::Matrix::inverse(m_rCam->getViewMatrix()));		
	*/

	osg::Vec3 vPos(0.0f,0.0f,0.0f);

	// get the boundingbox for the objects we need to make a shadow of
	osg::BoundingSphere bs;
	if ( m_rBounds.valid() )
	{
		bs = m_rBounds->getBound();

		osg::PositionAttitudeTransform* pPat = dynamic_cast<osg::PositionAttitudeTransform*>(m_rBounds.get());
		if ( pPat )
		{
			vPos = pPat->getPosition();
		}
	}
	else
	{
		bs = m_rRenderMe->getBound();
	}

	// distance from light to center of boundingbox
	float centerDistance = (m_rPosLight-(bs.center()+vPos)).length();

    float znear = centerDistance-bs.radius();
    float zfar  = centerDistance+bs.radius();
    float zNearRatio = 0.001f;
    if (znear<zfar*zNearRatio) znear = zfar*zNearRatio;

    float top   = (bs.radius()/centerDistance)*znear;
    float right = top;

	m_rCam->setReferenceFrame( osg::Camera::ABSOLUTE_RF );
	m_rCam->setProjectionMatrixAsFrustum(-right, right, -top, top, znear, zfar );
	m_rCam->setViewMatrixAsLookAt( m_rPosLight, bs.center()+vPos, osg::Vec3(0.0f,0.0f,1.0f) );

    // compute the matrix which takes a vertex from local coords into tex coords
    // will use this later to specify osg::TexGen..
    osg::Matrix MVPT = m_rCam->getViewMatrix() * 
                       m_rCam->getProjectionMatrix() *
                       osg::Matrix::translate(1.0,1.0,1.0) *
                       osg::Matrix::scale(0.5f,0.5f,0.5f);
                       
    m_rTexGenNode->getTexGen()->setMode( osg::TexGen::EYE_LINEAR );
    m_rTexGenNode->getTexGen()->setPlanesFromMatrix( MVPT );

	osg::Uniform* pTemp = m_rCam->getOrCreateStateSet()->getOrCreateUniform(  "osg_ViewMatrixInverse", osg::Uniform::FLOAT_MAT4 );
	pTemp->set(osg::Matrix::inverse(m_rCam->getViewMatrix()));		

}
