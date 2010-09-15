// SulGenHeightTexture.cpp

#include "stdafx.h"
#include "SulGenHeightTexture.h"
#include "SulCalcBoundingBox.h"

CSulGenHeightTexture::CSulGenHeightTexture( osg::Node* pRenderMe, sigma::uint32 texSizeXY ) :
osg::Group()
{
	m_rRenderMe = pRenderMe;

	// render to texture camera
	m_rTexCam = new CSulTexCam( texSizeXY, texSizeXY, CSulTexCam::HDR );
	m_rTexCam->setLODScale( 0.0 );
	m_rTexCam->addChild( pRenderMe );
	addChild( m_rTexCam );
}

void CSulGenHeightTexture::generate()
{
	// calc camera fustrum
	CSulCalcBoundingBox cbb( true );
	m_rRenderMe->accept( cbb );
	osg::BoundingBox bounds = cbb.getBoundBox();

	float l = bounds.xMin();
	float r = bounds.xMax();
	float b = bounds.yMin();
	float t = bounds.yMax();
	m_rTexCam->setProjectionMatrixAsOrtho( l, r, b, t, 0.1f, 100000000.0f );

	osg::Matrix viewMatrix;
	float m[16] =
	{
		 1, 0, 0, 0,
		 0, 1, 0, 0,
		 0, 0, 1, 0,
		 0, 0, 0, 1
	};
	viewMatrix.set( m );
	viewMatrix.setTrans( 0, 0, 100 );
	viewMatrix = viewMatrix.inverse( viewMatrix );
	m_rTexCam->setViewMatrix( viewMatrix );
}

osg::Texture2D* CSulGenHeightTexture::getTexture()
{
	return m_rTexCam->getTexture();
}