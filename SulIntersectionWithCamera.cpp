// SulIntersectionWithCamera.cpp

#include "stdafx.h"
#include "SulIntersectionWithCamera.h"
#include "SulShaderZ.h"

CSulIntersectionWithCamera::CSulIntersectionWithCamera( osg::Node* pNode, CSulIntersectionInfo* pIntersectionInfo )
{
	// render to texture (RTT)
	m_rTexCam = new CSulTexCam( 128, 128, CSulTexCam::ZVALUE_WITH_IMAGE );
	//m_rTexCam->setDataVariance( osg::Object::DYNAMIC );
	m_rTexCam->addChild( pNode );
	addChild( m_rTexCam );

	// set a z position shader on the camera
	new CSulShaderZ( m_rTexCam );

	// the drawcallback is used to notify when the camera/intersection test has rendered a frame
	m_rDrawCallback = new CSulIntersectionDrawCallback( pIntersectionInfo, m_rTexCam->getImage() );
	m_rTexCam->setFinalDrawCallback( m_rDrawCallback );

	// we need an update callback to ensure that we don't get threading problems with the drawcallback
	m_rSulIntersectionWithCameraCallback = new CSulIntersectionWithCameraCallback( m_rDrawCallback );
	setUpdateCallback( m_rSulIntersectionWithCameraCallback );
}

osg::Texture2D*	CSulIntersectionWithCamera::getTexture()
{
	return m_rTexCam->getTexture();
}

void CSulIntersectionWithCamera::setPosition( const osg::Vec3& pos )
{
	osg::Vec3d eye, center, up;
	m_rTexCam->getViewMatrixAsLookAt( eye, center, up );
	m_rTexCam->setViewMatrixAsLookAt( pos, center, osg::Z_AXIS );
}

void CSulIntersectionWithCamera::setDirection( const osg::Vec3& dir )
{
	osg::Vec3d eye, center, up;
	m_rTexCam->getViewMatrixAsLookAt( eye, center, up );
	m_rTexCam->setViewMatrixAsLookAt( eye, eye+dir, osg::Z_AXIS );
}

void CSulIntersectionWithCamera::setLine( const osg::Vec3& v0, const osg::Vec3& v1 )
{
	setPosition( v0 );
	setDirection( v1-v0 );
}

void CSulIntersectionWithCamera::shoot()
{
//	osg::notify(osg::NOTICE) << "MSG: CSulIntersectionWithCamera::shoot" << std::endl;

	m_rSulIntersectionWithCameraCallback->shoot();
}

bool CSulIntersectionWithCamera::IsReady()
{
	return m_rSulIntersectionWithCameraCallback->IsReady();
}


